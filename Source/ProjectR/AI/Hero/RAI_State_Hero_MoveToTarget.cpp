// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Hero_MoveToTarget.h"

#include "AI/RAI_Blackboard.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_Hero_MoveToTarget::Update( float DeltaTime )
{
	UpdateInternal_BeforeUpdate();

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
		return ResultBlackCheck;

	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget))
	{
		AActor* ForceTarget = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		if (IsValid(ForceTarget))
		{
			if (TargetActor != ForceTarget)
			{
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ForceTarget);
				return EAIStateTypeEnum::VE_MoveToTarget;
			}
		}
	}

	if (IsValidTargetActor() == false)
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		return EAIStateTypeEnum::VE_Idle;
	}



	/*
	Check close enough to attack
	*/
	auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (Owner_BattleEntityComponent )
	{
		bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), TargetActor.Get()));

		if (IsException == false)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get()))
			{
				if (UAIFunctionLibrary::AvoidOtherCharacter(OwnerActor.Get()) == false)
				{
					if (Owner_BattleEntityComponent->IsEnemy(TargetActor.Get()))
					{
						Stop();
						return EAIStateTypeEnum::VE_Attack;
					}
					else
						return EAIStateTypeEnum::VE_Idle;
				}
			}
		}
		else
			return EAIStateTypeEnum::VE_Idle;
	}

	// search enemy again
	// !!! do below code at regular intervals
	if (KeepTarget == false )
	{
		// Check focus attack
		AActor* ResultActor = UAIFunctionLibrary::CheckFocusTarget(OwnerActor.Get());
		if (ResultActor)
		{
			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);
			return EAIStateTypeEnum::VE_MoveToTarget;
		}
	}	

	// if owner is blocked by something..
	if (CheckStopState(DeltaTime))
	{
		Stop();
		return EAIStateTypeEnum::VE_Idle;
	}

	if (CanMove() == false)
	{
		Stop();
		return EAIStateTypeEnum::VE_Idle;
	}

	//UAIFunctionLibrary::PushOverlappedCharacter(OwnerActor.Get());

	UpdateInternal_AfterUpdate();

	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State_Hero_MoveToTarget::UpdateTargetActor(AActor* InTarget)
{

	return EAIStateTypeEnum::VE_None;
}