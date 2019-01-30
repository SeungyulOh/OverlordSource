// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "ContinuousSkillApplierActor.h"

#include "UtilFunctionIntegrated.h"



// Sets default values
AContinuousSkillApplierActor::AContinuousSkillApplierActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UseOnlyEffect = false;
}

// Called when the game starts or when spawned
void AContinuousSkillApplierActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AContinuousSkillApplierActor::Tick( float DeltaTime )
{
	if (false == Caster.IsValid())
	{
		DespawnSkill();
		return;
	}

	Super::Tick( DeltaTime );

	UpdateLocation();
	CheckApplyTickTime(DeltaTime); 

	// if Duration is less than 0, this skill lifetime is infinite.
	if (Duration > 0.0f)
	{
		if (IsEnd(DeltaTime))
		{
			DespawnSkill();
			return;
		}
	}
}

void AContinuousSkillApplierActor::UpdateLocation()
{
	if (AttachedActor.IsValid())
	{
		TargetLocation = AttachedActor->GetActorLocation();
		if (TargetSkill.IsValid())
		{
			TargetSkill->SetTargetLocation(TargetLocation);
		}
	}
}

void AContinuousSkillApplierActor::SpawnSkill(UISkill_V2* InSkill, FVector InTargetLocation, AActor* InAttachTarget )
{
	TargetSkill = InSkill;
	ParentSkillID = TargetSkill->GetSkillID();
	this->Caster = TargetSkill->GetCaster();
	this->TargetLocation = InTargetLocation;
	TargetLocation.Z += 20.0f;

	if (TargetSkill->IsPassiveSkill())
	{
		Duration = -1.0f;
		ApplyTickTime = TargetSkill->GetSkillInfo()->Interval;
		TickCheckElapsedTime = ApplyTickTime;
	}
	else
	{
		// Seperate GroundSkill Interval from Interval, Interval will be used for Slot-Duration & Aura.
		if (TargetSkill->GetSkillInfo()->CastingType == ESkillCastingTypeEnum::VE_Channeling)
		{
			Duration = TargetSkill->GetSkillInfo()->CastingTime;
			ApplyTickTime = TargetSkill->GetSkillInfo()->Interval;
			if (FMath::IsNearlyZero(ApplyTickTime)) ApplyTickTime = 1.0f;
			TickCheckElapsedTime = 0.0f;
		}
		else
		{
			Duration = TargetSkill->GetSkillInfo()->GroundDuration;
			ApplyTickTime = TargetSkill->GetSkillInfo()->GroundInterval;
			if (FMath::IsNearlyZero(ApplyTickTime)) 
			{
				// if GroundInterval equal to 0.0f, apply skill immediatly once. And keep effects until GroundDuration ends.
				TickCheckElapsedTime = Duration;
				ApplyTickTime = Duration;
			} 
			else if(ApplyTickTime < 0.0f)
			{
				// apply at the start of duration. and it does not fire at the ends.
				TickCheckElapsedTime = Duration;
				// pending 100ms
				ApplyTickTime *= -1.1f; 
			} 
			else 
			{
				// normaly apply effects at every intervals.
				TickCheckElapsedTime = 0.0f;
			}
		}
	}
	ElapsedTime = 0.0f;
	AttachedActor = InAttachTarget;
	SpawnEffect(TargetSkill.Get(), TargetLocation, InAttachTarget);
}

void AContinuousSkillApplierActor::SpawnEffect(UISkill_V2* InSkill, FVector InTargetLocation, AActor* InAttachTarget )
{
	if (IsValid(InSkill))
	{	
		if (InSkill->GetSkillResourceInfo() && InSkill->GetSkillResourceInfo()->AreaEffect.IsNull() == false)
		{
			if (InAttachTarget == nullptr)
				SkillEffect = UUtilFunctionLibrary::CustomSpawnEmitterAtLocation(this, UUtilFunctionLibrary::GetParticleSystemFromAssetID(InSkill->GetSkillResourceInfo()->AreaEffect), InTargetLocation, Caster->GetActorRotation() );
			else
				SkillEffect = UUtilFunctionLibrary::CustomSpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(InSkill->GetSkillResourceInfo()->AreaEffect), InAttachTarget->GetRootComponent(), SOCKET_ROOT, FVector::ZeroVector, FRotator::ZeroRotator);
		}	

		//Decal Color Setting
		auto OwnerActor = this->GetOwner();
		if (IsValid(OwnerActor) && IsValid(SkillEffect))
		{
			auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor);
			if (BattleEntityComp->AmIEnemyWithMyPlayer())
			{
				SkillEffect->SetVectorParameter(FName("SelectionColor"), EnemyTeamSkillAreaColor_Vector);
			}
			else
			{
				SkillEffect->SetVectorParameter(FName("SelectionColor"), OurTeamSkillAreaColor_Vector);
			}
		}
	}
}

void AContinuousSkillApplierActor::DespawnSkill()
{
	if (IsValid(SkillEffect))
		SkillEffect->DestroyComponent();

	this->Destroy();
}

void AContinuousSkillApplierActor::ClearSkillEffectForced()
{
	if (IsValid(SkillEffect))
	{
		SkillEffect->ActivateSystem(false);
		SkillEffect->DeactivateSystem();
		SkillEffect->KillParticlesForced();
		SkillEffect->DestroyComponent();
		SkillEffect = nullptr;
	}
}

bool AContinuousSkillApplierActor::IsEnd(float DeltaTime)
{	
	ElapsedTime += DeltaTime;

	if (ElapsedTime > Duration)
		return true;

	return false;
}

void AContinuousSkillApplierActor::CheckApplyTickTime(float DeltaTime)
{
	if (UseOnlyEffect)
		return;

	TickCheckElapsedTime += DeltaTime;

	if (TickCheckElapsedTime > ApplyTickTime)
	{
		if (TargetSkill.IsValid())
		{				
			TargetSkill->GetApplyTarget();
			TargetSkill->Apply();
			// Not needed target skill apply.. ex) summon
			/*USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
			if(IsValid(SkillComponent))
				SkillComponent->ApplyInstanceApplierGroup(Caster.Get(), TargetSkill->GetTarget(), TargetSkill->GetSkillInfo(), false, TargetSkill.Get());*/

			// !!! If target is passive aura, show action effect per apply tick.
			if( TargetSkill->IsPassiveSkill() )
				TargetSkill->AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);

			//auto CasterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
			//if (CasterSkillComp)
			//{
			//	CasterSkillComp->OnNotifySkillCasting.Broadcast(Caster.Get(), TargetSkill.Get());
			//}
		}
		TickCheckElapsedTime = 0.0f;
	}
}

void AContinuousSkillApplierActor::BeginDestroy()
{
	if (IsValid(SkillEffect))
		SkillEffect->DestroyComponent();

	Super::BeginDestroy();
}
