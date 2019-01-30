// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UCW_SpeechBubble.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUCW_SpeechBubble : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//UFUNCTION(BlueprintImplementableEvent, Category = UCW_SpeechBubble)
	//void OnShow(bool bShow);

	void Show(FName key, float showTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCW_SpeechBubble)
	UTextBlock*				TextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCW_SpeechBubble)
	UOverlay*				Overlay_All = nullptr;

private:
	float					ShowTime;
	bool					bShowUI = false;
};
