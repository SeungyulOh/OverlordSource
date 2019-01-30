// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "ChangeMaterialParmNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UChangeMaterialParmNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmNotify)
	FName				ScalarParmName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmNotify)
	float				ScalarParm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmNotify)
	FName				VectorParmName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmNotify)
	FLinearColor		VectorParm;
};
