// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RequestPermissionsState.h"
#include "RAuth.h"
#include "AuthStateMachine.h"


void URequestPermissionsState::OnEnter()
{
#if !API_NZIN
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
	return;
#endif
	//all code API_NZIN

	auto OnCheckPermissionsCompleteDelegate = FOnCheckPermissionsCompleteDelegate::CreateUObject(this, &URequestPermissionsState::OnCheckPermissionsComplete);
	auto OnRequestPermissionsCompleteDelegate = FOnRequestPermissionsCompleteDelegate::CreateUObject(this, &URequestPermissionsState::OnCheckPermissionsComplete);

	//RAuth->GetPublisherAPI()->AddOnCheckPermissionsCompleteDelegate_Handle(OnCheckPermissionsCompleteDelegate);
	//RAuth->GetPublisherAPI()->AddOnRequestPermissionsCompleteDelegate_Handle(OnRequestPermissionsCompleteDelegate);

	OnCheckPermissionsCompleteHandle = OnCheckPermissionsCompleteDelegate.GetHandle();
	OnRequestPermissionsCompleteHandle = OnRequestPermissionsCompleteDelegate.GetHandle();

	RAuth->GetPublisherAPI()->CheckPermissions();
}

void URequestPermissionsState::OnLeave()
{
	RAuth->GetPublisherAPI()->ClearOnCheckPermissionsCompleteDelegate_Handle(OnCheckPermissionsCompleteHandle);
	RAuth->GetPublisherAPI()->ClearOnRequestPermissionsCompleteDelegate_Handle(OnRequestPermissionsCompleteHandle);
}

void URequestPermissionsState::OnCheckPermissionsComplete(bool bSuccess, bool bGranted)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER OnCheckPermissionsComplete(%d, %d)"), (int32)bSuccess, (int32)bGranted);

	if (!bSuccess)
	{
		UE_LOG(LogRAuth, Log, TEXT("OnCheckPermissionsComplete failure"));

		// FIXME: proceding anyway
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);

		return;
	}

	if (bGranted)
	{
		// Procede
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
	}
	else
	{
		// TODO: Show Permissions Notice UI
		RAuth->GetPublisherAPI()->RequestPermissions();
	}
}

void URequestPermissionsState::OnRequestPermissionsComplete(bool bSuccess, bool bGranted)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER OnRequestPermissionsComplete(%d, %d)"), (int32)bSuccess, (int32)bGranted);

	if (!bSuccess)
	{
		UE_LOG(LogRAuth, Log, TEXT("OnRequestPermissionsComplete failure"));

		// FIXME: proceding anyway
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);

		return;
	}

	if (bGranted)
	{
		// Procede
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
	}
	else
	{
		// TODO: complaint to user

		// FIXME: proceding anyway
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
	}
}
