// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "RewardInfo.generated.h"

//class URGameInstance;

/**
 * 
 */
UCLASS()
class PROJECTR_API URewardInfo : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RewardInfo)
	EPLAY_CLEAR_GRADE	ClearGrade = EPLAY_CLEAR_GRADE::CLEAR_FAILED;
	UPROPERTY()
	TArray<FREWARD>				Rewards;
	UPROPERTY()
	TArray<FREWARDSET>			RewardSet;
	UPROPERTY()
	TArray<FHERO>				PlayDeckHeroInfoBeforeGetReward;
	UPROPERTY()
	TArray<FREWARD>				CampaignFirstRewards;
public:

	void Init(/*URGameInstance* InRGameInstance*/);

	void					SetPlayDeckHeroInfoBeforeGetReward(const TArray<FHERO>& InPlayHeroInfo);
	FORCEINLINE const TArray<FHERO>&	GetPlayDeckHeroInfoBeforeGetReward() { return PlayDeckHeroInfoBeforeGetReward; }

	void SetRewardData( TArray<FREWARDSET>& rewards);
	void SetRewardData( TArray<FREWARD>& rewards);
	void SetClearGrade(EPLAY_CLEAR_GRADE grade);

	void SetFirstReward(TArray<FREWARD>& rewards);

	void GetRewardArrayType(EREWARD_TYPE Type, TArray<FREWARD>& OutRewards);
	int32 GetRewardIntType(EREWARD_TYPE Type);
	int32 GetRewardIntType(EREWARD_TYPE Type, TArray<FREWARD>& rewards);

	bool IsRewardIntType(EREWARD_TYPE Type);
	
	EPLAY_CLEAR_GRADE GetClearGrade();

//private:
// 	UPROPERTY()
// 	URGameInstance* RGameInstance;
};