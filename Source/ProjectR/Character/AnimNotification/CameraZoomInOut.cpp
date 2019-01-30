// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Object/BaseCameraActor.h"
#include "CameraZoomInOut.h"



void UCameraZoomInOut::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

#if WITH_EDITOR
	return;
#endif

	//UE_LOG(LogBattle, Log, TEXT("Enter UAnimCamRotateState Begin"));
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
 	if (false == IsValid(RGameInstance))
 		return;
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			ABaseCameraActor* CurCamera = RGameInstance->CameraActor;
			if (IsValid(CurCamera))
			{
				CurCamera->AutoZoomCameraByValue(ZoomValue, ZoomTime, ZoomExp);
			}
		}
	}	
}