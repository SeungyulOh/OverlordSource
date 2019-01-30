// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SharedConstants/GlobalConstants.h"
#include "AnimVisibleCharacterObjectState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAnimVisibleCharacterObjectState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimVisibleCharacterObjectState)
	EAttachCharacterObjectEnum	CharacterObjectType;

private:
	void ApplyAnimStateImp(AActor* character, bool IsNotifyEnd);
};
