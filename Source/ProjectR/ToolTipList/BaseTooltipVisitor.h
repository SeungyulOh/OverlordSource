// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BaseTooltipVisitor.generated.h"

class UTextTooltip;
/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseTooltipVisitor : public UObject
{
	GENERATED_BODY()

public:
	virtual void TooltipVisit(UTextTooltip* InTextTooltip);
	
};
