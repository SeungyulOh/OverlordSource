// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ChangeAniSpeedState.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBlendAniSpeedType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_BlendLinear			UMETA(DisplayName = "Linear"),
	VE_BlendEaseIn			UMETA(DisplayName = "EaseIn"),
	VE_BlendEaseOut			UMETA(DisplayName = "EaseOut"),
	VE_BlendEaseInOut		UMETA(DisplayName = "EaseInOut")
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UChangeAniSpeedState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	enum eSTATE
	{
		eNoState,
		eChangeState,
		eWaitingState,
		eRestoreState,
	};

public:
		
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeAniSpeedState)
	float					ChangeAniSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeAniSpeedState)
	float					ChangeTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeAniSpeedState)
	EBlendAniSpeedType		BlendType = EBlendAniSpeedType::VE_BlendLinear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeAniSpeedState)
	float					BlendEaseValue = 2.0f;

private:
	eSTATE					CurState = eSTATE::eNoState;
	float					CurTime = 0.0f;
	float					TotalTime = 0.0f;
	float					RestoreTime = 0.0f;
	float					OriChangeAniSpeed = 1.0f;
	float					StartSpeed = 1.0f;
	float					EndSpeed = 1.0f;
};
