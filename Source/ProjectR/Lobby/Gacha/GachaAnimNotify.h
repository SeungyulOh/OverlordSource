// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GachaAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGachaBloomNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	UCurveFloat* Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float		LifeTime;
	
};

UCLASS()
class PROJECTR_API UGachaFadeOutNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float		LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FLinearColor FadeColor;
};

UCLASS()
class PROJECTR_API UGachaFadeInNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float		LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FLinearColor FadeColor;
};



