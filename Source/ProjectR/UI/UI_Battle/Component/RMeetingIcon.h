// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RMeetingIcon.generated.h"



class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URMeetingIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void    NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
	void	BtnEventOnClicked();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UButton*			Button_On;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UOverlay*			Cool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UImage*				Image_CoolGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UTextBlock*			Text_CoolTime;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;

	
private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	float CoolTime = 10.f;
	float RemainingTimeToReuse = 0.f;
};
