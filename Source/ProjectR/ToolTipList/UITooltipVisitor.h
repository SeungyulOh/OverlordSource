// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ToolTipList/BaseTooltipVisitor.h"
#include "UITooltipVisitor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUITooltipVisitor : public UBaseTooltipVisitor
{
	GENERATED_BODY()
	
public:
	void TooltipVisit(UTextTooltip* InTextTooltip) override;
	
	
	
};
