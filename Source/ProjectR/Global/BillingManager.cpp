// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "BillingManager.h"
#include "Network/PacketFunctionLibrary.h"
#include "UpdateService/Auth/RAuth.h"
#include "UpdateService/PlatformAuth.h"



UBillingManager::UBillingManager()
{

}

void UBillingManager::Initialize()
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER Initialize"));

	// Check if in-app billing is supported on the platform
	if (!FPlatformAuth::QueryBillingSupport())
	{
		UE_LOG(LogBillingManager, Log, TEXT("Initialized when in-app billing is not supported on the platform"));
		return;
	}

	// Register delegates to PlatformAuth
	auto OnQueryInventoryFinishedDelegate = FOnQueryInventoryFinishedDelegate::CreateUObject(this, &UBillingManager::OnPlatformQueryInventoryFinished);
	FPlatformAuth::OnQueryInventoryFinishedDelegate = OnQueryInventoryFinishedDelegate;
	auto OnPurchaseFinishedDelegate = FOnPurchaseFinishedDelegate::CreateUObject(this, &UBillingManager::OnPlatformPurchaseFinished);
	FPlatformAuth::OnPurchaseFinishedDelegate = OnPurchaseFinishedDelegate;
	auto OnConsumeFinishedDelegate = FOnConsumeFinishedDelegate::CreateUObject(this, &UBillingManager::OnPlatformConsumeFinished);
	FPlatformAuth::OnConsumeFinishedDelegate = OnConsumeFinishedDelegate;

	// TODO: Should ProcessOutstandingPurchases here because it's already logged in
	// URAuth::GetInstancePtr()->OnLoginComplete.AddDynamic(this, &UBillingManager::ProcessOutstandingPurchases);

	ProcessedOutstandingPurchases = false;
	ProcessingOutstandingPurchases = false;
	ResetState();

	UE_LOG(LogBillingManager, Log, TEXT("BillingManager initialized"));
}

void UBillingManager::Shutdown()
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER Shutdown"));
	// Remove delegates to PlatformAuth
	FPlatformAuth::OnQueryInventoryFinishedDelegate.Unbind();
	FPlatformAuth::OnPurchaseFinishedDelegate.Unbind();
	FPlatformAuth::OnConsumeFinishedDelegate.Unbind();
}

void UBillingManager::ProcessOutstandingPurchases(ELoginResult LoginResult, FString SavedClientKey)
{
	if (LoginResult != ELoginResult::VE_Ok)
	{
		return;
	}

	// Guard
	if (IsInProgress())
	{
		UE_LOG(LogBillingManager, Log, TEXT("ProcessOutstandingPurchases called while in progress"));
		return;
	}

	// Guard
	if (ProcessedOutstandingPurchases)
	{
		return;
	}
	ProcessedOutstandingPurchases = true;

	ProcessingOutstandingPurchases = true;
	IabAsyncInProgress = true;
	FPlatformAuth::QueryInventory();
}

bool UBillingManager::Purchase(const FString& ProductId)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER Purchase"));
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//checkf(RGameInstance != nullptr, TEXT("GameInstance is Empty"));

	if (ProcessingOutstandingPurchases)
	{
		UE_LOG(LogBillingManager, Log, TEXT("Purchase called while processing outstanding purchases; it may have stuck... therefore proceeding anyway."));
		ProcessingOutstandingPurchases = false;
	}

	if (InProgress || IabAsyncInProgress)
	{
		UE_LOG(LogBillingManager, Log, TEXT("Purchase called while in progress: %d %d"), (int32)InProgress, (int32)IabAsyncInProgress);
		return false;
	}

	InProgress = true;
	CurProductId = ProductId;

	UE_LOG(LogBillingManager, Log, TEXT("BILLING_TRANSACTION_CREATE_RQ for %s, %s"), *FPlatformAuth::GetPurchaseSystem(), *CurProductId);
	UPacketFunctionLibrary::BILLING_TRANSACTION_CREATE_RQ(FPlatformAuth::GetPurchaseSystem(), ProductId, TEXT(""));

	return true;
}

void UBillingManager::OnPlatformQueryInventoryFinished(int32 Response, TArray<FString> ProductIds, TArray<FString> Receipts)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER OnPlatformQueryInventoryFinished with response %d"), Response);
	IabAsyncInProgress = false;

	if (Response != BILLING_RESPONSE_RESULT_OK)
	{
		UE_LOG(LogBillingManager, Log, TEXT("OnPlatformQueryInventoryFinished: error querying inventory %d. Aborting processing outstanding purchases."), Response);
		ProcessingOutstandingPurchases = false;
		return;
	}

	UE_LOG(LogBillingManager, Log, TEXT("OnPlatformQueryInventoryFinished: got result of %d products and %d receipts"), ProductIds.Num(), Receipts.Num());
	ensure(ProductIds.Num() == Receipts.Num());

	RemainingProductIds = ProductIds;
	RemainingReceipts = Receipts;

	ProcessOutstandingPurchasesContinue();
}

void UBillingManager::OnPlatformPurchaseFinished(int32 Response, FString Receipt)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER OnPlatformPurchaseFinished with response %d"), Response);
	// CRITICAL SECTION

	IabAsyncInProgress = false;

	if (Response != BILLING_RESPONSE_RESULT_OK)
	{
		EBillingResultTypeEnum Result = (EBillingResultTypeEnum)FMath::Clamp(Response, BILLING_RESPONSE_RESULT_OK + 1, BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE + 1);
		FinishProcess(Result, Response, CurProductId, FAVATAR());
		return;
	}

	// continue CRITICAL SECTION

	// go straight to consume
	ProcessPurchase(CurProductId, Receipt, CurTransactionId);
}

void UBillingManager::OnPlatformConsumeFinished(int32 Response)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER OnPlatformConsumeFinished for %s"), *CurProductId);
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//checkf(RGameInstance != nullptr, TEXT("GameInstance is Empty"));
	// Branch on ProcessingOutstandingPurchases
	IabAsyncInProgress = false;

	if (Response != BILLING_RESPONSE_RESULT_OK)
	{
		// CRITICAL
		// Error consuming - user is blocked from using the item
		FinishProcess(EBillingResultTypeEnum::CONSUME_ERROR, Response, CurProductId, FAVATAR());
		return;
	}

	UE_LOG(LogBillingManager, Log, TEXT("BILLING_TRANSACTION_SUCCESS_RQ for %s, %s, %s"), *CurProductId, *CurReceipt, *CurTransactionId);
	UPacketFunctionLibrary::BILLING_TRANSACTION_SUCCESS_RQ(FPlatformAuth::GetPurchaseSystem(), CurReceipt, CurProductId, CurTransactionId);
}

void UBillingManager::ResetState()
{
	InProgress = false;
	IabAsyncInProgress = false;
	CurProductId.Empty();
	CurTransactionId.Empty();
	CurReceipt.Empty();
}

FString UBillingManager::ErrorCodeToString(int32 ErrorCode)
{
	return FString(TEXT("[")) + FString::FromInt(ErrorCode) + TEXT("]");
}

bool UBillingManager::IsInProgress()
{
	return ProcessingOutstandingPurchases || InProgress || IabAsyncInProgress;
}

// TODO: This should handle server response better
void UBillingManager::OnBillingTransactionCreateRp(bool IsSuccess, FString ProductId, FString TransactionId, FString Payload)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER OnBillingTransactionCreateRp"));

	if (!IsSuccess)
	{
		UE_LOG(LogBillingManager, Log, TEXT("OnBillingTransactionCreateRp: error received"));

		FinishProcess(EBillingResultTypeEnum::MISC_ERROR, 0, ProductId, FAVATAR());
		return;
	}

	check(!IabAsyncInProgress);
	check(CurProductId == ProductId);
	CurTransactionId = TransactionId; // TransactionId not used

	UE_LOG(LogBillingManager, Log, TEXT("OnBillingTransactionCreateRp: received success, launching purchase flow for %s"), *ProductId);

	IabAsyncInProgress = true;
	FPlatformAuth::Purchase(ProductId, Payload);
}

void UBillingManager::OnBillingTransactionSuccessRp(bool IsSuccess, FAVATAR Avatar)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER OnBillingTransactionSuccessRp with %s"), *CurProductId);
	if (!IsSuccess)
	{
		// CRITICAL
		// Purchase went through but the server returned error
		FinishProcess(EBillingResultTypeEnum::CONSUME_ACK_ERROR, 0, CurProductId, Avatar);
		return;
	}

	// DONE
	UE_LOG(LogBillingManager, Log, TEXT("OnBillingTransactionSuccessRp: DONE purchasing %s"), *CurProductId);
	FinishProcess(EBillingResultTypeEnum::OK, 0, CurProductId, Avatar);
}

FString UBillingManager::BillingResultToString(FBillingResult Result)
{
	FString OutString = TEXT("Billing error");
	switch (Result.ResultType)
	{
	case EBillingResultTypeEnum::OK:
		OutString = TEXT("OK");
		break;
	case EBillingResultTypeEnum::USER_CANCELED:
		OutString = TEXT("User canceled");
		break;
	case EBillingResultTypeEnum::SERVICE_UNAVAILABLE:
		OutString = TEXT("Billing is unavailable on the platform (SERVICE_UNAVAILABLE)");
		break;
	case EBillingResultTypeEnum::BILLING_UNAVAILABLE:
		OutString = TEXT("Billing is unavailable on the platform (BILLING_UNAVAILABLE)");
		break;
	case EBillingResultTypeEnum::ITEM_UNAVAILABLE:
		OutString = TEXT("Item unavailable");
		break;
	case EBillingResultTypeEnum::MISC_ERROR:
		OutString = TEXT("Encountered error while processing purchase") + ErrorCodeToString(Result.ErrorCode);
		break;
	case EBillingResultTypeEnum::CONSUME_ERROR:
		OutString = TEXT("Failed to consume purchased item ") + ErrorCodeToString(Result.ErrorCode);
		break;
	case EBillingResultTypeEnum::CONSUME_ACK_ERROR:
		OutString = TEXT("Failed to dispense purchased item ") + ErrorCodeToString(Result.ErrorCode);
		break;
	}
	return OutString;
}

void UBillingManager::FinishProcess(EBillingResultTypeEnum Result, int32 ErrorCode, FString ProductId, FAVATAR Avatar)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER FinishProcess with result %d, errorcode %d, for %s"), (uint8)Result, ErrorCode, *ProductId);

	if (!ProcessingOutstandingPurchases)
	{
		FBillingResult BillingResult;
		BillingResult.ResultType = Result;
		BillingResult.ErrorCode = ErrorCode;

		OnPurchaseFinished.Broadcast(BillingResult, ProductId, Avatar);

		ResetState();
	}
	else
	{
		UE_LOG(LogBillingManager, Log, TEXT("FinishProcess: while processing outstanding purchases"));
		ResetState();
		ProcessOutstandingPurchasesContinue();
	}

}

void UBillingManager::ProcessPurchase(FString ProductId, FString Receipt, FString TransactionId)
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER ProcessPurchase with %s"), *ProductId);
	CurProductId = ProductId;
	CurReceipt = Receipt;
	CurTransactionId = TransactionId;

	IabAsyncInProgress = true;
	FPlatformAuth::Consume(ProductId, Receipt);
}

void UBillingManager::ProcessOutstandingPurchasesContinue()
{
	UE_LOG(LogBillingManager, Log, TEXT("ENTER ProcessOutstandingPurchasesContinue with %d remaining"), RemainingProductIds.Num());
	if (RemainingProductIds.Num() == 0)
	{
		// DONE Processing outstanding purchases
		UE_LOG(LogBillingManager, Log, TEXT("DONE Processing outstanding purchases"));
		ProcessingOutstandingPurchases = false;
		return;
	}

	FString ProductId = RemainingProductIds.Pop();
	FString Receipt = RemainingReceipts.Pop();
	ProcessPurchase(ProductId, Receipt, TEXT(""));
}
