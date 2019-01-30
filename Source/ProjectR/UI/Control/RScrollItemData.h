// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ToolTipList/ToolTipDefs.h"
#include "RScrollItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTR_API URScrollItemData : public UObject
{
	GENERATED_BODY()
public:
	URScrollItemData(const class FObjectInitializer &PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URScrollItemData)
	FVector2D				WidgetSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URScrollItemData)
	bool					bSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URScrollItemData)
	bool					bSelectEnable = true;

	UPROPERTY()
	EToolTipEventTypeEnum	ToolTipEventType = EToolTipEventTypeEnum::VE_None;

	UPROPERTY()
	FName					ToolTipID;
};
