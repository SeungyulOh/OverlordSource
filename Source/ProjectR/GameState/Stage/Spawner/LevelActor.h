// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "Engine/TriggerBox.h"

#include "Global/GridMapManager.h"

#include "Object/SpawnerTransform.h"

#include "LevelActor.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPhaseClearEnumType : uint8
{
	VE_PlayerSpawn						UMETA(DisplayName = "PlayerSpawn"),
	VE_Dialogue							UMETA(DisplayName = "Dialogue"),
	VE_AllKill							UMETA(DisplayName = "AllKill"),
	VE_BossKill							UMETA(DisplayName = "BossKill"),
	VE_Resisting						UMETA(DisplayName = "Resisting"),
	VE_Protect							UMETA(DisplayName = "Protect"),
	VE_ProtectDisable					UMETA(DisplayName = "ProtectDisable"),
	VE_Move								UMETA(DisplayName = "Move"),
	VE_MonsterCountCheck				UMETA(DisplayName = "MonsterCountCheck"),
	VE_SpecificMonsterCountCheck		UMETA(DisplayName = "SpecificMonsterCountCheck"),
	VE_CharacterAllSurvival				UMETA(DisplayName = "CharacterAllSurvival"),
	VE_ResistingEnable					UMETA(DisplayName = "ResistingEnable"),
	VE_ResistingDisable					UMETA(DisplayName = "ResistingDisable"),
	VE_MAX								UMETA(DisplayName = "None")
};

UCLASS()
class ALevelActorPhase : public ATriggerBox
{
	GENERATED_BODY()
public:
	EPhaseClearEnumType				ClearCondition		=	EPhaseClearEnumType::VE_MAX;
	EPhaseClearEnumType				assistantCondition	=	EPhaseClearEnumType::VE_MAX;
	UPROPERTY()
	uint32							Parameter			=	0;
	UPROPERTY()
	uint32							MonsterDieCount		=	0;
	UPROPERTY()
	uint32							MonsterSpawnCount	=	0;
	UPROPERTY()
	float							ResistingTime		=	0.0f;
	UPROPERTY()
	TArray<FName>					SpecificMonsterSpawnID;
};

USTRUCT()
struct FProtectNpcList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<AActor*>	ActorList;
};

UCLASS()
class PROJECTR_API ALevelActor : public ATriggerBox
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ALevelActor)
	TMap<int32, ALevelActorPhase*>				PhaseList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ALevelActor)
	uint8							CurPhase = 0;
	UPROPERTY()
	FTimerHandle					TimeHandler_PhaseTime1;
	UPROPERTY()
	FTimerHandle					TimeHandler_PhaseTime2;
	UPROPERTY()
	TArray<AActor*>					SpecificMonster;
	UPROPERTY()
	bool							IsSpecificSpawn = false;
	UPROPERTY()
	bool							IsResisting = false;
private:
	UPROPERTY()
	uint8							MaxPhase = 0;
	UPROPERTY()
	bool							bPhaseCheck = false;
	UPROPERTY()
	TMap<int32, FProtectNpcList>    ProtectNpc;
	UPROPERTY()
	bool							IsResistingTimeOut = false;
public:
	ALevelActor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, Category = ALevelActor)
	void ExecutePhase(uint8 Step = 0);
	void ExecutePhase_Implementation(uint8 Step = 0);

// 	UFUNCTION(BlueprintNativeEvent, Category = ALevelActor)
// 	void GameStart();
// 	void GameStart_Implementation();

	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void StartPhaseCheck();

	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void SetPhaseEndCodition(EPhaseClearEnumType codition = EPhaseClearEnumType::VE_MAX, int32 InParameter = 0);
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void SetPhaseEndCodition2(TArray<FName> InSpecificMonsterSpawnID, EPhaseClearEnumType codition = EPhaseClearEnumType::VE_MAX, int32 InParameter = 0);
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void SetPhaseEndCodition3(EPhaseClearEnumType codition = EPhaseClearEnumType::VE_MAX, EPhaseClearEnumType assistantcodition = EPhaseClearEnumType::VE_MAX, int32 InParameter = 0);

	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	int32						GetMaxPhase() { return MaxPhase; }
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	int32						GetPhase() { return CurPhase; }
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void						SetPhase(int32 InMaxPhase);
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void 						ConditionCheck(EPhaseClearEnumType InMode);
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
	void						ConditionCheck2(EPhaseClearEnumType InMode);
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
		void Check_Have_InGameTuto();
	UFUNCTION(BlueprintCallable, Category = ALevelActor)
		void FirstSpawn();
	UFUNCTION()
	virtual void				PhaseTimeOut();
	UFUNCTION()
	float						GetElapsedGameTime();

	ALevelActorPhase*			GetLevelActorPhase();
	void						SetNpcHero(AActor* InHero);
	bool						isNpcHeroDie(AActor* InHero);

	void						SetPhaseCheck(bool bCheck) { bPhaseCheck = bCheck; }
	void						SetSpecificMonster(FName InSpecificMonsterSpawnID, AActor* InSpecificMonsterActor);

	FORCEINLINE  const TMap<int32, FProtectNpcList>& GetProtectList() { return ProtectNpc; }

	virtual void BeginDestroy() override;

private:
	void						SetMaxPhase(int32 InMaxPhase) { MaxPhase = InMaxPhase; }
	void						AllKillEnd();
	void						KillBossEnd();
	void						ResistingEnd();
	void						SetProtectNpc();
	void						ProtectNpcDisable();
	void 						NextPhase();
	void 						EndPhase();
	void						MonsterCountCheck();
	void						SetSpecificMonsterCountCheck();
	void						SetCharacterAllSurvivalCheck();

	void						ClearTimeHud();
}; 