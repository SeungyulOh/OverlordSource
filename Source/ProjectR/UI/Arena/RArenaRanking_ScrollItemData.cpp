// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RArenaRanking_ScrollItemData.h"



void URArenaRanking_ScrollItemData::SetSwitcherArenRankerDivision(UWidgetSwitcher* division_mark)
{
	if (IsValid(division_mark))
	{
		division_mark->SetActiveWidgetIndex(OrderIndex % 2);
	}
}

void URArenaRanking_ScrollItemData::SetSwitcherArenRankerMark(UWidgetSwitcher* rank_mark, UWidgetSwitcher* WidgetSwitcher_Rank, UTextBlock* Text_Rank)
{
	if (rank_mark)
	{
		if (10 >= RankerData.rank)
		{
			WidgetSwitcher_Rank->SetActiveWidgetIndex(0);
			rank_mark->SetActiveWidgetIndex(RankerData.rank - 1);
		}
		else
		{
			WidgetSwitcher_Rank->SetActiveWidgetIndex(1);
			rank_mark->SetActiveWidgetIndex(0);
			Text_Rank->SetText(FText::AsNumber(RankerData.rank));
		}
	}
}

void URArenaRanking_ScrollItemData::SetTextArenRankerInfo(UTextBlock* level, UTextBlock* nick, UTextBlock* guild)
{
	if (IsValid(level) && IsValid(nick) && IsValid(guild))
	{
		FText info_text;

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(RankerData.level));
		level->SetText(info_text);

		info_text = FText::FromString(RankerData.nick);
		nick->SetText(info_text);

		info_text = FText::FromString(RankerData.guild);
		guild->SetText(info_text);
	}
}

void URArenaRanking_ScrollItemData::SetTextArenRankPowerPoint(UTextBlock* power, UTextBlock* point)
{
	if (IsValid(power) && IsValid(point))
	{
		FText info_text;

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(RankerData.power));
		power->SetText(info_text);

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(RankerData.point));
		point->SetText(info_text);
	}
}