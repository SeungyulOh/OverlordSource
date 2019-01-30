// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BattleObjectInfo.generated.h"


/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBattleObjectInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectInfo)
	EBattleObjectTypeEnum	BattleObjectType = EBattleObjectTypeEnum::VE_None;

	// Attach Effect01
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	TAssetPtr< UParticleSystem >		Effect_1 = nullptr;

	// Attach Effect02
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	TAssetPtr< UParticleSystem >		Effect_2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	TAssetPtr< UParticleSystem >		DestroyEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	FName					ShapeTableRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	float					FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectInfo)
	int32					ShowHP;

};
