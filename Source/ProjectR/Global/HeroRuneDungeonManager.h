// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "HeroRuneDungeonManager.generated.h"

//class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UHeroRunekDungeonManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize( /*URGameInstance* InRGameInstance*/ );

	FORCEINLINE FName	GetSelectedEpisodeID()					{ return SelectedEpisodeID; }
	FORCEINLINE void	SetSelectedEpisodeID(FName InEpisodeID) { SelectedEpisodeID = InEpisodeID; }

	void	UpdateRuneDungeonStatus( TArray< FEPISODE >& InEpisodes, TArray<FString>& InRotations, int32 InRuneRefreshRemainTime);

	void		UpdateEpisode(const FEPISODE& EpisodeInfo);
	void		UpdateStageGrade(FName InEpisodeID, FName InStageID, EPLAY_CLEAR_GRADE InClearGrade);
	FEPISODE*	GetEPISODE(FName InEpisodeID);

	FORCEINLINE TArray< FName > GetHeroRuneEpisodeIDs() { return EpisodeIDs; }
	TArray< FEPISODE >			GetHeroRuneEpisodes();
	FORCEINLINE int32			GetNewRuneRefreshRemainTime() { return NewRuneRefreshRemainTime;  }
	int32						GetRestRefreshTime();
	TArray<FString>&		GetHeroRuneRotations() { return HeroRuneRotations;	}
	FText						GetHeroDisplayText();
	FName						GetHeroTextureKey();

	/*                                                                                                                                               
	Get rune result
	*/
	FORCEINLINE void		SetLatestAcquiredHeroID(FName InHeroID) { LatestAcquiredHeroID = InHeroID;  }
	FORCEINLINE FName		GetLatestAcquiredHeroID()				{ return LatestAcquiredHeroID; }

	FORCEINLINE void		SetLatestAcquiredRuneCount(int32 InRuneCount)	{ LatestAcquiredRuneCount = InRuneCount; }
	FORCEINLINE int32		GetLatestAcquiredRuneCount()					{ return LatestAcquiredRuneCount; }

	FORCEINLINE void		SetInitState(bool InbInitState) { bInitState = InbInitState; }
	FORCEINLINE bool		GetInitState()					{ return bInitState; }
	FORCEINLINE void		SetLastPlayStageIndex(int32 InLsatPlayStageIndex) { LastPlayStageIndex = InLsatPlayStageIndex; }
	FORCEINLINE int32		GetLastPlayStageIndex()			{ return LastPlayStageIndex; }
	void		ClearLastPlayInfos();

	bool		ExistTargetStage(int32 StageIndex);

	void					SetHeroEpisodesInfo(TArray<FEPISODE> recent, TArray<FEPISODE> Episodes);
	FORCEINLINE bool		IsExistRecentRecord() { return bRecentRecordExist; }
	bool					FindEpisodebyID(FName InEpisodeID, FEPISODE& outEpisode);
	FEPISODE*				FindEpisodebyID(FName InEpisodeID);

	void SetHeroRuneDungeonRetry();
	bool IsRetry() { return bHeroRunekDungeonRetry; }
	void EndRetry() { bHeroRunekDungeonRetry = false; }

private:
	UPROPERTY()
	FName							LatestAcquiredHeroID;
	int32							LatestAcquiredRuneCount = 2;

	
	FName							SelectedEpisodeID;
	UPROPERTY()
	TArray< FName >					EpisodeIDs;
	TMap< FName, FEPISODE >			HeroRuneDungoenEpisoides;	

	TArray<FString>					HeroRuneRotations;

	int32							NewRuneRefreshRemainTime = 0;
	
	UPROPERTY()
	TArray< int32 >					RemainedPlayCounts;
	UPROPERTY()
	TArray< int32 >					StageStateInfo;

// 	UPROPERTY()
// 	URGameInstance*					RGameInstance;

	int32							LastPlayStageIndex = -1;
	bool							bInitState = true;

	UPROPERTY()
	FDateTime						CurDateTime;
	
	bool							bRecentRecordExist;
	bool							bHeroRunekDungeonRetry = false;

public:
	FEPISODE						RecentlyEpisode;
	TArray<FEPISODE>				HeroEpisodes;
	FName							SelectHeroID;
	FName							ResetEpisodeID;
};
