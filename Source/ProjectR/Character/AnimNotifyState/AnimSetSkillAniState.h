// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimSetSkillAniState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAnimSetSkillAniState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	

private:
	UPROPERTY()
	ESkillAniTypeEnum CurrentSkillAniType = ESkillAniTypeEnum::VE_None;
};
