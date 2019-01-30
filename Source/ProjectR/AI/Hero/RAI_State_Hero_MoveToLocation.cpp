// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Hero_MoveToLocation.h"
#include "AI/RAI_Blackboard.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "Character/Components/AnimStateComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"
// #if WITH_EDITOR
// #define DRAW_DEBUG
// #endif

EAIStateTypeEnum URAI_State_Hero_MoveToLocation::Update( float DeltaTime )
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
	
	EAIStateTypeEnum CheckBlackResult = CheckBlackboard();
	if (CheckBlackResult != EAIStateTypeEnum::VE_None && CheckBlackResult != GetStateType() )
		return CheckBlackResult;

	
#ifdef DRAW_DEBUG
	DrawDebugDirectionalArrow(OwnerActor->GetWorld(), CurLocation, TargetLocation, 2.0f, FColor::Yellow);
#endif // DRAW_DEBUG

	if (UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), TargetLocation, 10.f))
	{
		if (UAIFunctionLibrary::AvoidOtherCharacter(OwnerActor.Get()) == false)
		{
			Stop();
			return EAIStateTypeEnum::VE_Idle;
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

void URAI_State_Hero_MoveToLocation::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();
	
	TargetLocation = OwnerBlackboard->GetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_TargetLocation);
	
	MoveToLocation();
}

void URAI_State_Hero_MoveToLocation::OnLeave()
{
	TargetLocation = FVector::ZeroVector;
}

void URAI_State_Hero_MoveToLocation::MoveToLocation()
{
	if (OwnerBlackboard.IsValid())
	{
		APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
		if (OwnerPawn)
		{
			ABaseAIController* BaseAIController = Cast< ABaseAIController >(OwnerPawn->GetController());
			if (BaseAIController)
				BaseAIController->CustomMoveToLocation(TargetLocation, EMovementTickAITypeEnum::VE_Normal, false, true, false, false, nullptr, true);
		}
	}
}
