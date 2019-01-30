// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "PlayStateInfo.generated.h"

//class URGameInstance;


// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnCharacterSpawnDelay, float, InDuration);

#define INVALID_CREW_INDEX INVALID_INDEXNUM

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UPlayStateInfo : public UObject
{
	GENERATED_BODY()

public:
	UPlayStateInfo();

	/*Function*/

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	ESpawnResultTypeEnum	CanSpawnHero(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					Init(/*URGameInstance* RGameInstance*/);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					ReadyToPlay();
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	TArray<FHeroSlotInfo>&	GetSelectedCharacterArray();
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	FHeroSlotInfo&			GetSelectedCharacterSlot(int32 SlotIndex);	

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	EHeroSpawnStateEnum		GetSpawnState(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					SetSpawnState(int32 SlotIndex, EHeroSpawnStateEnum SpawnState );

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					SetSpawnedCharacter(int32 SlotIndex, AActor* SpawnedCharacter);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					SetDeSpawnedCharacter(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	AActor*					GetOwnerCharacter(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void					DespawnAll();

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	bool					IsLeaderDead();

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	int32					GetHeroDieCount();

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	int32					GetHeroSpawnCount();
	
	int32					GetSelectedCharacterCount();

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	int32					GetHeroRemainSpawnCount();

	/*
	User input operations
	*/
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	bool					SelectSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	bool					IsPlayHero(AActor* character);
	
	void ActivateSlotSpawnedHeroRVO(bool bIsActive);

	/*
	TestPlay
	*/
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo) 
	void InitCharacterSlotForTestPlay(TArray<FName> CommandTestInfo, TArray<uint8> selectArr, TArray<int32> InHeroLevels, TArray<int32> InHeroGrades);

	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void AddCharacterSlotForTestPlay(FName characterid, int32 level, TArray<int32>& InSkillLevelList, TArray<FTestItemInfo>& InItemList , ECharacterRoleEnum role = ECharacterRoleEnum::VE_Leader);
	
	void AddSelectedCharacter(FHeroSlotInfo* HeroSlot, bool InsertFront = false);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void ClearSelectedCharacter(bool InIsDestory = false);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	int32					GetSelectedCharacterSlotIndex(AActor* character);
	FHeroSlotInfo*			GetSelectedCharacterSlotCharacter(AActor* character);
	FHeroSlotInfo*			GetSelectedCharacterSlotHeroUD(FString heroUD);
	FHeroSlotInfo*			GetCurrentSelectedCharacterSlotCharacter();
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	AActor*					GetCurrentSelectedCharacterActor();
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	AActor*					GetNextCombineSkillOwnerActor(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	AActor*					GetBattleActiveCrewActor(ECharacterRoleEnum InRole);
	AActor*					GetRestActiveCrewActor(ECharacterRoleEnum InRole);
	FHeroSlotInfo*			GetBattleActiveCrewSlot(ECharacterRoleEnum InRole);
	FHeroSlotInfo*			GetRestActiveCrewSlot(ECharacterRoleEnum InRole);
	FHeroSlotInfo*			GetBattleSlot(ECharacterRoleEnum InRole);
	FHeroSlotInfo*			GetRestSlot(ECharacterRoleEnum InRole);
	int32					GetBattleActiveCrewIdx(ECharacterRoleEnum InRole);
	int32					GetRestActiveCrewIdx(ECharacterRoleEnum InRole);
	FORCEINLINE int32		GetSelectedSlotIndex() { return SelectedSlotIndex; }
	void SpawnCharacters(FTransform SpawnTransform);
	/*
	Make HeroSlotInfo
	*/
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void MakeHeroSlot(TArray<FString>& HeroUDList);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	bool AddHeroSlotInfo(FString& HeroUD, int32 Index);
	UFUNCTION(BlueprintCallable, Category = PlayStateInfo)
	void MakeRealTimeHeroSlot();

	FORCEINLINE				void	SetbLeaderhasTarget(bool _input) { bLeaderhasTarget = _input; }
	FORCEINLINE				bool	GetbLeaderhasTarget() { return bLeaderhasTarget; }
	FORCEINLINE				void	SetClearTime(float InClearTime) {ClearTime = InClearTime;}
	FORCEINLINE				float	GetClearTime() const { return ClearTime; }
	FORCEINLINE				void	SetRemainingTime(float InRemainingTime) { RemainingTime = InRemainingTime; }
	FORCEINLINE				float	GetRemainingTime() const { return RemainingTime; }
	FORCEINLINE				bool	IsAllHeroDie() const { return bIsAllHeroDie; }
private:
	
	UPROPERTY()
	TArray<FHeroSlotInfo>	SelectedCharacterArray;
	int32					SelectedSlotIndex = -1;
	float					ClearTime = 0.0f;
	float					RemainingTime = 0.0f;
	bool					bIsAllHeroDie = false;
	bool					bLeaderhasTarget = false;
};