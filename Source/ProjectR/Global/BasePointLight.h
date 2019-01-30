// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePointLight.generated.h"

UCLASS()
class PROJECTR_API ABasePointLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePointLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	class UPointLightComponent* PointLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic_PointLight")
	float LifeTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic_PointLight")
	float AttenuationRadius = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic_PointLight")
	FLinearColor LightColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic_PointLight")
	float LightIntensity = 200.f;

	UPROPERTY()
	class UCurveFloat* PointLightCurve;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetBullet;

	UPROPERTY()
	bool isFollowingBullet = false;
private:
	float ElapsedTime = 0.f;
	class URPointLightManager* PointLightManager;
	float PrevVignetteIntensity = 0.f;
};
