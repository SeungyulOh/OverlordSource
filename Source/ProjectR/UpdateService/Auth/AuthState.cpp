// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAuth.h"
#include "AuthStateMachine.h"
#include "AuthState.h"
#include "UpdateService/Auth/RAuth.h"


void UAuthState::Init(UAuthStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	LoginUI = StateMachine->LoginUI;
	RAuth = URAuth::GetInstancePtr();
}
