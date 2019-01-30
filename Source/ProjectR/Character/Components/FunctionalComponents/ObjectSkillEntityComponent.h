// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillEntityComponent.h"
#include "ObjectSkillEntityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UObjectSkillEntityComponent : public USkillEntityComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectSkillEntityComponent();

	// Called when the game starts
	void InitializeComponent() override;

	void UninitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	
	void StartSkillApply() override;
	void EndSkillAction_V2() override;

	

private:
		
	bool				IsSkillUsing = false;
};
