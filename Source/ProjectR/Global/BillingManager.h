// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/ClientProtocols.h"
#include "UpdateService/Auth/RAuth.h"
#include "BillingManager.generated.h"


// responses
UENUM(BlueprintType)
enum class EBillingResultTypeEnum : uint8
{
	OK = 0,
	USER_CANCELED,
	SERVICE_UNAVAILABLE, // version of platform billing api is too low/not supported
	BILLING_UNAVAILABLE, // the platform does not support billing
	ITEM_UNAVAILABLE, // inconsistency in shop setup
	MISC_ERROR, // misc error

	// Serious errors
	// the user might need Customer Support to resolve the error
	CONSUME_ERROR, // failed to consume purchased item
	CONSUME_ACK_ERROR // server failed to acknowledge the consume
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBillingResult
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BillingManager)
	EBillingResultTypeEnum ResultType;

	UPROPERTY(EditAnyWHere, BlueprintReadWrite, Category = BillingManager)
	int32 ErrorCode;

};


// Params: IsSuccess, ProductId, FAVATAR
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBillingManagerOnPurchaseFinished, FBillingResult, BillingResult, FString, ProductId, FAVATAR, Avatar);

//class URGameInstance;

/**
Depends on PlatformAuth, RAuth, and HttpRequestClient

Do not instantiate if the platform does not support in-app billing
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UBillingManager : public UObject
{
	GENERATED_BODY()

public:
	UBillingManager();

	UFUNCTION(BlueprintCallable, Category = BillingManager)
	void Initialize(/*URGameInstance* InRGameInstance*/);

	UFUNCTION(BlueprintCallable, Category = BillingManager)
	void Shutdown();

	////// The Money

	// Should be called right after login
	// Delegate to RAuth.OnLoginComplete
	UFUNCTION(BlueprintCallable, Category = BillingManager)
	void ProcessOutstandingPurchases(ELoginResult LoginResult, FString SavedClientKey);

	// Return value of false means it is called while in progress of another purchase or
	// that processing outstanding purchases is not finished
	UFUNCTION(BlueprintCallable, Category = BillingManager)
	bool Purchase(const FString& ProductId);

	UPROPERTY(BlueprintAssignable, Category = BillingManager)
	FBillingManagerOnPurchaseFinished OnPurchaseFinished;

	UFUNCTION(BlueprintCallable, Category = BillingManager)
	bool IsInProgress();

	// Packet handler callbacks
	void OnBillingTransactionCreateRp(bool IsSuccess, FString ProductId, FString TransactionId, FString Payload);
	void OnBillingTransactionSuccessRp(bool IsSuccess, FAVATAR Avatar);

	// for testing
	UFUNCTION(BlueprintPure, Category = BillingManager)
	static FString BillingResultToString(FBillingResult Result);

private:
	// Finishes a Purchase process
	void FinishProcess(EBillingResultTypeEnum Result, int32 ErrorCode, FString ProductId, FAVATAR Avatar);

	// Purchase flow state
	bool InProgress;
	bool IabAsyncInProgress;
	FString CurProductId;
	FString CurTransactionId;
	FString CurReceipt;
	void ResetState();

	// consume and send receipt to the server
	void ProcessPurchase(FString ProductId, FString Receipt, FString TransactionId);

	// delegate functions to PlatformAuth
	void OnPlatformPurchaseFinished(int32 Response, FString Receipt);
	void OnPlatformQueryInventoryFinished(int32 Response, TArray<FString> ProductIds, TArray<FString> Receipts);
	void OnPlatformConsumeFinished(int32 Response);

	// ProcessOutstandingPurchases state
	bool ProcessingOutstandingPurchases;
	bool ProcessedOutstandingPurchases;
	TArray<FString> RemainingProductIds;
	TArray<FString> RemainingReceipts;

	void ProcessOutstandingPurchasesContinue();

	static FString ErrorCodeToString(int32 ErrorCode);

// 	UPROPERTY()
// 	URGameInstance* RGameInstance = nullptr;
};
