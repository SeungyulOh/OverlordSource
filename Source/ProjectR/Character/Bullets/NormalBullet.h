// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BulletBase.h"
#include "NormalBullet.generated.h"


struct FBulletTableInfo;


/**
 * 
 */
UCLASS()
class PROJECTR_API ANormalBullet : public ABulletBase
{
	GENERATED_BODY()
	
public:
	ANormalBullet(const FObjectInitializer& ObjectInitializer);

	// Bullet velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NormalBullet)
	float							BulletSpeed = 3000.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = NormalBullet)
	USphereComponent*				CollisionComp = nullptr;

	// Bullet velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NormalBullet)
	UProjectileMovementComponent*	ProjectileMovement = nullptr;

	// Set initial battle property
	UFUNCTION(BlueprintCallable, Category = NormalBullet)
	void		InitVelocity(FVector Dir, float TargetBulletSpeed);

		
	//UFUNCTION()
	//void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void	Init() override;

	void	SetActiveBullet(bool ActiveBullet);

	bool	IsActiveBullet = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	TWeakObjectPtr< AActor >		TargetActor;

	// for normal attack projectile
	void	Tick(float DeltaTime) override;
	bool	IsNormalAttack = false;
	float	BulletHeight = 0.0f;
	
// NormalBullet V2
public:
	void InitBullet(AActor* Shooter, float BulletSpeed, FVector ShotPos, AActor* ActorTarget, FVector LocationTarget);
	void SetNormalAttackBullet_V2();
	void SetSkillAttackBullet(UISkill_V2* Skill, float TargetDist = 0.0f);
	void HitTargetNormalAttack();
	bool MoveBullet(float DeltaTime);

protected:
	void SetBulletAimTarget(AActor* ActorTarget, FVector LocationTarget);

protected:
	UPROPERTY()
	FVector					ShotPosition = FVector::ZeroVector;
	UPROPERTY()
	TWeakObjectPtr<AActor>	AimTarget;
	UPROPERTY()
	FVector					AimLocation = FVector::ZeroVector;
	UPROPERTY()
	float					BulletHitLimitTime = 0.0f;
	float					FirstHitTargetDist = 0.0f;
	bool					bBulletHit = false;
	bool					bApplyHit = false;
};
