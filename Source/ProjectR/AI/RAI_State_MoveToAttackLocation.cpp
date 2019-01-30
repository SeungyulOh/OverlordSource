// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_MoveToAttackLocation.h"

#include "RAI_Blackboard.h"
#include "RAI_FSM.h"

#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_MoveToAttackLocation::Update( float DeltaTime )
{
	EAIStateTypeEnum BaseResult = Super::Update(DeltaTime);
	if (BaseResult != EAIStateTypeEnum::VE_None)
		return BaseResult;

	// search enemy
	AActor* ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get() );
	if (IsValid(ResultActor))
	{
		if (OwnerBlackboard.IsValid())
		{
			OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);
			return EAIStateTypeEnum::VE_MoveToTarget;
		}
	}


	return EAIStateTypeEnum::VE_None;
}


void URAI_State_MoveToAttackLocation::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();

	TargetLocation = OwnerBlackboard->GetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_AttackLocation);
	//DrawDebugBox(RGAMEINSTANCE(GEngine)->GetWorld(), TargetLocation, FVector(50.f, 50.f, 50.f), FColor::Red, false, 10.f, 0, 10.f);
	
	MoveToLocation();
}

void URAI_State_MoveToAttackLocation::OnLeave()
{
	TargetLocation = FVector::ZeroVector;	
	if (OwnerBlackboard.IsValid())
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_AttackLocation);
	}
}