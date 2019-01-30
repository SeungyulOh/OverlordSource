// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BossBaseComponent.h"



#include "Character/SharedBattle/BattleOperator.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

// Sets default values for this component's properties
UBossBaseComponent::UBossBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossBaseComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
	
}


// Called every frame
void UBossBaseComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UBossBaseComponent::SetOwner(AActor* character)
{
	BattleActor = character;

	if (IsValid(BattleActor))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
		if( BattleEntityComponent)
			BattleEntityComponent->SetBossComponent( this );


	}
}

FVector UBossBaseComponent::GetForwardVector()
{
	if (IsValid(BattleActor))
		return BattleActor->GetActorForwardVector();

	return FVector::ZeroVector;
}

void UBossBaseComponent::StartSkillApply()
{
	bSkillApply = true;
}

void UBossBaseComponent::EndSkillApply()
{
	bSkillApply = false;
}

void UBossBaseComponent::SetBossProtectType(EProtectType InProtectType)
{
	BossProtectType = InProtectType;
}
