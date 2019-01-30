// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_MultiRaid_StatCounter.h"
#include "Global/RGameInstance.h"
#include "Array.h"
#include "Global/RealTimeModManager.h"

void UUC_HUD_MultiRaid_StatCounter::NativeConstruct()
{
	RGAMEINSTANCE(this)->RealTimeModManager->OnReceiveStatCount.RemoveDynamic(this, &UUC_HUD_MultiRaid_StatCounter::OnRefreshUI);
	RGAMEINSTANCE(this)->RealTimeModManager->OnReceiveStatCount.AddDynamic(this, &UUC_HUD_MultiRaid_StatCounter::OnRefreshUI);

	if (Button_Open)
		Button_Open->OnClicked.AddDynamic(this, &UUC_HUD_MultiRaid_StatCounter::OpenExtend);
	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_HUD_MultiRaid_StatCounter::CloseExtend);
	FindChildWidgetsWithCertainName<UUC_HUD_MultiRaid_StatCounter_Line>(this, UC_HUD_MultiRaid_Stat_Mlist, TEXT("UC_HUD_MultiRaid_Stat_M_"));
}

void UUC_HUD_MultiRaid_StatCounter::NativeDestruct()
{
	if (Button_Open)
		Button_Open->OnClicked.RemoveDynamic(this, &UUC_HUD_MultiRaid_StatCounter::OpenExtend);
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_HUD_MultiRaid_StatCounter::CloseExtend);
}

void UUC_HUD_MultiRaid_StatCounter::OnRefreshUI(TArray<FString> nickList, TArray<int32> damageList)
{
	//DamageCount_Switcher->SetActiveWidgetIndex(1);

	int32 idx = 0;
	for (auto nick : nickList)
	{
		//
		if (idx == 0)
		{
			UC_HUD_MultiRaid_Stat_S->SetVisibility(ESlateVisibility::Visible);
			UC_HUD_MultiRaid_Stat_S->Text_Rank->SetText(FText::AsNumber(idx + 1));
			UC_HUD_MultiRaid_Stat_S->Text_NickName->SetText(FText::FromString(nick));
		}

		if (idx == 5)
			break;
		//
		UC_HUD_MultiRaid_Stat_Mlist[idx]->SetVisibility(ESlateVisibility::Visible);
		UC_HUD_MultiRaid_Stat_Mlist[idx]->Text_Rank->SetText(FText::AsNumber(idx + 1));
		UC_HUD_MultiRaid_Stat_Mlist[idx]->Text_NickName->SetText(FText::FromString(nick));
		idx++;
	}
	idx = 0;
	for (auto damage : damageList)
	{
		//
		if(idx == 0)
			UC_HUD_MultiRaid_Stat_S->Text_Score->SetText(FText::AsNumber(damage));

		if (idx == 5)
			break;
		//
		UC_HUD_MultiRaid_Stat_Mlist[idx]->Text_Score->SetText(FText::AsNumber(damage));
		//
		idx++;
	}

	for(int32 i = idx; i < 5; i++)
	{
		UC_HUD_MultiRaid_Stat_Mlist[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HUD_MultiRaid_StatCounter::OpenExtend()
{
	DamageCount_Switcher->SetActiveWidgetIndex(1);
}

void UUC_HUD_MultiRaid_StatCounter::CloseExtend()
{
	DamageCount_Switcher->SetActiveWidgetIndex(0);
}

void UUC_HUD_MultiRaid_StatCounter_Line::NativeConstruct()
{
	Text_Rank->SetText(FText());
	Text_Score->SetText(FText());
	Text_NickName->SetText(FText());
}

void UUC_HUD_MultiRaid_StatCounter_Line::NativeDestruct()
{

}
