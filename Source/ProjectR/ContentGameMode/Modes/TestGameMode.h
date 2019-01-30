// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "CustomStructures/SpawnStructures/SpawnerTransformInfo.h"
#include "TestGameMode.generated.h"
class UUC_HeroStatMgr_Info;
class ALevelSequenceActor;
/**
 * 
 */
UCLASS()
class PROJECTR_API ATestGameMode : public AProjectRGameMode
{
	GENERATED_BODY()
public:	
	ATestGameMode(const FObjectInitializer& ObjectInitializer);


	virtual void PrepareGameMode();		// Object Create, GamePlayMode Init(Time, Enemy..)

public:
	TArray<int32>			heroSkillLevelList;
	TArray<FTestItemInfo>	heroItemList;
	bool					bMapLoad = false;
	bool					bSaveKeyStart = false;
	FName					MapLoadName;
	FPlayTestDecks			Decks;
	TArray<int32>			HeroSpawnList;
	TMap<int32, FSpawnerTransformInfo>	SpawnTransformMap;

	TArray< TWeakObjectPtr<AActor> >	MyHeroList;
	FString					LastCommand;
	FString					CurKey;

	TWeakObjectPtr<AActor>	SelectActor;
	int32					SelectIdx = 0;

	UPROPERTY()
	TWeakObjectPtr<ALevelSequenceActor>		LevelSequenceActor;

	void ExeLastCommand();
	void ConsoleCommand(const FString& Command);
	void DeckLoadSave(FString strName, bool bLoad);

	bool IsSaveKeyStart() { return bSaveKeyStart; }
	TWeakObjectPtr<AActor>& GetSelectActor() { return SelectActor; }
	bool SetSelectNpcActor();
	void ClearSelectActor();
	void Test();

	UFUNCTION()
	void OnSequenceEnd(FName InSequenceKey);
	
	UFUNCTION()
	void CheckItemDrop(AActor* DeadActor, AActor* FromActor);

	void DropItem(AActor* ItemGiveActor);
	void ToggleStat();
private:
	void Clear();
	void CurDeckReLoad();
	void CommandLoadMap(TArray<FString>& Parsed);
	void CommandKeySave(TArray<FString>& Parsed);
	void CommandKeyLoad(TArray<FString>& Parsed);
	void CommandExeKey(TArray<FString>& Parsed);
	void CommandDeck(TArray<FString>& Parsed);
	void CommandHeroSpawn(TArray<FString>& Parsed);
	void CommandDamageFloater(TArray<FString>& Parsed);
	void CommandSpawn(ECharacterCategoryTypeEnum categroyType, TArray<FString>& Parsed);

	void SpawnHero(FName id, int32 level, bool bMy, FVector offset, TArray<int32>& inSkillLevelList, TArray<FTestItemInfo>& inItemList);
	void Spawn(ECharacterCategoryTypeEnum categroyType, FName id, int32 level, bool bMy, bool bAbs, FVector offset);

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FREWARD>					DropRewards;
};
