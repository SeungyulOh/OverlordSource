// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "HeroItemOptions_ScrollItemData.generated.h"

/**
 * used with UC_HeroItemOptions_ScrollItem
 * just ItemUD
 */
UCLASS()
class PROJECTR_API UHeroItemOptions_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	FString ItemUD;
};
