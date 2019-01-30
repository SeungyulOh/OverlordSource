// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../RAI_State_MoveToLocation.h"
#include "RAI_State_Hero_MoveToLocation.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_MoveToLocation : public URAI_State_MoveToLocation
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual void					OnEnter() override;
	virtual void					OnLeave() override;

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_MoveToLocation; }

	void							MoveToLocation();
protected:
	FVector							TargetLocation;
};
