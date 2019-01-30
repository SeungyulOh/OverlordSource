// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RSpecialIcon.generated.h"

UENUM(BlueprintType)
enum class ESpecialActionStateEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Prepare	 	UMETA(DisplayName = "Physical"),
	VE_Loop 		UMETA(DisplayName = "Fire"),
	VE_Action		UMETA(DisplayName = "Ice"),
};

class AWeaponBase;
class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URSpecialIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void	BtnEventOnClicked();
	
	UFUNCTION()
	void	CheckFunc();
	void	AttachTemp(bool InVisible);
	void	InitLp();
	void	PrepareAction(ESpecialActionStateEnum InActionState, bool InLoop = false, float InPlayRate = 1.0f);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UWidgetSwitcher*	Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UButton*			Button_On;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMeetingIcon)
	UButton*			Button_Off;
	
private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY()
	AWeaponBase*			TempWeapon = nullptr;

	bool	IsToggle = false;
	bool	IsReset = false;
	UPROPERTY()
	FTimerHandle	TimeHandler;
};
