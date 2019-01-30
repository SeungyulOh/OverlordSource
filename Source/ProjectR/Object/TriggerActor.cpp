// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TriggerActor.h"

#include "GlobalIntegrated.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"



#include "UtilFunctionIntegrated.h"



// Sets default values
ATriggerActor::ATriggerActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{ 	
}

void ATriggerActor::SetBoxExtent(FVector extent)
{
	UBoxComponent* boxComponent = Cast<UBoxComponent>(GetCollisionComponent());
	if (IsValid(boxComponent))
	{
		boxComponent->SetBoxExtent(extent);

		boxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ATriggerActor::OnBeginOverlap);
		boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::OnBeginOverlap);
		boxComponent->OnComponentEndOverlap.RemoveDynamic(this, &ATriggerActor::OnEndOverlap);
		boxComponent->OnComponentEndOverlap.AddDynamic(this, &ATriggerActor::OnEndOverlap);
	}
}

void ATriggerActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherComp))
	{
		if (OtherActor != this)
		{
			UShapeComponent* ShapeComp = Cast<UShapeComponent>(OtherComp);
			if (!IsValid(ShapeComp))
				return;

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (IsValid(RGameInstance) /*&& IsValid(RGameInstance->GameObjectMgr)*/)
			{
				IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OtherActor);
				if (BaseProperty)
				{
// 					if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetPartyID() == RGameInstance->GameObjectMgr->GetMyPartyID())
					if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetPartyID() == UObjectFunctionLibrary::GetMyPartyID())
					{
						OnOverlapEvent.Broadcast(ActorID);
						SetActorEnableCollision(false);
					}
				}
			}
		}
	}
}

void ATriggerActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}