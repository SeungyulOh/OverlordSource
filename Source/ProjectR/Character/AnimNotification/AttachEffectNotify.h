// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachEffectNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAttachEffectNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachEffectNotify)
	FName				Effectid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachEffectNotify)
	UParticleSystem*	EffectTemplate = nullptr; //todo del
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachEffectNotify)
	FName				AttachPointName;
};
