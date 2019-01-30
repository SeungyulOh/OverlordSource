// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RJudgeIcon.generated.h"



/**
 * 
 */
UCLASS(Abstract)
class PROJECTR_API URJudgeIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void BtnEventOnClicked();
	
	void SetInfo(EHudIcon InIconType, float InTime);
	void SetWidgetInfo(FVector2D InPos, FVector2D InSize);
	void SetTargetTexture(UTexture2D* InTexture);

	FORCEINLINE void SetIconType(EHudIcon InIconType) { IconType = InIconType; }
	

	UFUNCTION()
	void ClearTimer();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RJudgeIcon)
	UButton*	Button_Click;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RJudgeIcon)
	UImage*		TargetImage = nullptr;

private:
	UPROPERTY()
	class ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY()
	class URGameInstance*			RGameInstance = nullptr;

	UPROPERTY()
	FTimerHandle CheckTimerHandler;

	UPROPERTY()
	EHudIcon	IconType;

	UPROPERTY()
	EHudIcon	CheckIconType;

	float CheckTime;
};
