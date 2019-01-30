// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimTargetRotateState.h"
#include "Character/BattleBaseAnimInstance.h"


#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"


void UAnimTargetRotateState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimPlayingState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	CurAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (!CurAnimInstance.IsValid() )
	{
		return;
	}

	TotalTime = TotalDuration;

	if (bRestore)
	{
		RotateSpeed = FMath::Abs<float>(CurAnimInstance->AddRotatorValue.Yaw) / TotalTime;
	}

	AActor* Owner = AnimInstance->GetOwningActor();
	if (IsValid(Owner))
	{
		UBattleEntityComponent* BattleEntity = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Owner);
		if (IsValid(BattleEntity))
		{
			TargetActor = BattleEntity->GetCurrentAttackTarget();
		}
	}
}

void UAnimTargetRotateState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (bRestore)
	{
		if (CurAnimInstance.IsValid() && OwnerCharacter.IsValid() )
		{
			if (CurAnimInstance->AddRotatorValue.Yaw > 0.0f)
			{
				CurAnimInstance->AddRotatorValue.Yaw -= (RotateSpeed * FrameDeltaTime);
				if (CurAnimInstance->AddRotatorValue.Yaw <= 0.0f)
					CurAnimInstance->AddRotatorValue.Yaw = 0.0f;

				//UE_LOG(LogBattle, Log, TEXT("UAnimTargetRotateState2 %f"), CurAnimInstance->AddRotatorValue.Yaw);
			}
			else if (CurAnimInstance->AddRotatorValue.Yaw < 0.0f)
			{
				CurAnimInstance->AddRotatorValue.Yaw += (RotateSpeed * FrameDeltaTime);
				if (CurAnimInstance->AddRotatorValue.Yaw >= 0.0f)
					CurAnimInstance->AddRotatorValue.Yaw = 0.0f;

				//UE_LOG(LogBattle, Log, TEXT("UAnimTargetRotateState2 %f"), CurAnimInstance->AddRotatorValue.Yaw);
			}
		}
	}
	else
	{
		if (CurAnimInstance.IsValid() && OwnerCharacter.IsValid() && TargetActor.IsValid())
		{
			FVector dir = (TargetActor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal2D();
			FVector frontDir = dir;
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
			if(IsValid(BattleComp))
				dir = BattleComp->GetForwardVector();

			float fDegree = UUtilFunctionLibrary::GetBetweenDegree(frontDir, dir);

			if (fDegree > 0.0f)
			{
				CurAnimInstance->AddRotatorValue.Yaw += (RotateSpeed * FrameDeltaTime);
				if (CurAnimInstance->AddRotatorValue.Yaw >= LimitDegree)
					CurAnimInstance->AddRotatorValue.Yaw = LimitDegree;

				//UE_LOG(LogBattle, Log, TEXT("UAnimTargetRotateState1 %f %f %f"), CurAnimInstance->AddRotatorValue.Yaw, LimitDegree, fDegree);
			}
			else if (fDegree < 0.0f)
			{
				CurAnimInstance->AddRotatorValue.Yaw -= (RotateSpeed * FrameDeltaTime);
				if (CurAnimInstance->AddRotatorValue.Yaw <= -LimitDegree)
					CurAnimInstance->AddRotatorValue.Yaw = -LimitDegree;

				//UE_LOG(LogBattle, Log, TEXT("UAnimTargetRotateState1 %f %f %f"), CurAnimInstance->AddRotatorValue.Yaw, LimitDegree, fDegree);
			}
		}
	}
}

void UAnimTargetRotateState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimPlayingState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	if (bRestore)
	{
		if (CurAnimInstance.IsValid())
		{
			CurAnimInstance->AddRotatorValue.Yaw = 0.0f;
		}
	}
}



