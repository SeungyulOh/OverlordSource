// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI/RAI_State_MoveToTarget.h"
#include "RAI_State_Hero_ApproachToTarget.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_ApproachToTarget : public URAI_State_MoveToTarget
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;
	
	virtual void					OnEnter() override;
	virtual void					OnLeave() override;

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_ApproachToTarget; }

	

protected:
	float							ApproachDistance = 200.0f;
	
};
