// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "RAI_State_Hero_AvoidToLocation.h"
#include "AI/RAI_Blackboard.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/BaseAIController.h"
#include "GlobalIntegrated.h"

#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_Hero_AvoidToLocation::Update( float DeltaTime )
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
	
	
		
	FVector CurLocation = OwnerActor->GetActorLocation();
	//DrawDebugDirectionalArrow(OwnerActor->GetWorld(), CurLocation, TargetLocation, 2.0f, FColor::Yellow);
	TargetLocation.Z = CurLocation.Z;
	if (UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), TargetLocation, AcceptableDistance))
	{
		if (IsOverlappedOtherCharacter() )
		{
			if (GetTargetLocationFromAvoidanceVector())
			{
				MoveToLocation();
				//???
				//return EAIStateTypeEnum::VE_MoveToLocation;
			}
			else
			{
				Stop();
				return EAIStateTypeEnum::VE_Idle;
			}
		}		
		else
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


void URAI_State_Hero_AvoidToLocation::OnEnter()
{
	if (CanMove() == false)
	{
		return;
	}

	InitInternalData();

	if (OwnerBlackboard.IsValid())
	{
		AvoidVector = OwnerBlackboard->GetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_AvoidanceVector);
	}	
	GetTargetLocationFromAvoidanceVector(); // using grid map
	MoveToLocation();

	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
	if (IsValid(MovementComponent))
	{		
		MovementComponent->SetAvoidanceEnabled(false);
	}



	//SetCrowdSimulationState
}

void URAI_State_Hero_AvoidToLocation::OnLeave()
{
	TargetLocation = FVector::ZeroVector;
	if (OwnerBlackboard.IsValid())
	{
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_AvoidanceVector);
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_TargetLocation);
	}

	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
	if (IsValid(MovementComponent))
	{
		MovementComponent->SetAvoidanceEnabled(true);
	}
}


void URAI_State_Hero_AvoidToLocation::MoveToLocation()
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

void URAI_State_Hero_AvoidToLocation::Stop()
{
	TargetLocation = FVector::ZeroVector;
}

bool URAI_State_Hero_AvoidToLocation::IsOverlappedOtherCharacter()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return false;

	float CharacterOverlapScale = RGameInstance->GameEnvironmentVar.CharacterOverlapScale;
	TArray< TWeakObjectPtr<AActor> > ResultActors;
// 	AActor* OverlappedActor = RGameInstance->GameObjectMgr->GetOverlapTargets( ResultActors, OwnerActor.Get(), EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
	AActor* OverlappedActor = nullptr;
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		OverlappedActor = pGamePlayMode->GetOverlapTargets(ResultActors, OwnerActor.Get(), EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		OverlappedActor = pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, OwnerActor.Get(), EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
	}

	if (OverlappedActor == nullptr)
	{
		return false;
	}
	else
	{
		if (ResultActors.Num() == 1)
		{
			UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OverlappedActor);
			if (EntityAIComponent)
			{
				if (EntityAIComponent->GetCurrentAIState() == EAIStateTypeEnum::VE_AvoidToLocation)
					return false;
			}
		}
	}

	return true;
}

bool URAI_State_Hero_AvoidToLocation::GetTargetLocationFromAvoidanceVector()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return false;
		
	TargetLocation = OwnerActor->GetActorLocation() + AvoidVector * AvoidanceDistance;
#if LOG_AI
	UE_LOG(LogBattle, Log, TEXT("Avoid vector : %s "), *(AvoidVector.ToString()) );
#endif
#ifdef CHECK_NEXT_MOVE_VALIDATION
	if( UAIFunctionLibrary::CanMoveToTargetPosition(OwnerActor.Get(), TargetLocation))
	{
		return true;
	}
	return false;
#else
	return true;
#endif
}

