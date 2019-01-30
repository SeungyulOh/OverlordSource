// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildList_ScrollItemData.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"

//not using now
void UGuildList_ScrollItemData::SetGuildInfo(UObject* WorldContextObject, UWidgetSwitcher* GuildRankType, UTextBlock* GuildRank, UTextBlock* GuildName, UTextBlock* GuildLevel,
												UTextBlock* MemberCount, UTextBlock* MemberLimit, UWidgetSwitcher* GuildJoinType, UPanelWidget* GuildJoinWait, UTextBlock* GuildDesc, UWidgetSwitcher* GuildActionType, UWidgetSwitcher* backgroundSwitcher)
{
// 	if (WorldContextObject == nullptr)
// 		return;
// 	
// 	if (!IsValid(GuildRankType) || !IsValid(GuildRank) || !IsValid(GuildName) || !IsValid(GuildLevel) ||
// 		!IsValid(MemberCount) || !IsValid(MemberLimit) || !IsValid(GuildJoinType) || !IsValid(GuildJoinWait) || !IsValid(GuildDesc) || !IsValid(GuildActionType))
// 		return;
// 
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	if (0 < GuildInfo.point)
// 	{
// 		GuildRankType->SetActiveWidgetIndex(0);
// 	}
// 	else
// 	{
// 		GuildRankType->SetActiveWidgetIndex(1);
// 	}
// 	
// 	GuildRank->SetText(FText::AsNumber(GuildInfo.point_rank));
// 	GuildName->SetText(FText::FromString(GuildInfo.name));
// 	GuildLevel->SetText(FText::AsNumber(GuildInfo.level));
// 	MemberCount->SetText(FText::AsNumber(GuildInfo.member_count));
// 	MemberLimit->SetText(FText::AsNumber(GuildInfo.member_limit));
// 	GuildDesc->SetText(FText::FromString(GuildInfo.desc));
// 	GuildJoinWait->SetVisibility(ESlateVisibility::Collapsed);
// 	if (GuildInfo.auto_grade_up)
// 		GuildJoinType->SetActiveWidgetIndex(1);
// 	else
// 		GuildJoinType->SetActiveWidgetIndex(0);
// 	if (RGameInstance->GuildInfo->MyGuildExist)
// 	{
// 		GuildActionType->SetVisibility(ESlateVisibility::Visible);
// 		GuildActionType->SetActiveWidgetIndex(3);
// 		if (GuildInfo.guild_id == RGameInstance->GuildInfo->MyGuildInfo.guild_id)
// 		{
// 			GuildActionType->SetVisibility(ESlateVisibility::Collapsed);
// 			backgroundSwitcher->SetActiveWidgetIndex(1);
// 		}
// 	}
// 	else
// 	{
// 		backgroundSwitcher->SetActiveWidgetIndex(0);
// 		if (GuildInfo.auto_grade_up)
// 		{
// 			// free join button
// 			GuildActionType->SetActiveWidgetIndex(2);	
// 		}
// 		else
// 		{
// 			// request
// 			if (RGameInstance->GuildInfo->IsWaitingGuild(GuildInfo.guild_id))
// 			{
// 				// wait cancel button
// 				GuildActionType->SetActiveWidgetIndex(1);
// 				GuildJoinWait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 			}
// 			else
// 			{
// 				// request button
// 				GuildActionType->SetActiveWidgetIndex(0);
// 			}
// 		}
// 	}
}