// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI/AIDefs.h"
#include "RAI_State.h"
#include "RAI_State_MoveToTarget.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_MoveToTarget : public URAI_State
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual bool					IsValidTargetActor();

	virtual void					OnEnter() override;
	virtual void					OnLeave() override;

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_MoveToTarget; }

	AActor*							SearchNewEnemy();
	void							MoveToTarget();
	void							Stop();

	virtual void					PauseMove(bool bPause) override;


protected:
	UPROPERTY()
	bool							KeepTarget = false;
	UPROPERTY()
	TWeakObjectPtr< AActor >		TargetActor = nullptr;
	FVector							TargetLocation;
	float							AcceptableDistance = 50.0f;

	bool							SuccessMoveToTarget = false;
};
