// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HttpRequestClient.h"

#include "UpdateService/Auth/RAuth.h"
#include "UpdateService/UpdateService.h"

#include "Network/ClientProtocols.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/BinaryPacketFunctionLibrary.h"

#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

#include "Runtime/Core/Public/Misc/Compression.h"

#include "zlib.h"

#define MAX_QUEUE 16 //Need Delay Case

void UHttpRequestClient::Initialize(void)
{
	//RGameInstance = instance;
	auto RAuth = URAuth::GetInstancePtr();
	// In case prelogin happens again after RGameInstance initializatoin
	RAuth->OnPreloginComplete.AddDynamic(this, &UHttpRequestClient::RefreshHost);
	//RefreshHost(EPreloginResult::PreloginSuccess); //todo del - qsmname
	
	// In case login happens again after initialization
	RAuth->OnReceiveClientKey.AddDynamic(this, &UHttpRequestClient::OnRAuthReceiveClientKey);
	//OnRAuthReceiveClientKey(true); //todo del - qsmname
	RequestQueue.Reserve(MAX_QUEUE);

	deviceid = RAuth->GetDeviceId();
	//SetTargetHost(RAuth->PreloginServerHost);
}

void UHttpRequestClient::RefreshHost(EPreloginResult Result)
{
	if (Result != EPreloginResult::PreloginSuccess)
		return;

	// Set TargetHost
	// live/dev should be determined by now
	URAuth* RAuth = URAuth::GetInstancePtr();

	// This should handled more gracefully
	TargetHost = RAuth->ReleaseTarget.ServerHost;
	if (TargetHost.IsEmpty())
	{
		UE_LOG(LogRTS, Log, TEXT("RefreshHost: ServerHost not set in ReleaseTarget"));
		return;
	}
}

void UHttpRequestClient::SetTargetHost(const FString& InTargetHost)
{
	TargetHost = InTargetHost;
}

void UHttpRequestClient::OnRAuthReceiveClientKey(bool IsSuccess)
{
	if (IsSuccess)
	{
		clientkey = URAuth::GetInstancePtr()->GetClientKey();
	}
}

void UHttpRequestClient::TransmitRequest(FHttpRequestObject& Request)
{	
	RequestQueue.Emplace(Request);

	if (!GetBusy())
		StartTransmission();
}

void UHttpRequestClient::TransmitRequest(FString& RequestJsonString)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsValid(RGAMEINSTANCE(this)->PacketErrorManager))
	{
		RGAMEINSTANCE(this)->PacketErrorManager->SetLastRequestJsonString(RequestJsonString);
	}
	FHttpRequestObject RequestObject;
	RequestObject.Data = RequestJsonString;
	UE_LOG(LogRTS, Log, TEXT("HTTP Send [%s]"), *RequestJsonString);
	TransmitRequest(RequestObject);
}

void UHttpRequestClient::TransmitRequestBinary(FString AddString, TArray<uint8>& ar)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsValid(RGAMEINSTANCE(this)->PacketErrorManager))
	{
		RGAMEINSTANCE(this)->PacketErrorManager->SetLastRequestJsonString(AddString);
	}

	FHttpRequestObject RequestObject;
	RequestObject.BinAddData = AddString;
	RequestObject.ArData = ar;

	TransmitRequest(RequestObject);
}

void UHttpRequestClient::StartTransmission()
{
	if (RequestQueue.Num() <= 0)
	{
		SetBusy(false);
		return;
	}
		
	//if (FHttpModule::Get().IsHttpEnabled())
	//	return;
		
	SetBusy(true);
	CurrentRequest = RequestQueue.Pop();
	
#ifdef USE_KEEP_ALIVE
	// Reuse request object for keep-alive effect
	if (!HttpRequest.IsValid())
	{
		//UE_LOG(LogNetwork, Log, TEXT("Creating request object"));
		HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHttpRequestClient::OnResponseReceived);
		HttpRequest->SetHeader("Connection", "keep-alive");
	}
	else
	{
		//UE_LOG(LogNetwork, Log, TEXT("Reusing request object %d"), (int32)HttpRequest->GetStatus());
		// clear the request object
		HttpRequest->SetHeader(TEXT("Pragma"), TEXT(""));
		HttpRequest->SetHeader(TEXT("Expect"), TEXT(""));
	}

#else
	HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHttpRequestClient::OnResponseReceived);
#endif

	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("User-Agent", "ProjectR/0.1");

	// BinAddData is used, when native 
	if (CurrentRequest.BinAddData.IsEmpty())
	{
#ifdef USE_COMPRESS_PACKET
		UE_LOG( LogRTS, Log, TEXT("WebServer Send[%s]"), *(CurrentRequest.Data) );
		HttpRequest->SetURL(TargetHost + "/?compressed=1.deflate&");
		TArray<uint8> compressedData;
		CompressPacket(CurrentRequest.Data, compressedData );
		HttpRequest->SetContent(compressedData);
		HttpRequest->SetHeader(TEXT("Content-Type"), STREAMTYPE);
#else
		HttpRequest->SetURL(TargetHost);
		HttpRequest->SetContentAsString(CurrentRequest.Data);
		HttpRequest->SetHeader(TEXT("Content-Type"), JSONTYPE);
#endif

	}
	else
	{
		// for PD, attach bin data
		FString strUrl = TargetHost + TEXT("_bin/") + CurrentRequest.BinAddData;
		HttpRequest->SetURL(strUrl);
		HttpRequest->SetContent(CurrentRequest.ArData);
		HttpRequest->SetHeader("Content-Type", STREAMTYPE);

		//HttpRequest->OnRequestProgress().BindUObject(this, &UHttpRequestClient::OnProgressBinary);
	}

	HttpRequest->SetHeader(TEXT("Content-Length"), FString::Printf(TEXT("%d"), HttpRequest->GetContentLength()));

	/*
	UE_LOG(LogNetwork, Log, TEXT("URL: %s"), *HttpRequest->GetURL());
	UE_LOG(LogNetwork, Log, TEXT("Content type: %s"), *HttpRequest->GetContentType());
	// UE_LOG(LogNetwork, Log, TEXT("URL parameters: %s"), HttpRequest->GetURLParameter());
	auto headers = HttpRequest->GetAllHeaders();
	for (int i = 0; i < headers.Num(); i++)
	{
		UE_LOG(LogNetwork, Log, TEXT("Header: %s"), *headers[i]);
	}
	*/

	if (!HttpRequest->ProcessRequest())
	{
		UE_LOG(LogRTS, Log, TEXT("Unable to process HTTP request"));
		IsBusy = false;
		return;
	}	
}

void UHttpRequestClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSucessful)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	MessageBody.Reset();
	
	if (!Response.IsValid())
	{
		//MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		UE_LOG(LogJson, Log, TEXT("UHttpRequestClient::OnResponseReceived - [Unable to process HTTP Request!]"));

#ifdef USE_KEEP_ALIVE
		HttpRequest = nullptr;
#endif

		RGAMEINSTANCE(this)->PacketErrorManager->OnHTTPDisconnect();
		return;
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{		
		FString strPacketType = Response->GetHeader("Content-Type");
		
		if (strPacketType.Find(JSONTYPE) != INDEX_NONE)
		{
			MessageBody = Response->GetContentAsString();

			Closed(MessageBody);
		}
		else if (strPacketType.Find(STREAMTYPE) != INDEX_NONE)
		{			
#if (PLATFORM_MAC || PLATFORM_IOS)
			if (Request.IsValid())
			{
				ClosedBinary(Request->GetURL(), Response->GetContent());
			}
#else
			ClosedBinary(Response->GetURL(), Response->GetContent());
#endif
		}		
	}
	else
	{
		//MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		UE_LOG(LogJson, Log, TEXT("UHttpRequestClient::OnResponseReceived - [HTTP Error: %d]"), Response->GetResponseCode());
		UE_LOG(LogJson, Log, TEXT("UHttpRequestClient::OnResponseReceived - [HTTP BodyMessage: %s]"), *Response->GetContentAsString());
		
		ClosedPostProcess();

		return;
	}	
}

void UHttpRequestClient::OnProgressBinary(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
}

void UHttpRequestClient::Closed(const FString& Message)
{	
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	int32 PacketType = EPacketType::PACKET_ERROR;
	bool PacketLogicalResult = false;
	UPacketFunctionLibrary::DispatchNetMessage(RGAMEINSTANCE(this), Message, PacketType, PacketLogicalResult);
	ReplyPacketBroadcast(PacketType, PacketLogicalResult, Message);

	ClosedPostProcess();
}

void UHttpRequestClient::ClosedBinary(const FString& Url, const TArray<uint8>& ArData)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	int32 PacketType = EPacketType::PACKET_ERROR;
	bool PacketLogicalResult = false;
	if (UBinaryPacketFunctionLibrary::DispatchNetMessage(RGAMEINSTANCE(this), this, Url, ArData))
	{
		ClosedPostProcess();
	}	
}

void UHttpRequestClient::ClosedPostProcess()
{
	//SetBusy(false);

	// Retry transmission in case there are any queued messages (if not, nothing happens)
	StartTransmission();
}

void UHttpRequestClient::ReplyPacketBroadcast(int32 PacketType, bool PacketLogicalResult, const FString& Message)
{
#if UE_BUILD_SHIPPING != 1
	FString	PacketName = UUtilFunctionLibrary::GetEnumToString(TEXT("EPacketType"), (int32)PacketType);
	UE_LOG(LogJson, Log, TEXT("UHttpRequestClient PacketType: [%s], PacketLogicalResult: [%d]"), *PacketName, PacketLogicalResult);
	if (PacketLogicalResult == false)
	{
		URGameInstance* instance = RGAMEINSTANCE(this);
		//from jira temp message
		FText Error_Message = FText::Format(FText::FromString("UHttpRequestClient PacketType: [{0}], PacketLogicalResult: [{1}]"),
			FText::FromString(PacketName), FText::AsNumber(PacketLogicalResult));
		//FText Error_Message = FText::FromString("enter failed");
		//instance->RWidgetManager->ShowCommonPopup(FText::FromString(TEXT("CHAOS")), Error_Message, ECommonPopupType::VE_OK);
	}
#endif
	switch (PacketType)
	{
		// Account
	case EPacketType::LOGIN_RP:						OnLoginRpDelegate.Broadcast(PacketLogicalResult);					break;
	case EPacketType::ACCOUNT_CREATE_RP:			OnAccountCreateRpDelegate.Broadcast(PacketLogicalResult);			break;
		// Avatar
	case EPacketType::AVATAR_GAME_DATA_RP:			if (PacketLogicalResult) OnAvatarGameDataRpDelegate.Broadcast();	break;
		// PlayMode
	//case EPacketType::PLAY_MODE_SWEEP_RP:			OnPlayModeSweepRpDelegate.Broadcast(PacketLogicalResult);			break;
// 	case EPacketType::PLAY_MODE_RAID_RP:			OnPlayModeRaidRpDelegate.Broadcast();								break;
// 	case EPacketType::PLAY_MODE_RAID_DONE_RP:		OnPlayModeRaidDoneRpDelegate.Broadcast();							break;
		// Arena
//	case EPacketType::ARENA_MATCH_LIST_RP:			OnArenaGetMatchesInfoRpDelegate.Broadcast();						break;
//	case EPacketType::ARENA_SEASON_DONE_RP:			OnArenaSeasonDoneRpDelegate.Broadcast();							break;
//	case EPacketType::ARENA_PLAY_RP:				OnPlayModeArenaRpDelegate.Broadcast();								break;
//	case EPacketType::ARENA_PLAY_DONE_RP:			OnPlayModeArenaDoneRpDelegate.Broadcast();							break;
//	case EPacketType::ARENA_RANKING_LIST_RP:		OnArenaGetRankingListRpDelegate.Broadcast();						break;
//	case EPacketType::ARENA_HISTORY_RP:				OnArenaGetHistoryRpDelegate.Broadcast();							break;
		//
	case EPacketType::CONTENT_RESET_RP:				OnContentResetRpDelegate.Broadcast(PacketLogicalResult);			break;
// 	case EPacketType::CONTENT_RESET_STATUS_RP:		OnContentResetStatusRpDelegate.Broadcast();							break;
	}
}


void UHttpRequestClient::SetKID(int32 id)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	kId = id;

	if (IsValid(RGAMEINSTANCE(this)->UserInfo))
	{
		RGAMEINSTANCE(this)->UserInfo->kId = kId;
	}
}

int32 UHttpRequestClient::GetKID()
{
	return kId;
}

bool UHttpRequestClient::IsValidKID(int32 id)
{
	if (id == KID_NONE)
		return false;

	return (kId == id) ? true : false;
}

bool UHttpRequestClient::IsResponseSuccess(ECRUD is)
{
	return (is == ECRUD::CRUD_OK) ? true : false;
}

bool UHttpRequestClient::IsValidNetwork()
{
	return (kId != KID_NONE) ? true : false;
}

void UHttpRequestClient::SetBusy(bool bBusy)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsBusy == bBusy)
		return;
	IsBusy = bBusy;

	// checking if patch download is complete because HttpRequestClient may be used before patch
	//if (UUpdateService::GetInstancePtr()->IsContentPresent())
	if (URAuth::GetInstancePtr()->GetIsDataState())
		UUIFunctionLibrary::ShowWaitingPopup(bBusy);
}

bool UHttpRequestClient::GetBusy()
{
	return IsBusy;
}

void UHttpRequestClient::CompressPacket(FString InPacket, TArray<uint8>& OutCompressedData )
{
	TArray<uint8> UncompressedBuffer;

	FTCHARToUTF8 EchoStrUtf8(*InPacket);
	FMemoryWriter ArWriter(UncompressedBuffer);
	ArWriter.Serialize((void*)EchoStrUtf8.Get(), EchoStrUtf8.Length());

	OutCompressedData.SetNum(UncompressedBuffer.Num() * 1023, true);

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	stream.avail_in = UncompressedBuffer.Num();
	stream.next_in = (Bytef *)UncompressedBuffer.GetData();
	stream.avail_out = OutCompressedData.Num();
	stream.next_out = (Bytef *)OutCompressedData.GetData();

	if (ensure(Z_OK == deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY)))
	{
		if (ensure(Z_STREAM_END == deflate(&stream, Z_FINISH)))
		{
			if (ensure(Z_OK == deflateEnd(&stream)))
			{
			}
		}
		else
		{
			deflateEnd(&stream);
		}
	}

	OutCompressedData.RemoveAt(stream.total_out, OutCompressedData.Num() - stream.total_out, true);
}

void UHttpRequestClient::DecompressPacket(const TArray<uint8>& InCompressedData, FString& OutData)
{
	TArray<uint8> UncompressedBuffer;
	UncompressedBuffer.SetNum(InCompressedData.Num() * 1032);

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	stream.avail_in = InCompressedData.Num();
	stream.next_in = (Bytef *)InCompressedData.GetData();
	stream.avail_out = UncompressedBuffer.Num();
	stream.next_out = (Bytef *)UncompressedBuffer.GetData();

	int32 Result = inflateInit2(&stream, 15);

	if (Result != Z_OK)
		return;

	// Uncompress data.
	inflate(&stream, Z_FINISH);
	inflateEnd(&stream);

	OutData = FString(UTF8_TO_TCHAR(UncompressedBuffer.GetData()));
}