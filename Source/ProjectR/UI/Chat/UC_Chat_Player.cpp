// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UC_Chat_Player.h"
#include "UI/UI_Common//UP_CommonPopup.h"


void UUC_Chat_Player::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ChatManager = RGameInstance->ChatManager;

	Invalidate();
}


void UUC_Chat_Player::NativeDestruct()
{
	Super::NativeDestruct();
}

// TODO: May hook to ChatManager.OnBlockedUsersChange
void UUC_Chat_Player::Invalidate()
{
	if (IsValid(Text_Name))
		Text_Name->SetText(FText::FromString(ChatUser.who));

	if (IsValid(Button_Block))
		Button_Block->SetIsEnabled(!ChatManager->IsUserBlocked(ChatUser.kId));
}

void UUC_Chat_Player::BlockConfirm()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	// Show message

	auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000001"));
	FText Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Question_Message_000005"));
	auto ContentsFormatted = FText::Format(Contents, FText::FromString(ChatUser.who));

	auto Popup = UUIFunctionLibrary::ShowCommonPopup(Caption, ContentsFormatted, ECommonPopupType::VE_OKCANCEL);

	Popup->OnClickOK.Clear();
	Popup->OnClickOK.AddDynamic(this, &UUC_Chat_Player::Block);
}

void UUC_Chat_Player::Block()
{
	ChatManager->BlockUser(ChatUser);

	Invalidate();
}
