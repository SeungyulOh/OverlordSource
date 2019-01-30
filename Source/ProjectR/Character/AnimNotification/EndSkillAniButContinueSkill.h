// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndSkillAniButContinueSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UEndSkillAniButContinueSkill : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
	
};
