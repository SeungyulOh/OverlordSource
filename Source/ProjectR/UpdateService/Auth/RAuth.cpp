// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAuth.h"
#include "Online.h"
#include "OnlineExternalUIInterface.h"
#include "UpdateService/PlatformAuth.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/ClientProtocols.h"
#include "UtilFunctionIntegrated.h"
#include "PublisherAPI.h"
#include "PublisherAPIInterface.h"
#include "GlobalIntegrated.h"
#include "Global/RGameUserSetting.h"
#include "SaveObject/RSaveGame.h"

#include "OnlineSubsystemModule.h"
#include "OnlineSubsystem.h"

#include "UpdateService/LoginGameMode.h"

//#include "UpdateService/Android/AppsFlyerSDK.h"

//#include "AndroidMisc.h"



#define PROJECTR_SERVICE_URL TEXT("http://s3.ap-northeast-2.amazonaws.com/overlord-dl-service/")
#define JSONTYPE		"application/json"

URAuth *URAuth::InstancePtr = nullptr;
const FString URAuth::PreloginServerHost = TEXT("https://overlord-vcs.kokomogames.co.kr:443/overlord");
//const FString URAuth::ServerHost		= TEXT("https://overlord-vcs.kokomogames.co.kr:443/overlord");

URAuth::URAuth()
{
	
}

FString URAuth::GetBuildConfig()
{
#if UE_BUILD_SHIPPING
	return TEXT("Shipping");
#elif UE_BUILD_DEVELOPMENT
	return TEXT("Development");
#elif UE_BUILD_DEBUG
	return TEXT("Debug");
#else
	return TEXT("UnknownBuildConfig");
#endif

}

URAuth* URAuth::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< URAuth >((UObject*)GetTransientPackage(), URAuth::StaticClass());
		InstancePtr->Init();
		InstancePtr->AddToRoot();
	}

	return InstancePtr;
}

void URAuth::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
		InstancePtr = nullptr;
	}
}

void URAuth::Init()
{
#if API_NZIN
	// Disabled temporally for simplification
	// Load clientkey, maybe have it stored AES encrypted in a file.
	/*
	TArray<uint8> StoredClientKey;
	if (!FPlatformAuth::ReadData(StoredClientKey))
	{
		UE_LOG(LogRAuth, Warning, TEXT("Init: FPlatformAuth::ReadData failed"));
	}
	else
	{
		if (StoredClientKey.Num() > 0)
		{
			clientkey = BytesToString(StoredClientKey.GetData(), StoredClientKey.Num());
			UE_LOG(LogRAuth, Log, TEXT("Init: restored clientkey %s"), *clientkey);
		}
	}
	*/

	// Read deviceid
	GetPublisherAPI()->Initialize();
#endif

	// "version" for GET_TARGET_SERVER
	BuildVersion = TEXT(PREPROCESSOR_TO_STRING(PROJECTR_BUILD_VERSION));
	IsPreloggedIn = false;

	UE_LOG(LogRAuth, Log, TEXT("BuildVersion: %s"), *BuildVersion);
	UE_LOG(LogRAuth, Log, TEXT("DeviceId: %s"), *GetDeviceId());

}


void URAuth::Prelogin()
{
	// InvalidateClientKey();
	SetIsDataState(false);
	/*if (IsPreloggedIn)
	{
		UE_LOG(LogRAuth, Log, TEXT("Prelogin: already prelogged in"));
		FinishPrelogin(EPreloginResult::PreloginSuccess);
		return;
	}*/
	LoadAccountLocalRecord();

	//LoadReleaseTarget();
	UObject* pThis = GetInstancePtr();
	auto HttpClient = RGAMEINSTANCE(pThis)->HttpClient;
	if (HttpClient)
	{
		HttpClient->SetTargetHost(PreloginServerHost);
	}

	UPacketFunctionLibrary::GET_TARGET_SERVER_RQ();
}

void URAuth::OnGetTargetServerRp(bool bSuccess, FString ErrorCode, const FReleaseTarget* InReleaseTarget)
{
	if (bSuccess)
	{
		ReleaseTarget = *InReleaseTarget;

		FinishPrelogin(EPreloginResult::PreloginSuccess);
	}
	else
	{
		// TODO: parse ErrorCode
		UE_LOG(LogRAuth, Log, TEXT("URAuth::OnGetTargetServerRp: ErrorCode: %s"), *ErrorCode);

		FinishPrelogin(EPreloginResult::PreloginFailure);
	}
}

FString URAuth::GetReleaseTargetEnv()
{
	return ReleaseTarget.Env;
}

void URAuth::AvatarGameDataRp()
{
	UObject* pThis = GetInstancePtr();
	auto& AvatarData = RGAMEINSTANCE(pThis)->UserInfo->GetAvatarData();
	SetNickName(AvatarData.nick);
}

void URAuth::OverrideEnv(EReleaseEnv InReleaseTarget)
{
#if WITH_EDITOR || PLATFORM_WINDOWS || PLATFORM_MAC
	// Editor env override
	// using preset build numbers for development
	switch (InReleaseTarget)
	{
	case EReleaseEnv::None:
	case EReleaseEnv::DEV:
		BuildVersion = TEXT("c-dev-1");
		break;

	case EReleaseEnv::LIVE:
		BuildVersion = TEXT("c-live-1");
		break;

	default:
		UE_LOG(LogRAuth, Log, TEXT("URAuth::OverrideEnv: unhandled env"));
		BuildVersion = TEXT("c-dev-1");
		break;
	}
#endif
}

// TODO: very temp
//void URAuth::LoadReleaseInfo()
//{
//	FString Url = FString(PROJECTR_SERVICE_URL) + TEXT("Releases/") + TEXT("DEV-a05f7fa2d9a964f5") + TEXT(".json");
//
//	FHttpModule* HttpModule = &FModuleManager::LoadModuleChecked<FHttpModule>(TEXT("HTTP"));
//	FHttpRequestPtr HttpRequest = FHttpModule::Get().CreateRequest();
//	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URAuth::OnReleaseInfoRetrieveComplete);
//	HttpRequest->SetURL(Url);
//	HttpRequest->SetVerb(TEXT("GET"));
//	HttpRequest->ProcessRequest();
//}

void URAuth::FinishPrelogin(EPreloginResult Result)
{
	IsPreloggedIn = (Result == EPreloginResult::PreloginSuccess);

	UObject* pThis = GetInstancePtr();
	auto HttpClient = RGAMEINSTANCE(pThis)->HttpClient;
	if (HttpClient)
	{
		if (HttpClient->OnAvatarGameDataRpDelegate.IsBound() == false)
		{
			HttpClient->OnAvatarGameDataRpDelegate.AddDynamic(this, &URAuth::AvatarGameDataRp);
		}
	}

#if API_NZIN
	if (IsPreloggedIn)
	{
		GetPublisherAPI()->Start(); // TODO: called here because I don't yet know how to insert this into viewDidLoad, for iOS
	}
#endif

	OnPreloginComplete.Broadcast(Result);
}

void URAuth::LoginToGameServer()
{
	// need the result of IdProvider->LoadUnreadMessageCount if the operation is applicable for the IdProvider
/*
	bool bHandled = false;

	// TODO

	FKakaoPluginModule::OnGetUnreadMessageCount.AddUObject(this, &URAuth::OnGetUnreadMessageCount);
	UKakaoSDKBlueprintFunctionLibrary::LoadUnreadMessageCount();
	bHandled = true;
	
	if (!bHandled)
	{
		UPacketFunctionLibrary::LOGIN_RQ((int32)ECLIENT_SDK_FLAG::NONE);
	}
*/


	UPacketFunctionLibrary::LOGIN_RQ((int32)ECLIENT_SDK_FLAG::NONE);
}

void URAuth::LoadAccountLocalRecord()
{
	UObject* pThis = GetInstancePtr();
	auto RSaveGame = RGAMEINSTANCE(pThis)->GetSaveGame();

	if (RSaveGame->IsAlreadyUserInfo())
	{
		AccountInfo = RSaveGame->AccountInfo;
#if !WITH_EDITOR //test
		//if ((AccountInfo.Type == PUBLISHERID_DEV) == false)
#endif
		{
			IsCheckAccount = AccountInfo.bIsAcceptTerms;
		}
	}
}

void URAuth::SaveAccountLocalRecord()
{
	UObject* pThis = GetInstancePtr();
	auto RSaveGame = RGAMEINSTANCE(pThis)->SaveGame;
	if (RSaveGame)
	{
		RSaveGame->AccountInfo = AccountInfo;
		RSaveGame->Save();
	}

	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (IsValid(RGameUserSetting))
	{
		RGameUserSetting->ApplySettings(true);
	}
}

//void URAuth::OnReleaseTargetRetrieveComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
//{
//	if (!bSucceeded) {
//		UE_LOG(LogRAuth, Error, TEXT("OnReleaseTargetRetrieveComplete: Failed to GET %s"), *Request->GetURL());
//		FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//
//		return;
//	}
//	UE_LOG(LogRAuth, Log, TEXT("OnReleaseTargetRetrieveComplete: status code = %d"), Response->GetResponseCode());
//
//	if (!ReadReleaseTarget(Response->GetContentAsString())) {
//		UE_LOG(LogRAuth, Error, TEXT("Failed to read ReleaseTarget json"));
//		FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//		return;
//	}
//
//	UE_LOG(LogRAuth, Log, TEXT("Successfully read ReleaseTarget"));
//	FinishPrelogin(EPreloginResult::PRELOGIN_SUCCESS);
//	return;
//}
//
//bool URAuth::ReadReleaseTarget(const FString& ResponseString)
//{
//	TSharedPtr<FJsonObject> ReleaseTargetObject;
//	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
//
//	if (FJsonSerializer::Deserialize(Reader, ReleaseTargetObject) && ReleaseTargetObject.IsValid())
//	{
//		if (!ReleaseTargetObject->TryGetStringField(TEXT("name"), ReleaseTarget.Env))
//		{
//			return false;
//		}
//
//		// Signature of TryGetObjectField is strange
//		TSharedPtr<FJsonObject> UrlMapObject = ReleaseTargetObject->GetObjectField(TEXT("urlMap"));
//
//		bool UrlMapSuccess = true;
//		UrlMapSuccess &= UrlMapObject->TryGetStringField(TEXT("serverHost"), ReleaseTarget.ServerHost);
//		UrlMapSuccess &= UrlMapObject->TryGetStringField(TEXT("cdnRoot"), ReleaseTarget.Cdn);
//		UrlMapSuccess &= UrlMapObject->TryGetStringField(TEXT("tcpServerHost"), ReleaseTarget.TcpServerHost);
//		UrlMapSuccess &= UrlMapObject->TryGetStringField(TEXT("tcpServerPort"), ReleaseTarget.TcpServerPort);
//
//		if (!UrlMapSuccess)
//		{
//			return false;
//		}
//	}
//	else
//	{
//		UE_LOG(LogRAuth, Log, TEXT("ReadReleaseTarget: Failed to deserialize json"));
//		UE_LOG(LogRAuth, Log, TEXT("ResponseString: %s"), *ResponseString);
//		return false;
//	}
//
//	return true;
//}


//===================================================================================
//
// Login process
// Init_client_key -> Login -> create account
//
//===================================================================================

IPublisherAPI* URAuth::GetPublisherAPI()
{
	FPublisherAPIModule& PublisherModule = FModuleManager::GetModuleChecked<FPublisherAPIModule>(TEXT("PublisherAPI"));
	return PublisherModule.GetPublisherAPI();
}

void URAuth::InitClientKey()
{
	UE_LOG(LogRAuth, Log, TEXT("URAuth::Login(%s, %s, %s)"), *AccountInfo.Type, *AccountInfo.Id, *AccountInfo.Token);
	
	FString Empty;
	UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(
		AccountInfo.Type,		//publisher
		AccountInfo.Token,		//code
		AccountInfo.Token,		//access_token
		Empty,					//redirect_uri
		AccountInfo.Id,			//id
		Empty);					//pwd
}

void URAuth::Login(FString InPublisher, FString InClient_id, FString InAccess_token)
{
	UE_LOG(LogRAuth, Log, TEXT("URAuth::Login(%s, %s, %s)"), *InPublisher, *InClient_id, *InAccess_token);

	UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(InPublisher, TEXT(""), InAccess_token, TEXT(""), InClient_id, TEXT(""));
}

void URAuth::LoginWithCachedClientkey()
{
	OnReceiveClientKey.Broadcast(IsValidClientKey());
	LoginToGameServer();
}

void URAuth::LoginWithCachedClientkey(FString InClientKey)
{
	UE_LOG(LogRAuth, Log, TEXT("URAuth::LoginWithCachedClientkey(%s)"), *InClientKey);

	SetClientKey(InClientKey);
	LoginToGameServer();
}


void URAuth::RequestCreateAccount(FString InNickName, int32 InLanguage)
{
	UPacketFunctionLibrary::ACCOUNT_CREATE_RQ(InNickName, InLanguage);
}

void URAuth::OnGetUnreadMessageCount(int32 Result, int32 Count)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER OnGetUnreadMessageCount with %d, count %d"), (int32)Result, Count);

	/*
	FKakaoPluginModule::OnGetUnreadMessageCount.RemoveAll(this);
	*/

	if ((EIdpLoginResult)Result == EIdpLoginResult::VE_Ok && Count > 0)
	{
		UPacketFunctionLibrary::LOGIN_RQ((int32)ECLIENT_SDK_FLAG::KAKAO_MESSAGE_GET);
	}
	else
	{
		UPacketFunctionLibrary::LOGIN_RQ((int32)ECLIENT_SDK_FLAG::NONE);
	}
}

FString URAuth::GetClientKey()
{
	return AccountInfo.ClientKey;
}

void URAuth::SetClientKey(FString& InClientKey)
{
	UE_LOG(LogRAuth, Log, TEXT("SetClientKey: %s"), *InClientKey);
	//AccountInfo.bIsAcceptTerms = true;
	AccountInfo.ClientKey = InClientKey;

	OnReceiveClientKey.Broadcast(IsValidClientKey());
}

bool URAuth::IsValidClientKey()
{ 
	return !(AccountInfo.ClientKey.IsEmpty());
}

FString URAuth::GetDeviceId() const
{
#if API_NZIN
	return GetPublisherAPI()->GetDeviceId();
#endif

	return FPlatformMisc::GetDeviceId();
}

void URAuth::InvalidateClientKey()
{
	FString Empty;
	SetClientKey(Empty);
	TArray<uint8> EmptyData;
	if (!FPlatformAuth::StoreData(EmptyData))
	{
		UE_LOG(LogRAuth, Warning, TEXT("InvalidateClientKey: StoreData(EmptyData) failed"));
	}
}

void URAuth::StoreClientKey()
{
	TArray<uint8> ClientKeyData;
	ClientKeyData.AddUninitialized(128);
	int32 Size = StringToBytes(GetClientKey(), ClientKeyData.GetData(), 128) + 1;
	ClientKeyData.SetNum(Size);
	if (!FPlatformAuth::StoreData(ClientKeyData))
	{
		UE_LOG(LogRAuth, Warning, TEXT("SetClientKey: StoreData failed"));
	}
}

bool URAuth::OnINIT_CLIENT_KEY_RP(FINIT_CLIENT_KEY_RP& init_client_key_rp)
{
	if (init_client_key_rp.is != ECRUD::CRUD_OK)
	{
		// Return to GameServerLoginState
		UE_LOG(LogRAuth, Log, TEXT("OnINIT_CLIENT_KEY_RP: not ok with code=%s, name=%s"), *init_client_key_rp.error_info.code, *init_client_key_rp.error_info.name);
		OnLoginComplete.Broadcast(ELoginResult::VE_InitClientKey, TEXT(""));

		return false;
	}

	SetClientKey(init_client_key_rp.clientkey);
	LoginToGameServer();
	
	return true;
}

bool URAuth::OnLOGIN_RP(FLOGIN_RP& login_rp)
{
	ELoginResult Result = ELoginResult::VE_Ok;// = ELoginResult(ErrorCode);
	if (login_rp.is != ECRUD::CRUD_OK)
	{
		int32 ErrorCode = FCString::Atoi(*login_rp.error_info.code);
		switch (ErrorCode)
		{
			case 1100:	Result = ELoginResult::VE_InitClientKey;			break;
			case 1101:	Result = ELoginResult::VE_ExpiredClientKey;			break;
			case 1102:	Result = ELoginResult::VE_InvalidPublisherName;		break;
			case 1103:	Result = ELoginResult::VE_InvalidPublisher;			break;
			case 1104:	Result = ELoginResult::VE_InvalidPublisherCode;		break;
			case 1105:	Result = ELoginResult::VE_AccountNotFound;			break;
			case 1106:	Result = ELoginResult::VE_AccountSequenceError;		break;
			case 1107:	Result = ELoginResult::VE_NickAlreadyExists;		break;
			case 1108:	Result = ELoginResult::VE_AccountAlreadyHave;		break;
			case 1109:	Result = ELoginResult::VE_DoesNotHaveAvatar;		break;
			case 1110:	Result = ELoginResult::VE_BlockedAccount;			break;
			case 1111:	Result = ELoginResult::VE_EmptyNick;				break;
			case 1112:	Result = ELoginResult::VE_AreadyHave;				break;
			case 1113:	Result = ELoginResult::VE_PublisherIdAlreadyUsed;	break;
			case 1114:	Result = ELoginResult::VE_AccountCreateNickInvalid;	break;
			case 1115:	Result = ELoginResult::VE_SystemMaintenanced;		break;
			case 1116:	Result = ELoginResult::VE_ApiMaintenance;			break;
			default:	Result = ELoginResult::VE_AccountSequenceError;		break;
		}
	}
	else
	{
		if (AccountInfo.NickName.IsEmpty())
		{
			UPacketFunctionLibrary::AVATAR_GAME_DATA_RQ();
		}
	}
	
	OnLoginComplete.Broadcast(Result, GetClientKey());
	return (Result == ELoginResult::VE_Ok);
}

bool URAuth::OnACCOUNT_CREATE_RP(FACCOUNT_CREATE_RP& account_create_rp)
{
	ECreateAccountResult Result = ECreateAccountResult::VE_Ok;
	if (account_create_rp.is != ECRUD::CRUD_OK)
	{
		int32 ErrorCode = FCString::Atoi(*account_create_rp.error_info.code);
		
		switch (ErrorCode)
		{
			case 1114:	Result = ECreateAccountResult::VE_NickInvalid;			break;
			case 1304:	Result = ECreateAccountResult::VE_NickAlreadyExists;	break;
			default:	Result = ECreateAccountResult::VE_UnknownError;			break;
		}
	}

	OnCreateAccountRp.Broadcast(Result);
	return (Result == ECreateAccountResult::VE_Ok);
}

//FText URAuth::FormatErrorMessage(const FERROR_INFO& error_info)
//{
//	UE_LOG(LogRAuth, Log, TEXT("ENTER FormatErrorMessage with code=%s name=%s"), *error_info.code, *error_info.name);
//
//	// Query localized text message
//	FText LocalizedText;
//	{
//		FString LocNamespace = TEXT("Startup");
//		if (!FText::FindText(LocNamespace, error_info.code, /* out */ LocalizedText))
//		{
//			UE_LOG(LogRAuth, Error, TEXT("FormatErrorMessage: failed to find from loc tables ns=%s key=%"), *LocNamespace, *error_info.code);
//			return FText::FromString(FString::Printf(TEXT("(%s)"), *error_info.code));
//		}
//	}
//
//	FText TempArg;
//	FFormatOrderedArguments DescArguments;
//	for (auto DescArg : error_info.desc)
//	{
//		TempArg = FText::FromString(DescArg);
//		DescArguments.Add(MoveTemp(TempArg));
//	}
//
//	FText ErrorText = FText::Format(LocalizedText, DescArguments);
//
//	return ErrorText;
//}

template<typename OutStructType>
static bool GetUStructFromJsonObject(const TSharedRef<FJsonObject>& JsonObject, OutStructType* OutStruct)
{
	return FJsonHelper::JsonObjectToUStruct(JsonObject, OutStructType::StaticStruct(), OutStruct, 0, 0);
}

//void URAuth::LoadReleaseTarget()
//{
//	FString Url = FString(PROJECTR_SERVICE_URL) + TEXT("ReleaseTargets/") + TEXT("DEV-a05f7fa2d9a964f5") + TEXT(".json");
//
//	FHttpModule* HttpModule = &FModuleManager::LoadModuleChecked<FHttpModule>(TEXT("HTTP"));
//	FHttpRequestPtr HttpRequest = FHttpModule::Get().CreateRequest();
//	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URAuth::OnReleaseTargetRetrieveComplete);
//	HttpRequest->SetURL(Url);
//	HttpRequest->SetVerb(TEXT("GET"));
//	HttpRequest->ProcessRequest();
//}

//void URAuth::OnReleaseInfoRetrieveComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
//{
//	if (!bSucceeded) {
//		UE_LOG(LogRAuth, Error, TEXT("OnReleaseInfoRetrieveComplete: Failed to GET %s"), *Request->GetURL());
//		FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//
//		return;
//	}
//	UE_LOG(LogRAuth, Log, TEXT("OnReleaseInfoRetrieveComplete: status code = %d"), Response->GetResponseCode());
//
//	bool MustUpdate = false;
//
//	{
//		TSharedPtr<FJsonObject> JsonObject;
//		TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
//		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
//		{
//			UE_LOG(LogJson, Error, TEXT("OnReleaseInfoRetrieveComplete: Unable to deserialize. json=[%s]"), *Response->GetContentAsString());
//			FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//			return;
//		}
//
//		if (!JsonObject->TryGetBoolField(TEXT("mustUpdate"), MustUpdate))
//		{
//			UE_LOG(LogJson, Error, TEXT("OnReleaseInfoRetrieveComplete: Unable to read MustUpdate. json=[%s]"), *Response->GetContentAsString());
//		}
//
//		if (!JsonObject->TryGetStringField(TEXT("manifestBuildNumber"), ManifestBuildNumber))
//		{
//			UE_LOG(LogJson, Error, TEXT("OnReleaseInfoRetrieveComplete: Unable to read manifestBuildNumber. json=[%s]"), *Response->GetContentAsString());
//			FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//			return;
//		}
//
//		if (!JsonObject->TryGetStringField(TEXT("releaseTarget"), BuildNumber))
//		{
//			UE_LOG(LogJson, Error, TEXT("OnReleaseInfoRetrieveComplete: Unable to read ReleaseTargetHashed. json=[%s]"), *Response->GetContentAsString());
//			FinishPrelogin(EPreloginResult::PRELOGIN_FAILURE);
//			return;
//		}
//	}
//
//	UE_LOG(LogRAuth, Log, TEXT("OnReleaseInfoRetrieveComplete: Successfully read Release info"));
//	if (MustUpdate)
//	{
//		FinishPrelogin(EPreloginResult::PRELOGIN_LOW_VERSION);
//		return;
//	}
//	else
//	{
//		LoadReleaseTarget();
//	}
//}

ELoginProcess URAuth::IsAccountState()
{
	if(AccountInfo.Id.IsEmpty())
	{
		return ELoginProcess::VE_NeedLoginId;
	}
	else if (IsCheckAccount)
	{
		IsCheckAccount = false;

		if (AccountInfo.Type.IsEmpty() == false)
		{
			if (AccountInfo.Type == (PUBLISHERID_DEV))
			{
				return ELoginProcess::VE_OK;
			}

			UObject* pThis = GetInstancePtr();
			APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(RGAMEINSTANCE(pThis)->GetWorld(), 0);
			if (PlayerCon)
			{
				if (UKismetSystemLibrary::IsLoggedIn(PlayerCon))
				{
					return ELoginProcess::VE_NeedCheckAccount;
				}
			}
		}
		
		return ELoginProcess::VE_NeedPlatformLogin;
	}
	else if(AccountInfo.NickName.IsEmpty())
	{
		return ELoginProcess::VE_NeedNickName;
	}
	
	/*else if (AccountInfo.Type.IsEmpty())
	{
		//system error
	}
	else if (AccountInfo.Token.IsEmpty())
	{
		return ELoginProcess::VE_NeedReTry;
	}
	else if (AccountInfo.ClientKey.IsEmpty())
	{
		return ELoginProcess::VE_NeedReTry;
	}*/
	
	return ELoginProcess::VE_OK;
}

void URAuth::InvalidateIdpCredentials()
{
	// HACK: calling async operation as if it's synchronous
#if API_NZIN
	GetPublisherAPI()->CustomLogout();
#endif
	EmptyAccount();
}

void URAuth::SetAccountInfo(FOnlineAccountInfo& InAccountInfo)
{
	AccountInfo = InAccountInfo; 
}

void URAuth::MakeAccount()
{
	AccountInfo.bIsAcceptTerms = true;
	AccountInfo.Type = ConversionPublisherToServer();
}

void URAuth::EmptyAccount()
{
	AccountInfo.bIsAcceptTerms = false;
	AccountInfo.Id.Empty();
	AccountInfo.Type.Empty();
	AccountInfo.NickName.Empty();
	AccountInfo.ClientKey.Empty();
	AccountInfo.Token.Empty();
}

void URAuth::Logout()
{
#if API_NZIN
	GetPublisherAPI()->Logout();
#endif

	EmptyAccount();
	
	SaveAccountLocalRecord();
	SetIsDataState(false);

	FName TargetSystemName = FName(*GetPublisherName());
	IOnlineIdentityPtr OnlineIdentity = Online::GetIdentityInterface(TargetSystemName);
	if (OnlineIdentity.IsValid())
	{
		OnlineIdentity->Logout(0);
	}
}

void URAuth::PreaprePlatform(EPublisher InPublisher)
{
	SetPublisher(InPublisher);
	//return;

	FName TargetSystemName = FName(*GetPublisherName());
	IOnlineIdentityPtr OnlineIdentity = Online::GetIdentityInterface(TargetSystemName);
	if (OnlineIdentity.IsValid())
	{
		bool bResult = OnlineIdentity->AutoLogin(0);
		ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			FText InCaption = FText::FromString(TEXT("Warning"));
			FText InDesc = FText::FromString(TEXT("Platform Login Error"));
			GameMode->ShowExceptionLoginPopup(InCaption, InDesc);
		}
	}

	return;

	//FName TargetSystemName = FName(*GetPublisherName());
	FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>(TEXT("OnlineSubsystem"));
	//OnlineSubsystemModule.RegisterPlatformService(TargetSystemName, );
	IOnlineSubsystem* System = OnlineSubsystemModule.GetOnlineSubsystem(TargetSystemName);
	IOnlineIdentityPtr IdentityPtr = System->GetIdentityInterface();
	IOnlineExternalUIPtr ExternalUIPtr = System->GetExternalUIInterface();
	
	int32 LocalUserNum = 0;
	//auto OnLoginSuccessDelegate = IdentityPtr->AddOnLoginCompleteDelegate_Handle(LocalUserNum,
	//	FOnLoginCompleteDelegate::CreateThreadSafeSP(this, &URAuth::ProcessLoginPlatform));
	//IdentityPtr->AutoLogin(0);
	
	FOnLoginUIClosedDelegate Handle;
	ExternalUIPtr->ShowLoginUI(0, true, false, Handle);
	//FOnLoginCompleteDelegate CompletionDelegate;
	//CompletionDelegate = FOnLoginCompleteDelegate::CreateRaw(this, 
	//	&FOnlineExternalUIFacebook::OnAccessTokenLoginComplete, Delegate);

	

	
	//DECLARE_DELEGATE_TwoParams(FOnLoginUIClosedDelegate, TSharedPtr<const FUniqueNetId> /*UniqueId*/, const int /*ControllerIndex*/);


	//DEFINE_ONLINE_PLAYER_DELEGATE_THREE_PARAM(MAX_LOCAL_PLAYERS, OnLoginComplete, bool, const FUniqueNetId&, const FString&);
}

/*void URAuth::OnAccessTokenLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error, FOnLoginUIClosedDelegate Delegate)
{
	if (bWasSuccessful)
	{

	}

	FName TargetSystemName = FName(*GetPublisherName());
	FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>(TEXT("OnlineSubsystem"));
	IOnlineSubsystem* System = OnlineSubsystemModule.GetOnlineSubsystem(TargetSystemName);
	TSharedPtr<const FUniqueNetId> StrongUserId = UserId.AsShared();
	System->ExecuteNextTick([StrongUserId, LocalUserNum, Delegate]()
	{
		Delegate.ExecuteIfBound(StrongUserId, LocalUserNum);
	});
}*/

void URAuth::ProcessLoginPlatform(int32 ControllerIndex, bool bWasSuccessful,
	const FUniqueNetId& UserId, const FString& ErrorString)
{
	if (bWasSuccessful)
	{
		LoginCompletePlatform();
	}
	else
	{
		//ErrorString
		/*ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			FText InCaption = FText::FromString(TEXT("Warning"));
			FText InDesc = FText::FromString(TEXT("Platform Login Error"));
			GameMode->ShowExceptionLoginPopup(InCaption, InDesc);
		}*/
	}
}

void URAuth::LoginCompletePlatform()
{
	FName TargetSystemName = FName(*GetPublisherName());
	IOnlineIdentityPtr OnlineIdentity = Online::GetIdentityInterface(TargetSystemName);
	if (OnlineIdentity.IsValid())
	{
		//AccountInfo.bIsAcceptTerms = true;
		MakeAccount();

		UObject* pThis = GetInstancePtr();
		ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(pThis)->GetWorld()));
		int32 i = 0;
		for (auto& Element : OnlineIdentity->GetAllUserAccounts())
		{
			if (GameMode)
			{
				AccountInfo.Id = Element->GetUserId()->ToString();
				AccountInfo.Token = Element->GetAccessToken();

				++i;
				FString InDesc = "log";
				InDesc = InDesc.FromInt(i) + "/////" + AccountInfo.Id + "////" + AccountInfo.Token;
				FText InCaption = FText::FromString(TEXT("Warning"));
				GameMode->ShowExceptionLoginPopup(InCaption, FText::FromString(InDesc));
			}
		}

		AccountInfo.Id = OnlineIdentity->GetUniquePlayerId(0)->ToString();
		if (AccountInfo.Id.IsEmpty())
		{
			AccountInfo.Id = PUBLISHERID_DEV;
		}
		//SetUserID(AccountInfo.Id);
		//AccountInfo.Type = ConversionPublisherToServer();
		AccountInfo.Token = OnlineIdentity->GetAuthToken(0);
	}
	IsCheckAccount = true;

	return;


	/*

	FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>(TEXT("OnlineSubsystem"));

	FName TargetSystemName = FName(*GetPublisherName());
	IOnlineSubsystem* OnlineSubsystem = OnlineSubsystemModule.GetOnlineSubsystem(TargetSystemName);
	//IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	if (OnlineIdentity.IsValid())
	{
		//TSharedPtr<FUserOnlineAccount> UserAccount = OnlineIdentity->GetUserAccount(0);
	}

	//RGameInstance->HttpClient->clientkey = 
	//void UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd);
	//APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	

	//FString PublisherName = GetPublisherName();
	AccountInfo.Type = GetPublisherName();
	if (OnlineIdentity.IsValid())
	{
		AccountInfo.Id = OnlineIdentity->GetUniquePlayerId(0)->ToString();
		//SetUserID(AccountInfo.Id);
		AccountInfo.Token = OnlineIdentity->GetAuthToken(0);
	}
	else
	{
		//AccountInfo.NickName = OnlineIdentity->GetPlayerNickname(0);
	}

	
	//Login();

	
	Login(AccountInfo.Type,
		AccountInfo.Id,
		AccountInfo.Token);*/

	/*FString publisher, code, access_token, redirect_uri, id, pwd;
	FOnlineAccountCredentials account;
	int32 PlayerIndex = 0;
	publisher = ;
	code = ;
	access_token = OnlineIdentity->GetAuthToken(PlayerIndex);
	redirect_uri = ;
	id = OnlineIdentity->GetPlayerNickname(PlayerIndex);
	pwd = ;


	//void URAuth::Login(FString publisher, FString client_id, FString access_token)
	//	UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(publisher, TEXT(""), access_token, TEXT(""), client_id, TEXT(""));
	


	UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(publisher, code, access_token, redirect_uri, id, pwd);

	UPacketFunctionLibrary::LOGIN_RQ((int32)ECLIENT_SDK_FLAG::NONE); //google
	*/
}

void URAuth::LoginFailPlatform()
{

}


FString URAuth::ConversionPublisherToServer()
{
	FString PublisherName;
	switch (GetPublisher())
	{
		case EPublisher::Kokomo:	PublisherName = PUBLISHERID_DEV;			break;
		case EPublisher::Facebook:	PublisherName = PUBLISHERID_FACEBOOK;		break;
		case EPublisher::Google:	PublisherName = PUBLISHERID_GOOGLEPLUS;		break;
		case EPublisher::IOS:		PublisherName = PUBLISHERID_IOS;			break;
		case EPublisher::NZIN:		PublisherName = PUBLISHERID_NZIN;			break;
		case EPublisher::Guest:		PublisherName = PUBLISHERID_DEV;			break;
		default: break;
	}
	return PublisherName;
}

FString URAuth::GetPublisherName()
{
	FString PublisherName;
	switch (GetPublisher())
	{
		case EPublisher::Kokomo:	PublisherName = PUBLISHERID_DEV;			break;
		case EPublisher::Facebook:	PublisherName = PUBLISHERID_FACEBOOK;		break;
		case EPublisher::Google:	PublisherName = PUBLISHERID_GOOGLEPLAY;		break;
		case EPublisher::IOS:		PublisherName = PUBLISHERID_IOS;			break;
		case EPublisher::NZIN:		PublisherName = PUBLISHERID_NZIN;			break;
		case EPublisher::Guest:		PublisherName = PUBLISHERID_DEV;			break;
		default: break;
	}
	return PublisherName;
}

void URAuth::SettingPublisher(FString InString)
{
	/*if (InString == PUBLISHERID_DEV)
	{
		TryGuest();
	}
	else if (InString == PUBLISHERID_GOOGLEPLUS)
	{
		TryGoogle();
	}
	else if (InString == PUBLISHERID_IOS)
	{
		TryIOS();
	}
	else if (InString == PUBLISHERID_FACEBOOK)
	{
		TryFacebook();
	}*/
}

void URAuth::SetNickName(FString& InValue)
{
	if (GetPublisher() == EPublisher::Kokomo)
	{
		SetUserID(InValue);
	}
	AccountInfo.NickName = InValue;
}

void URAuth::EmptyNickName()
{
	AccountInfo.NickName.Empty();
}
