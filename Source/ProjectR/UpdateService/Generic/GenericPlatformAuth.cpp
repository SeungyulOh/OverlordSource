
#include "ProjectR.h"
#include "GenericPlatformAuth.h"

#define GENERICPLATFORMAUTH_FILENAME TEXT("clientkey.txt")

bool FGenericPlatformAuth::StoreData(const TArray<uint8>& InData)
{
	FString Path = FPaths::ProjectDir() / TEXT("UpdateService") / GENERICPLATFORMAUTH_FILENAME;

	return FFileHelper::SaveArrayToFile(InData, *Path);
}

bool FGenericPlatformAuth::ReadData(TArray<uint8>& OutData)
{
	FString Path = FPaths::ProjectDir() / TEXT("UpdateService") / GENERICPLATFORMAUTH_FILENAME;

	return FFileHelper::LoadFileToArray(OutData, *Path);
}

FString FGenericPlatformAuth::GetPurchaseSystem()
{
	return TEXT("");
}

bool FGenericPlatformAuth::QueryBillingSupport()
{
	return false;
}

void FGenericPlatformAuth::QueryInventory()
{
	// not implemented
	// better assert
}

FOnQueryInventoryFinishedDelegate FGenericPlatformAuth::OnQueryInventoryFinishedDelegate;

void FGenericPlatformAuth::Purchase(FString ProductId, FString Payload)
{
	// not implemented
}

FOnPurchaseFinishedDelegate FGenericPlatformAuth::OnPurchaseFinishedDelegate;

void FGenericPlatformAuth::Consume(FString ProductId, FString Receipt)
{
	// not implemented
}

FOnConsumeFinishedDelegate FGenericPlatformAuth::OnConsumeFinishedDelegate;
