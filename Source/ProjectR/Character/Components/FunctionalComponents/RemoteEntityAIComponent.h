// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EntityAIComponent.h"
#include "RemoteEntityAIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API URemoteEntityAIComponent : public UEntityAIComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URemoteEntityAIComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// don't ai restart in remote entity component
//	virtual void			AIRestart() override;
};
