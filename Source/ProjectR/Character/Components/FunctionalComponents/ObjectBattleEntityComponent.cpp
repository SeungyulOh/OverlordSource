// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ObjectBattleEntityComponent.h"

#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "UtilFunctionIntegrated.h"

// Sets default values for OwnerActor.Get() component's properties
UObjectBattleEntityComponent::UObjectBattleEntityComponent()
{
	
}


bool UObjectBattleEntityComponent::IsSearchSkip()
{
	if (OwnerActor.IsValid() == false)
		return false;

	ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(OwnerActor.Get());
	if (IsValid(BattleObject))
	{
		switch (BattleObject->BattleObjectType)
		{
		case EBattleObjectTypeEnum::VE_FloorType:
		case EBattleObjectTypeEnum::VE_GimmickType:
		case EBattleObjectTypeEnum::VE_TrapType:
			return true;
		}
	}

	return false;
}

bool UObjectBattleEntityComponent::IsDead()
{
	if (OwnerActor.IsValid() == false)
		return false;

	ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(OwnerActor.Get());
	if (IsValid(BattleObject))
	{
		switch (BattleObject->BattleObjectType)
		{
		case EBattleObjectTypeEnum::VE_CannonType:
		case EBattleObjectTypeEnum::VE_TotemType:
		case EBattleObjectTypeEnum::VE_TotemTickType:
		{
			return Super::IsDead();
		}
		break;
		}
	}

	return false;
}

FVector UObjectBattleEntityComponent::GetShotPointLocation(FName SocketName)
{
	if (OwnerActor.IsValid() == false)
		return FVector::ZeroVector;

	ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(OwnerActor.Get());
	if (IsValid(BattleObject))
	{
		return BattleObject->GetSocketLocation(SocketName);
	}

	return FVector::ZeroVector;
}

void UObjectBattleEntityComponent::EnterAttack()
{
	if (OwnerActor.IsValid() == false)
		return;

	if (UCharacterHelpFunctionLibrary::IsRemoteEntity(OwnerActor.Get()))
		return;

	/*
	TODO : Should i check skill state by myself?
	*/
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());

	if (IsValid(OwnerActor_SkillEntityComponent))
	{
		if (OwnerActor_SkillEntityComponent->IsSkillCasting())
			return;
		if (OwnerActor_SkillEntityComponent->IsSkillActing())
			return;
	}

	auto CurrentAttackTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget.Get());
	if (false == IsValid(CurrentAttackTarget_BattleEntityComponent))
		return;
	if (CurrentAttackTarget_BattleEntityComponent->IsDead())
		return;

	/*
	TODO : Rotate to target on attack start.
	*/

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartAttack);
	}
}

void UObjectBattleEntityComponent::Dead( bool InIsDestroy /*= false*/)
{
	Super::Dead();

	if (!OwnerActor.IsValid())
		return;

	ABattleBaseObject* BObject = Cast<ABattleBaseObject>(OwnerActor.Get());
	if (IsValid(BObject))
	{
		switch (BObject->BattleObjectType)
		{
		case EBattleObjectTypeEnum::VE_TotemType:
		{
			BObject->OverlapMap.Reset();
		}
		break;
		case EBattleObjectTypeEnum::VE_CannonType:
		{
			BObject->OnDead();
			UUtilFunctionLibrary::PlaySoundSceneComponent(TEXT("TowerExplosion"), BObject->GetRootComponent());
		}
		break;
		}
	}
}