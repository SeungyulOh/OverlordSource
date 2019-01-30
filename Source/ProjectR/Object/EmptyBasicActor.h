// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EmptyBasicActor.generated.h"

UCLASS()
class PROJECTR_API AEmptyBasicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmptyBasicActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;

public:
	UPROPERTY()
	class USceneComponent* Scene = nullptr;
};
