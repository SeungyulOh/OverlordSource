// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PatchState.h"

#include "UI/ContentDownload/ContentDownloadUI.h"
#include "UpdateService/UpdateService.h"
#include "AuthStateMachine.h"
#include "UI/UI_Login/OnlineIdentityLoginUI.h"
#include "MediaPlayer.h"
#include "GlobalIntegrated.h"

void UPatchState::OnEnter()
{
	UE_LOG(LogClass, Log, TEXT("PatchState Enter"));
	//LoginUI->Show_Panel_Waiting();

#if WITH_EDITOR || PLATFORM_WINDOWS || PLATFORM_MAC || PROJECTR_NOPATCH_BUILD
	
	// Transition
	// Skip PatchState if WITH_EDITOR
	// OR if no-patch build

	UpdateService = UUpdateService::GetInstancePtr();
	UpdateService->SetNoPatchBuild();
	StateMachine->ChangeState(EAuthStateEnum::VE_ReadyToGoLobby);
	return;
#endif

	UpdateService = UUpdateService::GetInstancePtr();

	UpdateService->OnUpdatePending.AddDynamic(this, &UPatchState::OnUpdatePending);
	UpdateService->OnFinish.AddDynamic(this, &UPatchState::OnUpdateServiceFinish);
	UpdateService->CheckUpdate();
}

void UPatchState::OnLeave()
{
	//StateMachine->LoginBackground_Movie->Play();

	if (IsValid(ContentDownloadUI))
	{
		// Tear down ContentDownloadUI
		ContentDownloadUI->RemoveFromParent();
		ContentDownloadUI->OnContentDownloadUIFinish.RemoveAll(this);
		ContentDownloadUI = nullptr;
	}
	UpdateService = nullptr;
}

void UPatchState::LaunchContentDownloadUI()
{
	// Patch process is driven by ContentDownloadUI

	// Pause login background movie while displaying ContentDownloadUI
	//StateMachine->LoginBackground_Movie->Pause();

	ContentDownloadUI = LoginUI->ShowContentDownloadUI();
	ContentDownloadUI->OnContentDownloadUIFinish.AddDynamic(this, &UPatchState::ContentDownloadUIOnFinish);
	return;

	// Set up ContentDownloadUI
	UClass* WidgetUClass = LoginUI->ContentDownloadUIClass.Get();
	if (WidgetUClass == nullptr)
	{
		LoginUI->ContentDownloadUIClass.ToSoftObjectPath().TryLoad();
		WidgetUClass = LoginUI->ContentDownloadUIClass.Get();
	}

	ContentDownloadUI = CreateWidget<UContentDownloadUI>(StateMachine->RGameInstance, WidgetUClass);
	ContentDownloadUI->OnContentDownloadUIFinish.AddDynamic(this, &UPatchState::ContentDownloadUIOnFinish);

	ContentDownloadUI->AddToViewport(99999);
	ContentDownloadUI->Init();
}

void UPatchState::ContentDownloadUIOnFinish()
{
	StateMachine->ChangeState(EAuthStateEnum::VE_ReadyToGoLobby);
}

void UPatchState::OnUpdatePending()
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER UPatchState::OnUpdatePending"));

	UpdateService->OnUpdatePending.RemoveAll(this);
	UpdateService->OnFinish.RemoveAll(this);

	LaunchContentDownloadUI();
}

void UPatchState::OnUpdateServiceFinish(bool bSuccess, FString Message)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER UPatchState::OnUpdateServiceFinish"));

	UpdateService->OnUpdatePending.RemoveAll(this);
	UpdateService->OnFinish.RemoveAll(this);

	if (bSuccess)
	{
		StateMachine->ChangeState(EAuthStateEnum::VE_ReadyToGoLobby);
	}
	else
	{
		// Retry popup on failure
		FText Caption = FText::FromString(TEXT("Error"));

		auto PopupUI = StateMachine->ShowPopup(Caption, FText::FromString(Message), ECommonPopupType::VE_Error);
		StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
	}
}
