// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DeadActor.generated.h"


UCLASS()
class PROJECTR_API ADeadActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADeadActor(const FObjectInitializer& ObjectInitializer);
};
