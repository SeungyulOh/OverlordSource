// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"
#include "UC_Chat_Ingame.h"



void UUC_Chat_Ingame::NativeConstruct()
{
	LoadConfig();

	Super::NativeConstruct();

	// TODO localization
	// auto ChannelString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_BattleField_Channel"));

	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		this->SetVisibility(ESlateVisibility::Collapsed);

	FText HintText = FText::FromString(TEXT("Enter Message"));

	VirtualKeyboardEntry = FVirtualKeyboardEntry::Create(this, HintText);

	if (Button_Show)
	{
		Button_Show->OnClicked.Clear();
		Button_Show->OnClicked.AddDynamic(this, &UUC_Chat_Ingame::OnClick_Show);
	}
	if (Button_Hide)
	{
		Button_Hide->OnClicked.Clear();
		Button_Hide->OnClicked.AddDynamic(this, &UUC_Chat_Ingame::OnClick_Hide);
	}

}

void UUC_Chat_Ingame::NativeDestruct()
{
	if(VirtualKeyboardEntry.IsValid())
		VirtualKeyboardEntry->Owner = nullptr;
	
	SaveConfig();

	Super::NativeDestruct();
}

void UUC_Chat_Ingame::InputUserMessageVirtualKeyboard()
{
	FSlateApplication::Get().ShowVirtualKeyboard(true, 0, VirtualKeyboardEntry);
}

void UUC_Chat_Ingame::InvalidateNative()
{
	Super::InvalidateNative();

	// Set current channel menu
	SetCurrentChannelMenu(GetChatListName(ViewChatList));

	InvalidateInputCooldown();
}


void UUC_Chat_Ingame::InvalidateInputCooldown()
{
	OnInputCooldownChange(ChatManager->GetOnInputCooldown(), ChatManager->GetInputCooldownRemainingSeconds());
}


void UUC_Chat_Ingame::OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds)
{
	// Patchy
	if (bOnCooldown && RemainingSeconds == 3)
	{
		SetInputCooldown(bOnCooldown, RemainingSeconds);
	}
}

FText UUC_Chat_Ingame::GetChatListName(EChatListEnum ChatListEnum) const
{
	if (ChatListEnum == EChatListEnum::VE_BATTLE) {

		EGamePlayModeTypeEnum PlayModeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
		if (PlayModeType == EGamePlayModeTypeEnum::VE_GuildRaid)
		{
			return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_GuildRaid_2"));
		}
	}

	return Super::GetChatListName(ChatListEnum);
}

void UUC_Chat_Ingame::OnClick_Show()
{
	if (WidgetSwitcher_Chat->IsValidLowLevel())
		WidgetSwitcher_Chat->SetActiveWidgetIndex(0);
}

void UUC_Chat_Ingame::OnClick_Hide()
{
	if (WidgetSwitcher_Chat->IsValidLowLevel())
		WidgetSwitcher_Chat->SetActiveWidgetIndex(1);
}

TSharedRef<FVirtualKeyboardEntry> FVirtualKeyboardEntry::Create(UUC_Chat* InOwner, const FText& InHintText)
{
	return MakeShareable(new FVirtualKeyboardEntry(InOwner, InHintText));
}

// This better be only called from main thread
void FVirtualKeyboardEntry::SetTextFromVirtualKeyboard(const FText& InNewText, ETextEntryType TextEntryType)
{
	UE_LOG(LogUI, Log, TEXT("ENTER SetTextFromVirtualKeyboard(%s, %d, %d)"), *InNewText.ToString(), (int32)TextEntryType);

	// TODO: Change of API. Revise this.
#if 0
#if !PLATFORM_ANDROID
	if (SetTextType == ESetTextType::Commited)
#else
	if (SetTextType == ESetTextType::Changed)
#endif
	{
		if (CommitType == ETextCommit::OnUserMovedFocus)
		{
			if (Owner != nullptr)
			{
				UE_LOG(LogUI, Log, TEXT("SetTextFromVirtualKeyboard: InputUserMessage"));
				Owner->InputUserMessageTargetInput(InNewText.ToString());
			}
		}
	}
#endif
}

FText FVirtualKeyboardEntry::GetText() const
{
	return FText::FromString(TEXT(""));
}

FText FVirtualKeyboardEntry::GetHintText() const
{
	return HintText;
}

EKeyboardType FVirtualKeyboardEntry::GetVirtualKeyboardType() const
{
	return Keyboard_Default;
}

bool FVirtualKeyboardEntry::IsMultilineEntry() const
{
	return false;
}

FVirtualKeyboardEntry::FVirtualKeyboardEntry(UUC_Chat* InOwner, const FText& InHintText): Owner(InOwner), HintText(InHintText)
{
}
