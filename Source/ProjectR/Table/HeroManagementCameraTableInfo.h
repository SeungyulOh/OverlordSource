#pragma once
#include "HeroManagementCameraTableInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FHeroManagementCameraTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FHeroManagementCameraTableInfo();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraMinPitch;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraMaxPitch;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraMinDistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraMaxDistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float TouchSensitivity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float PinchSensitivity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	bool EnableHeadTracking;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float MaxHeadYawAngle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float MaxHeadPitchAngle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float HeadRotationSpeedMagnifier;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float HeadTrackingYawCompensationValue;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraNearElevation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float CameraFarElevation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FVector CameraPositionPivot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FVector CharacterPositionPivot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float DefaultDistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float DefaultCameraAzimuth;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float DefaultCameraPitch;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float DefaultCharacterYaw;


};
