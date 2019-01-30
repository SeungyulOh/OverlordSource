#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GameModeSequenceTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGameModeSequenceTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGameModeSequenceTableInfo)
	FName	StartSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGameModeSequenceTableInfo)
	FName	WinSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGameModeSequenceTableInfo)
	FName	DefeatSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FGameModeSequenceTableInfo)
	EGameModeSequenceTypeEnum GameModeSequenceType = EGameModeSequenceTypeEnum::VE_None;
};
