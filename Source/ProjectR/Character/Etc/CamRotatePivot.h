// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CamRotatePivot.generated.h"


UCLASS()
class PROJECTR_API ACamRotatePivot : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamRotatePivot)
	UArrowComponent*				LeftComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamRotatePivot)
	UArrowComponent*				RightComponent;

	// Sets default values for this actor's properties
	ACamRotatePivot();
};
