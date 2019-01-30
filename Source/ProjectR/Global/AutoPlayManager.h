// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/UIDefs.h"
#include "CustomStructures/SpawnStructures/SpawnerTransformInfo.h"
#include "AutoPlayManager.generated.h"


//class URGameInstance;
class UAutoSpawner;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAutoplayModeTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Campaign			UMETA(DisplayName = "Campaign"),
	VE_Campaign_Target	UMETA(DisplayName = "Campaign_Target"),
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UAutoPlayManager : public UObject
{
	GENERATED_BODY()

public:

	void					Initialize();
	
	UFUNCTION(BlueprintCallable, Category = RGameInstance)
	void					ActivateAutoPlay(EAutoplayModeTypeEnum InAutoPlayModeType, int32 count);

	EAutoplayModeTypeEnum	UseAutoPlay();
	void					DecreaseAutoPlayModeCount();
	EAutoplayModeTypeEnum	GetAutoPlayMode();

	//-------------------------------------------------------------------------------------------
	// Campaign auto playmode
	//-------------------------------------------------------------------------------------------
	UFUNCTION()
	void		OnEnterStateSelect(FName DeckID);

// 	UFUNCTION()
// 	void		OnReadyToStart();

	UFUNCTION()
	void		OnCompleteCampaignResult();

	
	//void		InGameProcessAutoPlay();
	FVector		GetValidSpawnPointNearByControlledHero();


	FORCEINLINE FSpawnerTransformInfo	GetNextWayPointTransform() {return NextWayPointTransform; }
	void SetNextWayPointTransform(FSpawnerTransformInfo InSpawnerTransform);
	void SetResetNextWayPoint(FSpawnerTransformInfo InSpawnerTransform);


	FORCEINLINE void				SetDeck(FPlay_Heroes_Info& PlayHeroInfo) { CurPlayHeroInfo = PlayHeroInfo; }
	FORCEINLINE FPlay_Heroes_Info	GetDeck()			{ return CurPlayHeroInfo; }

private:
// 	UPROPERTY()
// 	URGameInstance*				GameInstance = nullptr;						// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..

	UPROPERTY()
	EAutoplayModeTypeEnum		CurrentAutoPlayMode = EAutoplayModeTypeEnum::VE_None;

	UPROPERTY()
	int32						AutoPlayCount = 0;
	
	UPROPERTY()
	FPlay_Heroes_Info			CurPlayHeroInfo;
	
	UPROPERTY()
	bool						IsWayPointMove = false;



	/*
	Stage helper..
	*/
	//UPROPERTY()
	FSpawnerTransformInfo		NextWayPointTransform;
public:
	void ClearWayPoint();
};