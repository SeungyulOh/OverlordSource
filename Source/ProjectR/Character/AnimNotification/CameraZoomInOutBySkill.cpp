// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CameraZoomInOutBySkill.h"

#include "Character/BattleBaseAnimInstance.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"

void UCameraZoomInOutBySkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	if (false == UUtilFunctionLibrary::IsLeaderCharacter(Owner))
		return;

	if (false == RGameInstance->OptionManager->IsUltimateMatineeView())
		return;

	if (URealTimeModManager::IsRealTimeGameMode())
		return;
	
	if (IsValid(RGameInstance->CameraActor))
	{
		RGameInstance->CameraActor->AutoZoomInOutCameraByPercent(bStartZoomIn, ZoomPercent, StartTime, DurationTime, EndTime, Curve_ZoomInOutBySkill, AddEndCurve_ZoomInOutBySkill);
	}

}