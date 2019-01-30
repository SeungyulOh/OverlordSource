// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_State.h"
#include "RAI_State_MoveToLocation.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_MoveToLocation : public URAI_State
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual void					OnEnter() override;
	virtual void					OnLeave() override;

		
	void							MoveToLocation();
	void							Stop();

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_MoveToLocation; }
		
protected:
	UPROPERTY()
	FVector							TargetLocation;
	UPROPERTY()
	float							AcceptableDistance = 50.0f;
};
