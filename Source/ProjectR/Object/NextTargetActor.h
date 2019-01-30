// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NextTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ANextTargetActor : public AActor
{
	GENERATED_BODY()
	
public:
	ANextTargetActor(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category = ANextTargetActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent*	EffectComponent = nullptr;

	UPROPERTY(Category = ANextTargetActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*		StaticMeshComponent = nullptr;

	UPROPERTY(Category = ANextTargetActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent*			ParentMeshComponent = nullptr;

	UPROPERTY(Category = ANextTargetActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent*	BeamEffectComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = Battle)
	void		SetTarget(AActor* Source, AActor* Dest, float lifeTime);

	void		Tick(float DeltaSecond) override;
	void		Hide();
	bool		IsUseAble();

	UFUNCTION()
	void ActiveTick();
	void InitActiveTickTimer(bool InSetTimer);


private:
	void		UpdateEffect();
	void		UpdateStaticMesh(FVector pos, float scale);

protected:
	TWeakObjectPtr< AActor >		SourceObject;
	TWeakObjectPtr< AActor >		DestObject;
	bool							bSelfDestroy = false;
	float							LifeTime = -1.0f;

	UPROPERTY()
	FTimerHandle		ActiveHandler;
};
