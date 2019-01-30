// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Common/State/BaseHeroScrollViewState.h"
#include "HeroManagementScrollViewState.generated.h"

class URScrollItem;
class UUC_BaseHeroScrollView;

/**
 * 
 */
UCLASS()
class PROJECTR_API UHeroManagementScrollViewState : public UBaseHeroScrollViewState
{
	GENERATED_BODY()

public:
	bool SetSelected(URScrollItem* InSelectedItem, UUC_BaseHeroScrollView* InBaseHeroScrollView) override;
	
	
};
