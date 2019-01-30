// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CreateAccountState.h"

#include "UI/ContentDownload/UP_Popup.h"
#include "GlobalIntegrated.h"

#include "UI/UI_Login/OnlineIdentityLoginUI.h"
#include "AuthStateMachine.h"
#include "Network/PacketFunctionLibrary.h"


#define LOCTEXT_NAMESPACE "Startup" 

void UCreateAccountState::OnEnter()
{
	RAuth->OnCreateAccountRp.AddDynamic(this, &UCreateAccountState::OnCreateAccountRp);
	//RAuth->GetAccountInfo();
	
	//LoginUIOnClickCreateAccount(RAuth->GetUserID());
	

	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	ELanguageType LanguageType = RGameUserSetting->LaguageType;

	LoginUI->Show_Panel_Waiting();
	//RAuth->RequestCreateAccount(NickName, (int32)LanguageType);
	UPacketFunctionLibrary::ACCOUNT_CREATE_RQ(RAuth->GetUserID(), (int32)LanguageType);
}

void UCreateAccountState::OnLeave()
{
//	LoginUI->OnClickCreateAccount.RemoveAll(this);

	RAuth->OnCreateAccountRp.RemoveAll(this);
}

void UCreateAccountState::LoginUIOnClickCreateAccount(FString NickName)
{
	if (RAuth->IsValidUserId())
	{
		RAuth->EmptyAccount();
		LoginUI->SetLoginError();

		// Show popup
		UUP_Popup* Popup = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
			FText::FromString(TEXT("id fail")), ECommonPopupType::VE_Error);
		StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
		return;
	}

	/*if (NickName.IsEmpty())
	{
		// Show popup
		UUP_Popup* PopupUI = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"), 
		LOCTEXT("startup_warning_nickname_empty", "Enter nickname"), ECommonPopupType::VE_Error);
		PopupUI->OnClickOK.Clear();

		return;
	}*/

	// Check the validity of nickname given
	// TODO: ? For Hangeul should be 1~8 characters?

	// Set at RGameInstance::Init
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	ELanguageType LanguageType = RGameUserSetting->LaguageType;

	LoginUI->Show_Panel_Waiting();
	RAuth->RequestCreateAccount(NickName, (int32)LanguageType);
	//UPacketFunctionLibrary::ACCOUNT_CREATE_RQ(InNickName, InLanguage);
}

void UCreateAccountState::OnCreateAccountRp(ECreateAccountResult Result)
{
	LoginUI->Hide_Panel_Waiting();

	switch(Result)
	{
		case ECreateAccountResult::VE_Ok:
		{
			//LoginUI->Text_NickName->SetText(FText::FromString(TEXT("")));

			// Transition back to GameServerLoginState to relogin
			StateMachine->ChangeState(EAuthStateEnum::VE_GameServerLogin);
		}
		break;
		case ECreateAccountResult::VE_NickAlreadyExists:
		{
			// Show popup
			UUP_Popup* PopupUI = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"), 
				LOCTEXT("startup_nick_already_exists", "Nickname already exists."), ECommonPopupType::VE_Error);
			PopupUI->OnClickOK.Clear();

			//RETRY!
			RAuth->EmptyNickName();
			StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);

		}
		break;
		case ECreateAccountResult::VE_NickInvalid:
		{
			if (RAuth->GetIsCheckAccount())
			{
				RAuth->SetIsCheckAccount(false);
				RGAMEINSTANCE(this)->EventManager->OnUpdatePage.Broadcast();
				//LoginUI->OnClickButton();
			}
			else
			{
				// Show popup
				// TODO: Broken. Server doesn't know the user's locale!?
				UUP_Popup* PopupUI = StateMachine->ShowPopup(LOCTEXT("UI_startup_warning_popup_title", "Warning"),
					LOCTEXT("startup_account_create_nick_invalid", "Nickname is invalid."), ECommonPopupType::VE_Error);
				PopupUI->OnClickOK.Clear();

				//RETRY!
				RAuth->EmptyNickName();
				StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
			}
			
		}
		break;
		default:
		{
			//LoginUI->Text_NickName->SetText(FText::FromString(TEXT("")));

			// Something bad might have happened. Go back to the beginning
			StateMachine->InvalidateClientKey();
			StateMachine->InvalidateIdpCredentials();
			StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
		}
		break;
	}
}


/*void UCreateAccountState::OnCreateFail()
{
	RAuth->EmptyAccount();
	StateMachine->ChangeState(EAuthStateEnum::VE_PublisherLogin);
}*/


#undef LOCTEXT_NAMESPACE