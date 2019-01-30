// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EmptyBasicActor.h"
#include "FollowTargetActor.generated.h"

UCLASS()
class PROJECTR_API AFollowTargetActor : public AEmptyBasicActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowTargetActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;

public:
	UFUNCTION(BlueprintCallable, Category = FollowTargetActor)
	void SetFollowTarget(AActor* target) { FollowTarget = target; }

private:
	UPROPERTY()
	TWeakObjectPtr< AActor > FollowTarget;
	
};
