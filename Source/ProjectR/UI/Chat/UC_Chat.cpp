// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Chat.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"


void UUC_Chat::NativeConstruct()
{
	Super::NativeConstruct();

	// Set up things
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 	{
// 		return;
// 	}


	ChatManager = RGameInstance->ChatManager;
	if (!IsValid(ChatManager))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_Chat::NativeContruct: No ChatManager"));
		return;
	}

	// Hook delegates
// 	ChatManager->OnNewMessage.AddDynamic(this, &UUC_Chat::OnNewMessage);
// 	ChatManager->OnSubscriptionChange.AddDynamic(this, &UUC_Chat::OnSubscriptionChange);
// 	ChatManager->OnInputCooldownChange.AddDynamic(this, &UUC_Chat::OnInputCooldownChange);
// 
// 	if (ScrollBox_ChatList)
// 	{
// 		ScrollBox_ChatList->OnUserScrolledToEnd.Clear();
// 		ScrollBox_ChatList->OnUserScrolledToEnd.AddDynamic(this, &UUC_Chat::OnUserScrolledToEnd);
// 	}

	// Set initial View
	// => PlayMode is responsible for specifying own chat detail
//	if (ChatManager->GetRealTimeGameMode() != EChatRealTimeGameMode::VE_NONE)
//	{
//		ViewChatList = EChatListEnum::VE_BATTLE;
//	}
//
//	InvalidateNative();
}

void UUC_Chat::NativeDestruct()
{
	if (ChatManager)
	{
		ChatManager->OnNewMessage.RemoveAll(this);
		ChatManager->OnSubscriptionChange.RemoveAll(this);
		ChatManager->OnInputCooldownChange.RemoveAll(this);
	}

	if (IsValid(ScrollBox_ChatList))
	{
		ScrollBox_ChatList->OnUserScrolledToEnd.Clear();
	}

	Super::NativeDestruct();
}

void UUC_Chat::OnUserScrolledToEnd()
{
// 	if (IsValid(Notify_AdditionalMessage))
// 	{
// 		Notify_AdditionalMessage->SetVisibility(ESlateVisibility::Hidden);
// 	}
}

void UUC_Chat::OnClickTab(EChatListEnum ChatListEnum)
{
// 	if (ViewChatList == ChatListEnum)
// 	{
// 		return;
// 	}
// 
// 	if (!ChatManager->GetChatList(ChatListEnum).GetIsActive())
// 	{
// 		// Issue message
// 		FText Message = MessageEnterAttemptInactiveChatList(ChatListEnum);
// 		IssueSystemMessageToCurrentChatList(Message);
// 		
// 		return;
// 	}
// 
// 	ViewChatList = ChatListEnum;
// 	InvalidateNative();
}

void UUC_Chat::ShowPopupMessage(FString ContentString)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	// Show message
// 	auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000000"));
// 	//auto Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000007"));
// 	FText Contents = FText::FromString(ContentString);
// 
// 	auto Popup = UUIFunctionLibrary::ShowCommonPopup(Caption, Contents, ECommonPopupType::VE_Error);
// 	InvalidateNative();
}

void UUC_Chat::InputUserMessage(FString Message, EChatListEnum ChatListEnum)
{
// 	if (Message.Len() == 0)
// 	{
// 		return;
// 	}
// 
// 	ChatManager->SendMessage(Message, ChatListEnum);
}

void UUC_Chat::InputUserMessageTargetInput(FString Message)
{
//	InputUserMessage(Message, ViewChatList);
}

void UUC_Chat::OnClickClose()
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_Chat))
// 	{
// 		RGameInstance->RWidgetManager->RemoveUserWidget(EWidgetBluePrintEnum::WBP_Chat);
// 	}
}

void UUC_Chat::OnClickKakaoGuildChat()
{
	// TODO
	/*
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->KakaoManager))
	{
		RGameInstance->KakaoManager->OpenKakaoGuildChat();
	}
	*/
}

void UUC_Chat::InvalidateNative()
{
// 	Invalidate();
// 
// 	InvalidateChatList();
// 
// 	SetBattleChannelName(GetChatListName(EChatListEnum::VE_BATTLE));
// 
// 	for (int i = 0; i < (int)EChatListEnum::VE_ENUM_MAX; i++)
// 	{
// 		SetChannelEnabled((EChatListEnum)i, ChatManager->GetChatList((EChatListEnum)i).GetIsActive());
// 	}
}

void UUC_Chat::InvalidateChatList()
{
// 	//UE_LOG(LogChatManager, Log, TEXT("ENTER InvalidateChatList with %d"), (int32)NextChatId);
// 
// 	// Empty ChatList
// 	if (IsValid(ScrollBox_ChatList))
// 	{
// 		ScrollBox_ChatList->ClearChildren();
// 	}
// 	
// 
// 	// Populate ChatList
// 	FChatList& ChatList = ChatManager->GetChatList(ViewChatList);
// 
// 	CHATID_T ChatId;
// 	int Size;
// 	ChatList.GetWindow(ChatId, Size);
// 
// 
// 	NextChatId = ChatId - Size + 1;
// 
// 	//UE_LOG(LogChatManager, Log, TEXT("InvalidateChatList: %d"), (int32)NextChatId);
// 
// 	UpdateScrollBox_ChatList();
}

void UUC_Chat::UpdateScrollBox_ChatList()
{
	//UE_LOG(LogChatManager, Log, TEXT("ENTER UpdateScrollBox_ChatList"));

// 	if (!IsValid(ScrollBox_ChatList))
// 	{
// 		//UE_LOG(LogUI, Error, TEXT("UpdateChatList: ScrollBox_ChatList is null"));
// 		return;
// 	}
// 
// 	ScrollBox_ChatList->SetAnimateScroll();
// 
// 	FChatList& ChatList = ChatManager->GetChatList(ViewChatList);
// 
// 	// Determine new ChatItems using NextChatId
// 	CHATID_T ChatId;
// 	int Size;
// 	ChatList.GetWindow(ChatId, Size);
// 
// 	assert(NextChatId <= ChatId);
// 
// 	bool bForceScroll = false;
// 
// 	// Add ChatItemView
// 	FChatItem ChatItem;
// 	bool bNew = false;
// 	for (; NextChatId <= ChatId; NextChatId++)
// 	{
// 		ChatItem = ChatList.GetChatItem(NextChatId);
// 
// 		if (ChatItem.UIType == EChatItemUIType::VE_MINE)
// 		{
// 			bForceScroll = true;
// 		}
// 
// 		auto ChatItemView = GetChatItemView(ChatItem);
// 		if (IsValid(ChatItemView))
// 		{
// 			ScrollBox_ChatList->AddChild(ChatItemView);
// 		}
// 
// 		bNew = true;
// 	}
// 
// 	if (bForceScroll || !ScrollBox_ChatList->GetIsUserScrolled())
// 	{
// 		ScrollBox_ChatList->RequestForceScroll();
// 	}
// 	else
// 	{
// 		if (IsValid(Notify_AdditionalMessage))
// 		{
// 			Notify_AdditionalMessage->SetVisibility(ESlateVisibility::Visible);
// 		}
// 	}
// 
// 	ChatManager->GetChatList(ViewChatList).SetAsSeen();
// 
// 	if (bNew) {
// 		SetLastChatItem(ChatItem);
// 	}
}

// Add new ChatItemView
void UUC_Chat::OnNewMessage(TArray<EChatListEnum> AffectedChatLists)
{
	// UE_LOG(LogUI, Log, TEXT("ENTER OnNewMessage"));

	// Determine if right ChatList
	/*
	if (ChatListEnum != ViewChatlist)
	{
		return;
	}
	*/

	UpdateScrollBox_ChatList();
}

void UUC_Chat::OnSubscriptionChange(TArray<EChatListEnum> AffectedChatLists)
{
	InvalidateNative();
}

void UUC_Chat::OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds)
{
	UE_LOG(LogUI, Log, TEXT("ENTER OnInputCooldownChange(%d, %d)"), (int32)bOnCooldown, RemainingSeconds);
}

void UUC_Chat::OnPopupMessage(FString Message)
{
	// TODO
}

FString UUC_Chat::FormatDateTime(const FDateTime& DateTime)
{
	// TODO ??? localization
	return DateTime.ToString(TEXT("%H:%M"));
}

FText UUC_Chat::MessageEnterAttemptInactiveChatList(EChatListEnum ChatListEnum)
{
	FText Message = FText::FromString(TEXT("FixMe"));

	if (ChatListEnum == EChatListEnum::VE_PARTY)
	{
		Message = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_msg2"));;
	}
	else if (ChatListEnum == EChatListEnum::VE_GUILD)
	{
		Message = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_msg3"));;
	}

	return Message;
}

void UUC_Chat::IssueSystemMessageToCurrentChatList(FText Message)
{
	EChatItemType ItemType = (EChatItemType)((int)EChatItemType::VE_SYSTEM_CHAT_GENERAL + (int)ViewChatList);
	ChatManager->IssueMessage(Message, ItemType);
}

FText UUC_Chat::GetChatListName(EChatListEnum ChatListEnum) const
{
	FText Res = FText::FromString(TEXT("FixMe"));

	if (ChatListEnum == EChatListEnum::VE_GENERAL)
	{
		// TODO
		Res = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_ChannelTitle"));
	}
	else if (ChatListEnum == EChatListEnum::VE_BATTLE)
	{
		EGamePlayModeTypeEnum PlayModeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
		FName UIStringKey;
		switch (PlayModeType)
		{
			case EGamePlayModeTypeEnum::VE_BattleField:
			UIStringKey = TEXT("UI_lobby_title_Bloodyfield");
			break;
			case EGamePlayModeTypeEnum::VE_BattleGround:
			UIStringKey = TEXT("UIPage_BattleZone2");
			break;
			case EGamePlayModeTypeEnum::VE_GuildRaid:
			UIStringKey = TEXT("UI_Chatting_GuildRaid_1");
			break;
		}
		if (!UIStringKey.IsNone())
		{
			Res = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UIStringKey);
		}
	}
	else if (ChatListEnum == EChatListEnum::VE_PARTY)
	{
		Res = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000003"));
	}
	else if (ChatListEnum == EChatListEnum::VE_GUILD)
	{
		Res = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UIPage_Guild"));
	}

	return Res;
}

UUC_ChatItem* UUC_Chat::GetChatItemView(const FChatItem& InChatItem)
{
	// TODO
	// Instantiate ChatItem widget
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 	{
// 		return nullptr;
// 	}

	UClass* ChatItemClassPtr = UUtilFunctionLibrary::GetBaseWidgetFromAssetID(ChatItemClass);
	if (ChatItemClassPtr)
	{
		UUC_ChatItem* ChatItemView = CreateWidget<UUC_ChatItem>(RGameInstance, ChatItemClassPtr);
		
		ChatItemView->SetChatItem(InChatItem);

// 		ChatItemView->OnClickPlayerName.Clear();
// 		ChatItemView->OnClickPlayerName.AddDynamic(this, &UUC_Chat::OnClickPlayerName);
		
		return ChatItemView;
	}

	return nullptr;
}