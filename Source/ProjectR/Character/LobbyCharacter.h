// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BattleBaseCharacter.h"
#include "LobbyCharacter.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API ALobbyCharacter : public ABattleBaseCharacter
{
	GENERATED_BODY()
	
public:

	ALobbyCharacter(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void Destroyed() override;
	void Tick( float DeltaSecond ) override;
	
};
