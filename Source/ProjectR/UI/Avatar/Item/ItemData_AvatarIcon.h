// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"
#include "UI/Control/RScrollItemData.h"
#include "ItemData_AvatarIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UItemData_AvatarIcon : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName					Image_Hero;

	UPROPERTY()
	FName					TableKey;

	bool					IsLock = true;
};
