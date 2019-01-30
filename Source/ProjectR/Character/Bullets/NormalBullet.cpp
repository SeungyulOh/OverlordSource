// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "NormalBullet.h"

#include "Character/BattleObject/BattleBaseObject.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"


#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Table/BulletTableInfo.h"


ANormalBullet::ANormalBullet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InitMembers();
}

void ANormalBullet::Init()
{
	Super::Init();

	BulletSpeed = 3000.0f;
	IsActiveBullet = false;
}

void ANormalBullet::InitVelocity(FVector Dir, float TargetBulletSpeed)
{
	
}

void ANormalBullet::SetActiveBullet(bool ActiveBullet)
{
	IsActiveBullet = ActiveBullet;
}

void ANormalBullet::Tick(float DeltaTime)
{
	bool bFlying = MoveBullet(DeltaTime);

	if (bBulletHit && false == bApplyHit)
	{
		if (IsNormalAttack)
		{
			HitTargetNormalAttack();
		}
		else if (OwnerSkill_V2)
		{
			OwnerSkill_V2->ApplySkillBullet();
		}
		bApplyHit = true;
	}

	if (false == bFlying)
	{
		Destroy();
	}
}

//////////////////////////////////////////////////////////////////////////
//
// NormalBullet V2
//
//////////////////////////////////////////////////////////////////////////
void ANormalBullet::InitBullet(AActor* Shooter, float ProjectileSpeed, FVector ShotPos, AActor* ActorTarget, FVector LocationTarget)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	if (IsValid(Shooter))
	{
		OwnerCharacter = Shooter;

		float RealBulletSpeed = BulletSpeed;
		if (0.0f < ProjectileSpeed)
		{
			RealBulletSpeed = ProjectileSpeed;
		}
		BulletSpeed = RealBulletSpeed * RGameInstance->GameEnvironmentVar.BulletSpeedMultifier;

		ShotPosition = ShotPos;

		FVector OwnerLoc = OwnerCharacter->GetActorLocation();
		FVector BulletLoc = GetActorLocation();
		BulletHeight = BulletLoc.Z - OwnerLoc.Z;

		SetBulletAimTarget(ActorTarget, LocationTarget);

		IsActiveBullet = true;
	}
}

void ANormalBullet::SetNormalAttackBullet_V2()
{
	IsNormalAttack = true;
}

void ANormalBullet::SetSkillAttackBullet(UISkill_V2* Skill, float TargetDist)
{
	if (IsValid(Skill))
	{
		OwnerSkill_V2 = Skill;
		FirstHitTargetDist = TargetDist;
	}
}

void ANormalBullet::HitTargetNormalAttack()
{
	if (OwnerCharacter.IsValid())
	{
		auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		if (IsValid(Owner_BattleEntityComponent))
		{
			if (ECharacterPropertyTypeEnum::VE_PhysicalAttackPower == Owner_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackPropertyType)
			{
				Owner_BattleEntityComponent->HitTargetWithBullet(AimTarget.Get(), true);
			}
			else
			{
				Owner_BattleEntityComponent->HitTargetWithBullet(AimTarget.Get(), false);
			}
		}
	}
}

bool ANormalBullet::MoveBullet(float DeltaTime)
{
	BulletHitLimitTime -= DeltaTime;
	if (0.0f >= BulletHitLimitTime)
	{
		return false;
	}

	FVector BulletLoc = GetActorLocation();
	FVector TargetLoc = FVector::ZeroVector;
	if (AimTarget.IsValid())
	{
		TargetLoc = AimTarget->GetActorLocation();
		TargetLoc.Z = TargetLoc.Z + BulletHeight;
	}
	else if (false == AimLocation.IsNearlyZero())
	{
		TargetLoc = AimLocation;
	}

	FVector FlyDir = TargetLoc - BulletLoc;
	float RemainDist = FlyDir.Size();
	FlyDir.Normalize();
	float MovableDist = DeltaTime * BulletSpeed;

	if (0.0f < FirstHitTargetDist)
	{
		FirstHitTargetDist -= MovableDist;
		if (0.0f >= FirstHitTargetDist)
		{
			bBulletHit = true;
		}
	}

	if (50.0f < RemainDist || MovableDist < RemainDist)
	{
		FVector NextBulletPos = BulletLoc + (FlyDir * MovableDist);
		FRotator NextBulletRot = FRotationMatrix::MakeFromX(FlyDir).Rotator();
		SetActorRotation(NextBulletRot);
		SetActorLocation(NextBulletPos, false, nullptr, ETeleportType::TeleportPhysics);
		return true;
	}

	if (IsNormalAttack || (OwnerSkill_V2 && false == bBulletHit))
	{
		bBulletHit = true;
	}

	return false;
}

void ANormalBullet::SetBulletAimTarget(AActor* ActorTarget, FVector LocationTarget)
{
	AimTarget = ActorTarget;
	AimLocation = LocationTarget;

	float FlyDist = 0.0f;
	if (AimTarget.IsValid())
	{
		FlyDist = FVector::Dist(ShotPosition, AimTarget->GetActorLocation());
	}
	else if (false == AimLocation.IsNearlyZero())
	{
		FlyDist = FVector::Dist(ShotPosition, AimLocation);
	}
	BulletHitLimitTime = (FlyDist / BulletSpeed) + 0.1f;
}
