// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChangeSituationState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

#include "Character/Components/BossComponents/AkhanComponent.h"

void UChangeSituationState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UChangeSituationState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AnimInstance->GetOwningActor());
		if (IsValid(BattleEntityComponent))
		{
			BattleEntityComponent->ChangeSituation(StartSituationState);
		}

		UAkhanComponent* AkhanComponent = UCharacterHelpFunctionLibrary::GetAkhanComponent(AnimInstance->GetOwningActor());
		if (IsValid(AkhanComponent))
		{
			AkhanComponent->ChangeSituation(StartSituationState, TotalDuration);
		}
	}
}

void UChangeSituationState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UChangeSituationState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UChangeSituationState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AnimInstance->GetOwningActor());
		if (IsValid(BattleEntityComponent))
		{
			if (EndSituationState == ECharacterSituationState::VE_Air ||
				EndSituationState == ECharacterSituationState::VE_Ground)
			{
				UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
				if (IsValid(BattleBaseAnimInstance))
				{
					BattleBaseAnimInstance->EndSkillAniState();
				}
			}

			BattleEntityComponent->ChangeSituation(EndSituationState);

			UAkhanComponent* AkhanComponent = UCharacterHelpFunctionLibrary::GetAkhanComponent(AnimInstance->GetOwningActor());
			if (IsValid(AkhanComponent))
			{
				AkhanComponent->ChangeSituation(EndSituationState, 0.0f);
			}
		}
	}
}
