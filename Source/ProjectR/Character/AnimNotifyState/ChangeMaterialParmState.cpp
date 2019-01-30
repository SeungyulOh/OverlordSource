// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChangeMaterialParmState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "UtilFunctionIntegrated.h"



void UChangeMaterialParmState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
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
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Owner);
			if (IsValid(EntityRenderComponent))
			{
				if (ScalarParmName.IsValid() && !ScalarParmName.IsNone())
				{
					for (auto OriginalMaterial : EntityRenderComponent->OriginalMaterialList)
					{
						if ( IsValid( OriginalMaterial ) )
						{
							OriginalMaterial->GetScalarParameterValue(ScalarParmName, OriScalarParm);
							OriginalMaterial->SetScalarParameterValue(ScalarParmName, ScalarParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmState Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}

				if (VectorParmName.IsValid() && !VectorParmName.IsNone())
				{
					for (auto OriginalMaterial : EntityRenderComponent->OriginalMaterialList)
					{
						if ( IsValid( OriginalMaterial))
						{
							OriginalMaterial->GetVectorParameterValue(VectorParmName, OriVectorParm);
							OriginalMaterial->SetVectorParameterValue(VectorParmName, VectorParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmState Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}
			}
		}
	}
}

void UChangeMaterialParmState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UChangeMaterialParmState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Owner);
			if (IsValid(EntityRenderComponent))
			{
				if (ScalarParmName.IsValid() && !ScalarParmName.IsNone())
				{
					for (auto OriginalMaterial : EntityRenderComponent->OriginalMaterialList)
					{
						if ( IsValid( OriginalMaterial ) )
						{
							OriginalMaterial->SetScalarParameterValue(ScalarParmName, OriScalarParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmState Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}

				if (VectorParmName.IsValid() && !VectorParmName.IsNone())
				{
					for (auto OriginalMaterial : EntityRenderComponent->OriginalMaterialList)
					{
						if ( IsValid( OriginalMaterial ))
						{
							OriginalMaterial->SetVectorParameterValue(VectorParmName, OriVectorParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmState Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}
			}
		}
	}
}