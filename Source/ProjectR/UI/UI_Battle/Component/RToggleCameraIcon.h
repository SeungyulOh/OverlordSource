// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RToggleCameraIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URToggleCameraIcon : public URBaseWidget
{
	GENERATED_BODY()
	


public:
	void				NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = URToggleCameraIcon)
	void				OnClick_Icon();

	UFUNCTION(BlueprintCallable, Category = URToggleCameraIcon)
	void				SetActiveIconIndex(int32 Index);

	int32				GetCurrentToggleIndex() { return CurrentToggleIndex; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URToggleCameraIcon)
	UWidgetSwitcher*	Switcher_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URToggleCameraIcon)
	int32				ToggleImageCount		= 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URToggleCameraIcon)
	UButton*			Button_CameraViewToggle  = nullptr;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URToggleCameraIcon)
	float				ClickDelayedSecond		= 0.5f;

private:
	void				SetClickDelayTimer();

	UFUNCTION()
	void				CallbackClickDelayed();


private:

	FTimerHandle		TimerHandle_ClickDelayed;
	int32				CurrentToggleIndex		= 0;
	bool				IsClickDelayed			= false;
	
	
};
