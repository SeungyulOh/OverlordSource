// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "HeroSlotInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FHeroSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FHeroSlotInfo();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	int32					UserKID = KID_NONE;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	int32					PlayerUK = PLAYER_UK_NONE;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	FString					HeroUD;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	int32					HeroLevel = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	int32					HeroGrade = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	int32					HeroExp = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	FName					PortraitTextureKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroSlotInfo)
	FName					HeroIDName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	ECharacterCategoryTypeEnum	CharacterCategoryType = ECharacterCategoryTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	EClassTypeEnum			CharacterClassType = EClassTypeEnum::VE_ClassTypeMax;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
// 	int32					MaxSpawnCount = 1;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
// 	int32					SpawnCount = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	EHeroSpawnStateEnum		SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	AActor*	SpawnedTargetActor = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	TArray< AActor* >	SpawnedMinions;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	ECharacterRoleEnum		CharacterRole = ECharacterRoleEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	ECharacterCrewStateEnum	CharacterCrewState = ECharacterCrewStateEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroSlotInfo)
	float					HeroHP = 0.0f;
	UPROPERTY()
	float					PrevHeroHP = 1.0f;
	UPROPERTY()
	FVector					DeadLocation;
	/*
	Test Data
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HEroSlotInfo)
	TArray<int32>			TestSkillLevelList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HEroSlotInfo)
	TArray<FTestItemInfo>	TestItemList;

//	void Init();

	/*Setter*/
	void SetSpawnedCharacter(AActor* SpawnedTarget);
	void SetSpawnState(EHeroSpawnStateEnum SpawnState);
// 	void SetSpawnCount(int32 InSpawnCount);
	
//	void DespawnCharacter(AActor* SpawnedTarget);
	void DespawnAll();
		
//	bool IsAlreadySpawned();
	

	void Save();
	void Load();
	void Dead();
	/*Getter*/
// 	int32 GetSpawnCount();

	ECharacterCategoryTypeEnum	GetCharacterCategoryType();
	ESpawnResultTypeEnum		CanSpawn();
#define HERO_GRADE_5 5
	bool IsActiveCombineSkill() { return HERO_GRADE_5 <= HeroGrade ; }
	bool IsAlive() { return 0 < HeroHP; }
};