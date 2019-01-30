// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CameraZoomInOutBySkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCameraZoomInOutBySkill : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	bool bStartZoomIn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	float	ZoomPercent = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	float	StartTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	float	DurationTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	float	EndTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	UCurveFloat*	Curve_ZoomInOutBySkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoomInOutBySkill")
	UCurveFloat*	AddEndCurve_ZoomInOutBySkill = nullptr;
};
