// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_Particle.generated.h"

/**
 * this is designed to show 3d particle system effect on UMG.
 */
UCLASS()
class PROJECTR_API UUC_Particle : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void ActivateSelectedParticle();
	void DeactivateSelectedParticle();
	bool isActivated();

	void SwapParticle();
	void SetTemplate_SelectedParticle();
	void Set_InitialState();

	UParticleSystemComponent* GetPSC();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	bool bActive_AtSpawned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	UParticleSystem* SelectedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	UParticleSystem* PendingParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY()
	UParticleSystem* InitialParticle;

	UPROPERTY()
	UImage* Image_Target;
	

	UPROPERTY()
	TWeakObjectPtr<AActor> BP_Capture_Particle;

	UPROPERTY()
	TWeakObjectPtr<AActor> BP_Capture_ParticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	FTransform	Transform_CaptureParticle = FTransform(FRotator(0.f, -180.f, 0.f), FVector(480.f, 1100.f, 0.f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Particle")
	FTransform	Transform_ParticleActor = FTransform(FRotator(0.f, 90.f, 0.f), FVector(-200.f, 1100.f, -100.f));
	
};
