// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimTargetRotateState.generated.h"

class UBattleBaseAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UAnimTargetRotateState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetRotateState)
	float		LimitDegree = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetRotateState)
	float		RotateSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetRotateState)
	bool		bRestore = false;

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >			OwnerCharacter;
	UPROPERTY()
	TWeakObjectPtr< AActor >			TargetActor;
	UPROPERTY()
	TWeakObjectPtr< UBattleBaseAnimInstance >		CurAnimInstance;
	UPROPERTY()
	float							TotalTime = 0.0f;
	UPROPERTY()
	float							CurTime = 0.0f;
};
