// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BattleBaseCharacter.h"
#include "NpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ANpcCharacter : public ABattleBaseCharacter
{
	GENERATED_BODY()
	
public:
	ANpcCharacter(const FObjectInitializer& ObjectInitializer);

};
