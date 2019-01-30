// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "CampaignEpisodeScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCampaignEpisodeScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FEPISODE	EpisodeData;

	FName		ChapterID;
	FName		PrevEpisodeID;
	int32		EpisodeIdx = 0;
	
	void SetEpisodeData(FEPISODE& InData);
	
};
