// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CheckBox.h"
#include "CheckBox_Radio.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegate_RadioGroupoComponentStateChanged, int32, UniqueID, bool, bIsChecked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckBoxRadioCheck, bool, bIsChecked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCheckBoxRadioLockEvent);

// non-dynamic delegate that works same as FCheckBoxRadioCheck, but with payload support
DECLARE_DELEGATE_OneParam(FCheckBoxRadioCheck_StaticDelegate, bool /* bIsChecked */);

/**
 * 
 */
UCLASS()
class PROJECTR_API UCheckBox_Radio : public UCheckBox
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void RadioButtonOnCheckStateChanged(bool bIsChecked);

	UPROPERTY()
	FDelegate_RadioGroupoComponentStateChanged RadioGroupoComponentStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCheckBox_Radio)
	bool			bLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCheckBox_Radio)
	UWidgetSwitcher*		Switcher = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "CheckBox|Event")
	FCheckBoxRadioCheck OnCheckRadioCheck;

	UPROPERTY(BlueprintAssignable, Category = "CheckBox|Event")
	FCheckBoxRadioLockEvent OnCheckRadioLockEvent;

	FCheckBoxRadioCheck_StaticDelegate OnCheckRadioCheck_StaticDelegate;

public:

	// UWidget interface
	void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	// Begin UObject interface
	void PostLoad() override;
	// End of UObject interface
};
