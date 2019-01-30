 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Table/CharacterTableInfo.h"
#include "Table/BattleObjectTableInfo.h"
#include "WorkedCharacterTableInfo.generated.h"


/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FWorkedCharacterTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FCharacterTableInfo		Data;

	

public:
	void	SetDataFromCharacterInfo( FCharacterTableInfo* CharacterInfo, int32 Level );	

	float	GetLevelValue( TArray< float >& GrowthTable, int32 Level );		

	void	ApplyLevel(FName PatternKey, FName MultiplierPatternKey, int32 Level);
	void	ApplyLevelNPC(FName PatternKey, FName MultiplierPatternKey, int32 Level);
};
