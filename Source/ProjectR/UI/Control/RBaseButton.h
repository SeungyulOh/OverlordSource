// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RBaseButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonDelegate_Click);
/**
 * 
 */
UCLASS()
class PROJECTR_API URBaseButton : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = URBaseButton)
	FButtonDelegate_Click		OnButtonClick_Delegate;
};
