// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ContinuousSkillApplierActor.generated.h"

class UISkill_V2;

UCLASS()
class PROJECTR_API AContinuousSkillApplierActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AContinuousSkillApplierActor();

	// Called when the game starts or when spawned
	virtual void	BeginPlay() override;

	// Called every frame
	virtual void	Tick(float DeltaSeconds) override;

	virtual void	BeginDestroy() override;


	virtual void	SpawnSkill(UISkill_V2* InSkill, FVector InTargetLocation, AActor* InAttachTarget = nullptr );
	virtual void	DespawnSkill();
	
	void			ClearSkillEffectForced();
	void			SetAttachedTarget(AActor* InOwner) { AttachedActor = InOwner; }

	virtual bool	IsEnd(float DeltaTime);
	virtual void	CheckApplyTickTime(float DeltaTime);

	virtual void	SetUsingOnlyEffect(bool OnlyEffect) { UseOnlyEffect = OnlyEffect; }
	virtual void	SetEspecialDurationTime(float Time) { Duration = Time; }

protected:
	virtual void	UpdateLocation();
	virtual void	SpawnEffect(UISkill_V2* InSkill, FVector InTargetLocation, AActor* InAttachTarget = nullptr );
	

public: 
	
	
	float							ElapsedTime;
	float							TickCheckElapsedTime;
	

	
	FVector							TargetLocation;

	TWeakObjectPtr<UISkill_V2>		TargetSkill;
	FName							ParentSkillID;
	float							Duration;
	float							ApplyTickTime;
	
	UPROPERTY()
	UParticleSystemComponent*		SkillEffect;
	
	TWeakObjectPtr< AActor >		Caster;
	TWeakObjectPtr< AActor >		AttachedActor;	

	bool							UseOnlyEffect = false;
};
