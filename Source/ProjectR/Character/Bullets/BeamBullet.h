// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BulletBase.h"
#include "BeamBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ABeamBullet : public ABulletBase
{
	GENERATED_BODY()
	
public:
	ABeamBullet(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BeamBullet)
	UParticleSystemComponent*	EffectComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = Battle)
	void		InitTarget(AActor* Target);

	void		Tick(float DeltaSecond) override;
	void		UpdateEffect();

protected:
	UPROPERTY()
	AActor*		TargetObject = nullptr;
};
