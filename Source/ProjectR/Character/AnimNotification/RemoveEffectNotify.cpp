// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RemoveEffectNotify.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"


void URemoveEffectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			if (Effectid.IsValid() && !Effectid.IsNone())
			{
				UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Owner);
				if (IsValid(EntityRenderComponent))
				{
					EntityRenderComponent->RemoveEffect(Effectid);
				}
			}
		}
	}
}



