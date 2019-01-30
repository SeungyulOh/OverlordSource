// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../RAI_State_Attack.h"
#include "RAI_State_Hero_Attack.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_Attack : public URAI_State_Attack
{
	GENERATED_BODY()

public:
	virtual EAIStateTypeEnum Update(float DeltaTime) override;
	virtual void OnEnter() override;

	EAIStateTypeEnum UpdateTargetActor(AActor* InTarget);
};
