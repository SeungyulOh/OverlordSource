
#pragma once

// Platform purchase definitions

// Billing response codes
#define BILLING_RESPONSE_RESULT_OK 0
#define BILLING_RESPONSE_RESULT_USER_CANCELED 1
#define BILLING_RESPONSE_RESULT_SERVICE_UNAVAILABLE 2
#define BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE 3
#define BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE 4

// callback delegate types

// Params: ResponseCode, ProductIds, Receipts
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnQueryInventoryFinished, int32, TArray<FString>, TArray<FString>);
typedef FOnQueryInventoryFinished::FDelegate FOnQueryInventoryFinishedDelegate;

// Params: ResponseCode, Receipt
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPurchaseFinished, int32, FString);
typedef FOnPurchaseFinished::FDelegate FOnPurchaseFinishedDelegate;

// Params: ResponseCode
DECLARE_MULTICAST_DELEGATE_OneParam(FOnConsumeFinished, int32);
typedef FOnConsumeFinished::FDelegate FOnConsumeFinishedDelegate;

class PROJECTR_API FGenericPlatformAuth
{
public:

	// Will store at UpdateService/clientkey.txt
	static bool StoreData(const TArray<uint8>& InData);

	// Call will succeed with Length=0 when StoreData was never called on the installation
	static bool ReadData(TArray<uint8>& OutData);


	////// Platform purchase interface for BillingManager

	static FString GetPurchaseSystem();

	// return false if not supported on the platform
	static bool QueryBillingSupport();

	static void QueryInventory();
	static FOnQueryInventoryFinishedDelegate OnQueryInventoryFinishedDelegate;

	static void Purchase(FString ProductId, FString Payload);
	static FOnPurchaseFinishedDelegate OnPurchaseFinishedDelegate;

	static void Consume(FString ProductId, FString Receipt);
	static FOnConsumeFinishedDelegate OnConsumeFinishedDelegate;
};