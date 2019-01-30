// Fill out your copyright notice in the Description page of Project Settings.
//#if WITH_KOKOMO

#pragma once

#include "Components/ActorComponent.h"
#include "SDamageFloater.h"
#include "SlateDamageFloaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API USlateDamageFloaterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlateDamageFloaterComponent();

	// Called when the game starts
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void AddDamageFloater(int32 damagecount);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

private:
	TWeakObjectPtr<AActor> TargetActor;
	TArray<TSharedRef<SDamageFloater>>	DamageFloaters;

	TSharedRef<SDamageFloater> SpawnDamageFloater(int32 damagecount);
};

//#endif