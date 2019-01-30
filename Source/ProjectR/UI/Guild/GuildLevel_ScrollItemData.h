// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "GuildLevel_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildLevel_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 ShowLevelUpValue = 1;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildLevel_ScrollItemData)
	void SetGuildLevel(UObject* WorldContextObject, UImage* GuildLevelIcon, UTextBlock* Text_GuildLevelDesc, UWidgetSwitcher* Switcher_GuildLevel, UWidgetSwitcher* Switcher_GuildLevel2);
};