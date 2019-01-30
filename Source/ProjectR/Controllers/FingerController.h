// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "FingerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AFingerController : public ABasePlayerController
{
	GENERATED_BODY()
	
public:
	AFingerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

private:

};

