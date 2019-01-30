// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ChangeMaterialParmState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UChangeMaterialParmState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmState)
	FName				ScalarParmName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmState)
	float				ScalarParm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmState)
	FName				VectorParmName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChangeMaterialParmState)
	FLinearColor		VectorParm;

private:
	UPROPERTY()
	float					OriScalarParm;
	UPROPERTY()
	FLinearColor			OriVectorParm;
};
