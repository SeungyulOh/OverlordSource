#include "ProjectR.h"
#include "HeroManagementCameraTableInfo.h"

FHeroManagementCameraTableInfo::FHeroManagementCameraTableInfo()
{
	CameraMinPitch = -10.0f;
	CameraMaxPitch = 60.0f;
	CameraMinDistance = 150.0f;
	CameraMaxDistance = 600.0f;
	TouchSensitivity = 2.0f;
	PinchSensitivity = 2.0f;
	EnableHeadTracking = true;
	HeadRotationSpeedMagnifier = 10.0f;
	MaxHeadYawAngle = 65.0f;
	MaxHeadPitchAngle = 60.0f;
	HeadTrackingYawCompensationValue = -25.0f;
	CameraNearElevation = 30.0f;
	CameraFarElevation = 0.0f;
	CameraPositionPivot = FVector::ZeroVector;
	CharacterPositionPivot = FVector::ZeroVector;
	DefaultDistance = 350.0f;
	DefaultCameraAzimuth = 180.0f;
	DefaultCameraPitch = 0.0f;
	DefaultCharacterYaw = 0.0f;
}