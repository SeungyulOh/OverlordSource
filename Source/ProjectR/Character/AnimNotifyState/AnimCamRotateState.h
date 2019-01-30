// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimCamRotateState.generated.h"

class ABaseCameraActor;
class ACamRotatePivot;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBlendRotateType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_BlendRotateLinear	UMETA(DisplayName = "Linear"),
	VE_BlendRotateEaseIn	UMETA(DisplayName = "EaseIn"),
	VE_BlendRotateEaseOut	UMETA(DisplayName = "EaseOut"),
	VE_BlendRotateEaseInOut	UMETA(DisplayName = "EaseInOut")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ERotateType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_RotateLinear			UMETA(DisplayName = "Linear"),
	VE_RotateEaseIn			UMETA(DisplayName = "EaseIn"),
	VE_RotateEaseOut		UMETA(DisplayName = "EaseOut"),
	VE_RotateEaseInOut		UMETA(DisplayName = "EaseInOut")
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UAnimCamRotateState : public UAnimNotifyState
{
	GENERATED_BODY()

	enum eSTATE
	{
		eNoState,
		eBlendState,
		eRotateState,
	};

public:
	
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	float					CamInitDistance = 1300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	float					CamBlendTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	EBlendRotateType		BlendRotateType = EBlendRotateType::VE_BlendRotateLinear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	float					BlendEaseValue = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	ERotateType				RotateType = ERotateType::VE_RotateLinear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	float					EaseValue = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCamRotateState)
	float					Radius = 10.0f;

private:
	UPROPERTY()
	ABaseCameraActor*				CurCamera = nullptr;
	UPROPERTY()
	ACamRotatePivot*				CamPivot = nullptr;
	float							InitCameraDistance = 0.0f;
	float							InitYawDegree = 0.0f;
	float							LastYawDegree = 0.0f;
	float							DiffYawDegree = 0.0f;
	float							TotalTime = 0.0f;
	float							CurTime = 0.0f;
	float							RotateYawDegree = 0.0f;
	float							Length = 0.0f;
	eSTATE							eCurState = eSTATE::eBlendState;
	FVector							StartPos;
	FVector							BlendPos;
	FVector							EndPos;
	FVector							CenterPos;
};
