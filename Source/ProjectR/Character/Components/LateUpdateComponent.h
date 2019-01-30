// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LateUpdateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API ULateUpdateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULateUpdateComponent();

	// Called when the game starts
	void BeginPlay() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OnComponentCreated() override;
	
public:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	UPROPERTY()
	bool					IsDeadClearCall = true;
};
