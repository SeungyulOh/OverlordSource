// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CameraShakeNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCameraShakeNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float ShakeIntensity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	TSubclassOf<class UCameraShake >	CameraShakeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	bool IsSelectedCharacterExecute = true;
	
};
