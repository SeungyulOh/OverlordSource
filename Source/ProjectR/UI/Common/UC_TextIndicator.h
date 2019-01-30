// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_TextIndicator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_TextIndicator : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowIndicatorString(FText& InText, float InDuration = 0.0f);
	void ShowIndicatorStringNoAni(FText& InText, float InDuration = 0.0f);
	void ShowIndicatorStringBottom(FText& InText, float InDuration = 0.0f);

	void SetIndicatorOffset(float InOffsetX, float InOffsetY);
	bool IsShowIndicator();

	UFUNCTION()
	void HideWidget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ref")
	UWidgetSwitcher*		TextWidgetSwitcher = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ref")
	UTextBlock*				IndicatorText = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ref")
	URRichTextBlock*		RichIndicatorText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ani")
	UWidgetAnimation*		DisplayAni = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ani")
	UBorder*				NoAniBorder = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ref")
	UOverlay*				Lock_Message = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_TextIndicator|Ref")
	ULocalizingTextBlock*	LocalizingTextBlock_0 = nullptr;
private:
	bool	bPlaying = false;
	float	fDuration = 0.0f;
};
