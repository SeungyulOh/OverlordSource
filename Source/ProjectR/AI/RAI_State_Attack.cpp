// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Attack.h"

#include "RAI_Blackboard.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_Attack::Update(float DeltaTime)
{
	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState != EAIStateTypeEnum::VE_None)
		return ResultAbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_None)
		return ResultSkillCheck;

	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_Idle;

	UAnimStateComponent* Owner_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(Owner_AnimStateComponent))
	{
		if (false == Owner_AnimStateComponent->IsCancelToCurrentAnim())
		{
			return EAIStateTypeEnum::VE_None;
		}
	}

			
	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();	
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType() )
	{
		return ResultBlackCheck;
	}

	/*
	Check force target, if force target is set, change current target actor to force target.
	*/
	EAIStateTypeEnum CheckForceTargetResult = CheckForceTarget( TargetActor.Get() );
	if (CheckForceTargetResult != EAIStateTypeEnum::VE_None)
		return CheckForceTargetResult;


	// Is valid target actor?	
	if (IsValidTargetActor() == false)
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		return EAIStateTypeEnum::VE_Idle;
	}

	/*
	Check follow summoner
	*/
	/*EAIStateTypeEnum FollowSummonerResult = FollowSummoner();
	if (FollowSummonerResult != EAIStateTypeEnum::VE_None)
		return FollowSummonerResult;*/

	// Target is in attack range?
	UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(Owner_BattleEntityComponent) && Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get(), 0.0f, 30.0f ) == false)
		return EAIStateTypeEnum::VE_MoveToTarget;

	// Search nearer than current target
	if( KeepTarget == false )
	{
		AActor* ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get());
		if (IsValid(ResultActor))
		{
			// Attack new target
			if (ResultActor != TargetActor.Get())
			{
				if (OwnerBlackboard.IsValid())
				{
					OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor );
					return EAIStateTypeEnum::VE_MoveToTarget;
				}
			}
		}
	}

	UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor.Get());

	return EAIStateTypeEnum::VE_None;
}



bool URAI_State_Attack::IsValidTargetActor()
{	
	if (TargetActor.IsValid() == false)
		return false;

	UBattleEntityComponent* Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
	if (Target_BattleEntityComponent)
	{
		if (Target_BattleEntityComponent->IsDead())
		{
			return false;
		}

		if (Target_BattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_hide))
			return false;
	}

	return true;
}

void URAI_State_Attack::OnEnter()
{
	InitInternalData();

	if (OwnerBlackboard.IsValid())
	{
		// Is set force target?
		if (OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget))
		{
			KeepTarget = true;
			TargetActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, TargetActor.Get());
		}
		else
		{
			KeepTarget = false;
			TargetActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);			
		}
		//Attack();
	}	
}

void URAI_State_Attack::OnLeave()
{
	TargetActor = nullptr;
	KeepTarget = false;

	UAIFunctionLibrary::StopAttack(OwnerActor.Get());
}

void URAI_State_Attack::Attack()
{
	if (OwnerBlackboard.IsValid())
	{		
		UAIFunctionLibrary::Attack( OwnerActor.Get(), TargetActor.Get() );
	}
}