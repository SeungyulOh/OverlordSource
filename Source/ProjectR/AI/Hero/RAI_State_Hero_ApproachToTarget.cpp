// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Hero_ApproachToTarget.h"
#include "UtilFunctionIntegrated.h"
#include "AI/RAI_Blackboard.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "GlobalIntegrated.h"


EAIStateTypeEnum URAI_State_Hero_ApproachToTarget::Update( float DeltaTime )
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

	

	AActor* SearchActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get() );
	if (IsValid(SearchActor))
	{
#if LOG_AI
		UE_LOG(LogBattle, Log, TEXT("********************************Update SearchEnemy %s %s"), *OwnerActor->GetName(), *SearchActor->GetName());
#endif
		OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, SearchActor);
		return EAIStateTypeEnum::VE_MoveToTarget;
	}

	
	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType())
		return ResultBlackCheck;

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
	if (Owner_BattleEntityComponent)
	{
		bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), TargetActor.Get()));

		if (IsException == false)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get(), ApproachDistance))
			{
				if (UAIFunctionLibrary::AvoidOtherCharacter(OwnerActor.Get()) == false)
				{
					if (Owner_BattleEntityComponent->IsEnemy(TargetActor.Get()))
						return EAIStateTypeEnum::VE_Attack;
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
	if (KeepTarget == false)
	{
		// Check focus attack
		AActor* ResultActor = UAIFunctionLibrary::CheckFocusTarget(OwnerActor.Get());
		if (ResultActor)
		{
			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);
			return EAIStateTypeEnum::VE_MoveToTarget;
		}

		ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get());
		if (IsValid(ResultActor) && ResultActor != TargetActor)
		{
			if (OwnerBlackboard.IsValid())
			{
				TargetActor = ResultActor;
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);
				MoveToTarget();
			}
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
	
	UpdateInternal_AfterUpdate();

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_Hero_ApproachToTarget::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	Super::OnEnter();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
		ApproachDistance = RGameInstance->GameEnvironmentVar.FollowLeaderDistance;
}

void URAI_State_Hero_ApproachToTarget::OnLeave()
{
	Super::OnLeave();
}
