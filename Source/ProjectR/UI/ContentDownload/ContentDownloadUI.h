// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/ContentDownload/UP_Popup.h"
#include "UpdateService/UpdateService.h"
#include "UpdateService/Auth/RAuth.h"
#include "ContentDownloadUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContentDownloadUIFinish);

/**
 * 
 */
UCLASS()
class PROJECTR_API UContentDownloadUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = ContentDownloadUI)
	void Init();

	UFUNCTION(BlueprintCallable, Category = ContentDownloadUI)
	void RefreshUI();

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
	UFUNCTION(BlueprintCallable, Category = ContentDownloadUI)
	UUP_Popup*	ShowPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType);
	UFUNCTION(BlueprintCallable, Category = ContentDownloadUI)
	void	HidePopup();

	UFUNCTION(BlueprintCallable, Category = ContentDownloadUI)
	void UpdateVersionString();
	
	UFUNCTION()
	void OnUpdateServiceFinish(bool IsSuccess, FString Message);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadUI)
	UProgressBar*				UpdateProgressBar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadUI)
	UTextBlock*					UpdateProgressStatusText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadUI)
	UTextBlock*					Text_CurrentSuccessValue;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadUI)
	UTextBlock*					VersionText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ContentDownloadUI)
	UTextBlock*					ExtraVersionInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentDownloadUI)
	TAssetSubclassOf<UUP_Popup> PopupUIClass;

	UPROPERTY(BlueprintAssignable, BlueprintAssignable, Category = ContentDownlaodUI)
	FOnContentDownloadUIFinish OnContentDownloadUIFinish;

private:
	UFUNCTION()
	void OnExit();

	void AskUserWhetherToRetry(FText Message);

private:
	UPROPERTY()
	UUP_Popup* PopupUI = nullptr;
	
	UPROPERTY()
	UUpdateService* UpdateService = nullptr;
	UPROPERTY()
	URAuth* RAuth = nullptr;

	// Display progress
	float PreviousProgress = 0.f;

	float TickDuration = 0.f;

	bool PreLoggedIn = false;

};
