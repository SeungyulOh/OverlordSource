#include "ProjectR.h"
#include "BinaryPacketFunctionLibrary.h"
#include "ClientProtocols.h"
#include "ReceivePacketHandler.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "HttpRequestClient.h"
#include "PacketFunctionLibrary.h"

 
bool UBinaryPacketFunctionLibrary::DispatchNetMessage(URGameInstance* instance, UHttpRequestClient* ReuqestClient, const FString& Url, const TArray<uint8>& ArData)
{
	if (!IsValid(instance))
		return true;

	if (Url.Contains(TEXT("/read/")))
	{
	}
	else if (Url.Contains(TEXT("/save/")))
	{

	}
	else
	{
#ifdef USE_COMPRESS_PACKET
		FString OutJsonString;

		if (IsValid(ReuqestClient))
		{
			ReuqestClient->DecompressPacket(ArData, OutJsonString);

			if (OutJsonString.Len() > 0)
			{
				ReuqestClient->Closed(OutJsonString);
				return false;
			}

		}
#endif // USE_COMPRESS_PACKET
	}

	return true;
}

void UBinaryPacketFunctionLibrary::PD_Save_RQ(UObject* Caller, int32 kId, TArray<FName>& herolist, TArray<uint8>& arData)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	FString addStr = TEXT("save/") + FString::FromInt(kId);
	addStr += TEXT("/0.");

	if (herolist.Num() > 0)
	{
		for (int32 i = 0; i < herolist.Num(); ++i)
		{
			addStr += herolist[i].ToString();
			if (i < (herolist.Num() - 1))
			{
				addStr += TEXT(".");
			}
		}

		addStr += TEXT("/");
	}

	RGameInstance->HttpClient->TransmitRequestBinary(addStr, arData);
}

void UBinaryPacketFunctionLibrary::PD_Load_RQ(UObject* Caller, int32 kId)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	FString addStr = TEXT("read/") + FString::FromInt(kId);
	addStr += TEXT("/0.");

	TArray<uint8> arData;
	arData.Empty();

	RGameInstance->HttpClient->TransmitRequestBinary(addStr, arData);
}

