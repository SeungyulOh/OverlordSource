// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SkillEntityComponent.h"
#include "RemoteSkillEntityComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API URemoteSkillEntityComponent : public USkillEntityComponent
{
	GENERATED_BODY()
		 
public:
	// Sets default values for this component's properties
	URemoteSkillEntityComponent();

	// Called when the game starts
	void InitializeComponent() override;

	void UninitializeComponent() override;

	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EEndPlayReason) override;

	void UpdateSkillComponent(float DeltaSecond) override;	
};
