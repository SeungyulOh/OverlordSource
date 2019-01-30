// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_Blackboard.h"
#include "AIDefs.h"
#include "RAI_State.generated.h"

UCLASS()
class PROJECTR_API URAI_State : public UObject
{
	GENERATED_BODY()

public:

	virtual void					Init(AActor* InOwnerActor, URAI_Blackboard* InRai_Blackboard );
	virtual EAIStateTypeEnum		Update( float DeltaTime )  PURE_VIRTUAL(URAI_State::Update, return EAIStateTypeEnum::VE_None;);

	virtual void					OnEnter() PURE_VIRTUAL(URAI_State::OnEnter, return;);
	virtual void					OnLeave() PURE_VIRTUAL(URAI_State::OnLeave, return;);

	virtual EAIStateTypeEnum		CheckBlackboard();
	virtual EAIStateTypeEnum		CheckSkillState();
	virtual EAIStateTypeEnum		CheckAbnormalState();

	bool							CheckStopState( float DeltaTime );

	virtual EAIStateTypeEnum		GetStateType() PURE_VIRTUAL(URAI_State::GetStateType, return EAIStateTypeEnum::VE_None;);


	virtual bool					CanMove();
	virtual void					PauseMove( bool bPause ) {};


	/*
	Follow leader
	*/
	EAIStateTypeEnum				FollowLeader();
	EAIStateTypeEnum				FollowSummoner();

	/*
	Check force target
	*/
	EAIStateTypeEnum				CheckForceTarget(AActor* InCurrentTarget);



protected:
	virtual void					InitInternalData();
	virtual void					UpdateInternal_BeforeUpdate();
	virtual void					UpdateInternal_AfterUpdate();

	virtual EAIStateTypeEnum		ProcessMeetState();

protected:
	UPROPERTY()
	TWeakObjectPtr< AActor >			OwnerActor = nullptr;
	UPROPERTY()
	TWeakObjectPtr< URAI_Blackboard >	OwnerBlackboard = nullptr;

	UPROPERTY()
	FVector							CurrentLocation;		
	UPROPERTY()
	FVector							PrevLocation;		// for block checking
	UPROPERTY()
	float							StateElapsedTime = 0.0f;
	UPROPERTY()
	float							StopCheckTime = 1.0f;
	float							TickTime = 0;
};