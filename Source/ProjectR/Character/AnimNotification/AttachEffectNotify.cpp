// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AttachEffectNotify.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"

void UAttachEffectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance)
			{
				if (RGameInstance->OptionManager)
				{
					if (RGameInstance->OptionManager->IsShowParticle() == false )
						return;
				}
			}

			if (Effectid.IsValid() && !Effectid.IsNone() && IsValid(EffectTemplate))
			{
				UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Owner);
				if (IsValid(EntityRenderComponent))
				{
					//UParticleSystem* TargetEffect = UUtilFunctionLibrary::GetParticleSystemFromTable(Effectid);
					EntityRenderComponent->AttachEffect(Effectid, EffectTemplate, AttachPointName);
				}
			}
		}
	}
}



