 
#include "ProjectR.h"
#include "SkillCastingEffectComponent.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Table/SkillCastingEffectTableInfo.h"

USkillCastingEffectComponent::USkillCastingEffectComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void USkillCastingEffectComponent::SetSkillDecalColorType(ESkillDecalColorType InDecalColorType)
{
	SkillDecalColorType = InDecalColorType;
}

void USkillCastingEffectComponent::SetIsHero(bool InIsHero)
{
	IsHero = InIsHero;
}

// Called every frame
void USkillCastingEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false == IsActiveComponent)
		return;

	CurrentTime += DeltaTime;

	if (CurrentTime >= Duration)
	{
		EndSkillCasting();
		return;
	}
}

void USkillCastingEffectComponent::StartSkillCasting(float InDuration, ESearchShapeTypeEnum ShapeType, float SectorAngle, float SkillApplyRange, float SkillApplyWidth, const FVector& CastingLocation)
{
	// early end of previous skill casting
	if (IsActiveComponent)
	{
		EndSkillCasting();
	}

	if (!IsValid(DecalActorComponent))
	{
		DecalActorComponent = NewObject<UChildActorComponent>(GetOwner());
		DecalActorComponent->OnComponentCreated();
		DecalActorComponent->RegisterComponent();
		DecalActorComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// reset progress state
	Duration = InDuration;
	CurrentTime = 0.0f;

	// Look up SkillCastingEffect table
	EDecalShapeEnum DecalShape = EDecalShapeEnum::Sector90;
	switch (ShapeType)
	{
	case ESearchShapeTypeEnum::VE_Circle:
		DecalShape = EDecalShapeEnum::Sector360;
		break;

	case ESearchShapeTypeEnum::VE_Sector:
		if (SectorAngle == 90.0f)
		{
			DecalShape = EDecalShapeEnum::Sector90;
		}
		else if (SectorAngle == 180.0f)
		{
			DecalShape = EDecalShapeEnum::Sector180;
		}
		else
		{
			UE_LOG(LogGamePlay, Log, TEXT("USkillCastingEffectComponent::StartSkillCasting: unhandled SectorAngle"));
			return;
		}
		break;

	case ESearchShapeTypeEnum::VE_Box:
		DecalShape = EDecalShapeEnum::Rectangle;
		break;

	default:
		UE_LOG(LogGamePlay, Log, TEXT("USkillCastingEffectComponent::StartSkillCasting: unhandled ShapeType"));
		return;
	}

	FSkillCastingEffectTableInfo* EffectInfo = FSkillCastingEffectTableInfo::LookUp(DecalShape, SkillDecalColorType);
	if (EffectInfo == nullptr)
	{
		UE_LOG(LogGamePlay, Log, TEXT("USkillCastingEffectComponent::StartSkillCasting: failed to look up EffectInfo"));
		return;
	}

	// Set SkillCastingDecal actor
	if (!EffectInfo->SkillCastingDecal.IsNull())
	{
		TSubclassOf<ASkillCastingDecal> SkillCastingDecalBP = EffectInfo->SkillCastingDecal.Get();
		if (!IsValid(SkillCastingDecalBP))
		{
			SkillCastingDecalBP = EffectInfo->SkillCastingDecal.LoadSynchronous();
		}
		DecalActorComponent->SetChildActorClass(SkillCastingDecalBP.Get());

		auto SkillCastingDecal = Cast<ASkillCastingDecal>(DecalActorComponent->GetChildActor());
		if (!IsValid(SkillCastingDecal))
		{
			UE_LOG(LogGamePlay, Log, TEXT("USkillCastingEffectComponent::StartSkillCasting: failed to get SkillCastingDecal"));
			return;
		}
		SkillCastingDecal->SkillDecalColorType = SkillDecalColorType;
		SkillCastingDecal->StartAnimation(InDuration);

		// Adjust scale
		switch (ShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		case ESearchShapeTypeEnum::VE_Sector:
			DecalActorComponent->SetRelativeScale3D(FVector(1.0, SkillApplyRange * 0.01, SkillApplyRange * 0.01));
			break;

		case ESearchShapeTypeEnum::VE_Box:
			DecalActorComponent->SetRelativeScale3D(FVector(1.0, SkillApplyRange * 0.01, SkillApplyWidth * 0.01));
			break;
		}
	}


	// Adjust location
	if (CastingLocation.IsNearlyZero())
	{
		SetRelativeLocation(CapsuleHeightOffset);
	}
	else
	{
		//TODO: Location different.
		//CastingLocation.Z += CapsuleHeightOffset.Z;
		SetWorldLocation(CastingLocation);
	}

	// Particle_Deco
	if (IsHero && !EffectInfo->ParticleSystem.IsNull())
	{
		UParticleSystem* ParticleSystem = EffectInfo->ParticleSystem.Get();
		if (ParticleSystem == nullptr)
		{
			ParticleSystem = EffectInfo->ParticleSystem.LoadSynchronous();
		}

		Particle_Deco = UGameplayStatics::SpawnEmitterAttached(ParticleSystem, this);

		Particle_Deco->SetRelativeScale3D(FVector(SkillApplyRange * 0.01, SkillApplyRange * 0.01, 1.0));		
	}

	IsActiveComponent = true;

	SetVisibility(true);
}

void USkillCastingEffectComponent::EndSkillCasting()
{
	if (!IsActiveComponent)
		return;

	// only handle Particle_Deco as ASkillCastingDecal already handles duration.

	SetVisibility(false);

	IsActiveComponent = false;

	if (IsValid(Particle_Deco))
	{
		Particle_Deco->DestroyComponent();
		Particle_Deco = nullptr;
	}

	if (IsValid(DecalActorComponent))
	{
		DecalActorComponent->DestroyComponent();
		DecalActorComponent = nullptr;
	}
}

void USkillCastingEffectComponent::SetCapsuleHeightOffset(FVector InCapsuleHeightOffset)
{
	CapsuleHeightOffset = InCapsuleHeightOffset;
}
