// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "NextTargetActor.h"

#include "Character/BattleObject/BattleBaseObject.h"

#include "GlobalIntegrated.h"

#include "Object/BaseCameraActor.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


ANextTargetActor::ANextTargetActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#ifdef RULE_TIMERACTOR
	PrimaryActorTick.bCanEverTick = false;
#else
	PrimaryActorTick.bCanEverTick = true;
#endif

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Scene")));
	EffectComponent = ObjectInitializer.CreateDefaultSubobject< UParticleSystemComponent >(this, TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(RootComponent);

	ParentMeshComponent = ObjectInitializer.CreateDefaultSubobject< USceneComponent >(this, TEXT("MeshParent"));
	ParentMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject< UStaticMeshComponent >(this, TEXT("MeshComponent"));
	StaticMeshComponent->SetupAttachment(ParentMeshComponent);

	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
}

void ANextTargetActor::SetTarget(AActor* Source, AActor* Dest, float lifeTime)
{
	SetActorHiddenInGame(false);

	SourceObject = Source;
	DestObject = Dest;

	LifeTime = lifeTime;

	bSelfDestroy = true;
	if (LifeTime <= 0.0f)
	{
		bSelfDestroy = false;
	}
	
	UpdateEffect();

	//SetSelfDestory(1.0f);

	if (IsValid(BeamEffectComponent))
	{
		BeamEffectComponent->SetHiddenInGame(false);
	}
	InitActiveTickTimer(true);
}

void ANextTargetActor::Hide()
{
	bSelfDestroy = false;

	SourceObject = nullptr;
	DestObject = nullptr;

	SetActorHiddenInGame(true);
	
	if (IsValid(BeamEffectComponent))
	{
		BeamEffectComponent->SetHiddenInGame(true);
	}
	InitActiveTickTimer(false);
}

bool ANextTargetActor::IsUseAble()
{
	if (SourceObject == nullptr && DestObject == nullptr)
		return true;

	return false;
}

void ANextTargetActor::Tick(float DeltaSecond)
{
#ifndef RULE_TIMERACTOR
	Super::Tick(DeltaSecond);
#endif

	UpdateEffect();

	if (bSelfDestroy)
	{
		LifeTime -= DeltaSecond;
		if (LifeTime <= 0.0f)
		{
			Hide();

			RGAMEINSTANCE(this)->PlayStateInfo->SetbLeaderhasTarget(false);
		}
	}
	else
	{
		if (!SourceObject.IsValid() || !DestObject.IsValid())
		{
			Hide();
		}

		if (DestObject.IsValid())
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(DestObject.Get());
			if (IsValid(BattleEntityComponent))
			{
				if (BattleEntityComponent->IsDead())
				{
					Hide();
				}
			}
		}
	}
}

void ANextTargetActor::ActiveTick()
{
	Tick(TIMER_ACTIVE);
}

void ANextTargetActor::InitActiveTickTimer(bool InSetTimer)
{
#ifndef RULE_TIMERACTOR
	return;
#endif

	if (ActiveHandler.IsValid())
	{
		this->GetWorldTimerManager().ClearTimer(ActiveHandler);
	}
	if (InSetTimer)
	{
		this->GetWorldTimerManager().SetTimer(ActiveHandler, this, &ANextTargetActor::ActiveTick, TIMER_ACTIVE, true);
	}
}

void ANextTargetActor::UpdateEffect()
{
	if (!SourceObject.IsValid() || !DestObject.IsValid())
		return;

	if (!IsValid(EffectComponent))
		return;

	if (!IsValid(StaticMeshComponent))
		return;

	ABattleBaseObject* battleSourceObject = Cast<ABattleBaseObject>(SourceObject.Get());
	if (IsValid(battleSourceObject))
	{
		FVector pos = battleSourceObject->GetActorLocation();

		if (IsValid(battleSourceObject->GetStaticMesh()))
		{
			pos = battleSourceObject->GetStaticMesh()->GetSocketLocation(SOCKET_ROOT);
		}

		SetActorLocation(pos);
	}
	else
	{
		ACharacter* TargetSourceCharacter = Cast< ACharacter >(SourceObject.Get());
		if (IsValid(TargetSourceCharacter))
		{
			FVector pos = TargetSourceCharacter->GetActorLocation();

			if (IsValid(TargetSourceCharacter->GetMesh()))
			{
				pos = TargetSourceCharacter->GetMesh()->GetSocketLocation(SOCKET_ROOT);
			}

			SetActorLocation(pos);
		}			
	}

	float fScale = 1.0f;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(DestObject.Get());
	if (IsValid(BattleEntityComponent))
	{
		if (IsValid(BattleEntityComponent->GetBattleInfo()))
		{
			int32 unitRadius = BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<int32>(ECharacterPropertyTypeEnum::VE_UnitRadius);
			fScale = unitRadius / RADIUSSIZE_DEFAULT;
		}
	}

	FVector destPos = FVector::ZeroVector;
	ABattleBaseObject* battleObject = Cast<ABattleBaseObject>(DestObject.Get());
	if (IsValid(battleObject))
	{
		FVector pos = battleObject->GetActorLocation();
		//EffectComponent->SetBeamTargetPoint(0, pos, 0);
		UpdateStaticMesh(pos, fScale);

		if (IsValid(battleObject->GetStaticMesh()))
		{
			destPos = battleObject->GetStaticMesh()->GetSocketLocation(SOCKET_ROOT);
		}
	}
	else
	{
		ACharacter* TargetCharacter = Cast< ACharacter >(DestObject.Get());
		if (IsValid(TargetCharacter))
		{
			FVector pos = TargetCharacter->GetActorLocation();
			//EffectComponent->SetBeamTargetPoint(0, pos, 0);
			UpdateStaticMesh(pos, fScale);

			if (IsValid(TargetCharacter->GetMesh()))
			{
				destPos = TargetCharacter->GetMesh()->GetSocketLocation(SOCKET_ROOT);
			}
		}			
	}

	if (!destPos.IsNearlyZero())
	{
		if (IsValid(BeamEffectComponent))
		{
			BeamEffectComponent->SetBeamTargetPoint(0, destPos, 0);
		}
		else
		{
			UParticleSystem* BeamEffect = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("NextTargetBeam"));
			if (IsValid(BeamEffect))
			{				
				BeamEffectComponent = UUtilFunctionLibrary::CustomSpawnEmitterAttached(BeamEffect, RootComponent, SOCKET_ROOT, FVector::ZeroVector, FRotator::ZeroRotator);
				if (IsValid(BeamEffectComponent))
				{
					BeamEffectComponent->SetBeamTargetPoint(0, destPos, 0);
				}
			}
		}
	}
}

void ANextTargetActor::UpdateStaticMesh(FVector pos, float scale)
{
	if (IsValid(ParentMeshComponent))
	{
		ParentMeshComponent->SetWorldLocation(pos);

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->CameraActor))
		{
			FVector forward = pos - RGameInstance->CameraActor->GetActorLocation();
			forward.Normalize();
			FRotator rot = UKismetMathLibrary::Conv_VectorToRotator(forward);
			FVector scl = FVector(scale, scale, scale);
			ParentMeshComponent->SetWorldScale3D(scl);
			ParentMeshComponent->SetWorldRotation(rot.Quaternion());
		}
	}
}