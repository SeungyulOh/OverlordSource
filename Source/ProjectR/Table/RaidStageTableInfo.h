#pragma once

#include "SharedConstants/SharedGlobalConstants.h"
#include "RaidStageTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FRaidStageTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					RaidStageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32					StageIdx;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	ERaidBossClassType		Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	ERaidDifficultyType		Difficulty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					WalletID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	ERaidDayType			Day;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					PlayMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					MapTableKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32					Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					OpenKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					BossImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32					RecommendPowerValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					AttributeTypeString;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	FName					RewardPreviewKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	int32					SingleTrialMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RaidStageTableInfo)
	TArray<FName>			MonsterIDList;
};