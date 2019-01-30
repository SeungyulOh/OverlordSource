// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Hero_Idle.h"
#include "AI/RAI_Blackboard.h"
#include "AI/RAI_FSM.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/AnimState/IAnimState.h"
#include "UtilFunctionIntegrated.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"

EAIStateTypeEnum URAI_State_Hero_Idle::Update( float DeltaTime )
{
	if (bPause)
		return EAIStateTypeEnum::VE_Idle;

	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState != EAIStateTypeEnum::VE_None)
		return ResultAbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_None)
		return ResultSkillCheck;

	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_Idle;

	//Bryan Added. Why? 
	//if our character is on animation playing state, we need to stop to skip next state.
	UAnimStateComponent* AnimState = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (AnimState)
	{
		if(!AnimState->GetCurrentAnimState()->IsCancelAnim())
			return EAIStateTypeEnum::VE_Idle;
	}
			

	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType() )
		return ResultBlackCheck;
	
	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget))
	{
		AActor* ForceTarget = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		if (IsValid(ForceTarget))
		{
			bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), ForceTarget));
			if (IsException == false)
			{
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ForceTarget);


				auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
				if (Owner_BattleEntityComponent)
				{
					if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(ForceTarget))
					{
						return EAIStateTypeEnum::VE_Attack;
					}
				}
				return EAIStateTypeEnum::VE_MoveToTarget;
			}
			else
			{
				OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget);
				return EAIStateTypeEnum::VE_None;
			}			
		}
	}

	// Check focus attack
	AActor* ResultActor = UAIFunctionLibrary::CheckFocusTarget(OwnerActor.Get());
	if (ResultActor)
	{
		OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);

		auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (Owner_BattleEntityComponent)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(ResultActor))
			{
				return EAIStateTypeEnum::VE_Attack;
			}
		}
		return EAIStateTypeEnum::VE_MoveToTarget;
	}

	if (!OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor))
	{
		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));


		// search enemy	
		ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get(), false, EPriorityTypeEnum::VE_None );
		if (IsValid(ResultActor))
		{
			ABasePlayerController*	pBasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
			if (pBasePlayerController->IsSelectedCharacter(OwnerActor.Get())
				&& pBasePlayerController->IsStageDirectionProcess() == false)
			{
				auto Location = ResultActor->GetActorLocation();
				pBasePlayerController->SetStageDirection(true, Location);
			}

			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);

			auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (Owner_BattleEntityComponent)
			{
				if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(ResultActor))
				{
					return EAIStateTypeEnum::VE_Attack;
				}
			}

			return EAIStateTypeEnum::VE_MoveToTarget;
		}
	}

	/*
	Get next way point
	*/


	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
	AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
	if (IsValid(SelectedCharacter))
	{
		bool IsSelectedCharacter = (OwnerActor.Get() == SelectedCharacter);
		if (IsSelectedCharacter)
		{
			if (UAIFunctionLibrary::CheckNextWayPoint(OwnerActor.Get()))
			{
				auto Location = OwnerBlackboard->GetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_AttackLocation);
				BasePlayerController->SetStageDirection(true, Location);
			}
			else
			{
				UAIFunctionLibrary::ScanEnemyOutsideSightRange(OwnerActor.Get());
			}
		}
		

		// follow leader check
		UAnimStateComponent* SelectCharacter_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(SelectedCharacter);
		if(IsValid(SelectCharacter_AnimStateComponent))
		{
			if (SelectCharacter_AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run)
			{
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_LeaderToFollow, SelectedCharacter);	
			}
				
		}
		
	}
	
	return EAIStateTypeEnum::VE_None;
}

void URAI_State_Hero_Idle::PauseMove(bool InPause)
{
	bPause = InPause;
}
