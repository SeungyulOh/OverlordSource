// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "AvoidanceComponent.h"
#include "Character/Components/AnimStateComponent.h"




#include "GlobalIntegrated.h"
#include "LeaderControlHelper.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/WaypointComponent.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Runtime/Engine/Classes/AI/Navigation/AvoidanceManager.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Character/AnimState/IAnimState.h"
#include "Character/AnimState/AttackAnimState.h"

UAvoidanceComponent::UAvoidanceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

// Called when the game starts
void UAvoidanceComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetMovementMode(MOVE_NavWalking);
}


// Called every frame
void UAvoidanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FrameCount = 0;

	//SetMovementMode(MOVE_NavWalking);
#ifndef RULE_TESTWIDGET
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#endif

	////--------------------------
	//// Test by fourmoon
	//if (0.0f < ForcedMovingTime)
	//{
	//	ForcedMovingTime -= DeltaTime;
	//	return;
	//}
	////--------------------------

	if (bEnableStopSearch)
		UpdateSelectCharacter(DeltaTime);

	if(bEnableAIMovementCondition)
		CheckAIMovementCondition(DeltaTime);

#ifdef RULE_TESTWIDGET
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#endif
}

void UAvoidanceComponent::StopMovementImmediately()
{
	Super::StopMovementImmediately();

	if (IsValid(PawnOwner))
	{
		SameLocTime = 0.0f;
		PrevLocation = PawnOwner->GetActorLocation();
	}
}

void UAvoidanceComponent::InitAvoidanceMovement(UEntityAIComponent* EntityAIComp)
{
	if (IsValid(PawnOwner))
	{
		IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(PawnOwner);
		if (OwnerActor_BaseProperty)
		{
			switch (OwnerActor_BaseProperty->GetCharacterCategory())
			{
			case ECharacterCategoryTypeEnum::VE_Hero:
				LockTimeAfterAvoid = 0.2f;
				break;
			default:
				LockTimeAfterAvoid = 1.0f;
				break;
			}
		}

		EntityAIComponent = EntityAIComp;
		BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PawnOwner);
		PrevLocation = PawnOwner->GetActorLocation();
	}
}

void UAvoidanceComponent::UpdateSelectCharacter(float DeltaTime)
{
	float searchTime = 0.5f;
	float rvoTime = 0.3f;

	if (IsValid(PawnOwner))
	{
		IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(PawnOwner);
		if (BaseProperty && IsValid(BattleEntityComponent))
		{
			if (BaseProperty->IsReady() && !BattleEntityComponent->IsDead())
			{
				if (bSelectCharacterRVOOff)
				{
					RVOOffTime -= DeltaTime;
					if (RVOOffTime <= 0.0f)
					{
						bSelectCharacterRVOOff = false;
#ifdef RULE_TESTWIDGET
						this->SetAvoidanceEnabled(true);
						//todo - merge SetAvoidanceEnabled call
#else
						UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(PawnOwner);
						if (IsValid(MovementComponent))
						{
							MovementComponent->SetAvoidanceEnabled(true);
						}
#endif
					}
				}
				else
				{
					if (!BattleEntityComponent->IsInUnActableState())
					{
						UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(PawnOwner);
						if (IsValid(AnimStateComponent))
						{
							if (AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run)
							{
								FVector curPos = PawnOwner->GetActorLocation();
								FVector diff = PrevSelectCharacterPos - curPos;
								if (diff.IsNearlyZero())
								{
									if (!bSamePosSearch)
									{
										bSamePosSearch = true;
										SameSearchTime = searchTime;
									}
									else
									{
										SameSearchTime -= DeltaTime;
										if (SameSearchTime <= 0.0f)
										{
											bSelectCharacterRVOOff = true;
											RVOOffTime = rvoTime;
#ifdef RULE_TESTWIDGET
											this->SetAvoidanceEnabled(false);
#else
											UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(PawnOwner);
											if (IsValid(MovementComponent))
											{
												MovementComponent->SetAvoidanceEnabled(false);
											}
#endif
										}
									}
								}
								else
								{
									bSamePosSearch = false;
									SameSearchTime = searchTime;
								}

								PrevSelectCharacterPos = curPos;
							}
						}
					}
				}
			}
		}
	}
}

void UAvoidanceComponent::CheckAIMovementCondition(float DeltaTime)
{
	if (IsValid(PawnOwner) && IsValid(EntityAIComponent))
	{
		EAIStateTypeEnum CurrAIState = EntityAIComponent->GetCurrentAIState();
		if (CurrAIState == EAIStateTypeEnum::VE_MoveToTarget ||
			CurrAIState == EAIStateTypeEnum::VE_MoveToLocation ||
			CurrAIState == EAIStateTypeEnum::VE_MoveToAttackLocation ||
			CurrAIState == EAIStateTypeEnum::VE_FollowLeader ||
			CurrAIState == EAIStateTypeEnum::VE_AvoidToLocation ||
			CurrAIState == EAIStateTypeEnum::VE_ApproachToTarget)
		{
			if (IsValid(BattleEntityComponent) && !BattleEntityComponent->IsDead())
			{
				FVector loc = PawnOwner->GetActorLocation();
				if (PrevLocation.Equals(loc, 0.001f))
				{
					SameLocTime += DeltaTime;
					//UE_LOG(LogGamePlay, Log, TEXT("%s Same Loc(%.04f, %.04f, %.04f) Time(%.04f)"),
					//	*(PawnOwner->GetName()), loc.X, loc.Y, loc.Z, SameLocTime);
				}
				else
				{
					SameLocTime = 0.0f;
				}

				PrevLocation = loc;

				if (0.5f < SameLocTime)
				{
					SameLocTime = 0.0f;

					EntityAIComponent->StopMove();
					EntityAIComponent->ForceSetStateIdle();
				}
			}
		}
	}
}

FVector UAvoidanceComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	FVector SlideDelta = UMovementComponent::ComputeSlideVector(Delta, Time, Normal, Hit);

	if (IsValid(PawnOwner))
	{
		FVector dir = -PawnOwner->GetActorForwardVector();
		
		if (dir.Equals(Normal))
		{
			FRotator rotator;
			rotator.Pitch = 0.0f;
			rotator.Yaw = 0.2f;
			rotator.Roll = 0.0f;
			dir = rotator.RotateVector(dir);
		}

		FVector crossVector = FVector::CrossProduct(Normal, dir);
		crossVector.Normalize();

		FVector finalVector = FVector::CrossProduct(Normal, crossVector);
		finalVector.Normalize();
		
		SlideDelta = finalVector * Time * 2.0f;

		//FVector pos = PawnOwner->GetActorLocation();
		//UE_LOG(LogBattle, Log, TEXT("UAvoidanceComponent::ComputeSlideVector %f %f %f == %f %f %f == %f %f %f == %f %f %f == %f %f %f [%d]"), dir.X, dir.Y, dir.Z, Normal.X, Normal.Y, Normal.Z, crossVector.X, crossVector.Y, crossVector.Z, finalVector.X, finalVector.Y, finalVector.Z, pos.X, pos.Y, pos.Z, FrameCount);
	}

	++FrameCount;
	PrevNormal = Normal;

	return SlideDelta;
}

void UAvoidanceComponent::CalcAvoidanceVelocity(float DeltaTime)
{
	UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();
	if (AvoidanceWeight >= 1.0f || AvoidanceManager == NULL || GetCharacterOwner() == NULL)
	{
		return;
	}

	if (GetCharacterOwner()->Role != ROLE_Authority)
	{
		return;
	}

	
	//Adjust velocity only if we're in "Walking" mode. We should also check if we're dazed, being knocked around, maybe off-navmesh, etc.
	UCapsuleComponent *OurCapsule = GetCharacterOwner()->GetCapsuleComponent();
	if (!Velocity.IsZero() && IsMovingOnGround() && OurCapsule)
	{
		//See if we're doing a locked avoidance move already, and if so, skip the testing and just do the move.
		if (AvoidanceLockTimer > 0.0f)
		{
			Velocity = AvoidanceLockVelocity;
		}
		else
		{
			FVector NewVelocity = AvoidanceManager->GetAvoidanceVelocityForComponent(this);
			if (bUseRVOPostProcess)
			{
				PostProcessAvoidanceVelocity(NewVelocity);
			}

			if (!NewVelocity.Equals(Velocity))		//Really want to branch hint that this will probably not pass
			{
				//Had to divert course, lock this avoidance move in for a short time. This will make us a VO, so unlocked others will know to avoid us.
				Velocity = NewVelocity;
				//SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterAvoid);
				SetAvoidanceVelocityLock(AvoidanceManager, 0.25);				
			}
			else
			{
				//Although we didn't divert course, our velocity for this frame is decided. We will not reciprocate anything further, so treat as a VO for the remainder of this frame.
				SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterClean);	//10 ms of lock time should be adequate.
			}
		}
		//RickH - We might do better to do this later in our update
		AvoidanceManager->UpdateRVO(this);

		bWasAvoidanceUpdated = true;
	}
}

bool UAvoidanceComponent::MoveProcess_From_BasePlayerController()
{
	AActor* CurOwner = GetCharacterOwner();
	FVector2D DPadMoveValue = RGAMEINSTANCE(this)->InputManager->GetDPadMoveValue();

	auto SelectCharacter_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(CurOwner);
	if (false == IsValid(SelectCharacter_AnimStateComponent))
		return false;

	AStagePlayerController*	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));

	EAnimStateEnum SelectCharacterState = SelectCharacter_AnimStateComponent->GetCurrentAnimState()->GetAnimStateType();
	
	if (Exception_LeaderMoveProcess() == false)
	{
		
		ABaseCameraActor* CameraActor = BasePlayerController->GetCameraActor();
		if (CameraActor == nullptr)
		{
			return false;
		}

		if (false == IsValid(BattleEntityComponent))
		{
			return false;
		}

		UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
		if (BattleInfo == nullptr)
		{
			return false;
		}

		FRotator MapRot = CameraActor->GetActorRotation();
		MapRot.Pitch = 0.0f;
		MapRot.Roll = 0.0f;

		if (CameraActor->GetOwnerRotationCamera())
		{
			MapRot.Yaw = CameraActor->GetStaticRotation().Yaw;
		}

		FVector ScaledAxisX, ScaledAxisY, ScaledAxisZ;
		FRotationMatrix(MapRot).GetScaledAxes(ScaledAxisX, ScaledAxisY, ScaledAxisZ);
		ScaledAxisX *= DPadMoveValue.X;
		ScaledAxisY *= DPadMoveValue.Y;

		float MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
		FVector MoveVec = ScaledAxisX + ScaledAxisY;
		MoveVec.Normalize();
		FVector ForwardVector = MoveVec;
		MoveVec *= MoveSpeed;
		MoveVec.Z = 0;


		
		auto CharacterMovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(CurOwner);
		if (IsValid(CharacterMovementComp) == false)
		{
			return false;
		}
		CharacterMovementComp->RequestDirectMove(MoveVec, true);
		
		//TODO: Move to Rotate instantly
		FRotator lookatRotation = FRotationMatrix::MakeFromX(ForwardVector).Rotator();
		CurOwner->SetActorRotation(lookatRotation);

		if (SelectCharacter_AnimStateComponent->GetCurrentAnimStateType() != EAnimStateEnum::VE_Run)
		{
			SelectCharacter_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartMove);
		}

		return true;
	}
	else
		return false;
}

bool UAvoidanceComponent::Exception_LeaderMoveProcess()
{
	AStagePlayerController*	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));
	if (!IsValid(BasePlayerController))
		return true;

	if (BattleEntityComponent->IsDead())
		return true;

	if (BattleEntityComponent->IsInUnMovableState())
	{
		return true;
	}

	//when manual mode, if leader has a target, move process should be ignored.
	if (BattleEntityComponent->GetCurrentAttackTarget())
	{
		UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(GetCharacterOwner());
		if (AIComp)
		{
			if (GetCharacterOwner() == BasePlayerController->GetSelectedCharacter() &&
				BasePlayerController->GetLeaderAIModeType() == EAIControlModeEnum::VE_ManualMode &&
				AIComp->GetCurrentAIState() == EAIStateTypeEnum::VE_Attack)
				return true;
		}

	}

	auto SelectCharacter_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(GetCharacterOwner());
	if (false == IsValid(SelectCharacter_AnimStateComponent))
		return true;

	if (!SelectCharacter_AnimStateComponent->IsCancelToCurrentAnim())
		return true;

	EAnimStateEnum SelectCharacterState = SelectCharacter_AnimStateComponent->GetCurrentAnimState()->GetAnimStateType();
	switch (SelectCharacterState)
	{
	case EAnimStateEnum::VE_Die:
	case EAnimStateEnum::VE_Abnormal:
	{
		return true;
	}
	break;
	case EAnimStateEnum::VE_SkillAttack:
	{
		auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(GetCharacterOwner());
		if (false == IsValid(SkillEntityComponent))
			return true;

		if (false == SelectCharacter_AnimStateComponent->IsCancelToCurrentAnim() &&
			false == SkillEntityComponent->IsCurrentSkillChanneling())
		{
			return true;
		}
	}
	break;
	}

	if (SelectCharacter_AnimStateComponent->IsAbleToRunAnim() == false)
		return true;

	if (URealTimeModManager::IsRealTimeGameMode())
	{
		if (SelectCharacter_AnimStateComponent)
		{
			UAttackAnimState* AttackAnimState = Cast<UAttackAnimState>(SelectCharacter_AnimStateComponent->GetAnimState(EAnimStateEnum::VE_NormalAttack));
			if (AttackAnimState)
			{
				if (!AttackAnimState->AttackQueue.IsEmpty())
					return true;

				/*if (AttackAnimState->bBlockMoveRTS)
					return true;*/
			}
		}
	}

	return false;
}
