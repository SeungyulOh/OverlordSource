// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "MirrorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AMirrorPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	AMirrorPlayerController(const FObjectInitializer& ObjectInitializer);
};

