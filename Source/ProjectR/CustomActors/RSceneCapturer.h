// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSceneCapturer.generated.h"

UCLASS()
class PROJECTR_API ARSceneCapturer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSceneCapturer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFOV(float FOV);
	void AlignWithCamPosition();

	FORCEINLINE void SetActive(bool InActive) { bActiveCapture = InActive; }

public:
	UPROPERTY()
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY()
	UCanvasRenderTarget2D* RenderTarget;

private:
	UPROPERTY()
	bool bActiveCapture = false;

	
	
};
