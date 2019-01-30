// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlaySoundNotify.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class PROJECTR_API UPlaySoundNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UPlaySoundNotify();

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	// End UAnimNotify interface

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify)
	FName		SoundKey;

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify)
	float		VolumeMultiplier;

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify)
	float		PitchMultiplier;

	//////////////////////////////////////////////////////////////////////////
	// 3D

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify)
	bool		Is3DSound;

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify, meta = (EditCondition = "Is3DSound"))
	bool		bFollow;

	UPROPERTY(EditAnywhere, Category = PlaySoundNotify, meta = (EditCondition = "bFollow"))
	FName		AttachName;

};
