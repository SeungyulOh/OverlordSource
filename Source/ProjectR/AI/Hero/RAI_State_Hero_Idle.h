// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../RAI_State_Idle.h"
#include "RAI_State_Hero_Idle.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_Idle : public URAI_State_Idle
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual void					PauseMove(bool InPause) override;

private:
	bool							bPause = false;
};
