// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomActors/SkillCastingDecal.h"
#include "Character/Components/SkillCastingDecalComponent.h"
#include "SkillCastingEffectTableInfo.generated.h"

UENUM(BlueprintType)
enum class EDecalShapeEnum : uint8
{
	Sector90,
	Sector180,
	Sector360,
	Rectangle,
};

/**
 * For SkillCastingEffectComponent
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillCastingEffectTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	// BEGIN index

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSkillCastingEffectTableInfo)
	EDecalShapeEnum DecalShape;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSkillCastingEffectTableInfo)
	ESkillDecalColorType SkillDecalColorType;
	
	// END index
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSkillCastingEffectTableInfo)
	TAssetSubclassOf<ASkillCastingDecal> SkillCastingDecal = nullptr;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSkillCastingEffectTableInfo)
	//FLinearColor SkillCastingDecalColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSkillCastingEffectTableInfo)
	TSoftObjectPtr<UParticleSystem> ParticleSystem = nullptr;

	static FSkillCastingEffectTableInfo* LookUp(EDecalShapeEnum InDecalShape, ESkillDecalColorType InSkillDecalColorType);
};
