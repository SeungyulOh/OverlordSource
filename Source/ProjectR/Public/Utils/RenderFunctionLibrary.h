// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RenderFunctionLibrary.generated.h"

struct FMapTableInfo;


UCLASS()
class PROJECTR_API URenderFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetDarken(bool bEnableDrawing);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void IgnoreDarkenTarget(AActor* Target, bool bIgnore);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetEngineFlags(const FString cmd, bool state);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static bool GetEngineFlags(const FString cmd);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void ShowUntaggedActors(FName tagforignoring, bool show);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void EnableDOF(bool InEnable);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetVignetteIntensity(float InIntensity);

	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetPostProcessSetting(FName MapID);

	
	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetOutLineEffect(float TimeAcc);
	
	
	UFUNCTION(BlueprintCallable, Category = URenderFunctionLibrary)
	static void SetBloomEffectWithViewTarget(bool isOn, float InPower);

public:
	
	

	TSet<TWeakObjectPtr<AActor>> IgnoreDarkenActors;
	//static bool Darken;
};