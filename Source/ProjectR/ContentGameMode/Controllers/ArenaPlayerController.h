// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BasePlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "ArenaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AArenaPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	
	friend class ULeaderSkillControl;
	friend class ULeaderMoveControl;
public:
	AArenaPlayerController(const FObjectInitializer& ObjectInitializer);
};

