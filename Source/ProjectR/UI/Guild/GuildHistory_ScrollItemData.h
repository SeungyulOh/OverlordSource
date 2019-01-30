// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "GuildHistory_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildHistory_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGUILD_HISTORY		GuildHistory;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildHistory_ScrollItemData)
	void SetGuildHistory(UObject* WorldContextObject, UTextBlock* TextYear, UTextBlock* TextMonth, UTextBlock* TextDay, 
						UTextBlock* TextHour, UTextBlock* TextMinute, UTextBlock* TextSecond, UTextBlock* TextHistory);

};