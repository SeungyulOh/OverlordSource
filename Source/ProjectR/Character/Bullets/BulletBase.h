// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

class UISkill;
class UISkill_V2;

/**
 * 
 */
UCLASS()
class PROJECTR_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:
	ABulletBase(const FObjectInitializer& ObjectInitializer);


	// bullet owner who shot this bullet.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletBase)
	TWeakObjectPtr< AActor >			OwnerCharacter;	


	/*
	Blueprint Callable Functions
	*/
	// Set initial properties
	UFUNCTION(BlueprintCallable, Category = BulletBase)
	void		InitMembers();

	UFUNCTION(BlueprintCallable, Category = BulletBase)
	UISkill_V2*	GetOwnerSkill_V2();

	
	/*
	Overrided functions
	*/
	void	BeginPlay() override;
	void	Destroyed() override;
	void	Tick(float DeltaSecond) override;
		
	void	SetSkill_V2(UISkill_V2* Skill);
	void	UpdateDestroy(float DeltaSecond);
	void	SetSelfDestory(float Time);

	/*
	declare my delegate
	*/	


	/*
	Pause Setting
	*/
	void			SetForcePause(bool bPause, float time = 0.0f);
private:
	void			ForcePauseStart(float time = 0.0f);
	void			ForcePauseEnd();
	void			SetPauseSetting(bool bPause);

	UPROPERTY()
	FTimerHandle	ForcePauseHandler;

protected:
	virtual	void Init();

	UPROPERTY()
	FVector		TargetDir;
	UPROPERTY()
	FVector		OriginalDir;

protected:
	UPROPERTY()
	UISkill_V2*			OwnerSkill_V2 = nullptr;

	UPROPERTY()
	bool				bSelfDestroy = false;

	UPROPERTY()
	float				DestroyTime = 0.0f;
	UPROPERTY()
	bool				IsSelfDestroyBullet = false;
	UPROPERTY()
	bool				IsTargetRedirect = false;
};
