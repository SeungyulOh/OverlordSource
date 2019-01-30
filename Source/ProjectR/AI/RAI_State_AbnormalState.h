// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_State.h"
#include "RAI_State_AbnormalState.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_AbnormalState : public URAI_State
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;


	virtual void					OnEnter();
	virtual void					OnLeave();

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_AbnormalState; }
};
