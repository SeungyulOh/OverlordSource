// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SharedConstants/GlobalConstants.h"
#include "AnimBuffPhaseState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAnimBuffPhaseState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAnimBuffPhaseState)
	FName				SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAnimBuffPhaseState)
	FName				BuffName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAnimBuffPhaseState)
	EProtectType		ProtectType = EProtectType::VE_None;
};
