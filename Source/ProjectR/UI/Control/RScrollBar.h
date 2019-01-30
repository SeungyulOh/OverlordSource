// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Components/ScrollBar.h"
#include "RScrollBar.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTR_API URScrollBar : public UScrollBar
{
	GENERATED_BODY()

public:
	URScrollBar(const FObjectInitializer& ObjectInitializer);

	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	// Begin UObject interface
	virtual void PostLoad() override;
	// End of UObject interface
};