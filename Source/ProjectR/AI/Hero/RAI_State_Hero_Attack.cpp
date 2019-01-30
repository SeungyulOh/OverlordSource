// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "SharedConstants/GlobalConstants.h"
#include "AI/RAI_Blackboard.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/TestPlayerController.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/AnimState/AttackAnimState.h"

#include "UtilFunctionIntegrated.h"
#include "RAI_State_Hero_Attack.h"

#include "RSkillSet_New.h"

#include "LeaderControlHelper.h"



EAIStateTypeEnum URAI_State_Hero_Attack::Update(float DeltaTime)
{
	//Check vaild
	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_Idle;

	UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(Owner_BattleEntityComponent) == false)
		return EAIStateTypeEnum::VE_None;


	//Check State
	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState != EAIStateTypeEnum::VE_None)
		return ResultAbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_None)
		return ResultSkillCheck;

	
	// If result is follow leader, skip check anim interception state.
	UAnimStateComponent* Owner_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(Owner_AnimStateComponent))
	{
		if (false == Owner_AnimStateComponent->IsCancelToCurrentAnim())
		{
			return EAIStateTypeEnum::VE_None;
		}
	}

	bool isTacticState = OwnerBlackboard->GetBlackboardBoolValue(ERAIBalckboardValueEnum::VE_CrewTacticsMove);
	if (isTacticState)
		return EAIStateTypeEnum::VE_TaticsMove;
	
	// Check ForceTarget
	AActor* CheckActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
	if (IsValid(CheckActor))
	{
		return UpdateTargetActor(CheckActor);
	}

	
	// Check focus attack
	CheckActor = UAIFunctionLibrary::CheckFocusTarget(OwnerActor.Get());
	if (CheckActor)
	{
		return UpdateTargetActor(CheckActor);
	}

	

	// Check CurrentTargetActor
	CheckActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
	if (IsValid(CheckActor))
	{
		return UpdateTargetActor(CheckActor);
	}


	// check result black value
	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType() )
	{
		return ResultBlackCheck;
	}

	// Is valid target actor?	
	if (IsValidTargetActor() == false)
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		return EAIStateTypeEnum::VE_Idle;
	}

	return UpdateTargetActor(CheckActor);
}

void URAI_State_Hero_Attack::OnEnter()
{
	Super::OnEnter();

	if (URealTimeModManager::IsRealTimeGameMode())
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (BattleEntityComponent)
		{
			BattleEntityComponent->SendRTS_AttackNow(true, 0);
		}
	}
}

EAIStateTypeEnum URAI_State_Hero_Attack::UpdateTargetActor(AActor* InTarget)
{
	if (InTarget == nullptr)
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		return EAIStateTypeEnum::VE_Idle;
	}

	if (TargetActor != InTarget)
	{
		OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, InTarget);
		TargetActor = InTarget;
	}

	bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), TargetActor.Get()));
	if (IsException)
	{
		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld()));
		if (StagePC && StagePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
		{
			if (!StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMATTACK))
				StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMATTACK, true);
		}

		return EAIStateTypeEnum::VE_Idle;
	}

	/*RTS*/
	if (URealTimeModManager::IsRealTimeGameMode())
		return EAIStateTypeEnum::VE_None;
	/**/

	UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	bool IsCloseEnoughForAttack = Owner_BattleEntityComponent->IsCloseEnoughForAttack(TargetActor.Get(), 0.0f, 30.0f);
	if (IsCloseEnoughForAttack)
	{
		UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor.Get());

		//Only For Leader
		ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor.Get()->GetWorld());
		if (BasePC->GetLeaderAIModeType() == EAIControlModeEnum::VE_ManualMode && !BasePC->InputAttackHold)
		{
			if (OwnerActor.Get() == BasePC->GetSelectedCharacter()
				|| Cast<ATestPlayerController>(BasePC))
			{
				UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
				if (AIComp)
				{
					AIComp->AIStop();
					return EAIStateTypeEnum::VE_Idle;
				}
			}
		}
		return EAIStateTypeEnum::VE_None;
	}
	
	return EAIStateTypeEnum::VE_MoveToTarget;
}