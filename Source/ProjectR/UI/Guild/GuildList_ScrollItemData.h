// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "GuildList_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildList_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGUILD_INFO		GuildInfo;
	bool isWaiting;
	bool isMyGuild;
	bool isInfoPage;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildList_ScrollItemData)
	void SetGuildInfo(UObject* WorldContextObject, UWidgetSwitcher* GuildRankType, UTextBlock* GuildRank, UTextBlock* GuildName, UTextBlock* GuildLevel,
						UTextBlock* MemberCount, UTextBlock* MemberLimit, UWidgetSwitcher* GuildJoinType, UPanelWidget* GuildJoinWait, UTextBlock* GuildDesc, UWidgetSwitcher* GuildActionType, UWidgetSwitcher* backgroundSwitcher);

private:
	UWorld*	OwnedWorld = nullptr;
};