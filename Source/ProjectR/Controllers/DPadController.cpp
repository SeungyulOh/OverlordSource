// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/BaseAIController.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/SharedBattle/BattleInfoBlackboard.h"

#include "Character/AnimState/IAnimState.h"


#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
//


#include "GlobalIntegrated.h"

#include "UI/UI_Battle/RAttackIcon.h"
#include "SVirtualJoystick.h"
#include "DPadController.h"
#include "UtilFunctionIntegrated.h"


ADPadController::ADPadController(const FObjectInitializer& ObjectInitializer) : Super( ObjectInitializer )
{	
}


bool ADPadController::AttackProcess()
{
	if (/*false == SelectCharacter.IsValid()*/ LeaderCharacter == nullptr)
		return false;

	bool AttackResult = true;// AttackToNearestTarget();
// 	bool AttackResult = AttackToCurrentTarget();
// 	if (false == AttackResult)
// 	{
// 		AttackResult = AttackToNearestTarget();
// 	}

	return AttackResult;
}

bool ADPadController::AttackHoldProcess()
{
	if (/*false == SelectCharacter.IsValid()*/ LeaderCharacter == nullptr)
		return false;

	bool AttackResult = AttackToCurrentTarget();

	return AttackResult;
}

bool ADPadController::AttackToCurrentTarget()
{
	//return true;
	if (LeaderCharacter.IsValid() == false )
		return false;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
	if (false == IsValid(BattleEntityComponent))
		return false;

	AActor* CurrentTarget = BattleEntityComponent->GetCurrentAttackTarget();
	if (CurrentTarget)
	{
		if (URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(LeaderCharacter.Get(), CurrentTarget))
		{
			if (BattleEntityComponent->IsCloseEnoughForAttack(CurrentTarget))
			{
				BattleEntityComponent->EnterAttack();
				return true;
			}
		}
	}

	return false;
}

bool ADPadController::AttackToNearestTarget()
{
	if (/*false == SelectCharacter.IsValid()*/ LeaderCharacter == nullptr)
		return false;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
	if (false == IsValid(BattleEntityComponent))
		return false;
	auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
	if (false == IsValid(EntityAIComponent))
		return false;
	
	AActor* NearestTarget = EntityAIComponent->GetNearestSearchTarget();
		
	if (BattleEntityComponent->IsCloseEnoughForAttack(NearestTarget))
	{
		BattleEntityComponent->SetCurrentAttackTarget(NearestTarget);
		BattleEntityComponent->EnterAttack();
		return true;
	}

	return false;
}

