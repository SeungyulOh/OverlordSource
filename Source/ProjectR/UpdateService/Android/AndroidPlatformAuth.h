
#pragma once

#include "ProjectR.h"
#include "UpdateService/Generic/GenericPlatformAuth.h"
#include <jni.h>
#include "Android/AndroidJNI.h"


// random name for clientkey store
#define ANDROIDPLATFORMAUTH_FILENAME TEXT("res.conf")


////// BEGIN In-app billing API response codes

#define BILLING_RESPONSE_RESULT_DEVELOPER_ERROR 5
#define BILLING_RESPONSE_RESULT_ERROR 6
#define BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED 7
#define BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED 8

// IAB Helper error codes
#define IABHELPER_ERROR_BASE (-1000)
#define IABHELPER_REMOTE_EXCEPTION (-1001)
#define IABHELPER_BAD_RESPONSE (-1002)
#define IABHELPER_VERIFICATION_FAILED (-1003)
#define IABHELPER_SEND_INTENT_FAILED (-1004)
#define IABHELPER_USER_CANCELLED (-1005)
#define IABHELPER_UNKNOWN_PURCHASE_RESPONSE (-1006)
#define IABHELPER_MISSING_TOKEN (-1007)
#define IABHELPER_UNKNOWN_ERROR (-1008)
#define IABHELPER_SUBSCRIPTIONS_NOT_AVAILABLE (-1009)
#define IABHELPER_INVALID_CONSUMPTION (-1010)
#define IABHELPER_SUBSCRIPTION_UPDATE_NOT_AVAILABLE (-1011)

// My definitions
#define IABHELPER_ASYNC_OPERATION_IN_PROGRESS (-1100)

////// END

class PROJECTR_API FAndroidPlatformAuth
{
public:
	static bool StoreData(const TArray<uint8>& InData);

	// Call will succeed with Length=0 when StoreData was never called on the installation
	// Will only read 4096 bytes because I lazy
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


private:

	// JNI Callers definitions
	static void InitializeJavaMethods();
	static bool bJavaMethodsInitialized;
	static jmethodID AndroidThunkJava_IabQueryBillingSupport;
	static jmethodID AndroidThunkJava_IabQueryInventory;
	static jmethodID AndroidThunkJava_IabPurchase;
	static jmethodID AndroidThunkJava_IabConsume;

	static bool AndroidThunkCpp_IabQueryBillingSupport();
	static void AndroidThunkCpp_IabQueryInventory();
	static void AndroidThunkCpp_IabPurchase(FString ProductID, FString Payload);
	static void AndroidThunkCpp_IabConsume(FString ProductID, FString Receipt);

	// JNI Callees definitions

};
