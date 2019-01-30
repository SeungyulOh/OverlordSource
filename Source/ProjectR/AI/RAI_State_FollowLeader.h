// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI/AIDefs.h"
#include "RAI_State.h"
#include "RAI_State_FollowLeader.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_FollowLeader : public URAI_State
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual bool					IsValidTargetActor();

	virtual void					OnEnter() override;
	virtual void					OnLeave() override;

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_FollowLeader; }

	AActor*							SearchNewEnemy();
	void							MoveToTarget();
	void							MoveToLocation();
	void							Stop();


protected:
	UPROPERTY()
	float							FollowDistance = 100.0f;
	UPROPERTY()
	TWeakObjectPtr< AActor >		TargetActor = nullptr;
	UPROPERTY()
	FVector							TargetLocation;

	bool							bMoveToLocation = false;
};
