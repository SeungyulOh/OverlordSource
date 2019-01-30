 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_History.h"
#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UI/Guild/GuildHistory_ScrollItemData.h"

void UUC_Guild_History::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildHistoryRpDelegate.AddDynamic(this, &UUC_Guild_History::OnGuildHistoryRpDelegate);
	}

	if (ScrollView_HistoryList)
	{
		ScrollView_HistoryList->Init();
		ScrollView_HistoryList->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUC_Guild_History::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildHistoryRpDelegate.RemoveDynamic(this, &UUC_Guild_History::OnGuildHistoryRpDelegate);
	}

	Super::NativeDestruct();
}

void UUC_Guild_History::OpenHistory()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UPacketFunctionLibrary::GUILD_HISTORY_RQ();
}

void UUC_Guild_History::CloseHistory()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_Guild_History::OnGuildHistoryRpDelegate()
{
	this->SetVisibility(ESlateVisibility::Visible);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!IsValid(ScrollView_HistoryList))
		return;

	ScrollView_HistoryList->ClearScrollItemData();
	ScrollView_HistoryList->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_HISTORY> GuildHistoryList = RGameInstance->GuildInfo->HistoryList;
	
	for (FGUILD_HISTORY& GuildHistory : GuildHistoryList)
	{
		UGuildHistory_ScrollItemData* ScrollItemData = Cast<UGuildHistory_ScrollItemData>(ScrollView_HistoryList->AddScrollItemData(UGuildHistory_ScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->GuildHistory = GuildHistory;
		}
	}

	ScrollView_HistoryList->InvalidateData();
}
