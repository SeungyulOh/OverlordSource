// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ScreenCaptureActor.generated.h"

UCLASS()
class PROJECTR_API AScreenCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreenCaptureActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = AScreenCaptureActor)
	void RequestScreenCapture();

	UPROPERTY()
	UTexture2D * ScreenCaptureTexture;

	UPROPERTY()
	UMaterialInstanceDynamic * dynamicmaterial;

	UFUNCTION()
	void ProcessConsoleCommand(FString Command);

	UFUNCTION()
	void OnScreenCaptured(int32 xres, int32 yres, UTexture2D * screencapture);
};
