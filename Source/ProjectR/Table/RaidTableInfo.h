// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "RaidTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FWorldBossStageInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBossStageInfo)
	FName StageIdx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FString DesignDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName BossName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName BossID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName PlayMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName MapTableKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32 Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName OpenKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName ClearRewardTableKey;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FBossStage : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName DesignDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName FloorBossLobbyCharacterId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonBossPT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonBossImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonDifficultySpriteDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonDifficultySpriteActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonBossTitleNameString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SingleTitleNameString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiTitleNameString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonEntryString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonEntryCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonPlayTimeString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	int32 CommonPlayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonRecommendPowerString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	int32 CommonRecommendPowerValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonRewardString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SingleSelectDifficultyString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SinglePlayDifficultyString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SingleTrialCountString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SingleTrialMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonPlayCostInfoString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName CommonCurrencyPropertyString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName SingleEnterString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiPartyCreateString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiGuildPartyCreateString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiPartyRoomNumberString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiEnterString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName MultiLockInfoString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
	FName UseCurrency;
};

// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FWorldBossStageInfo : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBossStageInfo)
// 		FName StageIdx;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FString DesignDescription;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName BossName;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Difficulty;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName BossID;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName PlayMode;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MapTableKey;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 Stamina;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName OpenKey;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName ClearRewardTableKey;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName WorldBossPT;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName WorldBossImage;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName DifficultySpriteDefault;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName DifficultySpriteActive;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName WorldBossTitleNameString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName SingleTitleNameString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MultiTitleNameString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName EntryString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName PlayTimeString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 RecommendPower;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName RewardString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName SelectLevelString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName PlayLevelString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName PlayCostString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName SingleEnterString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MultiPartyCreateString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MultiPartyNumberString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MultiEnterString;
// };
// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FRaidStageTableInfo : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Boss;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Mode;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName MapTableKey;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Title;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Sub_Title;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Story;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName RewardTableKey;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Bossskill1;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Bossskill2;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName BossskillTitle1;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName BossskillTitle2;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Playtip;
// 
// 	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RaidStageTableInfo|None")
// 	//int32 Chancesleft;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 recommendlevel;
// 
// 	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RaidStageTableInfo|None")
// 	//FName HeroPickStringKeys;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 PlayLevel;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 Stamina;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		FName Texture;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		EOVERLORD		UnLockInfoEnum;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		TArray<FName> TipStringKeys;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		TArray<FName> RecommendSkillStringKeys;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		TArray<FName> RecommendHeroList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		TArray<FName> MonsterIDList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		TArray<FName> AttackTextureKeys;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 OpenLevel;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
// 		int32 RecommendedPower;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 		FName					PreviewItemIcon;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampaignEpisodeTableInfo)
// 		FName					PreviewItemStringKey;
// };
// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FBossStage : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		int32	CommonEntryCount;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		int32	CommonPlayTime;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		int32	CommonRecommendPowerValue;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		int32	SingleTrialMax;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		FName	CommonPlayTimeString;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FBossStage)
// 		FName	FloorBossLobbyCharacterId;
// };