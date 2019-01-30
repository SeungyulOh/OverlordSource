// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../RAI_State.h"
#include "RAI_State_Hero_AvoidToLocation.generated.h"


UCLASS()
class  PROJECTR_API URAI_State_Hero_AvoidToLocation : public URAI_State
{
	GENERATED_BODY()

public:

	virtual EAIStateTypeEnum		Update(float DeltaTime)  override;

	virtual void					OnEnter() override;
	virtual void					OnLeave() override;


	void							MoveToLocation();
	void							Stop();

	virtual EAIStateTypeEnum		GetStateType() override { return EAIStateTypeEnum::VE_AvoidToLocation; }


protected:

	bool							IsOverlappedOtherCharacter();
	bool							GetTargetLocationFromAvoidanceVector();
	

	FVector							TargetLocation;
	float							AcceptableDistance = 10.0f;
	float							AvoidanceDistance = 50.0f;

	FVector							AvoidVector;
	FVector							PrevAvoidVector = FVector::ZeroVector;
};
