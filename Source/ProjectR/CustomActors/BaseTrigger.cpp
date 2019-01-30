// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseTrigger.h"
#include "GlobalIntegrated.h"



#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"

ABaseTrigger::ABaseTrigger(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

// Called when the game starts or when spawned
void ABaseTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UShapeComponent*>	comps;
	GetComponents<UShapeComponent>(comps);

	if (comps.Num() > 0)
	{
		CollisionComponent = comps[0];
	}

	if (IsValid(CollisionComponent))
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseTrigger::OnBeginOverlap);
	}
}

void ABaseTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherComp))
	{
		if (OtherActor != this)
		{			
			UShapeComponent* ShapeComp = Cast<UShapeComponent>(OtherComp);
			if (!IsValid(ShapeComp))
				return;

			IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OtherActor);
			if (BaseProperty)
			{
				if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetPartyID() == UObjectFunctionLibrary::GetMyPartyID())
				{
					OnTriggerEnter_Delegate.Broadcast(this);
					SetActorEnableCollision(false);
				}			
			}
		}
	}
}
