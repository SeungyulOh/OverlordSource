// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_TaticsMove.h"

#include "UtilFunctionIntegrated.h"

#include "Character/BaseAIController.h"



EAIStateTypeEnum URAI_State_TaticsMove::Update(float DeltaTime)
{
	StateElapsedTime += DeltaTime;
	if (StayTime < StateElapsedTime)
		return EAIStateTypeEnum::VE_Idle;

	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState == EAIStateTypeEnum::VE_AbnormalState)
	{
		UAIFunctionLibrary::Stop(OwnerActor.Get());
		return EAIStateTypeEnum::VE_None;
	}

	UAIFunctionLibrary::SetCrewLocation(OwnerActor.Get(), TargetLocation);

	if (UAIFunctionLibrary::IsCloseEnoughToLocation(OwnerActor.Get(), TargetLocation, 100.f)) // is close enough to target location
	{
		UAnimStateComponent* AnimState = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (AnimState && AnimState->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run)
			AnimState->DoCommand(ECharacterCommandEnum::VE_StopMove);

		return EAIStateTypeEnum::VE_None;
	}
	else
		MoveToLocation();


	return EAIStateTypeEnum::VE_None;
}

void URAI_State_TaticsMove::OnEnter()
{
	InitInternalData();

	UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (AIComponent)
	{
		AIComponent->OnStart_TacticsMove.Broadcast(StayTime);
	}


	UAIFunctionLibrary::SetCrewLocation(OwnerActor.Get(), TargetLocation);
	OwnerActor->SetActorLocation(TargetLocation);

	ABattleBaseCharacter* BattleCharacter = Cast<ABattleBaseCharacter>(OwnerActor.Get());
	UParticleSystem* SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("Crew_Spawn_Effect"));
	if (SpawnEffectClass)
	{
		UParticleSystemComponent* SpawnEffect = UUtilFunctionLibrary::SpawnEmitterAttached(SpawnEffectClass, OwnerActor.Get(), FName("Socket_Root"));
		static float StandardRadius = 96.0f;

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (IsValid(SpawnEffect) && IsValid(BattleEntityComponent))
		{
			UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
			if (BattleInfo)
			{
				float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
				float EffectScale = UnitRadius / StandardRadius;

				SpawnEffect->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
			}
		}
	}
	
}

void URAI_State_TaticsMove::OnLeave()
{
	OwnerBlackboard->SetBlackboardBoolValue(ERAIBalckboardValueEnum::VE_CrewTacticsMove, false);

	UAIFunctionLibrary::Stop(OwnerActor.Get());

	
}

void URAI_State_TaticsMove::MoveToLocation()
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
