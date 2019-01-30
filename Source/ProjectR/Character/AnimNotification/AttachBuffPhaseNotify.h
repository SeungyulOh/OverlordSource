// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "SharedConstants/GlobalConstants.h"
#include "AttachBuffPhaseNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAttachBuffPhaseNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachBuffPhaseNotify)
	FName				SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachBuffPhaseNotify)
	FName				BuffName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachBuffPhaseNotify)
	EProtectType		ProtectType = EProtectType::VE_None;
};
