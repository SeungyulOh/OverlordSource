// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildListScrollItem.h"
#include "GuildList_ScrollItemData.h"
#include "UIFunctionLibrary.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UtilFunctionLibrary.h"


void UUC_GuildListScrollItem::NativeConstruct()
{
	if(Button_Request)
		Button_Request->OnClicked.AddDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinApply);
	if (Button_WaitCancel)
		Button_WaitCancel->OnClicked.AddDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinCancel);
	if (Button_FreeJoin)
		Button_FreeJoin->OnClicked.AddDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinFree);
	if (Button_Visit)
		Button_Visit->OnClicked.AddDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildVisit);
}

void UUC_GuildListScrollItem::NativeDestruct()
{
	if (Button_Request)
		Button_Request->OnClicked.RemoveDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinApply);
	if (Button_WaitCancel)
		Button_WaitCancel->OnClicked.RemoveDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinCancel);
	if (Button_FreeJoin)
		Button_FreeJoin->OnClicked.RemoveDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildJoinFree);
	if (Button_Visit)
		Button_Visit->OnClicked.RemoveDynamic(this, &UUC_GuildListScrollItem::OnClick_GuildVisit);
}

void UUC_GuildListScrollItem::InvalidateData()
{
	UGuildList_ScrollItemData* guildData = Cast<UGuildList_ScrollItemData>(ItemData);
	FGUILD_INFO info = guildData->GuildInfo;

	SetGuildRanking(info.point_rank, false);
	SetGuildName(info.name, guildData->isWaiting);
	SetGuildLevel(info.level);
	SetGuildMemCnt(info.member_count, info.member_limit);
	SetGuildJoinType(info.auto_grade_up);
	SetGuildDesc(info.desc);
	SetGuildBtnType(info.auto_grade_up, guildData->isMyGuild, guildData->isInfoPage, guildData->isWaiting);
}

void UUC_GuildListScrollItem::OnClick_GuildJoinFree()
{
	UGuildList_ScrollItemData* guildData = Cast<UGuildList_ScrollItemData>(ItemData);
	FGUILD_INFO info = guildData->GuildInfo;

	if (RGAMEINSTANCE(this)->GuildInfo->IsFullWaitingGuildCount())
	{
		UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error2")),
			ECommonPopupType::VE_OK);
	}
	else
	{
		UPacketFunctionLibrary::GUILD_JOIN_RQ(info.guild_id);
	}
}

void UUC_GuildListScrollItem::OnClick_GuildJoinApply()
{
	UGuildList_ScrollItemData* guildData = Cast<UGuildList_ScrollItemData>(ItemData);
	FGUILD_INFO info = guildData->GuildInfo;

	if (RGAMEINSTANCE(this)->GuildInfo->IsFullWaitingGuildCount())
	{
		UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error2")),
			ECommonPopupType::VE_OK);
	}
	else
	{
		UPacketFunctionLibrary::GUILD_JOIN_RQ(info.guild_id);
	}
}

void UUC_GuildListScrollItem::OnClick_GuildJoinCancel()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error3")),
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(UP_CommonPopup))
	{
		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUC_GuildListScrollItem::WaitCancelGuildJoin);
		UP_CommonPopup->OnClickOK.AddDynamic(this, &UUC_GuildListScrollItem::WaitCancelGuildJoin);
	}
}

void UUC_GuildListScrollItem::OnClick_GuildVisit()
{
	UGuildList_ScrollItemData* guildData = Cast<UGuildList_ScrollItemData>(ItemData);
	FGUILD_INFO info = guildData->GuildInfo;
	//GUILD_GET_GUEST
	UPacketFunctionLibrary::GUILD_GET_GUEST_RQ(info.guild_id, 1);
}

void UUC_GuildListScrollItem::WaitCancelGuildJoin()
{
	UGuildList_ScrollItemData* guildData = Cast<UGuildList_ScrollItemData>(ItemData);
	FGUILD_INFO info = guildData->GuildInfo;

	UPacketFunctionLibrary::GUILD_AWAITER_CANCEL_RQ(info.guild_id);
}

void UUC_GuildListScrollItem::SetGuildRanking(int32 ranking, bool isNew)
{
	if (isNew)
	{
		WidgetSwitcher_Ranking->SetActiveWidgetIndex(1);
	}
	else
	{
		WidgetSwitcher_Ranking->SetActiveWidgetIndex(0);
		Text_GuildRank->SetText(FText::AsNumber(ranking));
	}
}

void UUC_GuildListScrollItem::SetGuildName(FString name, bool inWaiting)
{
	if (inWaiting)
		HorizontalBox_JoinWait->SetVisibility(ESlateVisibility::Visible);
	else
		HorizontalBox_JoinWait->SetVisibility(ESlateVisibility::Collapsed);
	
	Text_GuildName->SetText(FText::FromString(name));
}

void UUC_GuildListScrollItem::SetGuildLevel(int32 level)
{
	Text_GuildLevel->SetText(FText::AsNumber(level));
}

void UUC_GuildListScrollItem::SetGuildMemCnt(int32 curr, int32 limit)
{
	Text_MemberCount->SetText(FText::AsNumber(curr));
	Text_MemberLimit->SetText(FText::AsNumber(limit));
}

void UUC_GuildListScrollItem::SetGuildJoinType(bool isFree)
{
	if (isFree)
		WidgetSwitcher_JoinType->SetActiveWidgetIndex(1);
	else
		WidgetSwitcher_JoinType->SetActiveWidgetIndex(0);
}

void UUC_GuildListScrollItem::SetGuildDesc(FString desc)
{
	Text_GuildDesc->SetText(FText::FromString(desc));
}

void UUC_GuildListScrollItem::SetGuildBtnType(bool isFree, bool isMine, bool isinfopage, bool isWaiting)
{
	WidgetSwitcher_ActionType->SetVisibility(ESlateVisibility::Visible);
	if (isinfopage)
	{
		if (isMine)
			WidgetSwitcher_ActionType->SetVisibility(ESlateVisibility::Collapsed);
		else
			WidgetSwitcher_ActionType->SetActiveWidgetIndex(3);
	}
	else
	{
		if (isFree)
			WidgetSwitcher_ActionType->SetActiveWidgetIndex(2);
		else if(isWaiting)
			WidgetSwitcher_ActionType->SetActiveWidgetIndex(1);
		else
			WidgetSwitcher_ActionType->SetActiveWidgetIndex(0);
	}
}
