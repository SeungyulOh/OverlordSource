// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Components/ActorComponent.h"
#include "SkillCastingDecalComponent.h"
#include "SkillCastingEffectComponent.generated.h"

class ASkillCastingDecal;
class UParticleSystemComponent;
class UChildActorComponent;

/**
 * Effect for Skill Casting, for given shape, dimensions, duration, and the type of the character casting
 * Currently only handles shapes Circle, Box, Sector
 * Hosts one ASkillCastingDecal as actor and also hosts one UParticleSystemComponent as subcomponent.
 * looks up SkillCastingEffect DataTable for actual assets
 *
 * TODO: Handle bAirState for flying characters
 */
UCLASS()
class PROJECTR_API USkillCastingEffectComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	USkillCastingEffectComponent();

	void SetSkillDecalColorType(ESkillDecalColorType InDecalColorType);
	void SetIsHero(bool InIsHero);

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartSkillCasting(float InDuration, ESearchShapeTypeEnum ShapeType, float SectorAngle, float SkillApplyRange, float SkillApplyWidth, const FVector& CastingLocation);
	void EndSkillCasting();
	
	void SetCapsuleHeightOffset(FVector InCapsuleHeightOffset);

private:
	UPROPERTY()
	UChildActorComponent*	DecalActorComponent = nullptr;

	UPROPERTY()
	UParticleSystemComponent*	Particle_Deco = nullptr;

private:
	//// Set at the creation of the character actor
	ESkillDecalColorType SkillDecalColorType = ESkillDecalColorType::VE_HeroOwner;
	bool IsHero = true;

	//// Animation state
	bool IsActiveComponent = false;

	float Duration = 1.0f;
	float CurrentTime = 0.0f;

	FVector CapsuleHeightOffset = FVector::ZeroVector;
};
