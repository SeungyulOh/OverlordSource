// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Popup_Warning_Splash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UP_Popup_Warning_Splash : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_CommonPopupSplash)
		UTextBlock*					TextBlock_Comments = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = UUP_CommonPopupSplash)
		void RefreshUI(FText InDesc);

private:
	bool						bPlayStart = false;
	FString						CurAniName;
};
