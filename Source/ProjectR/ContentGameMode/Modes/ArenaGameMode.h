// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "ArenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AArenaRGameMode : public AProjectRGameMode
{
	GENERATED_BODY()
public:	
	AArenaRGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void InitGameMode();		// GameMode Initialize
	virtual void PrepareGameMode();		// Object Create, GamePlayMode Init(Time, Enemy..)
	virtual void ReadyGameMode();		// Start Matinee, Effect
	virtual void StartGameMode();		// Play Game
	virtual void EndGameMode();			// End Matinee, Effect
	virtual void ResultGameMode();

	virtual void GamePlayModeEnd();
	virtual EStageContextEnum GetStageContext() override;
	

public:
	
	void CheckGameModeEndToResult();
	void SpawnArenaMatchHeroes();

	UFUNCTION()
	void OnSequenceEndArenaWin(FName InSequenceKey);
	UFUNCTION()
	void OnSequenceEndArenaStart(FName InSequenceKey);
	UFUNCTION()
	void OnPlayModeArenaDoneRpDelegate();

	UFUNCTION()
	void EndGameModeDelay();

	UFUNCTION()
	void OnCompleteResourcePreLoading();
	UFUNCTION()
	void OnReadyToStart();
	UFUNCTION()
	void OnReadyToResult();
	UFUNCTION()
	void TimeUnPause();

private:
	UPROPERTY()
	int32				MyTeamKID = -1;
	UPROPERTY()
	int32				EnemyTeamKID = -1;
	UPROPERTY()
	EStageContextEnum	ResultContext;


	UPROPERTY()
	bool				ShowUI_PVP_VS = false;
	UPROPERTY()
	bool				ShowUI_PVP_Start = false;
	UPROPERTY()
	bool				WinSequenceEnd = false;
	UPROPERTY()
	bool				ReceiveArenaPlayDoneRp = false;

	UPROPERTY()
	FTimerHandle		EndGameModeDelayHandler;
	UPROPERTY()
	FTimerHandle		StartGameModeDelayHandler;
};
