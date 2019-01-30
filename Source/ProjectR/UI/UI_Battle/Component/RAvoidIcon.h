// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RAvoidIcon.generated.h"



class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URAvoidIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void	BtnEventOnPressed();
	UFUNCTION()
	void	BtnEventOnClicked();

	void	SetClickedButton(bool InEnableClick);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAvoidIcon|Ref")
	UButton*			Button_Click;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAvoidIcon|Ref")
	UImage*				Image_CoolGauge;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "RAvoidIcon|Ref")
	UTextBlock*			Text_CoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAvoidIcon|Ref")
	UWidgetSwitcher*	Switcher_IconImage;

private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	bool				IsTouchEnable = true;
	bool				IsCheckTick = false;
};
