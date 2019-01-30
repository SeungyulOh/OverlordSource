// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "SkillFireNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API USkillFireNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};


UCLASS()
class PROJECTR_API USkillCastingNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};


UCLASS()
class PROJECTR_API UPointLightNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointLightDynamic")
	float LifeTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointLightDynamic")
	float AttenuationRadius = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointLightDynamic")
	FLinearColor LightColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointLightDynamic")
	float LightIntensity = 200.f;
};

