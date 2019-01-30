// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../RAI_State_MoveToTarget.h"
#include "RAI_State_Hero_MoveToTarget.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_MoveToTarget : public URAI_State_MoveToTarget
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

private:
	EAIStateTypeEnum				UpdateTargetActor(AActor* InTarget);
};
