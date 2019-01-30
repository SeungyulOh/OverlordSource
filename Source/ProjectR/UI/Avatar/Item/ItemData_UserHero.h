// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"
#include "UI/Control/RScrollItemData.h"
#include "ItemData_UserHero.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UItemData_UserHero : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName	Image_Hero;

	UPROPERTY()
	FName	Text_UserName;

	int32	UseValue = 1;
	int32	UseMax = 1;
};
