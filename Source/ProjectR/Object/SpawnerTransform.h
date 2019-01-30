// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Engine/TriggerBox.h"

#include "SpawnerTransform.generated.h"



UCLASS()
class PROJECTR_API ASpawnerTransform : public ATriggerBox
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpawnerTransform();
	void BeginPlay() override;

	UFUNCTION()
	void InitTrigger();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnerTransform)
	int32							SpawnerTransformID = 0;
};
