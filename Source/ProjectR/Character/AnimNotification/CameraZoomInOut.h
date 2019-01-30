// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CameraZoomInOut.generated.h"

class ABaseCameraActor;
/**
 * 
 */
UCLASS()
class PROJECTR_API UCameraZoomInOut : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOut")
	float	ZoomValue = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOut")
	float	ZoomTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOut")
	float	ZoomExp = 1.0f;
};
