// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Common/State/BaseToggleState.h"
#include "RaidToggleState.generated.h"

class UUC_QuickToggle;
/**
 * 
 */
UCLASS()
class PROJECTR_API URaidToggleState : public UBaseToggleState
{
	GENERATED_BODY()
	
public:
	void OnClick_Toggle(UUC_QuickToggle* OwnerToggle) override;
	void SetActive(bool InbIsActive, UUC_QuickToggle* OwnerToggle) override;
};
