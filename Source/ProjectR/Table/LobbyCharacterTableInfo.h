// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
//#include "Character/Etc/LobbyActor.h"
#include "Character/BattleBaseCharacter.h"
#include "Item/WeaponBase.h"
#include "LobbyCharacterTableInfo.generated.h"

/**
 *
 */
USTRUCT()
struct PROJECTR_API FLobbyCharacterTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FName							DisplayName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FName							Nick;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
// 	EContinentTypeEnum				Continent = EContinentTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	int32							Frequence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FName							HeroID;
	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
// 	TAssetSubclassOf< ALobbyActor >	LobbyActor = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	TAssetSubclassOf< ABattleBaseCharacter >		LobbyHeroClass = nullptr;
	
	// Right weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	TAssetSubclassOf< AWeaponBase >		RWeaponType = nullptr;
	// Left weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	TAssetSubclassOf< AWeaponBase >		LWeaponType = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FName							LobbyIdleSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FName							LobbyTouchSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FVector							DefaultCameraPosition;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FRotator						DefaultCameraRotator;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FVector							ZoomInCameraPosition;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	FRotator						ZoomInCameraRotator;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	float							TouchSensitivity = 1.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	float							FieldOfView = 90.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	float							RotationMaxAngle = 90.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	float							RotationSpeed = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	float							ModelScale = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FLobbyCharacterTableInfo)
	bool							ZoonInInvert = false;

	//////////////////////////////////////////////////////////////////////////
	// Camera FPostProcessSettings

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings)
	bool EnablePostProcessSetting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings)
	FRotator DirectionalLightDirectionMovable = FRotator(-46.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DirectionalLightIntensityMovable = 10.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor DirectionalLightColorMovable = FLinearColor::White;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float AmbientLightIntensityMovable = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor AmbientLightColorMovable = FLinearColor(0.5f, 0.5f, 0.5f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	FVector RimLightViewSpaceDirection = FVector(-1.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float RimLightIntensity = 10.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor RimLightColor = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float RimLightWidth = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float RimLightFallOff = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DynamicShadowIntensity = 0.15f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (HideAlphaChannel))
	FLinearColor FogColor = FLinearColor(0.3f, 0.475f, 1.0f);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float FogStartDistance = 2500.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "0.001"))
	float FogDensity = 0.0001f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FPostProcessSettings, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float FogMaxOpacity = 1.0f;

//	UClass*		GetLobbyActorBlueprintClass();

	static FLobbyCharacterTableInfo* LookUp(FName InDisplayName);

	static FLobbyCharacterTableInfo* ChooseRandomly();
};