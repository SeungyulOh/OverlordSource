// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "LeftSkillAni.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ULeftSkillAni : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	
	
};
