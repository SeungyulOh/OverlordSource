// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RSearchTargetIcon.generated.h"



class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URSearchTargetIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;


	UFUNCTION()
	void	BtnEventOnClicked();

	void	SetClickedButton(bool InEnableClick);
	AActor*	ChangeTarget();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RSearchTargetIcon)
	//UWidgetSwitcher*	Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RSearchTargetIcon)
	UButton*			Button_Click;

	UPROPERTY()
	TArray< TWeakObjectPtr<AActor>>			TargetAlreadySelectedList;

private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class ARTargettingIndicator> TargettingIndicator;

	bool					IsTouchEnable = true;
	
};
