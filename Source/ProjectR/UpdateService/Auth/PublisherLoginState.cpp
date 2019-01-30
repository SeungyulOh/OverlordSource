// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PublisherLoginState.h"
#include "UI/UI_Login/OnlineIdentityLoginUI.h"
#include "RAuth.h"
#include "AuthStateMachine.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "UpdateService/LoginDefs.h"

void UPublisherLoginState::OnEnter()
{
	// Hook on to LoginUI
//	LoginUI->OnClickIdpLogin.AddDynamic(this, &UPublisherLoginState::LoginUIOnClickIdpLogin);
	LoginUI->OnPanelTouchClicked.RemoveDynamic(this, &UPublisherLoginState::OnPanelTouchClicked);
	LoginUI->OnPanelTouchClicked.AddDynamic(this, &UPublisherLoginState::OnPanelTouchClicked);

	//RAuth->OnLoginComplete.RemoveDynamic(this, &UPublisherLoginState::OnCheckPlatformAccount);
	//RAuth->OnLoginComplete.AddDynamic(this, &UPublisherLoginState::OnCheckPlatformAccount);

	// Hook on to RAuth
#if API_NZIN
	auto OnLoginSuccessDelegate = FOnLoginSuccessDelegate::CreateUObject(this, &UPublisherLoginState::RAuthOnIdpLoginComplete);
//	RAuth->GetPublisherAPI()->AddOnLoginSuccessDelegate_Handle(OnLoginSuccessDelegate);
	OnLoginSuccessDelegateHandle = OnLoginSuccessDelegate.GetHandle();
	
	RAuth->GetPublisherAPI()->Start(); // TODO: called here because I don't yet know how to insert this into viewDidLoad, for iOS
#endif
	LoginUI->Show_Panel_Touch();
}

void UPublisherLoginState::OnLeave()
{
	// Unhook from LoginUI
	LoginUI->OnPanelTouchClicked.RemoveAll(this);
//	LoginUI->OnClickIdpLogin.RemoveAll(this);

	//RAuth->OnLoginComplete.RemoveDynamic(this, &UPublisherLoginState::OnCheckPlatformAccount);

#if API_NZIN
	// Unhook from RAuth
	RAuth->GetPublisherAPI()->ClearOnLoginSuccessDelegate_Handle(OnLoginSuccessDelegateHandle);
#endif
}

void UPublisherLoginState::OnPanelTouchClicked()
{
	URAuth::GetInstancePtr()->MakeAccount();
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
	return;

#if PLATFORM_ANDROID //nzin
	StateMachine->ChangeState(EAuthStateEnum::VE_RequestPermissions);
#else
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
#endif
}

void UPublisherLoginState::OnCheckPlatformAccount(ELoginResult InResut, FString InClientKey)
{
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
}


void UPublisherLoginState::LoginUIOnClickIdpLogin(FString IdpCode)
{
	//StateMachine->IdpCode = IdpCode;
	
	StateMachine->UIWaitingState();

	URAuth::GetInstancePtr()->SetUserID(IdpCode);
#if API_NZIN
	RAuth->GetPublisherAPI()->CustomLogin(IdpCode);
#endif
}

void UPublisherLoginState::TryLogin(FString InPublisherType, FString InUserID, FString InClientId, FString InAccessToken)
{
	// Save
	URAuth::GetInstancePtr()->SetUserID(InUserID);
	//URAuth::GetInstancePtr()->SaveDevCredentials();

	/*FOnlineAccountCredentials Account;*/

	FOnlineAccountInfo Account;
	Account.Type = InPublisherType;
	Account.Id = InUserID;
	Account.Token = TEXT("");//InAccessToken;
	Account.ClientKey = URAuth::GetInstancePtr()->GetClientKey();

	URAuth::GetInstancePtr()->SetAccountInfo(Account);

	// Invalidate clientkey
	StateMachine->InvalidateClientKey();

#if PLATFORM_ANDROID
	StateMachine->ChangeState(EAuthStateEnum::VE_RequestPermissions);
#else
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
#endif
}

void UPublisherLoginState::LoginUIOnClickDevLogin(FString DevID)
{
	// Save
	URAuth::GetInstancePtr()->SetUserID(DevID);

	/*FOnlineAccountCredentials Account;
	Account.Type = IdpCode;
	Account.Id = DevID;
	Account.Token = Token;*/

	// Transition with dev credentials
	//StateMachine->IdpCode = TEXT("kokomo");
	//StateMachine->ClientId = DevID;
	//StateMachine->AccessToken = TEXT("");

	// Invalidate clientkey
	StateMachine->InvalidateClientKey();

#if PLATFORM_ANDROID
	StateMachine->ChangeState(EAuthStateEnum::VE_RequestPermissions);
#else
	StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
#endif
}

void UPublisherLoginState::RAuthOnIdpLoginComplete(int32 IdpLoginResult, FString IdpCode, FString ClientId, FString AccessToken, bool IsInReview, FString GameServerAddr, FString CdnAddr)
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER RAuthOnIdpLoginComplete(%d, IdpCode=%s, AccessToken=%s)"), (int32)IdpLoginResult, *IdpCode, *AccessToken);

	bool bSuccess = (EIdpLoginResult)IdpLoginResult == EIdpLoginResult::VE_Ok;

	if (bSuccess && !AccessToken.IsEmpty())
	{
		// Save AccessToken and transition
		//RAuth->TryNZIN();
		RAuth->PreaprePlatform(EPublisher::NZIN);
		// StateMachine->IdpCode = IdpCode;
		StateMachine->IdpCode = TEXT("nzin"); // Apparently this should be "nzin" for both kakao and guest login when using Kakao SDK
		StateMachine->ClientId = ClientId;
		StateMachine->AccessToken = AccessToken;

#if PLATFORM_ANDROID
		StateMachine->ChangeState(EAuthStateEnum::VE_RequestPermissions);
#else
		StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
#endif
	}
	else
	{
		// Stay on the login panel
		// Maybe show some message?.....

		StateMachine->InvalidateClientKey();
		StateMachine->UIClickState();
	}
}
