// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Guild_Join.h"

#include "GlobalIntegrated.h"


#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Guild/GuildList_ScrollItemData.h"
#include "UI/Guild/UC_Guild_Create.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Guild/UC_Guild_Create.h"
#include "UP_GuildLobby.h"
#include "UP_Guild_Management.h"

#define MAXGUILDLIST 50

void UUP_Guild_Join::NativeConstruct()
{
	Super::NativeConstruct();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->EventManager->OnRefreshGuildInfoUI.AddDynamic(this, &UUP_Guild_Join::OnRefreshGuildInfoUI);
	RGameInstance->HttpClient->OnGuildCreateRpDelegate.AddDynamic(this, &UUP_Guild_Join::OnGuildCreateRpDelegate);
	RGameInstance->HttpClient->OnGuildFindNameRpDelegate.AddDynamic(this, &UUP_Guild_Join::OnGuildFindNameRpDelegate);
	RGameInstance->HttpClient->OnGuildJoinRpDelegate.AddDynamic(this, &UUP_Guild_Join::OnGuildJoinRpDelegate);
	RGameInstance->HttpClient->OnGuildAwaiterCancelRpDelegate.AddDynamic(this, &UUP_Guild_Join::OnGuildAwaiterCancelRpDelegate);
	RGameInstance->HttpClient->OnGuildContributeRankingRpDelegate.AddDynamic(this, &UUP_Guild_Join::OnGuildContributeRankingRpDelegate);
	Button_PrevRanking->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildRankingPrev);
	Button_NextRanking->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildRankingNext);
	Button_FindGuildName->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildFind);
	Button_FindGuildRefresh->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnRefreshGuildInfoUI);
	Button_GuildJoinAuto->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildJoinAuto);
	Button_CreateGuild->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildCreateTabOpen);
	CheckBox_Radio_Join->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Join::OnClick_Guild_MainTab_Join);
	CheckBox_Radio_Ranking->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Join::OnClick_Guild_MainTab_Ranking);
	Button_RQGuildCreate->OnClicked.AddDynamic(this, &UUP_Guild_Join::OnClick_GuildCreate);
	CheckBox_Radio_AutoGradeUp->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Auto);
	CheckBox_Radio_WaitGradeUp->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Confirm);

	Init();
}

void UUP_Guild_Join::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->GuildInfo->ClearJoinUIData();
	RGameInstance->EventManager->OnRefreshGuildInfoUI.RemoveDynamic(this, &UUP_Guild_Join::OnRefreshGuildInfoUI);
	RGameInstance->HttpClient->OnGuildCreateRpDelegate.RemoveDynamic(this, &UUP_Guild_Join::OnGuildCreateRpDelegate);
	RGameInstance->HttpClient->OnGuildFindNameRpDelegate.RemoveDynamic(this, &UUP_Guild_Join::OnGuildFindNameRpDelegate);
	RGameInstance->HttpClient->OnGuildJoinRpDelegate.RemoveDynamic(this, &UUP_Guild_Join::OnGuildJoinRpDelegate);
	RGameInstance->HttpClient->OnGuildAwaiterCancelRpDelegate.RemoveDynamic(this, &UUP_Guild_Join::OnGuildAwaiterCancelRpDelegate);
	RGameInstance->HttpClient->OnGuildContributeRankingRpDelegate.RemoveDynamic(this, &UUP_Guild_Join::OnGuildContributeRankingRpDelegate);
	Button_PrevRanking->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildRankingPrev);
	Button_NextRanking->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildRankingNext);
	Button_FindGuildName->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildFind);
	Button_FindGuildRefresh->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnRefreshGuildInfoUI);
	Button_GuildJoinAuto->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildJoinAuto);
	Button_CreateGuild->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildCreateTabOpen);
	CheckBox_Radio_Join->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Join::OnClick_Guild_MainTab_Join);
	CheckBox_Radio_Ranking->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Join::OnClick_Guild_MainTab_Ranking);
	Button_RQGuildCreate->OnClicked.RemoveDynamic(this, &UUP_Guild_Join::OnClick_GuildCreate);
	CheckBox_Radio_AutoGradeUp->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Auto);
	CheckBox_Radio_WaitGradeUp->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Confirm);

	Super::NativeDestruct();
}

void UUP_Guild_Join::Init()
{
	GuildUIState = EGuildJoinTab::VE_Join;
	CurrentRankingPage = 1;
	RequestRankingPage = 1;
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	HintText_FindGuildName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Search"));
	HintText_GuildName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Creat_Desc_01"));
	HintText_GuildDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Creat_Desc_02"));
	
	Text_GuildName->SetHintText(HintText_GuildName);
	Text_GuildDesc->SetHintText(HintText_GuildDesc);

	GuildFindTimeOut();

	GuildCreateGold = 0;
	if (UC_CurrencyDisplay)
	{
		FName GuildLevelUpKey = FName(*FString::FromInt(1));
		FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
		if (GuildTableInfo)
		{
			GuildCreateGold = GuildTableInfo->gold;
		}
		UC_CurrencyDisplay->SetCurrency(WALLET_GOLD, GuildCreateGold);
	}
	//RequestGuildRanking();
	//OnClick_GuildFind();

	if (CachedGuildList.Num() == 0)
	{
		for(int32 i = 0; i < MAXGUILDLIST; i++)
		{ 
			UGuildList_ScrollItemData* ItemData = Cast<UGuildList_ScrollItemData>(ScrollView_Recommand_GuildList->CreateScrollItemData(UGuildList_ScrollItemData::StaticClass()));
			ItemData->WidgetSize = ScrollView_Recommand_GuildList->GetItemInitSize();
			CachedGuildList.Emplace(ItemData);
		}
	}

	UPacketFunctionLibrary::GUILD_GET_RQ();
}

//////////////////////////////////////////////////////////////////////////
// Tab Click
void UUP_Guild_Join::OnClick_GuildJoinTab(EGuildJoinTab InType)
{
	GuildUIState = InType;

	RefreshGuildJoinUI();
}

void UUP_Guild_Join::RefreshGuildJoinUI()
{
	switch (GuildUIState)
	{
	case EGuildJoinTab::VE_Join:	RefreshJoinUI();		break;
	case EGuildJoinTab::VE_Ranking:	RefreshRankingUI();	break;
	//case EGuildJoinTab::VE_Create:	BP_RefreshCreateUI();	break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Join Tab

void UUP_Guild_Join::OnClick_GuildFind()
{
	FText SearchGuildName;
	if (EditableTextBox_FindGuildName)
		SearchGuildName = EditableTextBox_FindGuildName->GetText();
	else
		SearchGuildName = FText();
	bool Refresh = true;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (Refresh)
	{
		if (RGameInstance->GetTimerManager().IsTimerActive(TimeHandler_GuildFind))
		{
			return;
		}

		RGameInstance->GetTimerManager().ClearTimer(TimeHandler_GuildFind);
		RGameInstance->GetTimerManager().SetTimer(TimeHandler_GuildFind, this, &UUP_Guild_Join::GuildFindTimeOut, 5.0f, false);

		if (Button_FindGuildRefresh)
		{
			Button_FindGuildRefresh->SetIsEnabled(false);
		}
	}
	UPacketFunctionLibrary::GUILD_FIND_NAME_RQ(SearchGuildName.ToString());
}

void UUP_Guild_Join::OnClick_GuildJoinAuto()
{
	UPacketFunctionLibrary::GUILD_JOIN_AUTO_RQ();
}

void UUP_Guild_Join::OnClick_Guild_MainTab_Join(bool isCheck)
{
	if (WidgetSwitcher_Radio_Join->GetActiveWidgetIndex() == 0)
		return;
	else
	{
		GuildUIState = EGuildJoinTab::VE_Join;
		WidgetSwitcher_JoinType->SetActiveWidgetIndex(0);
		WidgetSwitcher_Radio_Join->SetActiveWidgetIndex(0);
		WidgetSwitcher_Radio_Ranking->SetActiveWidgetIndex(1);
		OnClick_GuildFind();
	}
}

void UUP_Guild_Join::OnClick_Guild_MainTab_Ranking(bool isCheck)
{
	if (WidgetSwitcher_Radio_Ranking->GetActiveWidgetIndex() == 0)
		return;
	else
	{
		GuildUIState = EGuildJoinTab::VE_Ranking;
		WidgetSwitcher_JoinType->SetActiveWidgetIndex(1);
		WidgetSwitcher_Radio_Ranking->SetActiveWidgetIndex(0);
		WidgetSwitcher_Radio_Join->SetActiveWidgetIndex(1);
		RequestGuildRanking();
	}
}

void UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Auto(bool isCheck)
{
	if (isCheck == true)
		Create_AutoGradeUp = true;
}

void UUP_Guild_Join::OnClick_Guild_Create_Join_Type_Confirm(bool isCheck)
{
	if (isCheck == true)
		Create_AutoGradeUp = false;
}

void UUP_Guild_Join::GuildFindTimeOut()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
//	
	{
		RGameInstance->GetTimerManager().ClearTimer(TimeHandler_GuildFind);
	}

	if (Button_FindGuildRefresh)
	{
		Button_FindGuildRefresh->SetIsEnabled(true);
	}
}
// 
// void UUP_Guild_Join::SetScrollDataFindGuildList(URScrollView* ScrollView)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	if (!IsValid(ScrollView))
// 		return;
// 
// 	ScrollView->Init();
// 	ScrollView->ClearScrollItemData();
// 	ScrollView->SetVisibility(ESlateVisibility::Visible);
// 
// 	FGUILD_INFO* ptrGuildInfo = nullptr;
// 	TArray<int32>& outWaitingGuildIDs = RGameInstance->GuildInfo->WaitingGuildIDs;
// 	for (auto GuildID : outWaitingGuildIDs)
// 	{
// 		ptrGuildInfo = RGameInstance->GuildInfo->GetFindGuild(GuildID);
// 		if (ptrGuildInfo != nullptr)
// 		{
// 			UGuildList_ScrollItemData* ScrollItemData = Cast<UGuildList_ScrollItemData>(ScrollView->AddScrollItemData(UGuildList_ScrollItemData::StaticClass()));
// 			if (ScrollItemData)
// 			{
// 				ScrollItemData->GuildInfo = *ptrGuildInfo;
// 			}
// 		}
// 	}
// 
// 	TArray<FGUILD_INFO>& outGuildList = RGameInstance->GuildInfo->FindGuildList;
// 	for (FGUILD_INFO& FindGuildInfo : outGuildList)
// 	{
// 		if (RGameInstance->GuildInfo->IsWaitingGuild(FindGuildInfo.guild_id))
// 			continue;
// 
// 		UGuildList_ScrollItemData* ScrollItemData = Cast<UGuildList_ScrollItemData>(ScrollView->AddScrollItemData(UGuildList_ScrollItemData::StaticClass()));
// 		if (ScrollItemData)
// 		{
// 			ScrollItemData->GuildInfo = FindGuildInfo;
// 		}
// 	}
// 
// 	ScrollView->InvalidateData();
// }



void UUP_Guild_Join::SetScrollViewUI(URScrollView* ScrollView, TArray<FGUILD_INFO> dataList)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!IsValid(ScrollView))
		return;

	ScrollView->Init_NoClear(10);
	ScrollView->ClearScrollItemData();
	ScrollView->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_INFO>& outGuildList = dataList;
	TArray<int32>& waitingList = RGameInstance->GuildInfo->WaitingGuildIDs;

	int32 i = 0;
	for (FGUILD_INFO& FindGuildInfo : outGuildList)
	{
		UGuildList_ScrollItemData* ScrollItemData = CachedGuildList[i];
		if (ScrollItemData)
		{
			ScrollItemData->GuildInfo = FindGuildInfo;
			ScrollItemData->isWaiting = waitingList.Contains(FindGuildInfo.guild_id);
			ScrollItemData->isInfoPage = false;
			ScrollItemData->isMyGuild = RGameInstance->GuildInfo->CheckIsMyGuild(FindGuildInfo.guild_id);
			ScrollView->ItemDataList.Emplace(ScrollItemData);
		}
		i++;
	}
	ScrollView->InvalidateData();
}

//////////////////////////////////////////////////////////////////////////
// Ranking Tab

void UUP_Guild_Join::OnClick_GuildRankingPrev()
{
	if (Button_PrevRanking)
	{
		if (Button_PrevRanking->GetIsEnabled())
		{
			--RequestRankingPage;

			RequestGuildRanking();
		}
	}
}

void UUP_Guild_Join::OnClick_GuildRankingNext()
{
	if (Button_NextRanking)
	{
		if (Button_NextRanking->GetIsEnabled())
		{
			++RequestRankingPage;
			
			RequestGuildRanking();
		}
	}
}

void UUP_Guild_Join::RequestGuildRanking()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (Button_PrevRanking)
	{
		Button_PrevRanking->SetIsEnabled(false);
	}
	if (Button_NextRanking)
	{
		Button_NextRanking->SetIsEnabled(false);
	}

	if (0 >= RequestRankingPage)
		RequestRankingPage = 1;
	else if (10 < RequestRankingPage)
		RequestRankingPage = 10;

	UPacketFunctionLibrary::GUILD_CONTRIBUTE_RANKING_RQ(RequestRankingPage);
}

//////////////////////////////////////////////////////////////////////////
// Create Tab

void UUP_Guild_Join::OnClick_GuildCreate()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (Text_GuildName->GetText().IsEmpty())
	{
		UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Search")),
			ECommonPopupType::VE_OK);
		return;
	}
	
	if (RGameInstance->RInventory->GetGold() < GuildCreateGold)
	{
		UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error5")),
			ECommonPopupType::VE_OK);
		return;
	}

	UPacketFunctionLibrary::GUILD_CREATE_RQ(Text_GuildName->GetText().ToString(), Text_GuildDesc->GetText().ToString(), "", Create_AutoGradeUp);
}


void UUP_Guild_Join::OnClick_GuildCreateTabOpen()
{
	WidgetSwitcher_JoinType->SetActiveWidgetIndex(2);
}

void UUP_Guild_Join::CreateGuildSplash()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->GetTimerManager().ClearTimer(TimeHandler_CreateGuild);
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error6")));
}

//////////////////////////////////////////////////////////////////////////
// Delegate

void UUP_Guild_Join::OnRefreshGuildInfoUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (RGameInstance->GuildInfo->IsGuildExist())
	{
		UUP_GuildLobby* GuildLobby = Cast<UUP_GuildLobby>(ParentWidget);
		if (IsValid(GuildLobby))
			GuildLobby->Switcher->SetActiveWidgetIndex(1);
	}
	else
	{
		UPacketFunctionLibrary::GUILD_FIND_NAME_RQ(FString());
	}
}

void UUP_Guild_Join::OnGuildCreateRpDelegate()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	UUIFunctionLibrary::ShowCommonPopup(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error6")),ECommonPopupType::VE_OK);

	UPacketFunctionLibrary::GUILD_GET_RQ();

	RGameInstance->GetTimerManager().ClearTimer(TimeHandler_CreateGuild);
	RGameInstance->GetTimerManager().SetTimer(TimeHandler_CreateGuild, this, &UUP_Guild_Join::CreateGuildSplash, 1.0f, false);
}

void UUP_Guild_Join::OnGuildFindNameRpDelegate()
{
	if (GuildUIState == EGuildJoinTab::VE_Join)
		RefreshJoinUI();
	else 
		return;
}

void UUP_Guild_Join::OnGuildJoinRpDelegate(int32 GuildID, bool AutoGradeUp)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (AutoGradeUp)
	{
// 		UUIFunctionLibrary::ShowCommonPopup(
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error7")),
// 			ECommonPopupType::VE_OK);
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error7")));
		UPacketFunctionLibrary::GUILD_GET_RQ();
	}
	else
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRequestFinish")));
		RGameInstance->GuildInfo->AddWaitingGuildID(GuildID);
		RefreshGuildJoinUI();
	}
}

void UUP_Guild_Join::OnGuildAwaiterCancelRpDelegate(int32 guild_id)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRequestCancel")));
	RGameInstance->GuildInfo->RemoveWaitingGuildID(guild_id);

	RefreshGuildJoinUI();
}

void UUP_Guild_Join::OnGuildContributeRankingRpDelegate(bool ApplyData)
{
	if (Button_PrevRanking)
	{
		Button_PrevRanking->SetIsEnabled(true);
	}
	if (Button_NextRanking)
	{
		Button_NextRanking->SetIsEnabled(true);
	}

	if (ApplyData)
	{
		CurrentRankingPage = RequestRankingPage;
	}
	else
	{
		RequestRankingPage = CurrentRankingPage;
	}

	if (TextBlock_RankingPage)
	{
		TextBlock_RankingPage->SetText(FText::AsNumber(CurrentRankingPage));
	}

	if (GuildUIState == EGuildJoinTab::VE_Ranking)
	{
		RefreshRankingUI();
	}
}

void UUP_Guild_Join::RefreshJoinUI()
{
	WidgetSwitcher_JoinType->SetActiveWidgetIndex(0);
	SetScrollViewUI(ScrollView_Recommand_GuildList, RGAMEINSTANCE(this)->GuildInfo->FindGuildList);
}

void UUP_Guild_Join::RefreshRankingUI()
{
	WidgetSwitcher_JoinType->SetActiveWidgetIndex(1);
	SetScrollViewUI(ScrollView_Ranking_List, RGAMEINSTANCE(this)->GuildInfo->GuildRankingList);
}

//////////////////////////////////////////////////////////////////////////
