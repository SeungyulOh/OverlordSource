// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PlaySoundNotify.h"
#include "Classes/Sound/SoundBase.h"
#include "UtilFunctionIntegrated.h"

/////////////////////////////////////////////////////
// UAnimNotify_PlaySound

UPlaySoundNotify::UPlaySoundNotify()
	: Super()
{
	VolumeMultiplier = 1.f;
	PitchMultiplier = 1.f;
	Is3DSound = true;
	bFollow = false;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA
}

void UPlaySoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
//#if RULE_TODOMEMORY
//	return;
//#endif

	if (false == IsValid(MeshComp))
		return;

	if (MeshComp->ComponentHasTag(TAG_NOSOUND))
		return;

	if (SoundKey.IsNone())
		return;

	if (Is3DSound)
	{
		if (bFollow == true)
		{
			UUtilFunctionLibrary::PlaySoundSceneComponent(SoundKey, MeshComp, AttachName, VolumeMultiplier, PitchMultiplier);
		}
		else
		{
			UUtilFunctionLibrary::PlaySoundLocation(MeshComp->GetWorld(), SoundKey, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
		}
	}
	else
	{
		UUtilFunctionLibrary::PlaySound2D(MeshComp->GetWorld(), SoundKey, VolumeMultiplier, PitchMultiplier);
	}
}


FString UPlaySoundNotify::GetNotifyName_Implementation() const
{
	if (SoundKey.IsNone())
	{
		return Super::GetNotifyName_Implementation();
	}
	else
	{
		return SoundKey.ToString();
	}
}