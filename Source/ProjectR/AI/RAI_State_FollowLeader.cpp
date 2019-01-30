// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_FollowLeader.h"

#include "RAI_Blackboard.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
EAIStateTypeEnum URAI_State_FollowLeader::Update( float DeltaTime )
{
	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState != EAIStateTypeEnum::VE_None)
		return ResultAbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_None)
		return ResultSkillCheck;

	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_Idle;

	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType() )
		return ResultBlackCheck;

	if (IsValidTargetActor() == false)
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_LeaderToFollow);		
		return EAIStateTypeEnum::VE_Idle;
	}

	if (!OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor))
	{
		// search enemy	
		AActor* ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get(), false, EPriorityTypeEnum::VE_None);
		if (IsValid(ResultActor))
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
	}

	/*
	Check close enough to leader
	*/
	auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (Owner_BattleEntityComponent)
	{
		
		bool bTargetStillMoving = false;
		if (TargetActor.IsValid())
		{
			UAnimStateComponent* SelectCharacter_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(TargetActor.Get());
			if (SelectCharacter_AnimStateComponent)
			{
				if (SelectCharacter_AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run)
					bTargetStillMoving = true;
			}
		}

		if (bMoveToLocation == false)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughToActor(TargetActor.Get(), FollowDistance))
				return bTargetStillMoving ? EAIStateTypeEnum::VE_None : EAIStateTypeEnum::VE_Idle;
		}
		else
		{
			UAIFunctionLibrary::SetCrewLocation(OwnerActor.Get(), TargetLocation);

			if (!UMapFunctionLibrary::isTargetPointonNavMesh(TargetLocation))
				TargetLocation = TargetActor->GetActorLocation();

			if (UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), TargetLocation, FollowDistance)) // is close enough to target location
				return bTargetStillMoving ? EAIStateTypeEnum::VE_None : EAIStateTypeEnum::VE_Idle;
			else
				MoveToLocation();
		}
	}

	if (CanMove() == false)
	{
		Stop();
		return EAIStateTypeEnum::VE_Idle;
	}
	
	return EAIStateTypeEnum::VE_None;
}

bool URAI_State_FollowLeader::IsValidTargetActor()
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

AActor* URAI_State_FollowLeader::SearchNewEnemy()
{
	return UAIFunctionLibrary::SearchEnemy(OwnerActor.Get());
}

void URAI_State_FollowLeader::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (OwnerBlackboard.IsValid())
	{		
		TargetActor = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_LeaderToFollow);
		if ( TargetActor.IsValid() == false)
			return;

		UPlayStateInfo* pStateInfo = RGameInstance->PlayStateInfo;
		if (pStateInfo)
		{
			FHeroSlotInfo* slotInfo = pStateInfo->GetSelectedCharacterSlotCharacter(OwnerActor.Get());
			if (slotInfo)
			{
				if (slotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew1 || slotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew2)
				{
					UAIFunctionLibrary::SetCrewLocation(OwnerActor.Get(), TargetLocation);
					bMoveToLocation = true;
				}
			}
		}

		if (bMoveToLocation == false)
		{
			TargetLocation = TargetActor->GetActorLocation();
			if (OwnerActor.IsValid())
			{
				if (RGameInstance)
				{
					FollowDistance = RGameInstance->GameEnvironmentVar.FollowLeaderDistance;
				}
			}

			/*
			Check close enough to leader
			*/
			auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (Owner_BattleEntityComponent)
			{
				if (Owner_BattleEntityComponent->IsCloseEnoughToActor(TargetActor.Get(), FollowDistance) == false)
				{
					MoveToTarget();
				}
				else
				{
					TargetActor = nullptr;
					return;
				}
			}
		}
		else
			MoveToLocation();
				
	}	

//	UE_LOG(LogAI, Log, TEXT(">>>> OnEnter - FollowLeader - %s"), *(TargetActor->GetName()));
}

void URAI_State_FollowLeader::OnLeave()
{
// #if LOG_AI
// 	UE_LOG(LogAI, Log, TEXT("<<<< OnLeave - FollowLeader - %s"), TargetActor.IsValid()?*(TargetActor->GetName()):TEXT("None") );
// #endif

	TargetActor = nullptr;
	TargetLocation = FVector::ZeroVector;

	if (OwnerBlackboard.IsValid())
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_LeaderToFollow);
	}	
}

void URAI_State_FollowLeader::MoveToTarget()
{
	if (OwnerBlackboard.IsValid())
	{
		APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
		if (OwnerPawn)
		{
			ABaseAIController* BaseAIController = Cast< ABaseAIController >(OwnerPawn->GetController());
			if (BaseAIController)
				BaseAIController->CustomMoveToNearestMovableCellFromActor(TargetActor.Get(), EMovementTickAITypeEnum::VE_Normal, false, true, false, nullptr, true);
		}
	}
}

void URAI_State_FollowLeader::MoveToLocation()
{
	if (OwnerBlackboard.IsValid())
	{
		APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
		if (OwnerPawn)
		{
			ABaseAIController* BaseAIController = Cast< ABaseAIController >(OwnerPawn->GetController());
			if (BaseAIController)
			{
				bool IsSuccess = BaseAIController->CustomMoveToLocation(
					TargetLocation, EMovementTickAITypeEnum::VE_MoveToAttack, true, true, false, false, nullptr, true);

			}
		}
	}
}

void URAI_State_FollowLeader::Stop()
{	
	TargetActor = nullptr;
}