// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	FORCEINLINE UUserWidget* GetWidget() { return Widget; }
	
	
};
