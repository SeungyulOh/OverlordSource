// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_FSM.h"
#include "RAI_Blackboard.h"
#include "RAI_State.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Utils/CharacterHelpFunctionLibrary.h"
#include "Global/RealTimeModManager.h"


void URAI_FSM::Init(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
	FSM_Blackboard = NewObject< URAI_Blackboard >(this);
}

void URAI_FSM::SetState(EAIStateTypeEnum InState)
{	
	if (CurrentState.IsValid())
	{
		if (CurrentState->GetStateType() == InState)
			return;

		CurrentState->OnLeave();
	}

	if (StateTable.Contains(InState))
	{
		CurrentState = StateTable[InState];
		CurrentState->OnEnter();
	}
}

EAIStateTypeEnum URAI_FSM::GetCurrentState()
{
	if( CurrentState.IsValid() )
		return CurrentState->GetStateType();

	return EAIStateTypeEnum::VE_None;
}

void URAI_FSM::ActivateAI( bool InActivate )
{
	// on activated
	CurrentState = nullptr;
	SetState(EAIStateTypeEnum::VE_Idle);

	ActivatedAI = InActivate;	
}

URAI_Blackboard* URAI_FSM::GetBlackboard()
{
	return FSM_Blackboard;
}

void URAI_FSM::SetStateTableSize(int32 InSize)
{
	StateTable.Empty(InSize);
}

void URAI_FSM::AddState(EAIStateTypeEnum InStateType, URAI_State* InState)
{
	StateTable.Emplace(InStateType, InState);
}

void URAI_FSM::Update(float DeltaTime)
{
	if (!ActivatedAI)
		return;

#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	if (CurrentState.IsValid())
	{
		EAIStateTypeEnum ResultState = CurrentState->Update(DeltaTime);
		if (ResultState != EAIStateTypeEnum::VE_None)
		{
			NextToBeState = ResultState;
		}
	}

	if (NextToBeState != EAIStateTypeEnum::VE_None)
	{
		if (CurrentState.IsValid())
		{
			//UE_LOG(LogAI, Warning, TEXT("OnLeave Statetype - %d : %s"), (int32)CurrentState->GetStateType(), *(OwnerActor->GetName()) );
			CurrentState->OnLeave();
		}			

		if (NextToBeState == EAIStateTypeEnum::VE_SkillActing)
		{
			UE_LOG(LogAI, Warning, TEXT("Invalid ai state - %d"), (int32)NextToBeState);
		}

		if (StateTable.Contains(NextToBeState))
		{
			CurrentState = StateTable[NextToBeState];
			CurrentState->OnEnter();			
			//UE_LOG(LogAI, Warning, TEXT("OnEnter Statetype - %d : %s"), (int32)CurrentState->GetStateType(), *(OwnerActor->GetName()));
		}
		else
		{			
			UE_LOG(LogAI, Warning, TEXT("Invalid ai state - %d"), (int32)NextToBeState );
		}

#if LOG_TIME
		double EndTime = FPlatformTime::Seconds();
		double PrintTime = EndTime - StartTime;
		//UE_LOG(LogGamePlay, Log, TEXT("%f ____aistate(%s) - %d"), PrintTime, *(OwnerActor->GetName().ToString()),(int32)NextToBeState);
#endif

		NextToBeState = EAIStateTypeEnum::VE_None;
	}	
}

void URAI_FSM::PauseMove(bool bPause)
{
	if (CurrentState.IsValid())
		CurrentState->PauseMove(bPause);
}