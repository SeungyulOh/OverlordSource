// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "ObjectFunctionLibrary.generated.h"


/**
*
*/
UCLASS()
class PROJECTR_API UObjectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AActor* SpawnCharacter(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level = 1, int32 Grade = 1, bool IsAttachSkill = true, int32 InCommandSlotIndex = INVALID_INDEXNUM, bool Remote = false, bool bSpawnEffectFromCrewChange = false);
	static AActor* SpawnObject(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level = 1, int32 Grade = 1, bool IsAttachSkill = true, int32 InCommandSlotIndex = INVALID_INDEXNUM, bool Remote = false);
	static AActor* SpawnCharacterForArena(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level, int32 Grade, int32 InCommandSlotIndex);
	static AActor* SpawnCharacterWithRTS(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level = 1, int32 Grade = 1, bool IsAttachSkill = true, int32 InCommandSlotIndex = INVALID_INDEXNUM, bool Remote = false, bool bSpawnEffectFromCrewChange = false);

	static void SetSoundAndVibration(AActor* InSpawnedCharacter, ECharacterCategoryTypeEnum InType);
	
	static void EquipItemToObject(AActor* Obj);


	static void SetMyTeamKey(int32 InGrouopKey);
	static int32 GetMyTeamKey();

	static void SetMyPartyID(int32 InPartyID);
	static int32 GetMyPartyID();

	static ESpawnResultTypeEnum	CanSpawnHero(int32 SlotIndex);

	static bool IsAbleModifyActor(AActor* From);
	static void ModifyActor(AActor* From, bool bApply);
	//
private:
	static int32										MyGroupKey;
	static int32										MyPartyID;
};