// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SelectionIndicatorMeshComponent.h"

//#include "GameInfo/PlayStateInfo.h"
#include "GlobalIntegrated.h"
///#include "CustomStructures/HeroSlotInfo.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Character/Interfaces/EntityBaseProperty.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

const float MeshRadius = 135.5f;

void USelectionIndicatorMeshComponent::SetOwner(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return;

	OwnerActor = TargetActor;	
}

void USelectionIndicatorMeshComponent::SetScaleFactor(float InScaleFactor)
{
	MeshScaleFactor = InScaleFactor;

	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_BattleEntityComponent))
	{
		int32 fUnitRadius = OwnerActor_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_UnitRadius);

		/*float fFinalRadius = fUnitRadius / RADIUSSIZE_DEFAULT * MeshScaleFactor;*/
		float fFinalRadius = fUnitRadius / MeshRadius;
		SetRelativeScale3D(FVector(fFinalRadius, fFinalRadius, fFinalRadius));
	}
}
