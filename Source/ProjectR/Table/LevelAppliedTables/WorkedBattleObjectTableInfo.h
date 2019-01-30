// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "Table/BattleObjectTableInfo.h"
#include "WorkedBattleObjectTableInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FWorkedBattleObjectTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FBattleObjectTableInfo		Data;

public:
	void	SetDataFromBattleObjectInfo(FBattleObjectTableInfo* BattleObjectInfo, int32 Level );

	void	ApplyLevel(FName PatternKey, FName MultiplierPatternKey, int32 Level);
//	float	GetLevelValue(TArray< float >& GrowthTable, int32 Level);	

};
