// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildHistory_ScrollItemData.h"
#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"


void UGuildHistory_ScrollItemData::SetGuildHistory(UObject* WorldContextObject, UTextBlock* TextYear, UTextBlock* TextMonth, UTextBlock* TextDay, 
												UTextBlock* TextHour, UTextBlock* TextMinute, UTextBlock* TextSecond, UTextBlock* TextHistory)
{
	if (WorldContextObject == nullptr)
		return;
	
	if (!IsValid(TextYear) || !IsValid(TextMonth) || !IsValid(TextDay) ||
		!IsValid(TextHour) || !IsValid(TextMinute) || !IsValid(TextSecond) || !IsValid(TextHistory))
		return;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert); 
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	// History Time
	FDateTime HistoryTime;
	FDateTime::ParseIso8601(*(GuildHistory.time), HistoryTime);

	HistoryTime += RGameInstance->UTC_DiffTimespan;
	
	int32 HistoryYear, HistoryMonth, HistoryDay;
	HistoryTime.GetDate(HistoryYear, HistoryMonth, HistoryDay);
	
	TextYear->SetText(FText::FromString(FString::FromInt(HistoryYear)));
	TextMonth->SetText(UUIFunctionLibrary::Get_0n_Number(HistoryMonth));
	TextDay->SetText(UUIFunctionLibrary::Get_0n_Number(HistoryDay));
	TextHour->SetText(UUIFunctionLibrary::Get_0n_Number(HistoryTime.GetHour()));
	TextMinute->SetText(UUIFunctionLibrary::Get_0n_Number(HistoryTime.GetMinute()));
	TextSecond->SetText(UUIFunctionLibrary::Get_0n_Number(HistoryTime.GetSecond()));

	// History String
	FName GuildHistoryKey;
	FFormatOrderedArguments Args;
	Args.Emplace(FText::FromString(GuildHistory.nick));
	switch (GuildHistory.type)
	{
	case EGUILD_HISTORY_TYPE::JOIN:
		GuildHistoryKey = FName("UP_Guild_GuildReport_Join");
		break;
	case EGUILD_HISTORY_TYPE::GRADE_UP:
		GuildHistoryKey = FName("UP_Guild_GuildReport_GradeUp");
		Args.Emplace(FText::FromString(GuildHistory.target_nick));
		Args.Emplace(UResourceFunctionLibrary::GetGuildGradeTypeName(GuildHistory.grade));
		break;
	case EGUILD_HISTORY_TYPE::GRADE_DOWN:
		GuildHistoryKey = FName("UP_Guild_GuildReport_GradeDown");
		Args.Emplace(FText::FromString(GuildHistory.target_nick));
		Args.Emplace(UResourceFunctionLibrary::GetGuildGradeTypeName(GuildHistory.grade));
		break;
	case EGUILD_HISTORY_TYPE::KICK:
		GuildHistoryKey = FName("UP_Guild_GuildReport_Kick");
		Args.Emplace(FText::FromString(GuildHistory.target_nick));
		break;
	case EGUILD_HISTORY_TYPE::QUIT:
		GuildHistoryKey = FName("UP_Guild_GuildReport_Quit");
		break;
	case EGUILD_HISTORY_TYPE::CHANGE_NAME:
		GuildHistoryKey = FName("UP_Guild_GuildReport_ChangeName");
		break;
	case EGUILD_HISTORY_TYPE::CHANGE_INFO:
		GuildHistoryKey = FName("UP_Guild_GuildReport_ChangeInfo");
		break;
	case EGUILD_HISTORY_TYPE::LEVEL_UP:
		GuildHistoryKey = FName("UP_Guild_GuildReport_LevelUp");
		break;
	case EGUILD_HISTORY_TYPE::BOSS_SPAWN:
		GuildHistoryKey = FName("UP_Guild_GuildReport_BossSpawn");
		break;
	}

	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, GuildHistoryKey);
	LocalizedText = FText::Format(LocalizedText, Args);
	TextHistory->SetText(LocalizedText);
}