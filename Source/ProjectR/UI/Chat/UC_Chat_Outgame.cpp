// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Chat_Outgame.h"
#include "Network/Chat/ChatManager.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "RChatScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "ConstructorHelpers.h"
#include "Class.h"

#define MAXIMUM_CHAT_LINE 100
#define MAXIMUM_INVITE_LINE 20

void UUC_Chat_Outgame::NativeConstruct()
{
	// Initial InputChatLists
	Super::NativeConstruct();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	//event bind
	RGameInstance->EventManager->Receive_CurrentChannel.RemoveDynamic(this, &UUC_Chat_Outgame::Receive_ChannelData);
	RGameInstance->EventManager->Receive_CurrentChannel.AddDynamic(this, &UUC_Chat_Outgame::Receive_ChannelData);
	RGameInstance->EventManager->Receive_Message.RemoveDynamic(this, &UUC_Chat_Outgame::Receive_MessageData);
	RGameInstance->EventManager->Receive_Message.AddDynamic(this, &UUC_Chat_Outgame::Receive_MessageData);
	RGameInstance->EventManager->Receive_UserInOut.RemoveDynamic(this, &UUC_Chat_Outgame::Receive_UserInOut);
	RGameInstance->EventManager->Receive_UserInOut.AddDynamic(this, &UUC_Chat_Outgame::Receive_UserInOut);
	if(!Chat_ScrollView->ScrollItemList.Num())
		Chat_ScrollView->Init_NoClear(MAXSCROLLITEM);


	Chat_ScrollView->SetParentWidget(this);

	if (CheckBox_Radio_Tab_General && CheckBox_Radio_Tab_Party && CheckBox_Radio_Tab_Guild)
	{
		CheckBox_Radio_Tab_General->OnCheckRadioCheck.Clear();
		CheckBox_Radio_Tab_General->OnCheckRadioCheck.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioCheck_General);
		CheckBox_Radio_Tab_Party->OnCheckRadioCheck.Clear();
		CheckBox_Radio_Tab_Party->OnCheckRadioCheck.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioCheck_Party);
		CheckBox_Radio_Tab_Guild->OnCheckRadioCheck.Clear();
		CheckBox_Radio_Tab_Guild->OnCheckRadioCheck.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioCheck_Guild);
		CheckBox_Radio_Tab_Invite->OnCheckRadioCheck.Clear();
		CheckBox_Radio_Tab_Invite->OnCheckRadioCheck.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioCheck_Invite);
		//
		CheckBox_Radio_Tab_Party->OnCheckRadioLockEvent.Clear();
		CheckBox_Radio_Tab_Party->OnCheckRadioLockEvent.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioLockEvent_Party);
		CheckBox_Radio_Tab_Guild->OnCheckRadioLockEvent.Clear();
		CheckBox_Radio_Tab_Guild->OnCheckRadioLockEvent.AddDynamic(this, &UUC_Chat_Outgame::OnCheckRadioLockEvent_Guild);
	}
	
	if (Button_ChangeChannel)
		Button_ChangeChannel->OnClicked.AddDynamic(this, &UUC_Chat_Outgame::OnClickChangeChannel);
	if(Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Chat_Outgame::OnClickCloseButton);
	if (Button_Send)
		Button_Send->OnClicked.AddDynamic(this, &UUC_Chat_Outgame::OnClickSendButton);

	if (cachedChatItemDataList.Num() == 0)
	{
		UUC_ChatItemData* chatData = nullptr;
		for (int32 i = 0; i < MAXIMUM_CHAT_LINE; i++)
		{
			chatData = Cast<UUC_ChatItemData>(Chat_ScrollView->CreateScrollItemData(UUC_ChatItemData::StaticClass()));
			chatData->WidgetSize = Chat_ScrollView->GetItemInitSize();
			cachedChatItemDataList.Emplace(chatData);
		}

		UUC_ChatItemData* inviteData = nullptr;
		for (int32 i = 0; i < MAXIMUM_INVITE_LINE; i++)
		{
			inviteData = Cast<UUC_ChatItemData>(Chat_ScrollView->CreateScrollItemData(UUC_ChatItemData::StaticClass()));
			inviteData->WidgetSize = Chat_ScrollView->GetItemInitSize();
			cachedInviteItemDataList.Emplace(inviteData);
		}
	}
	BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(0);
	
	if (BP_TextBox_Input)
	{
		BP_TextBox_Input->SetHintText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chat_Message_Input_Guide")));
	}

	BindChatData();
}

void UUC_Chat_Outgame::NativeDestruct()
{
	if (IsValid(TextBox_Input))
	{
		TextBox_Input->OnTextChanged.Clear();
		TextBox_Input->OnTextCommitted.Clear();
	}

	if (Button_ChangeChannel)
		Button_ChangeChannel->OnClicked.RemoveDynamic(this, &UUC_Chat_Outgame::OnClickChangeChannel);
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Chat_Outgame::OnClickCloseButton);
	if (Button_Send)
		Button_Send->OnClicked.RemoveDynamic(this, &UUC_Chat_Outgame::OnClickSendButton);

	Super::NativeDestruct();
}

void UUC_Chat_Outgame::OnCheckRadioCheck_General(bool bIsChecked)
{
	eTabType = EChatListEnum::VE_GENERAL;
	BindChatData();
}

void UUC_Chat_Outgame::OnCheckRadioCheck_Party(bool bIsChecked)
{
	eTabType = EChatListEnum::VE_PARTY;
	BindChatData();
}

void UUC_Chat_Outgame::OnCheckRadioCheck_Guild(bool bIsChecked)
{
	eTabType = EChatListEnum::VE_GUILD;
	BindChatData();
}

void UUC_Chat_Outgame::OnCheckRadioCheck_Invite(bool bIsChecked)
{
	eTabType = EChatListEnum::VE_WHISPER;
	//BindInviteData();
	BindChatData();
}

void UUC_Chat_Outgame::GoTo_InviteTab()
{
	BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(3);
	OnCheckRadioCheck_Invite(true);
}

// void UUC_Chat_Outgame::OnCheckRadioCheck_Battle(bool bIsChecked)
// {
// 	OnClickTab(EChatListEnum::VE_BATTLE);
// }

void UUC_Chat_Outgame::OnCheckRadioLockEvent_Party()
{
	auto Message = MessageEnterAttemptInactiveChatList(EChatListEnum::VE_PARTY);
	ShowPopupMessage(Message.ToString());
}

void UUC_Chat_Outgame::OnCheckRadioLockEvent_Guild()
{
	auto Message = MessageEnterAttemptInactiveChatList(EChatListEnum::VE_GUILD);
	ShowPopupMessage(Message.ToString());
}

void UUC_Chat_Outgame::Receive_ChannelData(FString channel)
{
	BindChatData();
}

void UUC_Chat_Outgame::Receive_MessageData()
{
	BindChatData();
}

void UUC_Chat_Outgame::Receive_UserInOut()
{
	BindChatData();
}

void UUC_Chat_Outgame::OnClickInputTarget(EChatListEnum ChatListEnum)
{
	if (ChatListEnum == InputChatLists[(int)ViewChatList])
	{
		// Same as current
		return;
	}

// 	if (!ChatManager->GetChatList(ChatListEnum).GetIsActive())
// 	{
// 		FText Message = MessageEnterAttemptInactiveChatList(ChatListEnum);
// 		ShowPopupMessage(Message.ToString());
// 		return;
// 	}

	InputChatLists[(int)ViewChatList] = ChatListEnum;
	InvalidateInputTargets();
}

void UUC_Chat_Outgame::InputUserMessageTargetInput(FString Message)
{
	InputUserMessage(Message, InputChatLists[(int)ViewChatList]);
}

void UUC_Chat_Outgame::OnTextBoxChanged(const FText& InText)
{
	FString Text = InText.ToString();
	if (Text.Len() == 0)
	{
		return;
	}

	OnActualCommitted(Text);
}

void UUC_Chat_Outgame::OnTextBoxCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	// UE_LOG(LogUI, Log, TEXT("OnTextBoxCommitted: %s %d"), *InText.ToString(), (int32)CommitMethod);
#if PLATFORM_DESKTOP
	if (CommitMethod != ETextCommit::OnEnter)
	{
		return;
	}
#else
	if (CommitMethod != ETextCommit::OnUserMovedFocus)
	{
		return;
	}
#endif
	
	FString InString = InText.ToString();

	if (InString.Len() == 0)
	{
		return;
	}

	OnActualCommitted(InString);
	
}

void UUC_Chat_Outgame::OnClickChangeChannel()
{
	if (UC_Chatting_ChangeChannel->GetVisibility() == ESlateVisibility::Visible)
		UC_Chatting_ChangeChannel->SetVisibility(ESlateVisibility::Collapsed);
	else
		UC_Chatting_ChangeChannel->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Chat_Outgame::OnClickSendButton()
{
	FText message = BP_TextBox_Input->GetText();
	if (message.IsEmpty())
		return;
	RGAMEINSTANCE(this)->ChatManager->SendMessage(message.ToString(), eTabType);
	BP_TextBox_Input->SetText(FText());
}

void UUC_Chat_Outgame::OnClickCloseButton()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	RGAMEINSTANCE(this)->EventManager->SetChatMinimalize.Broadcast(true);
	RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Chat);
}

void UUC_Chat_Outgame::InvalidateNative()
{
	Super::InvalidateNative();

	InvalidateTabs();
	InvalidateInputTargets();
	InvalidateInputCooldown();
}

void UUC_Chat_Outgame::InvalidateTabs()
{
	if (IsValid(BP_CanvasPanel_RadioGroup_Tabs))
	{
		switch (ViewChatList)
		{
		case EChatListEnum::VE_GENERAL:
			BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(0);
		break;

		case EChatListEnum::VE_PARTY:
			BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(1);
		break;

		case EChatListEnum::VE_GUILD:
			BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(2);
		break;

		case EChatListEnum::VE_WHISPER:
			BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(3);
		break;
		}
	}

	auto ChannelString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_BattleField_Channel"));
	auto ChannelStringWithNumber = FString::Printf(TEXT("%d %s"), ChatManager->GetGeneralChannelNumber(), *ChannelString.ToString());
	SetGeneralChannelName(FText::FromString(ChannelStringWithNumber));
}

void UUC_Chat_Outgame::InvalidateInputTargets()
{
	for (int i = 0; i < (int)EChatListEnum::VE_ENUM_MAX; i++)
	{
		SetInputTargetName((EChatListEnum)i, GetChatListName((EChatListEnum)i));
	}

	SetCurrentInputTargetName(GetChatListName(InputChatLists[(int)ViewChatList]));
}

void UUC_Chat_Outgame::InvalidateInputCooldown()
{
	OnInputCooldownChange(ChatManager->GetOnInputCooldown(), ChatManager->GetInputCooldownRemainingSeconds());
}

void UUC_Chat_Outgame::OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds)
{
	if (!IsValid(TextBox_Input))
	{
		return;
	}

	if (bOnCooldown)
	{
		FText HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_msg4"));
		FText Formatted = FText::Format(HintText, FText::FromString(FString::FromInt(RemainingSeconds)));
		TextBox_Input->HintText = Formatted;
		TextBox_Input->SynchronizeProperties();
		TextBox_Input->SetIsEnabled(false);
	}
	else
	{
		auto HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_msg5"));
		
		TextBox_Input->HintText = HintText;
		TextBox_Input->SynchronizeProperties();
		TextBox_Input->SetIsEnabled(true);
	}
}

void UUC_Chat_Outgame::BindChatData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	Chat_ScrollView->ClearScrollItemData();
	std::queue<FChatItem> ChatLists;
	ChatLists = RGameInstance->ChatManager->ChatLists;
	int32 i = 0;
	FChatItem LastChatItem;
	FInviteData LastInvite;
	if (eTabType == EChatListEnum::VE_GENERAL)
	{
		while (!ChatLists.empty())
		{
			LastChatItem = ChatLists.front();
			if (LastChatItem.GroupType == EChatListEnum::VE_GENERAL || 
				LastChatItem.GroupType == EChatListEnum::VE_NOTIFY ||
				LastChatItem.UIType == EChatItemUIType::VE_INOUTALARM)
			{
				UUC_ChatItemData* scrollItemData = cachedChatItemDataList[i];
				scrollItemData->itemData = ChatLists.front();
				scrollItemData->OnClickPlayer.Clear();
				scrollItemData->OnClickPlayer.AddDynamic(this, &::UUC_Chat_Outgame::GoTo_InviteTab);
				scrollItemData->currTab = eTabType;
				Chat_ScrollView->ItemDataList.Emplace(scrollItemData);
				i++;
			}
			ChatLists.pop();
		}
	}
	else if (eTabType == EChatListEnum::VE_PARTY)
	{
		while (!ChatLists.empty())
		{
			LastChatItem = ChatLists.front();
			if (LastChatItem.GroupType == EChatListEnum::VE_PARTY || 
				LastChatItem.GroupType == EChatListEnum::VE_NOTIFY ||
				LastChatItem.UIType == EChatItemUIType::VE_INOUTALARM)
			{
				UUC_ChatItemData* scrollItemData = cachedChatItemDataList[i];
				scrollItemData->itemData = ChatLists.front();
				scrollItemData->currTab = eTabType;
				Chat_ScrollView->ItemDataList.Emplace(scrollItemData);
				i++;
			}
			ChatLists.pop();
		}
	}
	else if (eTabType == EChatListEnum::VE_GUILD)
	{
		while (!ChatLists.empty())
		{
			LastChatItem = ChatLists.front();
			if (LastChatItem.GroupType == EChatListEnum::VE_GUILD || 
				LastChatItem.UIType == EChatItemUIType::VE_INOUTALARM)
			{
				UUC_ChatItemData* scrollItemData = cachedChatItemDataList[i];
				scrollItemData->itemData = ChatLists.front();
				scrollItemData->currTab = eTabType;
				Chat_ScrollView->ItemDataList.Emplace(scrollItemData);
				i++;
			}
			ChatLists.pop();
		}
	}
	else if (eTabType == EChatListEnum::VE_WHISPER)
	{
		std::queue<FInviteData> list = RGameInstance->ChatManager->InviteLists;
		while (!list.empty())
		{
			LastInvite = list.front();
			UUC_ChatItemData* scrollItemData = cachedChatItemDataList[i];
			scrollItemData->InviteData = LastInvite;
			scrollItemData->currTab = eTabType;
			Chat_ScrollView->ItemDataList.Emplace(scrollItemData);
			i++;

			list.pop();
		}
	}

	Chat_ScrollView->InvalidateData();
	Chat_ScrollView->SetScrollToTail();

	//for chat current scroll item update
	//Chat_ScrollView->UpdateTargetScrollItem()
//	Chat_ScrollView->SetScrollUpdate(Cast<URScrollItem>(Chat_ScrollView->ScrollItemList.Last()), FVector2D());
}

void UUC_Chat_Outgame::BindInviteData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//Chat_ScrollView->ScrollItemBpClass = Cast<URScrollItem>(StaticLoadObject(UUC_InviteItem::StaticClass(), NULL,TEXT("WidgetBlueprint'/Game/UMG/Chatting/UC_Chatting_Invite_ScrollItem.UC_Chatting_Invite_ScrollItem_C'")));
	//Chat_ScrollView->ScrollItemBpClass = UUC_InviteItem::StaticClass();
	ConstructorHelpers::FObjectFinder<UClass> ItemObject(TEXT("Blueprint'/Game/UMG/Chatting/UC_Chatting_Invite_ScrollItem.UC_Chatting_Invite_ScrollItem_C'"));
	
	Chat_ScrollView->ScrollItemBpClass = ItemObject.Object;
	Chat_ScrollView->Init();
	Chat_ScrollView->ClearScrollItemData();
	std::queue<FInviteData> InviteLists;
	InviteLists = RGameInstance->ChatManager->InviteLists;
	FInviteData LastInviteItem;
	int32 i = 0;
	while (!InviteLists.empty())
	{
		LastInviteItem = InviteLists.front();
			
		UUC_ChatItemData* scrollItemData = cachedInviteItemDataList[i];
		scrollItemData->InviteData = InviteLists.front();
		scrollItemData->currTab = eTabType;
		Chat_ScrollView->ItemDataList.Emplace(scrollItemData);
		i++;
			
		InviteLists.pop();
	}
	Chat_ScrollView->InvalidateData();
}

void UUC_Chat_Outgame::OnActualCommitted(FString Message)
{
	InputUserMessageTargetInput(Message);

	TextBox_Input->SetText(FText::FromString(TEXT("")));
}
