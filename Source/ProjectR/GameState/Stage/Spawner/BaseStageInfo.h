// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "GameFramework/Actor.h"

#include "Global/RGameInstance.h"

#include "GameState/Stage/Spawner/LevelActor.h"

#include "CustomStructures/DialogueInfos.h"

#include "Table/MapTableInfo.h"

#include "BaseStageInfo.generated.h"

//	절대적으로 인게임 안에서만 해야한다.

UCLASS()
class PROJECTR_API UBaseStageInfo : public UObject
{
	GENERATED_BODY()
public:
	bool						bFirstStageTry = false;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	FName						MapTableKey;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	FName						FloorInfoId;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	FName						StageID;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	int32						StageIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	FName						ClearRewardTableKey;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UBaseStageInfo)
	int32						PlayLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UBaseStageInfo)
	FString						PlayUD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	TArray<AActor*>				ArrBossInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	TArray<AActor*>				ArrMonsterInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	TArray<AActor*>				ArrHeroInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	TArray<AActor*>				ArrPlayerInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	float						GamePlayTime		=	0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	EPhaseClearEnumType			ClearCondition = EPhaseClearEnumType::VE_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UBaseStageInfo)
	TArray<FREWARD>				DropRewards;

	UPROPERTY()
	bool							bPlayAlone = false;
private:
	UPROPERTY()
	FTimerHandle					TimeHandler_GameTime;
	UPROPERTY()
	bool							bIsTimeOut = false;
	UPROPERTY()
	ALevelActor*					LevelActor = nullptr;
public:
	UBaseStageInfo(const FObjectInitializer& ObjectInitializer);
	~UBaseStageInfo();

	void						Init();
	void						Reset();

	void						SetNormalNpc(AActor* InActor, int32 InGroupKey);
	void						SetMonster(AActor* InMonster);
	void						SetHero(AActor* InHero);
	void						SetBoss(AActor* InBoss);
	void						SetPlayer(AActor* InPlayer);

	void						DeleteMonster(AActor* InMonster);
	void						DeleteHero(AActor* InHero);
	void						DeleteBoss(AActor* InBoss);
	void						DeletePlayer(AActor* InPlayer);
	UFUNCTION()
	TArray<AActor*>				GetMonster() { return ArrMonsterInfo; }
	UFUNCTION()
	TArray<AActor*>				GetHero() { return ArrHeroInfo; }
	UFUNCTION()
	TArray<AActor*>				GetBoss() { return ArrBossInfo; }
	UFUNCTION()
	TArray<AActor*>				GetPlayer() { return ArrPlayerInfo; }
	UFUNCTION()
	FName						GetSelectedLevel();
	UFUNCTION()
	TArray< FName >				GetSubLevels();
	UFUNCTION()
	FName						GetSubLevel();
	UFUNCTION()
	EGamePlayModeTypeEnum		GetGameMode();
	UFUNCTION()
	float						GetPlayTime();
	UFUNCTION()
	void						SetSelectedMapInfo(FName InMapID, int32 InLevel = 1);
	UFUNCTION()
	FORCEINLINE FName			GetSelectedMapID() { return MapTableKey; }
	UFUNCTION()
	FORCEINLINE int32			GetPlayLevel() { return PlayLevel; }
	UFUNCTION()
	void						SetPlayTime();
	UFUNCTION()
	FORCEINLINE FTimerHandle	GetGameTimeHandler() { return TimeHandler_GameTime; }
	UFUNCTION()
	void						SetTimer(float TotalGameSeconds);
	UFUNCTION()
	void						PauseTimer();
	UFUNCTION()
	void						UnPauseTimer();
	UFUNCTION()
	bool						isPauseTimer();
	UFUNCTION()
	float						GetElapsedGameTime();
	UFUNCTION()
	float						GetRemainingGameTime();
	UFUNCTION()
	virtual void				TimeOut();
	UFUNCTION()
	FORCEINLINE bool			GetIsTimeOut() { return bIsTimeOut; }

	UFUNCTION()
	bool						isMultiBoss();
	UFUNCTION()
	void						BindHideDialogueEvent();
	UFUNCTION()
	void						OnHideDialog();
	UFUNCTION()
	void						BindPlayEventSequence(TArray<AActor*>& InBoss);
	UFUNCTION()
	void						OnSequenceEndSpawnActor(FName InSequence);
	UFUNCTION()
	void						AttachBossEffect();
	UFUNCTION()
	ALevelActor*				GetLevelActor() {return LevelActor;}
	UFUNCTION()
	void						SetLevelActorBP();
};