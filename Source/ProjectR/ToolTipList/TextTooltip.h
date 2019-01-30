// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ToolTipList/BaseTooltip.h"
#include "TextTooltip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UTextTooltip : public UBaseTooltip
{
	GENERATED_BODY()


public:
	void Accept() override;
	
	
	
};
