#pragma once

#include "MapPostProcessSettingInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FMapPostProcessSettingInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	// Directinal Setting
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	bool EnableDirectionalSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	FRotator DirectionalLightDirectionMovable = FRotator(-46.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DirectionalLightIntensityMovable = 10.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor DirectionalLightColorMovable = FLinearColor::White;


	// Ambient Setting
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	bool EnableAmbientSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float AmbientLightIntensityMovable = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor AmbientLightColorMovable = FLinearColor(0.5f, 0.5f, 0.5f);


	// Rim Light Setting
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	bool EnableRimLightSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	FVector RimLightViewSpaceDirection = FVector(-1.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float RimLightIntensity = 10.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor RimLightColor = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float RimLightWidth = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float RimLightFallOff = 0.0f;


	// Dynamic Shadow Setting
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	bool EnableDynamicShadowSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DynamicShadowIntensity = 0.15f;


	// Fog Setting
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings)
	bool EnableFogSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor FogColor = FLinearColor(0.3f, 0.475f, 1.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float FogStartDistance = 2500.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "0.001"))
	float FogDensity = 0.0001f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FMapPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float FogMaxOpacity = 1.0f;
};
