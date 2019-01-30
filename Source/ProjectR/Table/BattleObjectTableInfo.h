// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "Character/BattleBaseCharacter.h"
#include "Character/Bullets/BulletBase.h"
#include "CustomStructures/CharacterBattleData.h"
#include "BattleObjectTableInfo.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FBattleObjectTableInfo : public FCharacterBattleData
{
	GENERATED_USTRUCT_BODY()

public:
		
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		DisplayName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	int32		Level = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		Desc;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	EBattleObjectTypeEnum		ObjectType = EBattleObjectTypeEnum::VE_None;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		SkillSlot1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		SkillSlot2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		SkillSlot3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		SkillSlot4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		SkillSlot5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		InitialSkillSlot1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName		InitialSkillSlot2;


	// Attach Effect01
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	TAssetPtr< UParticleSystem >		Effect_1 = nullptr;

	// Attach Effect02
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	TAssetPtr< UParticleSystem >		Effect_2 = nullptr;

	// projectile info
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName								BulletID;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	TAssetPtr< UParticleSystem >				HitEffect = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	TAssetSubclassOf< ABattleBaseCharacter >		ModelClass = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName							PortraitTextureKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName							ShapeTableRow;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	float							FireDelay;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = BattleObjectTable)
	TAssetPtr< UParticleSystem >			DestroyEffect = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName							GrowthTableID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName							GrowthMultiplierID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	int32							ShowHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObjectTable)
	FName							Tooltip;

public:

	UClass*			GetModelClass();
};
