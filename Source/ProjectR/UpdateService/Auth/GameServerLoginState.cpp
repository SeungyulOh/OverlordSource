// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GameServerLoginState.h"

#include "Network/ClientProtocols.h"
#include "AuthStateMachine.h"
#include "UI/ContentDownload/UP_Popup.h"
#include "UI/UI_Login/OnlineIdentityLoginUI.h"

#define LOCTEXT_NAMESPACE "Startup" 

void UGameServerLoginState::OnEnter()
{
	UE_LOG(LogRAuth, Log, TEXT("ENTER UGameServerLoginState::OnEnter"));

	ReTry = 0;

	// Hook on to RAuth
	RAuth->OnLoginComplete.AddDynamic(this, &UGameServerLoginState::RAuthOnLoginComplete);
	//RAuth->Login();
	//return;

	if (RAuth->IsValidClientKey())
	{
		RAuth->LoginWithCachedClientkey();
		//RAuth->LoginWithCachedClientkey(StateMachine->GetClientKey());
	}
	else
	{
		RAuth->InitClientKey();
		//RAuth->Login(StateMachine->IdpCode, StateMachine->ClientId, StateMachine->AccessToken);
	}
}

void UGameServerLoginState::OnLeave()
{
	// Unhook from RAuth
	RAuth->OnLoginComplete.RemoveAll(this);
}

void UGameServerLoginState::RAuthOnLoginComplete(ELoginResult InLoginResult, FString InClientKey)
{
	UE_LOG(LogRAuth, Log, TEXT("UGameServerLoginState::RAuthOnLoginComplete(%d, %s)"), (int32)InLoginResult, *InClientKey);
	 
	switch (InLoginResult)
	{
		case ELoginResult::VE_Ok:
		{
			RAuth->SetClientKey(InClientKey);
			RAuth->SaveAccountLocalRecord();
			StateMachine->ChangeState(EAuthStateEnum::VE_Patch);
		}
		break;
		case ELoginResult::VE_InitClientKey:
		{
			if (ReTry > 2)
			{
				// Show popup
				UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
					FText::FromString(TEXT("Authorization Init Key error")), ECommonPopupType::VE_Error);
				Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
				return;
			}
			++ReTry;
			//RAuth->SetClientKey(InClientKey);
			FString Empty;
			RAuth->SetClientKey(Empty);
			RAuth->InitClientKey();
			return;

			/*//nzin
			UE_LOG(LogRAuth, Log, TEXT("UGameServerLoginState::RAuthOnLoginComplete: InitClientKeyFail. Invalidating Idp credentials"));

			// Back to the beginning
			StateMachine->InvalidateIdpCredentials();

			// Show popup
			UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"), 
				FText::FromString(TEXT("Authorization error")), ECommonPopupType::VE_Error);
			Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);*/
		}
		break;
		case ELoginResult::VE_ExpiredClientKey:
		{
			if (ReTry > 2)
			{
				// Show popup
				UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
					FText::FromString(TEXT("Authorization Expired Key error")), ECommonPopupType::VE_Error);
				Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
				return;
			}
			++ReTry;
			//RAuth->SetClientKey(InClientKey);
			FString Empty;
			RAuth->SetClientKey(Empty);
			RAuth->InitClientKey();
		}
		break;
		case ELoginResult::VE_InvalidPublisherName:
		case ELoginResult::VE_InvalidPublisher:
		{
			// Show popup
			UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
				FText::FromString(TEXT("PublisherError")), ECommonPopupType::VE_Error);
			Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
		}
		break;
		case ELoginResult::VE_AccountNotFound:
		{
			/*StateMachine->InvalidateIdpCredentials();

			LoginUI->SetLoginError();
			//StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
			
			// Show popup
			UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
				FText::FromString(TEXT("id fail")), ECommonPopupType::VE_Error);
			Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
			*/

			switch (RAuth->GetPublisher())
			{
				case EPublisher::Kokomo:
				{
					RAuth->EmptyAccount();
					LoginUI->SetLoginError();
					// Show popup
					UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
						FText::FromString(TEXT("id fail")), ECommonPopupType::VE_Error);
					Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
				}
				break;
				default:
				{
					RAuth->SetClientKey(InClientKey);
					//StateMachine->SetClientKey(InClientKey);
					// Transition to CreateAccountState
					StateMachine->ChangeState(EAuthStateEnum::VE_CreateAccount);
				}
				break;
			}
			
		}
		break;
		case ELoginResult::VE_ApiMaintenance:
		{
			// Show popup
			FText Caption = LOCTEXT("UI_startup_warning_popup_title", "Warning");
			FText Contents = FText::FromString(TEXT("Server Maintenance"));
			DefaultPopupFail(Caption, Contents);
		}
		break;
		default:
		{
			//UE_LOG(LogRAuth, Log, TEXT("RAuthOnLoginComplete not handled"));

			FText Caption = LOCTEXT("UI_startup_warning_popup_title", "Warning");
			FText Contents = FText::FromString(TEXT("Login UnknownError"));
			DefaultPopupFail(Caption, Contents);
		}
		break;
	}
}

void UGameServerLoginState::DefaultPopupFail(FText& InCaption, FText& InContents)
{
	UUP_Popup* Popup = StateMachine->ShowPopup(InCaption, InContents, ECommonPopupType::VE_Error);
	Popup->OnClickOK.AddDynamic(this, &UGameServerLoginState::OnLoginFail);
}

void UGameServerLoginState::OnLoginFail()
{
	RAuth->EmptyAccount();
	StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
}

#undef LOCTEXT_NAMESPACE