// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"

#include "Network/PacketDataStructures.h"

#include "UC_Arena_ranking_list.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_ranking_list : public URBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	URScrollView * BP_ScrollView = nullptr;

	UPROPERTY()
	TArray<class URArenaRanking_ScrollItemData*> CachedItemDataList;
	
	TArray<FARENA_RANK_USER> ArenaRank;
	
	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		SetRank(TArray<FARENA_RANK_USER>& InArenaRank);
};