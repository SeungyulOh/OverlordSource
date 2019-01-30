// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "RGuildRaidSRScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URGuildRaidSRScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	int32				RewardIndex;
	FITEM_ICON_INFO		ItemIconInfo;
	
	
};
