// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LateUpdateComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
// Sets default values for this component's properties
ULateUpdateComponent::ULateUpdateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULateUpdateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	this->SetTickGroup(ETickingGroup::TG_PostPhysics);
}


// Called every frame
void ULateUpdateComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (OwnerActor.IsValid() && IsDeadClearCall)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
		if (IsValid(BattleEntityComponent) && EntityBaseProperty && EntityBaseProperty->IsReady())
		{
			if (BattleEntityComponent->IsDead())
			{
				if (BattleEntityComponent->IsDying())
				{
					BattleEntityComponent->Dead();
					IsDeadClearCall = false;
				}
			}
		}
	}
}

void ULateUpdateComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	OwnerActor = GetOwner();
}


