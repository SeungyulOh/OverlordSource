// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ChangeChannelPopup.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "Network/Chat/ChatManager.h"
#include "Network/RTSManager.h"


void UUC_ChangeChannelPopup::NativeConstruct()
{
	Super::NativeConstruct();

// 	if (IsValid(TextBox_Channel))
// 	{
// 		FText HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000020"));
// 
// 		TextBox_Channel->HintText = HintText;
// 		TextBox_Channel->SynchronizeProperties();
// 	}
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_ChangeChannelPopup::OnClickOk);
	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_ChangeChannelPopup::OnClickClose);
	if (Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_ChangeChannelPopup::OnClickCancel);
}

void UUC_ChangeChannelPopup::NativeDestruct()
{
	if (Button_OK)
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_ChangeChannelPopup::OnClickOk);
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_ChangeChannelPopup::OnClickClose);
	if (Button_Cancel)
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_ChangeChannelPopup::OnClickCancel);

	Super::NativeDestruct();
}

void UUC_ChangeChannelPopup::OnClickOk()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString channelTo;
	if (BP_TextBox_Input)
		channelTo = BP_TextBox_Input->GetText().ToString();

	if (channelTo.IsEmpty())
		return;

	URTSManager* rtsManager = URTSManager::GetInstancePtr();
	rtsManager->CHAT_CHANNEL_MOVE(FString("C-DEV-") + channelTo);
	this->SetVisibility(ESlateVisibility::Collapsed);

	// Check sanity of input
// 	if (!IsValid(TextBox_Channel))
// 	{
// 		return true;
// 	}
// 
// 	FString Text = TextBox_Channel->GetText().ToString();
// 	int n = FCString::Atoi(*Text);
// 	if (!(n >= 1 && n <= 999))
// 	{
// 		// Input error
// 		// Show error popup
// 		auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000000"));
// 		auto Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000007"));
// 		auto Popup = UUIFunctionLibrary::ShowCommonPopup(Caption, Contents, ECommonPopupType::VE_Error);
// 		
// 		return false;
// 	}

	// Send subscribe
// 	if (!RGameInstance->ChatManager->ChangeChannel(n))
// 	{
// 		// Error sending subscribe message
// 		//  * Not connected to chat server
// 		//  * internal error (encoding)
// 		auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000000"));
// 		auto Contents = FText::FromString(TEXT("Unable to subscribe to the channel"));
// 		auto Popup = UUIFunctionLibrary::ShowCommonPopup(Caption, Contents, ECommonPopupType::VE_Error);
// 		return false;
// 	}

	// TODO return for now
	// In the future coordinate with ChatManager to show waiting UI / error popup
	//  * ChatManager::OnSubscriptionChange
	// RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ChangeChannel);
}

void UUC_ChangeChannelPopup::OnClickCancel()
{
	// ...
	this->SetVisibility(ESlateVisibility::Collapsed);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ChangeChannel);
// 	}
}

void UUC_ChangeChannelPopup::OnClickClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
