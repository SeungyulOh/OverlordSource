// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "CameraShakeNotify.h"



void UCameraShakeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (false == IsValid(AnimInstance))
		return;

	AActor* Owner = AnimInstance->GetOwningActor();
	if (false == IsValid(Owner))
		return;

	if (IsSelectedCharacterExecute)
	{
		if (false == UUtilFunctionLibrary::IsLeaderCharacter(Owner))
			return;
	}

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(MeshComp->GetWorld());
	if (IsValid(BasePlayerController))
	{
		BasePlayerController->PlayCameraShake(CameraShakeEffect, ShakeIntensity);
	}
}