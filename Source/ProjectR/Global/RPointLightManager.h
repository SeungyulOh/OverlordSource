// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPointLightManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URPointLightManager : public UObject
{
	GENERATED_BODY()
	
public:
	URPointLightManager();

public:
	void AddPointLight(FVector Location, float Inlifetime , float InAttenuationRadius, FLinearColor InLightColor, float InLightIntensity, bool isHaveTarget = false);

public:
	UPROPERTY()
	TArray<class ABasePointLight*> PointLightArray;

	UPROPERTY()
	TArray<TWeakObjectPtr<class ABulletBase>> TargetBulletArray;

};
