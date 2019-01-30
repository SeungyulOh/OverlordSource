// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "MirrorGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AMirrorGameMode : public AProjectRGameMode
{
	GENERATED_BODY()
public:	
	AMirrorGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void InitGameMode();		// GameMode Initialize
	virtual void PrepareGameMode();		// Object Create, GamePlayMode Init(Time, Enemy..)
	virtual void ReadyGameMode();		// Start Matinee, Effect
	virtual void StartGameMode();		// Play Game
	virtual void EndGameMode();			// End Matinee, Effect
	virtual void ResultGameMode();

	virtual void GamePlayModeEnd();

	virtual EStageContextEnum GetStageContext() override;

	void SpawnMirrorMatchHeroes();

	UFUNCTION()
	void OnReadyToStart();
	
	UFUNCTION()
	void	OnReadyToResult();

	UFUNCTION()
	void EndGameModeDelay();

	UFUNCTION()
	void OnSequenceEndMirrorStart(FName InSequenceKey);

	UFUNCTION()
	void OnSequenceEndMirrorWin(FName InSequenceKey);

	UFUNCTION()
	void OnCompleteResourcePreLoading();

	void OnPlayModeMirrorDoneRpDelegate();
	void CheckGameModeEndToResult();

	// 	virtual void StopGameMode() override;
// 	virtual void ResultUI();

private:
	int32				MyTeamKID = -1;
	int32				EnemyTeamKID = -1;
	EStageContextEnum	ResultContext;

	bool				ShowUI_PVP_VS = false;
	bool				ShowUI_PVP_Start = false;
	bool				WinSequenceEnd = false;
	bool				ReceiveMirrorPlayDoneRp = false;

	FTimerHandle		EndGameModeDelayHandler;

	

};
