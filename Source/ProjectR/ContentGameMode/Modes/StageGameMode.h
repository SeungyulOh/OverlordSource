// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "StageGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AStageGameMode : public AProjectRGameMode
{
	GENERATED_BODY()
public:	
	AStageGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	virtual void InitGameMode();		// GameMode Initialize
	virtual void PrepareGameMode();		// Object Create, GamePlayMode Init(Time, Enemy..)
	virtual void ReadyGameMode();		// Start Matinee, Effect
	virtual void StartGameMode();		// Play Game
	virtual void EndGameMode();			// End Matinee, Effect
	virtual void ResultGameMode();

	virtual void GamePlayModeEnd();
	virtual void StopGameMode() override;
	virtual void ResultUI();

	virtual void DestroyResource() override;

	virtual EStageContextEnum GetStageContext();


public:
	UFUNCTION()
	void OnCompleteResourcePreLoading();
	UFUNCTION()
	void OnCampResultEvent();
	UFUNCTION()
	void	OnReadyToStart();
	UFUNCTION()
	void	OnReadyToResult();

	UFUNCTION()
	void OnDelayedQuestCompletePopupHide();
	
	UFUNCTION()
	void	OnDelayedResultUI();
public:
	void ShowReadyToStartUI();
	
	void UpdateStarHUD(EPlayStageStarEnum StarCnt);

	UFUNCTION()
	void CheckItemDrop(AActor* DeadActor, AActor* FromActor);

	void DropItem(AActor* ItemGiveActor, bool bLastItem = false);
	void DropItemOrderCheck();

public:
	
	UPROPERTY()
	FTimerHandle		StartStageDelayTimeHandle;

	UPROPERTY()
	FTimerHandle		ResultUIDelayTimeHandle;

	UPROPERTY()
	TArray<FREWARD>		DropRewards;

	UPROPERTY()
	TArray<AActor*>		DropItemActors;

};
