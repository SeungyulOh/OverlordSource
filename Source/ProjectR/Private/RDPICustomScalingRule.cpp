// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RDPICustomScalingRule.h"

float URDPICustomScalingRule::GetDPIScaleBasedOnSize(FIntPoint Size) const
{
//	UE_LOG(LogGamePlayMode, Log, TEXT("GetDPIScaleBasedOnSize(%d,%d)"), Size.X, Size.Y);
	return Super::GetDPIScaleBasedOnSize(Size);
}
