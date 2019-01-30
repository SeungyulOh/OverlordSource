// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SharedConstants/GlobalConstants.h"
#include "VictoryPosition.generated.h"

UCLASS()
class PROJECTR_API AVictoryPosition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVictoryPosition();

	UFUNCTION(BlueprintCallable, Category = AVictoryPosition)
	void SetActorVictoryPosition();

	UFUNCTION(BlueprintCallable, Category = AVictoryPosition)
	void SetActorVictoryPosition_Campain();

	UFUNCTION(BlueprintCallable, Category = AVictoryPosition)
	void SetActorVictoryPosition_Arena();

	UFUNCTION(BlueprintCallable, Category = AVictoryPosition)
	void SetActorVictoryPosition_MirrorDungeon();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AVictoryPosition)
	TArray<USceneComponent*>	PositionCompList;
	
};
