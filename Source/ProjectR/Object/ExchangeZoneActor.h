// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExchangeZoneActor.generated.h"

UCLASS()
class PROJECTR_API AExchangeZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExchangeZoneActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;

	UFUNCTION()
	void SetExchangeZoneActive(bool bActive);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AExchangeZoneActor)
	UStaticMeshComponent*			StaticMeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AExchangeZoneActor)
	UParticleSystemComponent*		EffectComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AExchangeZoneActor)
	UMaterialInstance*				ActiveMaterialInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AExchangeZoneActor)
	UMaterialInstance*				NoneActiveMaterialInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AExchangeZoneActor)
	int32							ZoneIndex = 0;
};
