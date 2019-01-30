// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EntityAIComponent.h"
#include "ObjectEntityAIComponent.generated.h"

class AMatineeActor;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UObjectEntityAIComponent : public UEntityAIComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObjectEntityAIComponent();

	void			ForceSearchTarget() override;
	void			ForceDirection(FVector InTargetLocation) override;
	void			SetForcePauseBySkill(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee = true) override;
};
