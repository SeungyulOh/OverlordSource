// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "RewardInfo.h"


void URewardInfo::Init(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;

	ClearGrade = EPLAY_CLEAR_GRADE::CLEAR_FAILED;
	Rewards.Reset();
	CampaignFirstRewards.Reset();
}

void URewardInfo::SetRewardData( TArray<FREWARDSET>& rewards)
{	
	RewardSet = rewards;
	if (RewardSet.IsValidIndex(0))
		Rewards = RewardSet[0].rewards;
}

void URewardInfo::SetRewardData( TArray<FREWARD>& rewards)
{	
	Rewards = rewards;
}

void URewardInfo::SetFirstReward(TArray<FREWARD>& rewards)
{
	CampaignFirstRewards = rewards;
}

void URewardInfo::SetClearGrade(EPLAY_CLEAR_GRADE grade)
{
	ClearGrade = grade;
}

void URewardInfo::GetRewardArrayType(EREWARD_TYPE Type, TArray<FREWARD>& OutRewards)
{
	OutRewards = Rewards.FilterByPredicate([&](const FREWARD&reward) {
		return (reward.type == Type);
	});
}

bool URewardInfo::IsRewardIntType(EREWARD_TYPE Type)
{
	FREWARD* FindReward = Rewards.FindByPredicate([&](const FREWARD& reward) {
		return (reward.type == Type);
	});

	if (FindReward)
		return true;

	return false;
}

int32 URewardInfo::GetRewardIntType(EREWARD_TYPE Type)
{
	FREWARD* FindReward = Rewards.FindByPredicate([&](const FREWARD& reward) {
		return (reward.type == Type);
	});

	if (FindReward)
	{
		return FindReward->amt;
	}

	return 0;
}

int32 URewardInfo::GetRewardIntType(EREWARD_TYPE Type, TArray<FREWARD>& rewards)
{
	FREWARD* FindReward = rewards.FindByPredicate([&](const FREWARD& reward) {
		return (reward.type == Type);
	});

	if (FindReward)
	{
		return FindReward->amt;
	}

	return 0;
}

EPLAY_CLEAR_GRADE URewardInfo::GetClearGrade()
{
	return ClearGrade;
}

void URewardInfo::SetPlayDeckHeroInfoBeforeGetReward(const TArray<FHERO>& InPlayHeroInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	PlayDeckHeroInfoBeforeGetReward.Empty();
	for (auto HeroInfoElem : InPlayHeroInfo)
	{
		auto HeroInfo = RGameInstance->RInventory->GetHeroWithUD(HeroInfoElem.heroUD);
		if (HeroInfo == nullptr)
			continue;

		PlayDeckHeroInfoBeforeGetReward.Emplace(*HeroInfo);
	}
}