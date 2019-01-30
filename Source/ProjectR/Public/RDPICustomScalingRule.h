// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DPICustomScalingRule.h"
#include "RDPICustomScalingRule.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URDPICustomScalingRule : public UDPICustomScalingRule
{
	GENERATED_BODY()
public:
	virtual float GetDPIScaleBasedOnSize(FIntPoint Size) const override;
};
