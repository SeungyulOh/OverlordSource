// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PreloginAuthState.h"
#include "RAuth.h"
#include "AuthStateMachine.h"
#include "UtilFunctionIntegrated.h"
#include "UI/UI_Login/OnlineIdentityLoginUI.h"

void UPreloginAuthState::OnEnter()
{
	//RAuth = URAuth::GetInstancePtr();
	
	RAuth->OnPreloginComplete.AddDynamic(this, &UPreloginAuthState::OnPreloginComplete);
	RAuth->Prelogin();
}

void UPreloginAuthState::OnLeave()
{
	if (RAuth)
	{
		RAuth->OnPreloginComplete.RemoveDynamic(this, &UPreloginAuthState::OnPreloginComplete);
	}
}

void UPreloginAuthState::OnPreloginComplete(EPreloginResult Result)
{
	//RAuth->OnPreloginComplete.RemoveAll(this);

	if (Result != EPreloginResult::PreloginSuccess)
	{
		// TODO: display error popup as it is not handled by PacketErrorManager nor RAuth
		UE_LOG(LogBattle, Log, TEXT("UPreloginState OnPreloginComplete: Prelogin failed: %d"), (int32)Result);
		//return;
	}

	if (IsValid(LoginUI))
	{
		//UUtilFunctionLibrary::GetPlayerController(GetWorld())
		LoginUI->Init();
	}
	StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
}
