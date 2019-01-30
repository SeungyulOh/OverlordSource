// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIDefs.h"
#include "AI/RAI_State.h"
#include "RAI_FSM.generated.h"

class URAI_Blackboard;


/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UCLASS()
class  PROJECTR_API URAI_FSM : public UObject
{
	GENERATED_BODY()

public:
	void					Init( AActor* InOwnerActor );
	void					SetState(EAIStateTypeEnum InState);
	EAIStateTypeEnum		GetCurrentState();

	FORCEINLINE void		ChangeState(EAIStateTypeEnum InState) { NextToBeState = InState; }
	FORCEINLINE bool		IsActivateAI() { return ActivatedAI; }
	FORCEINLINE void		SetIsActivatedAI(bool Inbool) { ActivatedAI = Inbool; }

	void					ActivateAI(bool InActivate);

	URAI_Blackboard*		GetBlackboard();

	void					SetStateTableSize(int32 InSize);
	void					AddState(EAIStateTypeEnum InStateType, URAI_State* InState);

	void					Update(float DeltaTime);
	
	void					PauseMove(bool bPause);

private:

	bool										ActivatedAI = false;

	TWeakObjectPtr< AActor >					OwnerActor = nullptr;

	UPROPERTY()
	URAI_Blackboard*							FSM_Blackboard = nullptr;
		
	UPROPERTY()
	TMap< EAIStateTypeEnum, URAI_State* >		StateTable;

	TWeakObjectPtr< URAI_State >				CurrentState = nullptr;
	EAIStateTypeEnum							NextToBeState = EAIStateTypeEnum::VE_None;
};
