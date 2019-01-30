// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "SharedConstants/GlobalConstants.h"
#include "ShotProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UShotProjectile : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotProjectile")
	EBulltSocketType		SocketType = EBulltSocketType::VE_None;
};
