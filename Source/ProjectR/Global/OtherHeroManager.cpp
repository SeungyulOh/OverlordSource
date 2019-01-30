// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "OtherHeroManager.h"
#include "GlobalIntegrated.h"


void UOtherHeroManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
}

void UOtherHeroManager::InsertOtherHeroData(int32 UserKID, FEQUIPED_HERO& OtherHero)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		auto OtherHeroListPtr = OtherHeroMap.Find(UserKID);
		if (OtherHeroListPtr)
		{
			OtherHeroListPtr->Emplace(OtherHero);
		}
	}
	else
	{
		TArray<FEQUIPED_HERO> OtherHeroList;
		OtherHeroList.Emplace(OtherHero);
		OtherHeroMap.Emplace(UserKID, OtherHeroList);
	}
}

void UOtherHeroManager::InsertOtherHeroData(int32 UserKID, TArray<FEQUIPED_HERO>& OtherHeroList)
{
	OtherHeroMap.Emplace(UserKID, OtherHeroList);
}

void UOtherHeroManager::DeleteOtherHeroData(int32 UserKID, const FString& HeroUD)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		int32 FindIndex = OtherHeroMap[UserKID].IndexOfByPredicate([&](const FEQUIPED_HERO& HeroData) {
			return (HeroData.heroUD == HeroUD);
		});

		if (INDEX_NONE != FindIndex)
		{
			OtherHeroMap[UserKID].RemoveAt(FindIndex);
		}
	}
}

void UOtherHeroManager::RemoveOtherHeroData(int32 UserKID)
{
	OtherHeroMap.Remove(UserKID);
}

void UOtherHeroManager::RemoveAll()
{
	OtherHeroMap.Empty();
}

bool UOtherHeroManager::GetOtherHeroUDList(int32 UserKID, TArray<FString>& OtherHeroUDList)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		for (FEQUIPED_HERO& DeckHero : OtherHeroMap[UserKID])
		{
			OtherHeroUDList.Emplace(DeckHero.heroUD);
		}

		return true;
	}

	return false;
}

bool UOtherHeroManager::GetOtherHeroList(int32 UserKID, TArray<FEQUIPED_HERO>& OtherHeroList)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		OtherHeroList = *(OtherHeroMap.Find(UserKID));
		return true;
	}

	return false;
}

bool UOtherHeroManager::GetOtherHeroData(int32 UserKID, const FString& HeroUD, FEQUIPED_HERO& OtherHeroData)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		FEQUIPED_HERO* HeroDataPtr = OtherHeroMap[UserKID].FindByPredicate([&](const FEQUIPED_HERO& HeroData) {
			return (HeroData.heroUD == HeroUD);
		});

		if (HeroDataPtr)
		{
			OtherHeroData = *HeroDataPtr;
			return true;
		}
	}

	return false;
}

bool UOtherHeroManager::GetOtherHeroDataFromID(int32 UserKID, const FName& CharacterID, FEQUIPED_HERO& OtherHeroData)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		FEQUIPED_HERO* HeroDataPtr = OtherHeroMap[UserKID].FindByPredicate([&](const FEQUIPED_HERO& HeroData) {
			return (HeroData.heroId == CharacterID);
		});

		if (HeroDataPtr)
		{
			OtherHeroData = *HeroDataPtr;
			return true;
		}
	}

	return false;
}

bool UOtherHeroManager::IsValidOtherHeroData(int32 UserKID)
{
	if (OtherHeroMap.Contains(UserKID))
	{
		return true;
	}

	return false;
}