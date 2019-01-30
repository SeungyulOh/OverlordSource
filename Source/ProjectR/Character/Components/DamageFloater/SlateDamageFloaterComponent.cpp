// Fill out your copyright notice in the Description page of Project Settings.
//#if WITH_KOKOMO

#include "ProjectR.h"
#include "SlateDamageFloaterComponent.h"

// Sets default values for this component's properties
USlateDamageFloaterComponent::USlateDamageFloaterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USlateDamageFloaterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USlateDamageFloaterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		for (int32 i = 0; i < DamageFloaters.Num(); ++i)
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(DamageFloaters[i]);
		}
	}

	DamageFloaters.Empty();
	Super::EndPlay(EndPlayReason);
}

void USlateDamageFloaterComponent::AddDamageFloater(int32 damagecount)
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		TSharedRef<SDamageFloater> damagefloater = SpawnDamageFloater(damagecount);
		GEngine->GameViewport->AddViewportWidgetContent(damagefloater);
		DamageFloaters.Emplace(damagefloater);
	}
}

void USlateDamageFloaterComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		for (int i = DamageFloaters.Num() - 1; i >= 0 ; --i)
		{
			if (DamageFloaters[i].Get().GetRemainedLifeSeconds() <= 0.0f)
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(DamageFloaters[i]);
				DamageFloaters.RemoveAt(i);
			}
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TSharedRef<SDamageFloater> USlateDamageFloaterComponent::SpawnDamageFloater(int32 damagecount)
{
	TSharedRef<SDamageFloater> result = SNew(SDamageFloater)
		.ColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 0)))
		.OwnerActor(GetOwner())
		.Damage(FMath::Abs(damagecount))
		.Positive(damagecount >= 0)
		.Magnifier(1.0f)
		.YPivot(0.0f)
		.FloatingSpeed(100.0f)
		.LifeSeconds(1.0f);
	return result;
}

//#endif