// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "FadableStaticMeshActor.h"

#include "Components/StaticMeshComponent.h"


AFadableStaticMeshActor::AFadableStaticMeshActor()
{
	// For fading animation
	PrimaryActorTick.bCanEverTick = true;
}

void AFadableStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material instance for each material
	TArray<UMaterialInterface*> Materials = GetStaticMeshComponent()->GetMaterials();

	for (int32 i = 0; i < Materials.Num(); i++)
	{
		GetStaticMeshComponent()->CreateDynamicMaterialInstance(i);
	}
}

void AFadableStaticMeshActor::Tick(float DeltaSeconds)
{
	if (!bCurrentlyAnimating)
		return;


	Cur += (DeltaSeconds * Multiplier) / AnimationDuration;

	float Opacity = UKismetMathLibrary::Ease(MinimumOpacity, 1, Cur, InterpFunction, InterpBlendExp);

	TArray<UMaterialInterface *> Materials = GetStaticMeshComponent()->GetMaterials();
	for (UMaterialInterface* Material : Materials)
	{
		auto DynamicInstance = Cast<UMaterialInstanceDynamic>(Material);
		if (!IsValid(DynamicInstance))
		{
			UE_LOG(LogGamePlay, Log, TEXT("AFadableStaticMeshActor::Tick: failed to get MaterialDynamicInstance"));
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

void AFadableStaticMeshActor::FadeOut()
{
	Multiplier = -1.0;
	bCurrentlyAnimating = true;
}

void AFadableStaticMeshActor::FadeIn()
{
	Multiplier = 1.0;
	bCurrentlyAnimating = true;
}
