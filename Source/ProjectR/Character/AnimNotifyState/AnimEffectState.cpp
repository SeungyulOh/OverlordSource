// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimEffectState.h"


#include "Table/ResourceParticleTableInfo.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Particles/ParticleSystem.h"


void UAnimEffectState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimEffectState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

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
					FResourceFXTableInfo* FxInfo = UTableManager::GetInstancePtr()->GetResourceFXRow(Effectid);
					if (FxInfo == nullptr)
						return;
					
					auto Effect = UUtilFunctionLibrary::GetParticleSystemFromAssetID(FxInfo->Particle);
					if (!IsValid(Effect))
						return;

					EntityRenderComponent->AttachEffect(Effectid, Effect, AttachPointName);
				}
			}
		}
	}
}

void UAnimEffectState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimEffectState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimEffectState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

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