// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "FadableSkeletalMeshActor.h"

#include "Components/SkeletalMeshComponent.h"


AFadableSkeletalMeshActor::AFadableSkeletalMeshActor()
{
	// For fading animation
	PrimaryActorTick.bCanEverTick = true;
}

void AFadableSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material instance for each material
	TArray<UMaterialInterface*> Materials = GetSkeletalMeshComponent()->GetMaterials();

	for (int32 i = 0; i < Materials.Num(); i++)
	{
		GetSkeletalMeshComponent()->CreateDynamicMaterialInstance(i);
	}
}

void AFadableSkeletalMeshActor::Tick(float DeltaSeconds)
{
	if (!bCurrentlyAnimating)
		return;


	Cur += (DeltaSeconds * Multiplier) / AnimationDuration;

	float Opacity = UKismetMathLibrary::Ease(MinimumOpacity, 1, Cur, InterpFunction, InterpBlendExp);

	TArray<UMaterialInterface *> Materials = GetSkeletalMeshComponent()->GetMaterials();
	for (UMaterialInterface* Material : Materials)
	{
		auto DynamicInstance = Cast<UMaterialInstanceDynamic>(Material);
		if (!IsValid(DynamicInstance))
		{
			UE_LOG(LogGamePlay, Log, TEXT("AFadableSkeletalMeshActor::Tick: failed to get MaterialDynamicInstance"));
			continue;
		}

		DynamicInstance->SetScalarParameterValue(FName(TEXT("FadeOpacity")), Opacity);
	}

	if (Cur >= 1.0 || Cur <= 0.0)
	{
		bCurrentlyAnimating = false;
	}
	Cur = FMath::Clamp(Cur, 0.0f, 1.0f);
}

void AFadableSkeletalMeshActor::FadeOut()
{
	Multiplier = -1.0;
	bCurrentlyAnimating = true;
}

void AFadableSkeletalMeshActor::FadeIn()
{
	Multiplier = 1.0;
	bCurrentlyAnimating = true;
}
