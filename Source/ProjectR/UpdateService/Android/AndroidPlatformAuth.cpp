
#include "ProjectR.h"
#include <stdio.h>
#include <android_native_app_glue.h>
#include <android/native_activity.h>
#include "AndroidPlatformAuth.h"
#include "AndroidApplication.h"
#include "TaskGraphInterfaces.h"

#define DATAFILE_NAME TEXT("androidplatformauth_datafile")

extern struct android_app* GNativeAndroidApp;

// Access android app private storage at /data/data/<package>/files
// http://stackoverflow.com/questions/11294487/android-writing-saving-files-from-native-code-only

bool FAndroidPlatformAuth::StoreData(const TArray<uint8>& InData)
{
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ENTER FAndroidPlatformAuth::StoreData"));

	ANativeActivity* nativeActivity = GNativeAndroidApp->activity;
	const char* internalPath = nativeActivity->internalDataPath;

	FString DataDirPath = UTF8_TO_TCHAR(internalPath);
	FString FilePath = DataDirPath + TEXT("/") + DATAFILE_NAME;

	bool Result = true;


	// sometimes if this is the first time we run the app 
	// then we need to create the internal storage "files" directory
	struct stat sb;
	int32_t res = stat(TCHAR_TO_UTF8(*DataDirPath), &sb);
	if (0 == res && sb.st_mode & S_IFDIR)
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("'StoreData: files/' dir already in app's internal data storage."));
	}
	else if (ENOENT == errno)
	{
		res = mkdir(TCHAR_TO_UTF8(*DataDirPath), 0770);
	}

	if (0 == res)
	{
		// test to see if the config file is already present
		res = stat(TCHAR_TO_UTF8(*FilePath), &sb);
		if (0 == res && sb.st_mode & S_IFREG)
		{
			UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: File already present. Overwriting."));
		}

		FILE* appConfigFile = fopen(TCHAR_TO_UTF8(*FilePath), "w+");
		if (NULL == appConfigFile)
		{
			UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: fopen failed"));
			Result = false;
		}
		else
		{
			UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: fopen success"));
			res = fwrite((const char *)InData.GetData(), sizeof(char), InData.Num(), appConfigFile);
			if (InData.Num() != res)
			{
				UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: fwrite failed"));
				Result = false;
			}
			else
			{
				UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: fwrite success writing %d bytes"), res);
			}
			fclose(appConfigFile);
		}
	}
	else
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("StoreData: files/ directory not available."));
		Result = false;
	}

	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("EXIT FAndroidPlatformAuth::StoreData returning %s"), Result ? TEXT("true") : TEXT("false"));

	return Result;
}

bool FAndroidPlatformAuth::ReadData(TArray<uint8>& OutData)
{
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ENTER FAndroidPlatformAuth::ReadData"));

	ANativeActivity* nativeActivity = GNativeAndroidApp->activity;
	const char* internalPath = nativeActivity->internalDataPath;

	FString DataDirPath = UTF8_TO_TCHAR(internalPath);
	FString FilePath = DataDirPath + TEXT("/") + DATAFILE_NAME;

	bool Result = true;

	OutData.Empty();

	// test to see if the config file is already present
	struct stat sb;
	int32_t res = stat(TCHAR_TO_UTF8(*FilePath), &sb);
	if (0 == res && sb.st_mode & S_IFREG)
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: File present, reading"));

		FILE* appConfigFile = fopen(TCHAR_TO_UTF8(*FilePath), "r");
		if (NULL == appConfigFile)
		{
			UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: fopen failed"));
			Result = false;
		}
		else
		{
			UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: fopen success"));

			// 4096?
			OutData.AddUninitialized(4096);

			res = fread((char *)OutData.GetData(), sizeof(char), OutData.Num(), appConfigFile);
			if (!feof(appConfigFile))
			{
				UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: fread failed, returned %d"), res);
				OutData.Empty();
				Result = false;
			}
			else
			{
				UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: fread success, read %d"), res);
				OutData.SetNum(res);
			}
			fclose(appConfigFile);
		}
	}
	else
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ReadData: File not present. Not reading"));
	}

	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("EXIT FAndroidPlatformAuth::ReadData returning %s"), Result ? TEXT("true") : TEXT("false"));

	return Result;
}

FString FAndroidPlatformAuth::GetPurchaseSystem()
{
	return TEXT("google");
}

bool FAndroidPlatformAuth::QueryBillingSupport()
{
	// Disabled now
	//return false;

	InitializeJavaMethods();
	return AndroidThunkCpp_IabQueryBillingSupport();
}

void FAndroidPlatformAuth::QueryInventory()
{
	InitializeJavaMethods();
	AndroidThunkCpp_IabQueryInventory();
}

FOnQueryInventoryFinishedDelegate FAndroidPlatformAuth::OnQueryInventoryFinishedDelegate;

void FAndroidPlatformAuth::Purchase(FString ProductId, FString Payload)
{
	InitializeJavaMethods();
	AndroidThunkCpp_IabPurchase(ProductId, Payload);
}

FOnPurchaseFinishedDelegate FAndroidPlatformAuth::OnPurchaseFinishedDelegate;

void FAndroidPlatformAuth::Consume(FString ProductId, FString Receipt)
{
	InitializeJavaMethods();
	AndroidThunkCpp_IabConsume(ProductId, Receipt);
}

FOnConsumeFinishedDelegate FAndroidPlatformAuth::OnConsumeFinishedDelegate;


void FAndroidPlatformAuth::InitializeJavaMethods()
{
	if (!bJavaMethodsInitialized)
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		// Billing is not optional
		bool bIsOptional = false;

		// I am not sure if this is effective at all outside JNI_OnLoad
		jclass GameActivityClassID = FJavaWrapper::GameActivityClassID;
		AndroidThunkJava_IabQueryBillingSupport = FJavaWrapper::FindMethod(Env, GameActivityClassID, "AndroidThunkJava_IabQueryBillingSupport", "()Z", bIsOptional);
		AndroidThunkJava_IabQueryInventory = FJavaWrapper::FindMethod(Env, GameActivityClassID, "AndroidThunkJava_IabQueryInventory", "()V", bIsOptional);
		AndroidThunkJava_IabPurchase = FJavaWrapper::FindMethod(Env, GameActivityClassID, "AndroidThunkJava_IabPurchase", "(Ljava/lang/String;Ljava/lang/String;)V", bIsOptional);
		AndroidThunkJava_IabConsume = FJavaWrapper::FindMethod(Env, GameActivityClassID, "AndroidThunkJava_IabConsume", "(Ljava/lang/String;Ljava/lang/String;)V", bIsOptional);

		bJavaMethodsInitialized = true;

		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("DONE InitializeJavaMethods"));
	}
}

// JNI callers
bool FAndroidPlatformAuth::bJavaMethodsInitialized = false;

jmethodID FAndroidPlatformAuth::AndroidThunkJava_IabQueryBillingSupport;
jmethodID FAndroidPlatformAuth::AndroidThunkJava_IabQueryInventory;
jmethodID FAndroidPlatformAuth::AndroidThunkJava_IabPurchase;
jmethodID FAndroidPlatformAuth::AndroidThunkJava_IabConsume;

bool FAndroidPlatformAuth::AndroidThunkCpp_IabQueryBillingSupport() {
	bool bIsBillingSupported = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
		bIsBillingSupported = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FAndroidPlatformAuth::AndroidThunkJava_IabQueryBillingSupport);
	}
	else
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("GetJavaEnv returned null"));
	}
	return bIsBillingSupported;
}

void FAndroidPlatformAuth::AndroidThunkCpp_IabQueryInventory() {

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_IabQueryInventory);
	}
	else
	{
		// TODO Handle
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("GetJavaEnv returned null"));
	}
}

void FAndroidPlatformAuth::AndroidThunkCpp_IabPurchase(FString ProductID, FString Payload) {

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ProductIDJava = Env->NewStringUTF(TCHAR_TO_UTF8(*ProductID));
		jstring PayloadJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Payload));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_IabPurchase, ProductIDJava, PayloadJava);
		Env->DeleteLocalRef(ProductIDJava);
		Env->DeleteLocalRef(PayloadJava);
	}
	else
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("GetJavaEnv returned null"));
	}
}

void FAndroidPlatformAuth::AndroidThunkCpp_IabConsume(FString ProductID, FString Receipt)
{

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ProductIDJava = Env->NewStringUTF(TCHAR_TO_UTF8(*ProductID));
		jstring ReceiptJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Receipt));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_IabConsume, ProductIDJava, ReceiptJava);
		Env->DeleteLocalRef(ProductIDJava);
		Env->DeleteLocalRef(ReceiptJava);
	}
	else
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("GetJavaEnv returned null"));
	}
}


// JNI callees
extern "C" void Java_com_epicgames_ue4_GameActivity_nativeQueryInventoryCallback(JNIEnv* jenv, jobject thiz, jsize response, jobjectArray productIDs, jobjectArray receipts) {
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ENTER Java_com_KOKOMO_Overlord_GameActivity_nativeQueryInventoryCallback"));

	// See Java_com_epicgames_ue4_GooglePlayStoreHelper_nativeQueryComplete

	bool bSuccess = true;
	int32 IabResponseCode = response;
	TArray<FString> ProductIDsCpp;
	TArray<FString> ReceiptsCpp;

	if (jenv == nullptr)
	{
		UE_LOG(LogAndroidPlatformAuth, Log, TEXT("nativeQueryInventoryCallback: jenv is null"));
		bSuccess = false;
	}

	// marshal response code
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("nativeQueryInventoryCallback: response = %d"), response);

	if (IabResponseCode != BILLING_RESPONSE_RESULT_OK)
	{
		bSuccess = false;
	}

	if (bSuccess) {
		jsize NumProductIDs = jenv->GetArrayLength(productIDs);
		jsize NumReceipts = jenv->GetArrayLength(receipts);

		ensure(NumProductIDs == NumReceipts);

		for (jsize Idx = 0; Idx < NumProductIDs; Idx++)
		{
			// marshal productIDs to ProductIDsCpp
			jstring NextProductID = (jstring)jenv->GetObjectArrayElement(productIDs, Idx);
			const char* charsProductID = jenv->GetStringUTFChars(NextProductID, 0);
			ProductIDsCpp.Add(FString(UTF8_TO_TCHAR(charsProductID)));
			jenv->ReleaseStringUTFChars(NextProductID, charsProductID);
			jenv->DeleteLocalRef(NextProductID);

			// marshal receipts to ReceiptsCpp
			jstring NextReceipt = (jstring)jenv->GetObjectArrayElement(receipts, Idx);
			const char* charsReceipt = jenv->GetStringUTFChars(NextReceipt, 0);
			ReceiptsCpp.Add(FString(UTF8_TO_TCHAR(charsReceipt)));
			jenv->ReleaseStringUTFChars(NextReceipt, charsReceipt);
			jenv->DeleteLocalRef(NextReceipt);

			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("\nAdded Product Identifier: %s"), *ProductIDsCpp[Idx]);
		}
	}

	// inject to game thread

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {
			FAndroidPlatformAuth::OnQueryInventoryFinishedDelegate.ExecuteIfBound(IabResponseCode, ProductIDsCpp, ReceiptsCpp);
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("In-App inventory query was completed  %s\n"), bSuccess ? TEXT("successfully") : TEXT("unsuccessfully"));
		}),
		TStatId(), // what this?
		nullptr,
		ENamedThreads::GameThread
	);
	// FAndroidPlatformAuth::OnQueryInventoryFinishedDelegate (int32, TArray<FString>, TArray<FString>)


}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativePurchaseCallback(JNIEnv* jenv, jobject thiz, jsize response, jstring receipt) {
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ENTER Java_com_KOKOMO_Overlord_GameActivity_nativePurchaseCallback"));

	int32 IabResponseCode = response;

	bool bSuccess = true;
	FString ReceiptCpp;

	if (IabResponseCode != BILLING_RESPONSE_RESULT_OK)
	{
		bSuccess = false;
	}

	if (bSuccess)
	{
		// Marshal receipt
		const char* charsReceipt = jenv->GetStringUTFChars(receipt, 0);
		ReceiptCpp = FString(UTF8_TO_TCHAR(charsReceipt));
		jenv->ReleaseStringUTFChars(receipt, charsReceipt);
	}

	// inject to game thread
	// FOnPurchaseFinishedDelegate(Response, Receipt)
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {
			FAndroidPlatformAuth::OnPurchaseFinishedDelegate.ExecuteIfBound(IabResponseCode, ReceiptCpp);
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("In-App Purchase request was completed  %s\n"), bSuccess ? TEXT("successfully") : TEXT("unsuccessfully"));
		}),
		TStatId(), // what this?
		nullptr,
		ENamedThreads::GameThread
	);
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeConsumeCallback(JNIEnv* jenv, jobject thiz, jsize response) {
	UE_LOG(LogAndroidPlatformAuth, Log, TEXT("ENTER Java_com_KOKOMO_Overlord_GameActivity_nativeConsumeCallback"));

	int32 IabResponseCode = response;

	bool bSuccess = true;

	if (IabResponseCode != BILLING_RESPONSE_RESULT_OK)
	{
		bSuccess = false;
	}

	// inject to game thread
	// FOnPurchaseFinishedDelegate(Response, Receipt)
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]() {
			FAndroidPlatformAuth::OnConsumeFinishedDelegate.ExecuteIfBound(IabResponseCode);
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("In-App Consume request was completed  %s\n"), bSuccess ? TEXT("successfully") : TEXT("unsuccessfully"));
		}),
		TStatId(), // what this?
		nullptr,
		ENamedThreads::GameThread
	);
}