// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "BaseLevelScriptActor.generated.h"

class URGameInstance;
//class ALevelActor;
/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API ABaseLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ABaseLevelScriptActor(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Tick(float DeltaSecond) override;	
	
private:
	UPROPERTY()
	URGameInstance*						GameInstance = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void StageClearSequenceEnd();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void ShowVictoryAnim();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void EndVictorySequence();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void BossName();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void RaidBossName();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void RaidBossNameHide();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void SequenceSubTitle(FName InTableKey);
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void ColosseumStart_LeftAni();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void ColosseumStart_RightAni();

	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void GetNewHero_StartAni();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void GetNewHero_PauseSequence();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void PauseSequence();

	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void PauseCSSequence();
	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void ShowVS();

	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void PositionSetting();
	

	UFUNCTION(BlueprintCallable, Category = ABaseLevelScriptActor)
	void CreateWidget();

	// Game BP Events
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_GameStart();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_PlayerSpawn();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_PlayerDie(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_MonsterSpawn(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_MonsterDie(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_BossSpawn(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_BossDie(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_HeroNpcSpawn(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_HeroNpcDie(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_Hit(AActor* InAttackerActor,AActor* InTargetActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_Damaged(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_ChangeCrew(AActor* InActor);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_CastSkill(FName SkillNO, AActor* Caster);
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_Pause();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLevelScriptActor)
	void	BP_TimeOut();

public:
	float InitialVignetteIntensity = 0;
};
