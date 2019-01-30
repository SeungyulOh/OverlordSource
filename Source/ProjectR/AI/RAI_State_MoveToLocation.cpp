// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_MoveToLocation.h"

#include "RAI_Blackboard.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_MoveToLocation::Update( float DeltaTime )
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

	EAIStateTypeEnum CheckBlackResult = CheckBlackboard();
	if (CheckBlackResult != EAIStateTypeEnum::VE_None && CheckBlackResult != GetStateType() )
		return CheckBlackResult;

	/*
	Check force target, if force target is set, change current target actor to force target.
	*/
	EAIStateTypeEnum CheckForceTargetResult = CheckForceTarget(nullptr);
	if (CheckForceTargetResult != EAIStateTypeEnum::VE_None)
		return CheckForceTargetResult;
	
	if (UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), TargetLocation, AcceptableDistance))
	{
		Stop();
		return EAIStateTypeEnum::VE_Idle;
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

	//UpdateInternal_AfterUpdate();

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_MoveToLocation::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();

	TargetLocation = OwnerBlackboard->GetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_TargetLocation);
	MoveToLocation();
}

void URAI_State_MoveToLocation::OnLeave()
{	
	TargetLocation = FVector::ZeroVector;
	if (OwnerBlackboard.IsValid())
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_TargetLocation);

}

void URAI_State_MoveToLocation::MoveToLocation()
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

#if LOG_AI
				if (IsSuccess == false)
				{
					float PrintTime = 0;
					UE_LOG(LogGamePlay, Log, TEXT("%f ____movefail(%s) -"), PrintTime, *(OwnerActor->GetName()));
				}
#endif
			}
		}
	}
}

void URAI_State_MoveToLocation::Stop()
{	
	TargetLocation = FVector::ZeroVector;
}
