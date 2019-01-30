// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/IGameState.h"
#include "TutorialState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UTutorialState : public UIGameState
{
	GENERATED_BODY()
	
public:
	UTutorialState();

	void Enter() override;
	void Leave() override;
	void BeginState() override;
	
};
