// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/HorizontalBox.h"
#include "HorizontalBox_RadioGroup.generated.h"


/**
 * UCanvasPanel_RadioGroup but for UHorizontalBox
 */
UCLASS()
class PROJECTR_API UHorizontalBox_RadioGroup : public UHorizontalBox
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCanvasPanel_RadioGroup)
	bool			bWidgetSwitcher = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCanvasPanel_RadioGroup)
	bool			bSwitcherInverse = false;

	UFUNCTION()
	void RadioGroupoComponentStateChanged(int32 UniqueID, bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = UCanvasPanel_RadioGroup)
	void SetActiveRadioIndex(int32 InIndex, bool bEvent = false);

	int32 GetActiveRadioIndex() { return CacheIndex; }

public:
	/*
	override
	*/
	// UWidget interface
	void SynchronizeProperties() override;
	// End of UWidget interface
	
private:
	bool			IsIgnoreTouch = false;
	int32			CacheIndex = 0;
};
