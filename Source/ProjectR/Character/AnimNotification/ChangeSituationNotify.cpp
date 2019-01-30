// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChangeSituationNotify.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

#include "Character/Components/BossComponents/AkhanComponent.h"

void UChangeSituationNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AnimInstance->GetOwningActor());
		if (IsValid(BattleEntityComponent))
		{
			if (SituationState == ECharacterSituationState::VE_Air ||
				SituationState == ECharacterSituationState::VE_Ground)
			{
				UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
				if (IsValid(BattleBaseAnimInstance))
				{
					BattleBaseAnimInstance->EndSkillAniState();
				}
			}

			BattleEntityComponent->ChangeSituation(SituationState);

			UAkhanComponent* AkhanComponent = UCharacterHelpFunctionLibrary::GetAkhanComponent(AnimInstance->GetOwningActor());
			if (IsValid(AkhanComponent))
			{
				AkhanComponent->ChangeSituation(SituationState, 0.0f);
			}
		}

	}
}
