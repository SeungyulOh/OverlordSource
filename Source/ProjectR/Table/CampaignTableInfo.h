#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Engine/DataTable.h"
#include "CampaignTableInfo.generated.h"


/**
*
*/

USTRUCT(BlueprintType)
struct PROJECTR_API FFloorInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName FloorImg;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName FloorBoss;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyN1_Pre;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	int32 TreasureBoxKeyN1Idx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyN2_Pre;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyN2_Add;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	int32 TreasureBoxKeyN2Idx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyH1_Pre;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	int32 TreasureBoxKeyH1Idx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyH2_Pre;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName TreasureBoxKeyH2_Add;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	int32 TreasureBoxKeyH2Idx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName FloorTitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorInfo)
	FName FloorSubTilte;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FFloorStageDetail : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	FloorInfoId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	StageIdx;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	PlayType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	MapTableKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	ClearRewardTableKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	DropRewardTableKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	StageType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	Title;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	IngameSubTitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	ImageCardKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	BackgroundImg;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	OpenKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	StarCondition1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	StarCondition2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	Stamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	int32	RecommendedPower;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	TArray<FName>	MonsterIDList;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	Friend1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	Friend2;

	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	clearInfo1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FFloorStageDetail)
	FName	clearInfo2;


};
// 
// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FCampaignChapterTableInfo : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// 
// public:
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	TArray< FName >			EpisodeKeys;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	TArray< FName >			HardEpisodeKeys;
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	TArray<FName>			RewardKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	TArray<FName>			StarRewardKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	TArray<FName>			HeroId;
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	FName					ChapterStringKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	FName					ChapterBackGround;
// 
// 	bool					IsEpisode(FName InEpisodeName);
// 	bool					IsHardEpisode(FName InEpisodeName);
// };
//
// USTRUCT(BlueprintType)
// struct PROJECTR_API FCampaignEpisodeTableInfo : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// 
// public:
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EpisodeStep = 0;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					Title;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					Story;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					trialPerDay;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					RewardKey;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					StarRewardKey;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	TArray< FName >			StageKeys;
// 		
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	FName					EpisodeKey_Texture;	
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
// 	FName					Episode_BackGround_Key;
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					CardName;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					Detail;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	TArray<FName>			RewardHeros;
// 	
// 	/*
// 	for hero rune dungeon
// 	*/
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					RuneHeroID;
// 
// 	/*
// 	DeckSetting View String
// 	*/
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					HeroPickStringKeys;
// 
// 	/*
// 	for week dungeon
// 	*/
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		OpenDay01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartHour01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartMinute01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		CloseDay01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndHour01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndMinute01;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		OpenDay02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartHour02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartMinute02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		CloseDay02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndHour02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndMinute02;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		OpenDay03;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartHour03;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					StartMinute03;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	EDayOfWeekTypeEnum		CloseDay03;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndHour03;
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					EndMinute03;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					UnlockText1;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					UnlockText2;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	int32					UnlockRequire;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					PreviewItemIcon;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 	FName					PreviewItemStringKey;
// };
// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FCampaignStageTableInfo : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// 
// public:
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					StageIdx;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					MapTableKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					FirstRewardTableKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					NormalRewardTableKey;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					RewardStar1;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					RewardStar2;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					RewardStar3;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					Title;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					HeroSay;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					Story;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					InGameTitle;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					InGameTitleName;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					OpenKey;
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					PlayLevel;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					Stamina;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					Mileage;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					Sweeper;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					RewardHero;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					chardcount;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					OpenStage;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					trialPerDay;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	float					HPMultiplier;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	float					AtkMultiplier;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	FName					MemberMultiplier;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	int32					RecommendedPower;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	TArray<FName>			MonsterIDList;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignStageTableInfo)
// 	TArray<FName>			RecommendHeroList;
// };