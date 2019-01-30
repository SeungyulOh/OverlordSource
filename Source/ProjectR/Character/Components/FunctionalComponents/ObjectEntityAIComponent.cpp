// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ObjectEntityAIComponent.h"

#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"
#include "AIController.h"


#include "Table/BattleObjectTableInfo.h"


// 

#include "UtilFunctionIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

// Sets default values for OwnerCharacter.Get() component's properties
UObjectEntityAIComponent::UObjectEntityAIComponent()
{
	
}

void UObjectEntityAIComponent::ForceSearchTarget()
{
	if (UUtilFunctionLibrary::GetGamePlayModeEnum() == EGamePlayModeTypeEnum::VE_Arena ||
		UUtilFunctionLibrary::GetGamePlayModeEnum() == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		return;
	}

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(BattleEntityComponent) == false)
	{
		return;
	}

	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityProperty == nullptr)
	{
		return;
	}


	float					SightRange = URenewal_BattleFunctionLibrary::GetCustomizedSightRange(GetWorld(), BattleEntityComponent->GetBattleInfo());
	EPriorityTypeEnum		SearchPreferType = BattleEntityComponent->GetBattleInfo()->CharacterProperty.AIPriorityType;

	AActor*	ResultCharacter = URenewal_BattleFunctionLibrary::SearchEnemy(OwnerActor.Get(), OwnerActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, SightRange, 200.0f, SearchPreferType, EPriorityTypeEnum::VE_None );

	BattleEntityComponent->SetCurrentAttackTarget(ResultCharacter);

	if (IsValid(ResultCharacter))
	{
		ForceDirection(ResultCharacter->GetActorLocation());
	}
}

void UObjectEntityAIComponent::SetForcePauseBySkill(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee /*= true*/)
{
}

void UObjectEntityAIComponent::ForceDirection(FVector InTargetLocation)
{

}