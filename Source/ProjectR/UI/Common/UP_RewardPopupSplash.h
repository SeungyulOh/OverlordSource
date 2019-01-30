// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_RewardPopupSplash.generated.h"

class UUC_Item_Icons;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_RewardPopupSplash : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_RewardPopupSplash)
	UTextBlock*					ItemText = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_RewardPopupSplash)
	UTextBlock*					OwnerText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RewardPopupSplash)
	UUC_Item_Icons*				RewardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RewardPopupSplash)
	UWidgetAnimation*			SplashAnim = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = UUP_RewardPopupSplash)
	void RefreshUI(ERewardSplashType InType, FText& InOwnerName, FText& InItemName, TArray<FREWARD>& InRewardData);

private:
	bool						bPlayStart = false;
	ERewardSplashType			SplashType;
};
