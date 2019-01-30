// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RFocusAttackIcon.generated.h"



class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URFocusAttackIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	enum    ePSType{LEADER , CREW1 , CREW2};

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;


	UFUNCTION()
	void	BtnEventOnClicked();

	void	SetClickedButton(bool InEnableClick);

	bool	FocusAttack();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RFocusAttackIcon)
	UWidgetSwitcher*	Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RFocusAttackIcon)
	UButton*			Button_Click;

	UPROPERTY()
	TMap<TWeakObjectPtr<AActor> , class ARTargettingIndicator*> ActivePSCMap;



private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	bool					IsTouchEnable = true;
};
