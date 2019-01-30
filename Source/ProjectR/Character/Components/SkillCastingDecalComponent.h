// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Components/StaticMeshComponent.h"
#include "SkillCastingDecalComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillDecalColorType : uint8
{
	VE_HeroOwner			UMETA(DisplayName = "HeroOwner"),
	VE_Npc_Enemy			UMETA(DisplayName = "Npc_Enemy"),
	VE_Npc_Friend			UMETA(DisplayName = "Npc_Friend"),
	VE_Npc_Akhan			UMETA(DisplayName = "Npc_Akhan"),
	VE_Npc_Gigant			UMETA(DisplayName = "Npc_Gigant"),
};

UCLASS()
class PROJECTR_API USkillCastingDecalComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USkillCastingDecalComponent();

	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetOwner(AActor* InOwner);

	void StartSkillCasting();
	void EndSkillCasting();
	
	void ResetData(float CastingTime = 0.0f);
	void SetShapeType(ESearchShapeTypeEnum InType, float InShapeParam);
	void ShowSkillArea(float InRangeX, float InRangeY);
	void SetCastingLocation(FVector& CastingLocation);

	void SetSkillDecalColorType(ESkillDecalColorType InType) { DecalColorType = InType; }

	void SetIsHero(bool InIsHero) { IsHero = InIsHero; }
	void SetAddAirOffset(FVector InAddAirOffset) { AddAirOffset = InAddAirOffset; }

public:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*>	StaticMeshMaterialList;
	UPROPERTY()
	FName						ScalarParmName;
	UPROPERTY()
	float						ScalarParm = 0.0f;

	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	UPROPERTY()
	ESearchShapeTypeEnum		SkillShapeType = ESearchShapeTypeEnum::VE_None;
	UPROPERTY()
	float						SkillShapeParam = 0.0f;

	UPROPERTY()
	UParticleSystemComponent*	Particle_Deco = nullptr;

	UPROPERTY()
	FName						EffectName;

	UPROPERTY()
	float						CheckTime = 0.0f;
	UPROPERTY()
	float						CastingMaxTime = 0.0f;
	UPROPERTY()
	bool						IsActiveComponent = false;

	UPROPERTY()
	float						ZPos_SkillArea = 5.0f;
	UPROPERTY()
	FVector						CapsuleHeightOffset = FVector::ZeroVector;

	ESkillDecalColorType		DecalColorType = ESkillDecalColorType::VE_HeroOwner;
	bool						IsHero = false;

private:
	FVector						AddAirOffset = FVector::ZeroVector;
};
