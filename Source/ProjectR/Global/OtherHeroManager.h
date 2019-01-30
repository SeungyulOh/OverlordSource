// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "OtherHeroManager.generated.h"



//class URGameInstance;


/**
 * Caches opponent's info for PvP play modes
 */
UCLASS()
class PROJECTR_API UOtherHeroManager : public UObject
{
	GENERATED_BODY()
	
public:
	void		Initialize(/*URGameInstance* InRGameInstance*/);

	void		InsertOtherHeroData(int32 UserKID, FEQUIPED_HERO& OtherHero);
	void		InsertOtherHeroData(int32 UserKID, TArray<FEQUIPED_HERO>& OtherHeroList);
	void		DeleteOtherHeroData(int32 UserKID, const FString& HeroUD);
	void		RemoveOtherHeroData(int32 UserKID);
	void		RemoveAll();

	bool		GetOtherHeroUDList(int32 UserKID, TArray<FString>& OtherHeroUDList);
	bool		GetOtherHeroList(int32 UserKID, TArray<FEQUIPED_HERO>& OtherHeroList);
	bool		GetOtherHeroData(int32 UserKID, const FString& HeroUD, FEQUIPED_HERO& OtherHeroData);
	bool		GetOtherHeroDataFromID(int32 UserKID, const FName& CharacterID, FEQUIPED_HERO& OtherHeroData);
	
	bool		IsValidOtherHeroData(int32 UserKID);
private:
	TMap< int32, TArray<FEQUIPED_HERO> >		OtherHeroMap;

// 	UPROPERTY()
// 	URGameInstance* RGameInstance = nullptr;
};
