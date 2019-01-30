// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "SharedConstants/GlobalConstants.h"
#include "DetachBuffPhaseNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UDetachBuffPhaseNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DetachBuffPhaseNotify)
	FName				SkillName;

};
