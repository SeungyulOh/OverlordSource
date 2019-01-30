// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IGameState.h"
#include "Table/CampaignTableInfo.h"
#include "PlayState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UPlayState : public UIGameState
{
	GENERATED_BODY()
		
public:
	UPlayState();

	void Enter() override;
	void Leave() override;
	void BeginState() override;
	void Retry();
	void NextStage();
	
	UFUNCTION()
	void Callback_ReceivePlayPacket(bool isRetry);

	UFUNCTION(BlueprintCallable, Category = PlayState)
	void OnCompleteLoadingLevel();
	
	void CompleteLoadingState();

	DECLARE_DELEGATE(FOnReceivedPlayPacket)
	FOnReceivedPlayPacket OnReceivedPlayPacket;

protected:
	void LoadSubLevels();
private:
	UPROPERTY()
	TArray< FName > SubLevels;
	int32			SubLevelCount;

	

	double			CurTime = 0.0;

public:
	UPROPERTY()
	TMap<FName, class ASpawnBox*> SpawnBoxMap;
	UPROPERTY()
	TMap<FName, class ASpawnerTransform*> WaypointMap;
	UPROPERTY()
	TMap<FName, class AWallActor*> WallMap;


	FFloorStageDetail* NextStageTableInfo;

private:
	bool isPMapChanged = false;
};
