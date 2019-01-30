#pragma once

#include "EngineMinimal.h"
#include "IndoorHeroManagementViewTableInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FIndoorHeroManagementViewTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FVector CameraLocation;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FRotator CameraRotation;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	float Fov;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FVector CharacterLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HeroManagementCameraTableInfo")
	FRotator CharacterRotation;

};
