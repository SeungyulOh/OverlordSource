// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "IsVisibleBlobShadow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UIsVisibleBlobShadow : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VisibleBlobShadowNotify)
	bool IsVisibleShadow = true;
	
};
