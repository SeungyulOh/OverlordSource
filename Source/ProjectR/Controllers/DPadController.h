// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "DPadController.generated.h"


class URGameInstance;

/**
 * 
 */
UCLASS()
class PROJECTR_API ADPadController : public ABasePlayerController
{
	GENERATED_BODY()
	
public:
	ADPadController(const FObjectInitializer& ObjectInitializer);

private:
	// VE_Attack, VE_AttackHold
	bool AttackProcess();
	bool AttackHoldProcess();
	bool AttackToCurrentTarget();
	bool AttackToNearestTarget();
	
private:
	
};

