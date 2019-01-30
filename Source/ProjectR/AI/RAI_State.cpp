// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State.h"
#include "GlobalIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

void URAI_State::Init(AActor* InOwnerActor, URAI_Blackboard* InRai_Blackboard)
{
	OwnerActor = InOwnerActor;
	OwnerBlackboard = InRai_Blackboard;
}

void URAI_State::InitInternalData()
{
	PrevLocation = FVector::ZeroVector;
	StateElapsedTime = 0.0f;
}

void URAI_State::UpdateInternal_BeforeUpdate()
{
	PrevLocation = CurrentLocation;
	CurrentLocation = OwnerActor->GetActorLocation();
}

void URAI_State::UpdateInternal_AfterUpdate()
{
	//PrevLocation = CurrentLocation;
}


EAIStateTypeEnum URAI_State::CheckBlackboard()
{
	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_Idle;
	
	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_CrewTacticsMove))
		return EAIStateTypeEnum::VE_TaticsMove;

	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_AvoidanceVector))
		return EAIStateTypeEnum::VE_AvoidToLocation;

	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_TargetLocation))
		return EAIStateTypeEnum::VE_MoveToLocation;

	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_LeaderToFollow))
		return EAIStateTypeEnum::VE_FollowLeader;

	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_AttackLocation))
		return EAIStateTypeEnum::VE_MoveToAttackLocation;


	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State::CheckSkillState()
{
	if (OwnerActor.IsValid() == false)
		return EAIStateTypeEnum::VE_None;

	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillEntityComponent)
	{
		if (SkillEntityComponent->IsSkillCasting())
			return EAIStateTypeEnum::VE_SkillCasting;

		if (SkillEntityComponent->IsSkillActing())
			return EAIStateTypeEnum::VE_SkillActing;
	}

	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State::CheckAbnormalState()
{
	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->IsInUnActableState() || BattleEntityComponent->IsInUnMovableState())
		{
			UE_LOG(LogBattle, Log, TEXT("CheckAbnormalState ======> EAIStateTypeEnum::VE_AbnormalState true :::: %s."), *(OwnerActor.Get()->GetName()) );
			return EAIStateTypeEnum::VE_AbnormalState;
		}
			
	}

	return EAIStateTypeEnum::VE_None;
}

bool URAI_State::CheckStopState(float DeltaTime)
{
	if (OwnerActor.IsValid() == false)
		return false;

	TickTime += DeltaTime;
	if (DeltaTime < 0.2f)
	{
		return false;
	}
	else
	{
		TickTime = 0;
	}

	float Dist = FVector::DistSquared(PrevLocation, CurrentLocation);
	if (FMath::IsNearlyZero(Dist))
	{
		StateElapsedTime += DeltaTime;
		if (StateElapsedTime < StopCheckTime)
			return false;

		StateElapsedTime = 0.0f;
		return true;
	}
	else
	{
		StateElapsedTime = 0.0f;
	}

	return false;
}

bool URAI_State::CanMove()
{
	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_state_change))
			return false;
	}

	return true;
}


EAIStateTypeEnum URAI_State::FollowLeader()
{
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (BaseProperty)
	{
		if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Minion)
			return EAIStateTypeEnum::VE_None;

		if(BaseProperty->GetPartyID() != PartyID_NPC_HELP)
			return EAIStateTypeEnum::VE_None;
	}

	if (OwnerActor.IsValid())
	{
		ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld());
		if (BasePC)
		{
			AActor* Leader = BasePC->GetSelectedCharacter();
			if (!Leader)
				return EAIStateTypeEnum::VE_None;

			if (!UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), Leader->GetActorLocation(), 100.f))
			{
				OwnerBlackboard.Get()->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_LeaderToFollow, Leader);
			}
		}
	}

	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State::FollowSummoner()
{
	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (OwnerActor_BaseProperty)
	{
		if (OwnerActor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Minion)
		{
			UBattleEntityComponent* OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (OwnerBattleEntityComponent)
			{
				UCharacterBattleInfo* BattleInfo = OwnerBattleEntityComponent->GetBattleInfo();
				if (BattleInfo) 
				{
					float MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
					// owneractor.moveSpeed != 0
					if (MoveSpeed > 0.0f) 
					{
						AActor* SummonOwnerActor = OwnerBattleEntityComponent->GetSummonOwner();
						UBattleEntityComponent* SummonOwnerBattleComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SummonOwnerActor);
						if (IsValid(SummonOwnerBattleComponent))
						{
							if( !SummonOwnerBattleComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Hide) )
							{
								URGameInstance* RGameInstance = RGAMEINSTANCE(this);
								float ApproachDistance = 200.0f;
								if (RGameInstance)
									ApproachDistance = RGameInstance->GameEnvironmentVar.FollowLeaderDistance;
								if (OwnerBattleEntityComponent->IsCloseEnoughToActor(SummonOwnerActor, ApproachDistance + 100.0f) == false)
								{
									OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_LeaderToFollow, SummonOwnerActor);
									return EAIStateTypeEnum::VE_FollowLeader;
								}
							}
						}
					}
				}
			}
		}
	}		

	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State::CheckForceTarget(AActor* InCurrentTarget)
{
	if (OwnerBlackboard->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget))
	{
		AActor* ForceTarget = OwnerBlackboard->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		if (IsValid(ForceTarget))
		{
			if (ForceTarget != InCurrentTarget)
			{
				OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ForceTarget);
				return EAIStateTypeEnum::VE_MoveToTarget;
			}
		}
	}

	return EAIStateTypeEnum::VE_None;
}

EAIStateTypeEnum URAI_State::ProcessMeetState()
{
	FVector NextWayPoint = UAIFunctionLibrary::GetNextWayPoint(OwnerActor.Get());
	if (UAIFunctionLibrary::IsGoNextWayPoint(OwnerActor.Get(), NextWayPoint))
	{
		FVector TargetLoc = FVector::ZeroVector;
		if (NextWayPoint.IsNearlyZero())
		{
			TargetLoc = OwnerActor->GetActorForwardVector() * -1 * 500 + OwnerActor->GetActorLocation();
		}
		else
		{
			FVector DirectionVector = NextWayPoint - OwnerActor->GetActorLocation();
			DirectionVector.Z = 0;
			FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator() * -1;
		}
		TargetLoc.Z = OwnerActor->GetActorLocation().Z;
		OwnerBlackboard->SetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_TargetLocation, TargetLoc);

		return EAIStateTypeEnum::VE_MoveToLocation;
	}
	return EAIStateTypeEnum::VE_Idle;
}