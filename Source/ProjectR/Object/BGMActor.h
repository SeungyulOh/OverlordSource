// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BGMActor.generated.h"

UCLASS()
class PROJECTR_API ABGMActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGMActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;

	void PlayBGMIndex(int32 BGMIndex);
	void PauseBGM();
	void StopBGM();
	void FadeInBGM(float fTime = 1.0f);
	void FadeOutBGM(float fTime = 1.0f);
	void ResumeBGM();

	void SetVolume(float InVolume);

private:
	void CheckBGMComponent(int32 BGMIndex);

public:
	UPROPERTY()
	UAudioComponent*	BGMComponent = nullptr;
};
