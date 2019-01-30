// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_CommonPopupSplash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_CommonPopupSplash : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UTextBlock*					TextBlock_Contents = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UTextBlock*					TextBlock_Desc = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UTextBlock*					TextBlock_HeroTitle = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UTextBlock*					TextBlock_Nick = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UTextBlock*					TextBlock_Name = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	UImage*						Image_Hero = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	ECommonSplashType			CurSplashType = ECommonSplashType::VE_Normal;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
	ULocalizingTextBlock*		Levelup_Text = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = UUP_CommonPopupSplash)
	void RefreshUI(ECommonSplashType InType, FText InCaption, FText InDesc, FName InId = TEXT(""), bool bLevelUpTextShow = false);

private:
	bool						bPlayStart = false;
	FString						CurAniName;
};
