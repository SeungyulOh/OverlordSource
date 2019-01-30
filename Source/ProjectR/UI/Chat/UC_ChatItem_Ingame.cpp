// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Table/ChatStringAttributeTableInfo.h"
#include "UC_ChatItem_Ingame.h"
#include "GlobalIntegrated.h"



void UUC_ChatItem_Ingame::NativeConstruct()
{
	Super::NativeConstruct();

	if (bMinimized)
	{
		if (IsValid(Text_Message_Normal))
		{
			Text_Message_Normal->AutoWrapText = false;
			Text_Message_Normal->SynchronizeProperties();
		}
	}
}

FText UUC_ChatItem_Ingame::GetRichIngameMessage() const
{
	// For Normal and Mine messages
	return RichIngameMessage;
}

void UUC_ChatItem_Ingame::InvalidateNative()
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();

	static FChatStringAttributeTableInfo* UserNameInfo;
	if (UserNameInfo == nullptr)
	{
		UserNameInfo = TableManager->GetChatStringAttributeTableRow(TEXT("INGAME_User_name"));
	}

	FColor UserNameColor = FColor::Cyan;
	if (UserNameInfo)
	{
		UserNameColor = UserNameInfo->color;
	}

	FText Message = GetMessage();
	/*
	if (bMinimized)
	{
		if (ChatItem.UIType == EChatItemUIType::VE_SYSTEM)
		{
			Message = GetPossiblyTruncatedText(Message, CHATITEM_INGAME_SYSTEM_MINIMIZED_LIMIT_KO_KR);
		}
		else
		{
			Message = GetPossiblyTruncatedText(Message, FMath::Clamp(CHATITEM_INGAME_MINIMIZED_LIMIT_KO_KR - GetWho().Len() - 1, 0, CHATITEM_INGAME_MINIMIZED_LIMIT_KO_KR));
		}
	}
	*/

	FText RichUserName = ApplyColorRichText(FText::FromString(GetWho()), UserNameColor);
	//FText RichMessage = ApplyColorRichText(Message, GetColor());

	//RichIngameMessage = FText::Format(FText::FromString(TEXT("{0} {1}")), RichUserName, RichMessage);

	Super::InvalidateNative();
}

FText UUC_ChatItem_Ingame::ApplyColorRichText(FText InText, FColor InColor) const
{
	auto ColorInHex = InColor.ToHex();

	// TODO: Unescape InText

	return FText::FromString(FString(TEXT("<span color=\"#")) + ColorInHex + TEXT("\">") + InText.ToString() + TEXT("</>"));
}

FText UUC_ChatItem_Ingame::GetPossiblyTruncatedText(FText Text, int Limit)
{
	FString TextString = Text.ToString();
	if (TextString.Len() > Limit)
	{
		TextString = TextString.Left(Limit) + CHATITEM_INGAME_ELLIPSIS;
	}

	return FText::FromString(TextString);
}
