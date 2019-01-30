// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ArenaSeasonReward_ScrollItemData.h"


#include "UtilFunctionIntegrated.h"



void UArenaSeasonReward_ScrollItemData::SetSwitcherSeasonRewardDivision(UWidgetSwitcher* division_mark)
{
	if(IsValid(division_mark))
	{
		division_mark->SetActiveWidgetIndex(OrderIndex % 2);
	}
}

void UArenaSeasonReward_ScrollItemData::SetTextSeasonRewardKind(UTextBlock* reward_kind, UWidgetSwitcher* reward_mark)
{
	if (IsValid(reward_kind) && IsValid(reward_mark))
	{
		if (Top10Reward)
		{
			reward_kind->SetText(RewardKindText);
			reward_mark->SetActiveWidgetIndex(OrderIndex);
		}
		else
		{
			reward_kind->SetText(RewardKindText);
			reward_mark->SetActiveWidgetIndex(OrderIndex + 10); // Top10 index : 0 ~ 9
		}
	}
}

void UArenaSeasonReward_ScrollItemData::SetSeasonRewardKind(int32 index)
{
	OrderIndex = index;

	if (Top10Reward)
	{
		switch (OrderIndex)
		{
		case 0:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top1")); break;
		case 1:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top2")); break;
		case 2:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top3")); break;
		case 3:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top4")); break;
		case 4:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top5")); break;
		case 5:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top6")); break;
		case 6:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top7")); break;
		case 7:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top8")); break;
		case 8:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top9")); break;
		case 9:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Top10")); break;
		}
	}
	else
	{
		FText kind_text;
		switch (OrderIndex)
		{
		case 0:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section10")); break;
		case 1:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section20")); break;
		case 2:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section30")); break;
		case 3:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section40")); break;
		case 4:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section50")); break;
		case 5:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section60")); break;
		case 6:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section70")); break;
		case 7:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section80")); break;
		case 8:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section90")); break;
		case 9:	RewardKindText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonReward_Section100")); break;
		}
	}
}