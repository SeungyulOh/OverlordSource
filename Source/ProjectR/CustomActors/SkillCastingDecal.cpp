// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillCastingDecal.h"

#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"

#define SKILL_CASTING_PROGRESS_PARAMETER TEXT("SkillRangeProgress")
#define SKILL_CASTING_COLOR_PARAMETER TEXT("Color_1")

// Sets default values
ASkillCastingDecal::ASkillCastingDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASkillCastingDecal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASkillCastingDecal::StartAnimation(float InDuration)
{
	Duration = InDuration;
	CurrentAlpha = 0.0f;

	bCurrentlyProgressing = true;
}

// Called when the game starts or when spawned
void ASkillCastingDecal::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> DecalComponents = GetComponentsByClass(UDecalComponent::StaticClass());
	for (UActorComponent* DecalComponent : DecalComponents)
	{
		auto DecalComp = Cast<UDecalComponent>(DecalComponent);

		DecalComp->CreateDynamicMaterialInstance();

		auto Material = Cast<UMaterialInstanceDynamic>(DecalComp->GetDecalMaterial());
		if (!IsValid(Material))
			return;
	}

	if (bTesting)
	{
		bCurrentlyProgressing = true;
	}
}

// Called every frame
void ASkillCastingDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCurrentlyProgressing)
		return;

	CurrentAlpha += DeltaTime / Duration;

	if (CurrentAlpha > 1.0)
	{
		if (bTesting)
		{
			CurrentAlpha = 0.0f;
		}
		else
		{
			CurrentAlpha = 1.0f;
			bCurrentlyProgressing = false;
		}
	}

	float Progress = UKismetMathLibrary::Ease(0.0f, 1.0f, CurrentAlpha, InterpFunction, InterpBlendExp);

	TArray<UActorComponent*> DecalComponents = GetComponentsByClass(UDecalComponent::StaticClass());
	for (UActorComponent* DecalComponent : DecalComponents)
	{
		auto DecalComp = Cast<UDecalComponent>(DecalComponent);

		auto DynamicInstance = Cast<UMaterialInstanceDynamic>(DecalComp->GetDecalMaterial());
		if (!IsValid(DynamicInstance))
		{
			UE_LOG(LogGamePlay, Log, TEXT("ASkillCastingDecal::Tick: failed to get DynamicInstance"));
			continue;
		}

		DynamicInstance->SetScalarParameterValue(FName(SKILL_CASTING_PROGRESS_PARAMETER), Progress);

		FLinearColor DecalColor = FLinearColor::Red;
		switch (SkillDecalColorType)
		{
		case ESkillDecalColorType::VE_HeroOwner:
			DecalColor = DecalColor_HeroOwner;
			break;
		case ESkillDecalColorType::VE_Npc_Enemy:
			DecalColor = DecalColor_Npc_Enemy;
			break;
		case ESkillDecalColorType::VE_Npc_Friend:
			DecalColor = DecalColor_Npc_Friend;
			break;
		case ESkillDecalColorType::VE_Npc_Akhan:
			DecalColor = DecalColor_Npc_Akhan;
			break;
		case ESkillDecalColorType::VE_Npc_Gigant:
			DecalColor = DecalColor_Npc_Gigant;
			break;
		default:
			UE_LOG(LogGamePlay, Log, TEXT("ASkillCastingDecal::Tick: unhandled SkillDecalColorType"));
			break;
		}
		DynamicInstance->SetVectorParameterValue(FName(SKILL_CASTING_COLOR_PARAMETER), DecalColor);
	}
}

