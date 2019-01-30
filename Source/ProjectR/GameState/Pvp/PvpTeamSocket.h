// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SharedConstants/GlobalConstants.h"
#include "PvpTeamSocket.generated.h"

UCLASS()
class PROJECTR_API APvpTeamSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APvpTeamSocket();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = APvpTeamSocket)
	ETeamNO		TeamNO = ETeamNO::VE_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = APvpTeamSocket)
	TArray<USceneComponent*>	PositionCompList;
};
