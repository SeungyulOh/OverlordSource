// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/RAI_State.h"
#include "RAI_State_TaticsMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URAI_State_TaticsMove : public URAI_State
{
	GENERATED_BODY()
	
	
public:
	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;


	virtual void					OnEnter();
	virtual void					OnLeave();

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_TaticsMove; }

	void MoveToLocation();

public:
	UPROPERTY()
	FVector							TargetLocation;

	float							StayTime = 0.f;// 5.f;
	
	
};
