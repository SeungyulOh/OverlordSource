// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_State.h"
#include "RAI_State_Attack.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Attack : public URAI_State
{
	GENERATED_BODY()

public:
	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;


	virtual void					OnEnter();
	virtual void					OnLeave();

	void							Attack();


	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_Attack; }

	/*
	State check
	*/
	virtual bool					IsValidTargetActor();	

protected:
	bool							KeepTarget = false;
	TWeakObjectPtr< AActor >		TargetActor = nullptr;
};
