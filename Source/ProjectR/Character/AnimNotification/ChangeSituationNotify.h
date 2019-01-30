// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "SharedConstants/GlobalConstants.h"
#include "ChangeSituationNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UChangeSituationNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, Category = ChangeSituationNotify)
	ECharacterSituationState	SituationState;
};
