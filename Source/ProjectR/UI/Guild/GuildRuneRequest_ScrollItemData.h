// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "Network/PacketDataStructures.h"
#include "GuildRuneRequest_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildRuneRequest_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGUILD_RUNE_REQUESTER		RuneRequester;
};