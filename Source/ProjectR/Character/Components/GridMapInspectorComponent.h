// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Global/GridMapManager.h"
#include "GridMapInspectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UGridMapInspectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridMapInspectorComponent();

	// Called when the game starts
	void InitializeComponent() override;
	void UninitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	

	void			UpdateGridMap();
	FCellIndex&		GetCurrentGridIndex();	

private:
	UPROPERTY()
	FCellIndex		CurrentGridIndics;
	UPROPERTY()
	FCellIndex		ReserveTargetGridIndics;
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	
};
