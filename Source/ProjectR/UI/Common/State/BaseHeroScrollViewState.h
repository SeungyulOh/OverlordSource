// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BaseHeroScrollViewState.generated.h"

class URScrollItem;
class UUC_BaseHeroScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseHeroScrollViewState : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool SetSelected(URScrollItem* InSelectedItem, UUC_BaseHeroScrollView* InBaseHeroScrollView);
};
