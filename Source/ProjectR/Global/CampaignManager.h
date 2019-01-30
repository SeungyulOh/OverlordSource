// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"

#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "Network/ClientProtocols.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "Table/CampaignTableInfo.h"
#include "CampaignManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnClickFloorShortCut, int32, InFloor);

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECampaignStagePlayType: uint8
{
	VE_ProgressStage		UMETA(DisplayName = "ProgressStage"), 
	VE_RetryCurrentStage	UMETA(DisplayName = "CurrentStage"),
	VE_PrevStage			UMETA(DisplayName = "PrevStage"),
	VE_NextStage			UMETA(DisplayName = "NextStgae"),
};




UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECampaignEventTypeEnum : uint8
{
	VE_RewardHeros = 0 UMETA(DisplayName = "RewardHeros"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCampaignEvent
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	ECampaignEventTypeEnum		EventType = ECampaignEventTypeEnum::VE_RewardHeros;
	UPROPERTY()
	FName						EventName;
	UPROPERTY()
	FName						EventName1;

};





/**
 * 
 */
UCLASS()
class PROJECTR_API UCampaignManager : public UObject
{
	GENERATED_BODY()
public:
	UCampaignManager();

	void Initialize();
	void InvalidateData();


	bool RequestGameStart(FPlay_Heroes_Info& PlayHeroInfo);
	bool RequestGameResult(int32 StarGrade);

	bool InvalidateNextCampaignData(FName ChapterID, FName EpisodeID, FName StageID, int32 StageSequence);	
	
	
public:
	
	int32	GetCompletedStageLastIndex();
	void	ClearSelectedCampaign();

	

	bool IsAutoPlay() { return 0 < nCampaignRepeatCount; }
	void SetAutoPlayCount(int32 InCount) { nCampaignRepeatCount = InCount; }
	void DecreasePlayCount() { --nCampaignRepeatCount; }


	void CampaignResult(const TArray<FREWARD>& Reward);
	TArray<int32>& GetStageClearInfo() { return StageStateInfo; }
	void SetStageClearInfo(const TArray<int32>& InArray) { StageStateInfo = InArray; }

	// 최초보상팝업에서 사용 할 보상정보 저장
	void SaveHeroRewardsForRewardPopup(const TArray<FREWARD>& InRewards);
	void ClearHeroRewardsForRewardPopup();


	// @param InBossFloor : tower boss index ( zero base )
	bool IsUnlockTowerBoss(int32 InBossFloor);

	FORCEINLINE EStageDifficultyEnum GetStageDifficulty() { return StageDifficulty; }
	FORCEINLINE void SetStageDifficulty(EStageDifficultyEnum InDifficulty) { StageDifficulty = InDifficulty; }

	FORCEINLINE int32	GetNormalProgressIndex() { return NormalProgressIndex; }
	FORCEINLINE void	SetNormalProgressIndex(int32 InNormalIndex) { NormalProgressIndex = InNormalIndex; }
	FORCEINLINE int32	GetHardProgressIndex() { return HardProgressIndex; }
	FORCEINLINE void	SetHardProgressIndex(int32 InHardIndex) { HardProgressIndex = InHardIndex; }
	FORCEINLINE	int32	GetCurrentNormalFloor() { return CurrentNormalFloor; }
	FORCEINLINE void	SetCurrentNormalFloor(int32 InFloor) { CurrentNormalFloor = InFloor; }
	FORCEINLINE int32	GetCurrentHardFloor() { return CurrentHardFloor; }
	FORCEINLINE void	SetCurrentHardFloor(int32 InFloor) { CurrentHardFloor = InFloor; }

	void				CalculateFloorStageIndex();
	void SetTowerStatus(FTOWER_STATUS& InTowerStatus);
public:

	UPROPERTY()
	FTOWER_STATUS	TowerStatus;

	// 퀘스트단계가 변경되었는지
	bool			bChangeQuest;

	UPROPERTY()
	TArray<UPaperSprite*> TresureboxTexArray;

	bool isUpdatedFloor = false;

	UPROPERTY()
	TArray<FREWARD>		m_ResultReward;
	FAVATAR				m_ResultAvatar;
	
	UPROPERTY()
	TArray<FREWARD>		SavedHeroRewards;
	UPROPERTY()
	TArray<FREWARD>		SavedQuestRewards;

	bool				bLevelUp = false;


	DECLARE_DELEGATE(FOnReadyToPlayCampaign)
	FOnReadyToPlayCampaign OnReadyToPlayCampaign;


	FDelegate_OnClickFloorShortCut	OnClickFloorShortCut;
private:
	bool						bIsInitState			= true;

	FName						NextCampaignStageID;

	FString						PrevPlayUD;
	
	// Grade
	TArray< int32 >				StageStateInfo;
	TArray< int32 >				EpisodeRewardStateInfo;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor> >					SpawnLocationActors;

	bool DirectLink = false;

	int32						nCampaignRepeatCount = 0;
	bool						bAutoPlayOn;

	EStageDifficultyEnum	StageDifficulty = EStageDifficultyEnum::Normal;
	int32					NormalProgressIndex;
	int32					HardProgressIndex;
	int32					CurrentNormalFloor;
	int32					CurrentHardFloor;
};
