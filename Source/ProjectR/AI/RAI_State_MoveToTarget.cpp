// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_MoveToTarget.h"
// #include "Utils/AIFunctionLibrary.h"

#include "RAI_Blackboard.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_MoveToTarget::Update( float DeltaTime )
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

	/*
	Check force target, if force target is set, change current target actor to force target.
	*/
	EAIStateTypeEnum CheckForceTargetResult = CheckForceTarget(TargetActor.Get());
	if (CheckForceTargetResult != EAIStateTypeEnum::VE_None)
		return CheckForceTargetResult;

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

	/*
	Check close enough to attack
	*/
	auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (Owner_BattleEntityComponent )
	{
		if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get()))
		{			
			if (Owner_BattleEntityComponent->IsEnemy(TargetActor.Get()))
			{
				return EAIStateTypeEnum::VE_Attack;
			}
			else
			{
				return EAIStateTypeEnum::VE_Idle;
			}
		}
	}

	// search enemy again
	// !!! do below code at regular intervals
	if (KeepTarget == false )
	{
		AActor* ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get());
		if (IsValid(ResultActor) && ResultActor != TargetActor )
		{
			if (OwnerBlackboard.IsValid())
			{
				TargetActor = ResultActor;
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor );
				MoveToTarget();
			}
		}
	}

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

	return EAIStateTypeEnum::VE_None;
}

bool URAI_State_MoveToTarget::IsValidTargetActor()
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
	}
	
	return true;
}

AActor* URAI_State_MoveToTarget::SearchNewEnemy()
{
	return UAIFunctionLibrary::SearchEnemy(OwnerActor.Get() );
}

void URAI_State_MoveToTarget::OnEnter() 
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();

	if (OwnerBlackboard.IsValid())
	{
		// Is set force target?
		if (OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget))
		{
			KeepTarget = true;
			TargetActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, TargetActor.Get() );
		}
		else
		{
			KeepTarget = false;
			TargetActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		}		

		/*
		Check close enough to attack
		*/
		auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (Owner_BattleEntityComponent)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get()) == false )
			{
				MoveToTarget();
			}
		}		
	}	
}

void URAI_State_MoveToTarget::OnLeave()
{
	UAIFunctionLibrary::Stop(OwnerActor.Get());

	TargetActor = nullptr;
	KeepTarget = false;
}

void URAI_State_MoveToTarget::MoveToTarget()
{
	if (OwnerBlackboard.IsValid() && TargetActor.IsValid() )
	{
		APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
		if (OwnerPawn)
		{
			ABaseAIController* BaseAIController = Cast< ABaseAIController >(OwnerPawn->GetController());
			if (BaseAIController)
			{	
				TargetLocation = TargetActor->GetActorLocation();
				
				SuccessMoveToTarget = BaseAIController->CustomMoveToNearestMovableCellFromActor(
					TargetActor.Get(), EMovementTickAITypeEnum::VE_Normal, true, true, false, nullptr, true);
				
				if (SuccessMoveToTarget == false)
				{
					float PrintTime = 0;
					UE_LOG(LogGamePlay, Log, TEXT("%f ____movetargetfail(%s) -"), PrintTime, *(OwnerActor->GetName()));
				}
			}
		}
	}
}

void URAI_State_MoveToTarget::Stop()
{	
	TargetActor = nullptr;
}

void URAI_State_MoveToTarget::PauseMove(bool bPause)
{
	if (bPause == false)
		MoveToTarget();
}