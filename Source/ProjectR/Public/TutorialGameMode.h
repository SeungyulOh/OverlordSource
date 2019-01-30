// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "TutorialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ATutorialGameMode : public AProjectRGameMode
{
	GENERATED_BODY()

public:
	ATutorialGameMode(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type Type) override;

	void Tick(float DeltaTime) override;
	
	
	
	
};
