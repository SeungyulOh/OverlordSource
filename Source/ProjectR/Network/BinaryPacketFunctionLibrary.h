#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PacketDataEnum.h"
#include "BinaryPacketFunctionLibrary.generated.h"

class UHttpRequestClient;

UCLASS()
class PROJECTR_API UBinaryPacketFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static bool DispatchNetMessage(URGameInstance* instance, UHttpRequestClient* ReuqestClient, const FString& Url, const TArray<uint8>& ArData);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void PD_Save_RQ( UObject* Caller, int32 kId, TArray<FName>& herolist, TArray<uint8>& arData);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void PD_Load_RQ(UObject* Caller, int32 kId);	
};