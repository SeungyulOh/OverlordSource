// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_State_MoveToLocation.h"
#include "RAI_State_MoveToAttackLocation.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_MoveToAttackLocation : public URAI_State_MoveToLocation
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;


	virtual void					OnEnter();
	virtual void					OnLeave();

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_MoveToAttackLocation; }
};
