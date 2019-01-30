// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Global/InputManager.h"
#include "OrbitCameraController.generated.h"

UCLASS(Config=Game)
class PROJECTR_API UOrbitCameraController : public UObject	//, public FTickableGameObject
{
	GENERATED_BODY()

	//UPROPERTY(Config)
	//float CameraDistance;

	//UPROPERTY(Config)
	//float MinPitch;

	//UPROPERTY(Config)
	//float MaxPitch;

	//UPROPERTY(Config)
	//float MinDistance;

	//UPROPERTY(Config)
	//float MaxDistance;

	//UPROPERTY(Config)
	//float TouchSensitivity;

	//UPROPERTY(Config)
	//float PinchSensitivity;

	//UPROPERTY(Config)
	//float MaxHeadYawAngle;

	//UPROPERTY(Config)
	//float MaxHeadPitchAngle;

	//UPROPERTY(Config)
	//float HeadRotationSpeedMagnifier;

	//UPROPERTY(Config)
	//float CameraBaseZPivot;

public:
	enum E_MODE
	{
		E_MODE_ORBITCAMERA,
		E_MODE_ROTATEMARNNEQUIN,
	};

	UOrbitCameraController(const class FObjectInitializer &PCIP);

	static UOrbitCameraController* Get()
	{
		return instance;
	}

 	void FrameMove(float DeltaTime);
// 	virtual bool IsTickable() const override { return true; }
// 	virtual TStatId GetStatId() const override { return Super::GetStatID(); }

	void PostLoad() override;

	void UpdateCameraTransform(float DeltaTime);
	void UpdateCameraLookAt(float DeltaTime);

	void SetTarget(AActor* actor, FName heroid);
	void SetCamera(AActor* camera);

	void SetDistance(float _distance, bool relative);
	void SetOrbitAngle(float angle, bool relative);
	void SetMarnnequinYaw(float angle, bool relative);
	void SetOrbitPitch(float angle, bool relative);
	void SetLookAtBone(const FName bonename);

	void SetMode(E_MODE _mode) { mode = _mode; }
	void TurnOnHeadTracking(bool enable) { headtracking = enable; }

	UFUNCTION()
	void Drag(FDragInfo drag);
	void Drag(float InValue);

	UFUNCTION()
	void Pinch(FPinchInfo pinch);

private:
	static UOrbitCameraController* instance;

private:
	struct FHeroManagementCameraTableInfo * currentcameraspec;
	float distance;

	TWeakObjectPtr<AActor> targetactor;
	TWeakObjectPtr<AActor> cameratocontrol;
	FRotator camerarotation;
	FRotator marnnequinrotation;
	FVector lookatpivot;
	E_MODE mode;
	TWeakObjectPtr<UCameraComponent> cameracomponent;
	TWeakObjectPtr<USkeletalMeshComponent> meshcomponent;
	FName lookatbonename;
	int32 boneindex;
	FRotator curheadrotation;
	TArray<int32> childbones;
	bool headtracking;

	bool CompleteUpdate = false;
};
