// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "OrbitCameraController.h"

#include "../Table/HeroManagementCameraTableInfo.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

UOrbitCameraController* UOrbitCameraController::instance = nullptr;

UOrbitCameraController::UOrbitCameraController(const class FObjectInitializer &PCIP)
	: Super(PCIP)
{
	//maxpitch = 70.0f;
	//minpitch = 0.0f;
	//mindistance = 150.0f;
	//maxdistance = 600.0f;
	//touchsensitivity = 2.0f;
	//pinchsensitivity = 2.0f;
	//maxheadyawangle = 90.0f;
	//maxheadpitchangle = 60.0f;
	//headrotspeedmagnifier = 10.0f;
	//camerabasezpivot = 0.0f;
	//distance = 200.0f;

	mode = E_MODE_ROTATEMARNNEQUIN;
	lookatpivot = FVector::ZeroVector;

	camerarotation.Yaw = 0.0f;
	camerarotation.Pitch = 0.0f;
	marnnequinrotation.Yaw = 0.0f;
	marnnequinrotation.Pitch = 0.0f;

	lookatbonename = FName("Bip001-Head");
	boneindex = -1;
	currentcameraspec = nullptr;
}

template<typename T>
T* GetComponent(AActor* actor)
{
	if (IsValid(actor) == false) return nullptr;
	return Cast<T>(actor->GetComponentByClass(T::StaticClass()));
}

void UOrbitCameraController::SetTarget(AActor* actor, FName heroid)
{
	CompleteUpdate = false;
	targetactor = actor;
	if (targetactor.IsValid() == true)
	{
		SetLookAtBone(lookatbonename);
	}
	
	//FCharacterTableInfo
	auto charinfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroid);
	if (charinfo == nullptr)
	{
		currentcameraspec = UTableManager::GetInstancePtr()->GetHeroManagementCameraRow(FName("Default"));
		return;
	}
	currentcameraspec = UTableManager::GetInstancePtr()->GetHeroManagementCameraRow(charinfo->DisplayName);
	if(currentcameraspec == nullptr)
		currentcameraspec = UTableManager::GetInstancePtr()->GetHeroManagementCameraRow(FName("Default"));

	headtracking = currentcameraspec->EnableHeadTracking;
	SetDistance(currentcameraspec->DefaultDistance, false);
	SetMarnnequinYaw(currentcameraspec->DefaultCharacterYaw, false);
	SetOrbitAngle(/*currentcameraspec->DefaultCharacterYaw*/ 0.0f, false);
	SetOrbitPitch(currentcameraspec->DefaultCameraPitch, false);

	curheadrotation = FRotator(0.0f, currentcameraspec->DefaultCharacterYaw, 0.0f);
	targetactor->SetActorRotation(curheadrotation);
	targetactor->SetActorLocation(currentcameraspec->CharacterPositionPivot);
	//////////////////////////////////////////////////////////////////////////

	//UpdateCameraTransform(0.0f);
}

void UOrbitCameraController::SetCamera(AActor* camera)
{
	cameratocontrol = camera;
	if (cameratocontrol.IsValid() == true)
		cameracomponent = GetComponent<UCameraComponent>(cameratocontrol.Get());
}

void UOrbitCameraController::SetDistance(float _distance, bool relative = true)
{
	if (currentcameraspec == nullptr) return;

	_distance = relative == true ? distance + _distance : _distance;
	distance = FMath::Clamp(_distance, currentcameraspec->CameraMinDistance, currentcameraspec->CameraMaxDistance);
#if WITH_EDITOR
	UE_LOG(LogBattle, Log, TEXT("Camera controller distance :%d"), distance);
#endif
}

void UOrbitCameraController::SetOrbitAngle(float angle, bool relative = true)
{
	angle = relative == true ? camerarotation.Yaw + angle : angle;
	camerarotation.Yaw = angle;
}

void UOrbitCameraController::SetMarnnequinYaw(float angle, bool relative = true)
{
	angle = relative == true ? marnnequinrotation.Yaw + angle : angle;
	marnnequinrotation.Yaw = angle;
}

void UOrbitCameraController::SetOrbitPitch(float angle, bool relative = true)
{
	if (currentcameraspec == nullptr) return;

	angle = relative == true ? camerarotation.Pitch + angle : angle;
	camerarotation.Pitch = FMath::Clamp(angle, currentcameraspec->CameraMinPitch, currentcameraspec->CameraMaxPitch);
}

void UOrbitCameraController::Drag(FDragInfo drag)
{
	if(currentcameraspec == nullptr) return;

	switch (mode)
	{
	case E_MODE_ORBITCAMERA: SetOrbitAngle((drag.CurrentPos - drag.LastPos).X * currentcameraspec->TouchSensitivity); break;
	case E_MODE_ROTATEMARNNEQUIN: SetMarnnequinYaw(-(drag.CurrentPos - drag.LastPos).X * currentcameraspec->TouchSensitivity); break;
	}
	SetOrbitPitch((drag.CurrentPos - drag.LastPos).Y * currentcameraspec->TouchSensitivity);
	CompleteUpdate = false;
}

void UOrbitCameraController::Drag(float InValue)
{
	if (currentcameraspec == nullptr) return;

	switch (mode)
	{
	case E_MODE_ORBITCAMERA: SetOrbitAngle(InValue * currentcameraspec->TouchSensitivity); break;
	case E_MODE_ROTATEMARNNEQUIN: SetMarnnequinYaw(-InValue * currentcameraspec->TouchSensitivity); break;
	}
	CompleteUpdate = false;
}

void UOrbitCameraController::Pinch(FPinchInfo pinch)
{
	SetDistance(-pinch.DeltaDist * currentcameraspec->PinchSensitivity);
}

void UOrbitCameraController::SetLookAtBone(const FName bonename)
{
	if (targetactor.IsValid() == false) return;
	//meshcomponent = GetComponent<USkeletalMeshComponent>(targetactor.Get());
	meshcomponent = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(targetactor.Get());
	if (meshcomponent == nullptr)
	{
		lookatpivot = FVector::ZeroVector;
		lookatbonename = FName("");
		boneindex = INDEX_NONE;
		return;
	}
		
	lookatpivot = meshcomponent->GetSocketLocation(bonename) - targetactor.Get()->GetActorLocation();
	lookatbonename = bonename;
	boneindex = meshcomponent->GetBoneIndex(bonename);
	if (boneindex != INDEX_NONE)
	{
		meshcomponent->GetAllChildBones(boneindex, childbones);
	}
}

void UOrbitCameraController::FrameMove(float DeltaTime)
{
	if (instance == nullptr) instance = this;

	if (currentcameraspec == nullptr ||
		targetactor.IsValid() == false ||
		cameratocontrol.IsValid() == false ||
		cameracomponent.IsValid() == false) return;

	UpdateCameraTransform(DeltaTime);
	UpdateCameraLookAt(DeltaTime);
}

void UOrbitCameraController::PostLoad()
{
	UE_LOG(LogUI, Log, TEXT("Debug"));
}

void UOrbitCameraController::UpdateCameraTransform(float DeltaTime)
{
	if (CompleteUpdate)
		return;

	if (currentcameraspec == nullptr ||
		targetactor.IsValid() == false ||
		cameratocontrol.IsValid() == false ||
		cameracomponent.IsValid() == false) return;

	targetactor.Get()->SetActorRotation(marnnequinrotation);

	float cameramovementlength = FMath::Abs(currentcameraspec->CameraMaxDistance - currentcameraspec->CameraMinDistance);
	float t = (distance - currentcameraspec->CameraMinDistance) / cameramovementlength;
	float currentcameraelevation = FMath::Lerp(currentcameraspec->CameraNearElevation, currentcameraspec->CameraFarElevation, t);
	FVector basepos = targetactor->GetActorLocation() + FVector(0.0f, 0.0f, currentcameraelevation);
	FVector pivot = camerarotation.Vector() * distance;
	cameratocontrol->SetActorRelativeRotation((-pivot + lookatpivot).Rotation());
	cameratocontrol->SetActorLocation(basepos + pivot + currentcameraspec->CameraPositionPivot);
	cameracomponent.Get()->SetWorldLocation(basepos + pivot + currentcameraspec->CameraPositionPivot);
	CompleteUpdate = true;
}

void UOrbitCameraController::UpdateCameraLookAt(float DeltaTime)
{
	if (currentcameraspec == nullptr ||
		targetactor.IsValid() == false ||
		cameratocontrol.IsValid() == false ||
		cameracomponent.IsValid() == false) return;

	if (false == headtracking)
		return;
	if (boneindex == INDEX_NONE)
		return;
	if (false == meshcomponent.IsValid())
		return;


	FTransform* transform = meshcomponent->GetEditableBoneTransform(boneindex);
	FQuat boneoriginalrotation = transform->GetRotation();

	FVector pivot = camerarotation.Vector() * distance;
	FRotator pivotrot(FQuat((pivot - lookatpivot).Rotation()));
	FRotator meshrot(meshcomponent->GetComponentQuat());
	FRotator actorrot = targetactor->GetActorRotation();
	FRotator actormeshpivotrot = (actorrot - meshrot);
	float yawdelta = FMath::GetDeltaAngle(actorrot.Yaw, pivotrot.Yaw + currentcameraspec->HeadTrackingYawCompensationValue);
	bool returntoorigrot = (FMath::Abs(yawdelta) > currentcameraspec->MaxHeadYawAngle);
	float destpivotyaw = returntoorigrot == true ? actorrot.Yaw : pivotrot.Yaw;
	float destpivotpitch =
		(returntoorigrot == true || FMath::GetDeltaAngle(actorrot.Pitch, pivotrot.Pitch) > currentcameraspec->MaxHeadPitchAngle) ?
		actorrot.Pitch : pivotrot.Pitch;
	float tAngle = DeltaTime * currentcameraspec->HeadRotationSpeedMagnifier;
	curheadrotation.Yaw = FMath::LerpEulerAngle(curheadrotation.Yaw, destpivotyaw, tAngle);
	curheadrotation.Pitch = FMath::LerpEulerAngle(curheadrotation.Pitch, destpivotpitch, tAngle);
	curheadrotation.Roll = 0.0f;

	FQuat destrotation = targetactor->GetActorQuat().Inverse() *
		FQuat(actormeshpivotrot) *
		FQuat(curheadrotation) *
		FQuat(actormeshpivotrot).Inverse();

	meshcomponent->AddRotationToBone(boneindex, destrotation, childbones);
}