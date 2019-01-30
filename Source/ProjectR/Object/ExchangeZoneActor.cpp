// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ExchangeZoneActor.h"

// Sets default values
AExchangeZoneActor::AExchangeZoneActor()
{
 
}

// Called when the game starts or when spawned
void AExchangeZoneActor::BeginPlay()
{
	Super::BeginPlay();

	SetExchangeZoneActive(false);
}

void AExchangeZoneActor::SetExchangeZoneActive(bool bActive)
{
	if (IsValid(EffectComponent))
	{
		EffectComponent->SetHiddenInGame(!bActive);
	}

	if (IsValid(StaticMeshComponent))
	{
		if (bActive)
		{
			if (IsValid(ActiveMaterialInstance))
			{
				StaticMeshComponent->CreateDynamicMaterialInstance(0, ActiveMaterialInstance);
			}
		}
		else
		{
			if (IsValid(NoneActiveMaterialInstance))
			{
				StaticMeshComponent->CreateDynamicMaterialInstance(0, NoneActiveMaterialInstance);
			}
		}
	}
}