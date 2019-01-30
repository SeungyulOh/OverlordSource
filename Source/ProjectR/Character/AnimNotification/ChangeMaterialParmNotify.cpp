// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChangeMaterialParmNotify.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "UtilFunctionIntegrated.h"


void UChangeMaterialParmNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

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
							OriginalMaterial->SetScalarParameterValue(ScalarParmName, ScalarParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmNotify Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}

				if (VectorParmName.IsValid() && !VectorParmName.IsNone())
				{
					for (auto OriginalMaterial : EntityRenderComponent->OriginalMaterialList)
					{
						if ( IsValid( OriginalMaterial ) )
						{
							OriginalMaterial->SetVectorParameterValue(VectorParmName, VectorParm);
						}
						else
						{
							UE_LOG(LogCriticalErrors, Log, TEXT("UChangeMaterialParmNotify Material NULL - %s"), *(Owner->GetName()));
						}
					}
				}
			}
		}
	}
}
