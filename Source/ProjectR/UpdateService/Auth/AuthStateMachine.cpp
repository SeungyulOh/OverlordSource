// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAuth.h"
#include "PreloginAuthState.h"
#include "AuthStateMachine.h"
#include "PublisherLoginState.h"
#include "GameServerLoginState.h"
#include "CreateAccountState.h"
#include "PatchState.h"
#include "ReadyToGoLobbyState.h"
#include "RequestPermissionsState.h"
#include "UI/ContentDownload/ContentDownloadUI.h"
#include "UtilFunctionIntegrated.h"
#include "UI/UI_Login/OnlineIdentityLoginUI.h"
#include "MediaPlayer.h"

#define ERRORVIEW_POPUP 99999

void UAuthStateMachine::Init(URGameInstance* InRGameInstance)
{
	RGameInstance = InRGameInstance;

	RAuth = URAuth::GetInstancePtr();

	// Prepare LoginUI widget
	// REMEMBER: The entirety of /Game/StartupContent need to be packaged!
	FStringClassReference LoginUIBPRef(TEXT("/Game/StartupContent/UMG/Login_Main.Login_Main_C"));
	UClass* LoginUIBP = LoginUIBPRef.TryLoadClass<UOnlineIdentityLoginUI>();
	LoginUI = CreateWidget<UOnlineIdentityLoginUI>(RGameInstance, LoginUIBP);
	if (!IsValid(LoginUI))
	{
		UE_LOG(LogRAuth, Log, TEXT("Failed to create OnlineIdentityLoginUI widget"));
		return;
	}

	LoginUI->AddToViewport(1000);
	//LoginUI->Init(UUtilFunctionLibrary::GetPlayerController(GetWorld()));

	StateTable.Reserve(int32(EAuthStateEnum::VE_Max) - 1); //VE_None
	
	// Prepare states
	UAuthState* AuthState = NewObject<UPreloginAuthState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_Prelogin, AuthState);

	AuthState = NewObject<UPublisherLoginState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_PublisherLogin, AuthState);

	AuthState = NewObject<UGameServerLoginState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_GameServerLogin, AuthState);

	AuthState = NewObject<UCreateAccountState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_CreateAccount, AuthState);

	AuthState = NewObject<UPatchState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_Patch, AuthState);

	AuthState = NewObject<UReadyToGoLobbyState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_ReadyToGoLobby, AuthState);

	AuthState = NewObject<URequestPermissionsState>(this);
	AuthState->Init(this);
	AddState(EAuthStateEnum::VE_RequestPermissions, AuthState);

	// Start the machine
	ChangeState(EAuthStateEnum::VE_Prelogin);
}

void UAuthStateMachine::Shutdown()
{
	ChangeState(EAuthStateEnum::VE_None);

	if (LoginUI)
	{
		LoginUI->RemoveFromParent();
		LoginUI = nullptr;
	}

	if (PopupUI)
	{
		PopupUI->RemoveFromParent();
		PopupUI = nullptr;
	}
}

void UAuthStateMachine::ChangeState(EAuthStateEnum InState)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER ChangeState to %s"), 
		*UUtilFunctionLibrary::GetEnumToString(TEXT("EAuthStateEnum"), (int32)InState));


	if (IsValid(CurrentState))
	{
		CurrentState->OnLeave();
		CurrentState = nullptr;
	}

	if (InState != EAuthStateEnum::VE_None)
	{
		CurrentState = StateTable[InState];
		CurrentState->OnEnter();
	}
}

UUP_Popup* UAuthStateMachine::ShowPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType)
{
	if (!IsValid(PopupUI))
	{
		UClass* WidgetUClass = LoginUI->PopupUIClass.Get();
		if (WidgetUClass == nullptr)
		{
			LoginUI->PopupUIClass.ToSoftObjectPath().TryLoad();
			WidgetUClass = LoginUI->PopupUIClass.Get();
		}

		PopupUI = CreateWidget<UUP_Popup>(RGameInstance, WidgetUClass);
	}
	PopupUI->SetPopup(InCaption, InContents, InPopupType);
	PopupUI->AddToViewport(ERRORVIEW_POPUP);
	
	return PopupUI;
}


FMulticastScriptDelegate& UAuthStateMachine::ShowPopup(FText InCaption, FText InContents)
{
	if (!IsValid(PopupUI))
	{
		UClass* WidgetUClass = LoginUI->PopupUIClass.Get();
		if (WidgetUClass == nullptr)
		{
			LoginUI->PopupUIClass.ToSoftObjectPath().TryLoad();
			WidgetUClass = LoginUI->PopupUIClass.Get();
		}

		PopupUI = CreateWidget<UUP_Popup>(RGameInstance, WidgetUClass);
	}
	//PopupUI->SetPopup(InCaption, InContents, InPopupType);
	PopupUI->AddToViewport(99999);
	
	return PopupUI->OnClickOK;
}

void UAuthStateMachine::HidePopup()
{
	if (PopupUI)
	{
		PopupUI->RemoveFromParent();
		PopupUI = nullptr;
	}
}

void UAuthStateMachine::InvalidateIdpCredentials()
{
	IdpCode = TEXT("");
	AccessToken = TEXT("");
	ClientId = TEXT("");

	URAuth::GetInstancePtr()->InvalidateIdpCredentials();
}

void UAuthStateMachine::InvalidateClientKey()
{
	ClientKey = TEXT("");
}

void UAuthStateMachine::AddState(EAuthStateEnum InStateEnum, UAuthState* InState)
{
	StateTable.Emplace(InStateEnum, InState);
}

void UAuthStateMachine::UIPatchState()
{
	//LoginBackground_Movie->Pause();

}

void UAuthStateMachine::UIWaitingState()
{
	LoginUI->Show_Panel_Waiting();
}

void UAuthStateMachine::UIClickState()
{
	LoginUI->Show_Panel_Touch();
}


