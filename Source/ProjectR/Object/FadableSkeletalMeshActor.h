// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "FadableInterface.h"
#include "FadableSkeletalMeshActor.generated.h"

/**
 * see FadableStaticMeshActor
 */
UCLASS()
class PROJECTR_API AFadableSkeletalMeshActor : public ASkeletalMeshActor, public IFadableInterface
{
	GENERATED_BODY()
	
public:
	AFadableSkeletalMeshActor();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	//// Interface
	
	UFUNCTION(BlueprintCallable, Category = Fade)
	void FadeOut();

	UFUNCTION(BlueprintCallable, Category = Fade)
	void FadeIn();


	//// BP configurables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	float AnimationDuration = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	float MinimumOpacity = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	TEnumAsByte<EEasingFunc::Type> InterpFunction = EEasingFunc::Type::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	float InterpBlendExp = 2.0;
	
private:

	bool bCurrentlyAnimating = false;

	float Multiplier = 0.0;
	
	// opacity value normalized to 0.0~1.0
	float Cur = 1.0;
};
