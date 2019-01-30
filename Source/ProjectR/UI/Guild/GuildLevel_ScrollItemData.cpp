// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildLevel_ScrollItemData.h"
#include "GlobalIntegrated.h"

#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"


void UGuildLevel_ScrollItemData::SetGuildLevel(UObject* WorldContextObject, UImage* GuildLevelIcon, UTextBlock* Text_GuildLevelDesc, UWidgetSwitcher* Switcher_GuildLevel, UWidgetSwitcher* Switcher_GuildLevel2)
{
	if (WorldContextObject == nullptr)
		return;

	if (!IsValid(GuildLevelIcon) || !IsValid(Text_GuildLevelDesc) || !IsValid(Switcher_GuildLevel) || !IsValid(Switcher_GuildLevel2))
		return;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	FString LevelIconKey = "Icon_GuildLevel_";
	if (10 > ShowLevelUpValue)
		LevelIconKey += "0";
	LevelIconKey.AppendInt(ShowLevelUpValue);
	LevelIconKey += "_Sprite";
	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(FName(*LevelIconKey)), GuildLevelIcon);

	if (RGameInstance->GuildInfo->MyGuildInfo.level == ShowLevelUpValue)
	{
		Switcher_GuildLevel->SetActiveWidgetIndex(1);
		Switcher_GuildLevel2->SetActiveWidgetIndex(1);
	}
	else
	{
		Switcher_GuildLevel->SetActiveWidgetIndex(0);
		Switcher_GuildLevel2->SetActiveWidgetIndex(0);
	}

	FString GuildLevelDescString = "";
	FFormatOrderedArguments Args;
	int32 FormatOrdereIndex = 0;

	FName GuildLevelUpKey = FName(*FString::FromInt(ShowLevelUpValue));
	FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
	if (GuildTableInfo)
	{
		GuildLevelDescString += FString::Printf(TEXT("{0} %d{1}"), GuildTableInfo->member_limit);
		Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildMaxmember")));
		Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("USK_up_HeroNum")));

		FormatOrdereIndex = 1;
		if (GuildTableInfo->guildbuff)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}%d"), ++FormatOrdereIndex, ShowLevelUpValue);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBuff")));
		}

		GuildLevelDescString += LINE_TERMINATOR;
		GuildLevelDescString += FString::Printf(TEXT("{%d}%d"), ++FormatOrdereIndex, ShowLevelUpValue);
		Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildDailyReward")));

		if (GuildTableInfo->guildraid1)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}1"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBoss")));
		}
		if (GuildTableInfo->guildraid2)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}2"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBoss")));
		}
		if (GuildTableInfo->guildraid3)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}3"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBoss")));
		}
		if (GuildTableInfo->guildraid4)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}4"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBoss")));
		}
		if (GuildTableInfo->guildraid5)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}5"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenGuildBoss")));
		}

		if (GuildTableInfo->guildmatch)
		{
			GuildLevelDescString += LINE_TERMINATOR;
			GuildLevelDescString += FString::Printf(TEXT("{%d}"), ++FormatOrdereIndex);
			Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UP_Guild_Management_OpenMatch")));
		}
	}

	FText GuildLevelDescText = FText::FromString(GuildLevelDescString);
	GuildLevelDescText = FText::Format(GuildLevelDescText, Args);
	Text_GuildLevelDesc->SetText(GuildLevelDescText);
}