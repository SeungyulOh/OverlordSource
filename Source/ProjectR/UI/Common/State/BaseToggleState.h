// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

#include "BaseToggleState.generated.h"

class UUC_QuickToggle;
/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseToggleState : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnClick_Toggle(UUC_QuickToggle* OwnerToggle);
	virtual void SetActive(bool InbIsActive, UUC_QuickToggle* OwnerToggle);
};
