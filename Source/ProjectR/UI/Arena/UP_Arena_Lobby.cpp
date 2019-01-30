// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Arena_Lobby.h"

#include "Network/HttpRequestClient.h"

#include "UtilFunctionIntegrated.h"
#include "SharedConstants/GlobalConstants.h"

#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"
#include "Global/ContentsUnLockManager.h"

#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Common/HeroBaseScrollItem.h"

#include "UI/Arena/RArenaRanking_ScrollItemData.h"
#include "UI/Arena/ArenaSeasonReward_ScrollItemData.h"
#include "UI/Arena/UC_Arena_battle_record.h"
#include "UI/Arena/UC_Arena_ranking_list.h"
#include "UI/Arena/UC_Arena_reward_popup.h"
#include "UI/Arena/UC_Arena_charge_popup.h"

#include "UI/UI_Icon/HeroIconWithButton.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

void UUP_Arena_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	RGAMEINSTANCE(this)->ArenaManager->onACK_ARENA_STATUS.Unbind();
	RGAMEINSTANCE(this)->ArenaManager->onACK_ARENA_STATUS.BindUObject(this, &UUP_Arena_Lobby::Init);

	RGAMEINSTANCE(this)->ArenaManager->RequestStatus();
	RGAMEINSTANCE(this)->ArenaManager->RequestRankigList();

	if (IsValid(Button_DefendingTeam))
	{
		Button_DefendingTeam->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickDefendingTeam);
		Button_DefendingTeam->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickDefendingTeam);
	}

}

void UUP_Arena_Lobby::NativeDestruct()
{
	Reset();

	Super::NativeDestruct();
}

void UUP_Arena_Lobby::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUP_Arena_Lobby::Init()
{
	PlayAni(TEXT("Arena_In"));

	USoundBase* LobbySound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Arena_Lobby_Cue.UP_Arena_Lobby_Cue'")));
	if (IsValid(LobbySound))
	{
		USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), LobbySound);
		//UGameplayStatics::PlaySound2D(GetWorld(), LobbySound);
	}

	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	GameInstance->ArenaManager->bMirrorDeckSetting = false;
	if (IsValid(Button_RewardInfo))
	{
		Button_RewardInfo->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickRewardInfoPopUp);
		Button_RewardInfo->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickRewardInfoPopUp);
	}
	
	if (IsValid(Button_BattlePreparation))
	{
		Button_BattlePreparation->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickBattlePreparation);
		Button_BattlePreparation->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickBattlePreparation);
	}

	if (IsValid(Radio_Info))
	{
		Radio_Info->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Arena_Lobby::OnClickInformation);
		Radio_Info->OnCheckRadioCheck.AddDynamic(this, &UUP_Arena_Lobby::OnClickInformation);
	}
	if (IsValid(Radio_HistoryRecord))
	{
		Radio_HistoryRecord->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Arena_Lobby::OnClickHistory);
		Radio_HistoryRecord->OnCheckRadioCheck.AddDynamic(this, &UUP_Arena_Lobby::OnClickHistory);
	}
	if (IsValid(Radio_Ranking))
	{
		Radio_Ranking->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Arena_Lobby::OnClickRanking);
		Radio_Ranking->OnCheckRadioCheck.AddDynamic(this, &UUP_Arena_Lobby::OnClickRanking);
	}
	if (IsValid(Button_Reward))
	{
		Button_Reward->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickSeasonReward);
		Button_Reward->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickSeasonReward);
	}
	if (IsValid(Button_ArenaCountCharge))
	{
		Button_ArenaCountCharge->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickDailyBonusAdd);
		Button_ArenaCountCharge->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickDailyBonusAdd);
	}
	if (IsValid(Button_DefenceReward))
	{
		Button_DefenceReward->OnClicked.RemoveDynamic(this, &UUP_Arena_Lobby::Button_ClickDefenceReward);
		Button_DefenceReward->OnClicked.AddDynamic(this, &UUP_Arena_Lobby::Button_ClickDefenceReward);
	}

	if (IsValid(GameInstance->HttpClient))
	{
		GameInstance->HttpClient->OnContentResetRpDelegate.Clear();
		GameInstance->HttpClient->OnContentResetStatusRpDelegate.Clear();
		GameInstance->HttpClient->OnContentResetRpDelegate.AddDynamic(this, &UUP_Arena_Lobby::CountResetRp);
		GameInstance->HttpClient->OnContentResetStatusRpDelegate.AddDynamic(this, &UUP_Arena_Lobby::CountResetStatusRp);
	}

	LoadSeasonRewardListData();

	SetArenaUserInfo();
	SetTextArenaHistoryAvatar();

	if(InfoRadioSwitcher)
		TabButtonRadio.Emplace(InfoRadioSwitcher);
	if (RankingRadioSwitcher)
		TabButtonRadio.Emplace(RankingRadioSwitcher);
	if (HistoryRadioSwitcher)
		TabButtonRadio.Emplace(HistoryRadioSwitcher);
}

void UUP_Arena_Lobby::Reset()
{

}

void UUP_Arena_Lobby::Button_ClickRewardInfoPopUp()
{
	if(UC_Arena_reward_popup)
		UC_Arena_reward_popup->SetVisibility(ESlateVisibility::Visible);
}

void UUP_Arena_Lobby::Button_ClickDefendingTeam()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	UArenaManager* ArenaManager = GameInstance->ArenaManager;
	URWidgetManager* RWidgetManager = GameInstance->RWidgetManager;

	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(RWidgetManager != nullptr, TEXT("[%s]WidgetManager is Empty"), __FUNCTION__);

	ArenaManager->SetDefenceSetting(true);
	RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaDeckSetting);
}

void UUP_Arena_Lobby::Button_ClickBattlePreparation()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager	=	GameInstance->ArenaManager;

	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	ArenaManager->RequestTargetInfo();
}

void UUP_Arena_Lobby::Button_ClickSeasonReward()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	UArenaManager* ArenaManager = GameInstance->ArenaManager;
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	ArenaManager->RequestSeasonDone();
}

void UUP_Arena_Lobby::SetArenaUserInfo()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager = GameInstance->ArenaManager;
	URInventory*		RInventory = GameInstance->RInventory;

	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(RInventory != nullptr, TEXT("[%s]RInventory is Empty"), __FUNCTION__);

	FARENA_STATUS& MyArenaInfo = ArenaManager->GetArenaInfo();

	if (IsValid(UC_Arena_Tier))
	{
		UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(UC_Arena_Tier->GetWidgetFromName(TEXT("WidgetSwitcher_0")));

		if (IsValid(Switcher))
		{
			if (0 < MyArenaInfo.rankPosition)
				Switcher->SetActiveWidgetIndex(MyArenaInfo.rankPosition - 1);
			else
				Switcher->SetActiveWidgetIndex(20);
		}
	}

	if (IsValid(Text_Rank))
		Text_Rank->SetText(FText::AsNumber(MyArenaInfo.rank));
	if (IsValid(Text_MyPoint))
		Text_MyPoint->SetText(FText::AsNumber(MyArenaInfo.point));

	bool bProcessSeasonEnd = !(MyArenaInfo.seasonEnd);
	if (IsValid(Button_RewardInfo))
		Button_RewardInfo->SetIsEnabled(bProcessSeasonEnd);
	if (IsValid(Button_DefendingTeam))
		Button_DefendingTeam->SetIsEnabled(bProcessSeasonEnd);
	if (IsValid(Button_BattlePreparation))
		Button_BattlePreparation->SetIsEnabled(bProcessSeasonEnd);
	if (IsValid(Button_ArenaCountCharge))
		Button_ArenaCountCharge->SetIsEnabled(bProcessSeasonEnd);
	if (IsValid(Button_DefenceReward))
		Button_DefenceReward->SetIsEnabled(bProcessSeasonEnd);

	if (MyArenaInfo.seasonEnd)
	{
		WidgetSwitcher_Season->SetActiveWidgetIndex(1);
	}
	else
	{
		WidgetSwitcher_Season->SetActiveWidgetIndex(0);

		int32 nRemainTime = MyArenaInfo.seasonLeft;

		int32 Day = 0;
		int32 Hour = 0;

		if (0 < nRemainTime)
		{
			Day = (int32)(nRemainTime / 86400);
			Hour = (int32)(nRemainTime / 3600) % 24;
		}
		
		if (IsValid(TextBlock_Day))
		{
			FText day_text = UUIFunctionLibrary::Get_0n_Number(Day);
			TextBlock_Day->SetText(day_text);
		}
		if (IsValid(TextBlock_Hour))
		{
			FText hour_text = UUIFunctionLibrary::Get_0n_Number(Hour);
			TextBlock_Hour->SetText(hour_text);
		}

		if (IsValid(TextBlock_SeasonEnd))
		{
			FText txt = TextBlock_SeasonEnd->GetText();
			txt = FText::Format(txt, Day, Hour);
			TextBlock_SeasonEnd->SetText(txt);
		}

		if (IsValid(ProgressBar_TimeRemaining))
		{
			float Max = (60 * 60 * 24 * 14); //14day
			float percent = (Max - MyArenaInfo.seasonLeft) / Max;
			//float percent = (float)MyArenaInfo.seasonLeft / Max;
			
			ProgressBar_TimeRemaining->SetPercent(percent);
		}
	}
	 
	if (IsValid(TextBlock_SeasonCount))
		TextBlock_SeasonCount->SetText(FText::AsNumber(RInventory->GetArenaDailyBonusToken()));
	if (IsValid(Text_DefenceCount))
		Text_DefenceCount->SetText(FText::Format(FText::FromString("{0}/{1}"), MyArenaInfo.defenseCount, MyArenaInfo.defenseCountMax));

	if (MyArenaInfo.defenseRewardEnabled)
		Button_DefenceReward->SetIsEnabled(true);
	else
		Button_DefenceReward->SetIsEnabled(false);

	if (IsValid(Text_DefenceTime) && IsValid(Text_NextDefenceReward))
	{
		if (MyArenaInfo.defenseCountMax < 100)
		{
			Text_DefenceTime->SetVisibility(ESlateVisibility::Collapsed);
			Text_NextDefenceReward->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			int32 nHour = MyArenaInfo.seasonLeft / 3600;
			int32 nMin = (MyArenaInfo.seasonLeft % 3600) / 60;
			FText HourText = UUIFunctionLibrary::Get_0n_Number(nHour);
			FText MinText = UUIFunctionLibrary::Get_0n_Number(nMin);
			FText defenceText = FText::FromString(TEXT("{0}{1} {2}{3}"));
			Text_DefenceTime->SetText(FText::Format(defenceText, HourText, MinText, 
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonExit_Hour")), 
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Min"))));
			Text_DefenceTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_NextDefenceReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}


void UUP_Arena_Lobby::LoadSeasonRewardListData()
{
	if (!IsValid(UC_Arena_reward_popup))
		return;

	const int32 Max = 10;
	TArray<FName> RankerRewards;
	RankerRewards.Reserve(Max);

	TArray<FName> AllUserRewards;
	AllUserRewards.Reserve(Max);

	FName reward_key;
	for (int32 i = 0; i < Max; ++i)
	{
		reward_key = *(FString::Printf(TEXT("RAR-RANK_%d"), i + 1));
		RankerRewards.Emplace(reward_key);

		reward_key = *(FString::Printf(TEXT("RAR-RANK_PERCENT_%d"), Max * (i + 1)));
		AllUserRewards.Emplace(reward_key);
	}

	UC_Arena_reward_popup->SetArenaRankInfo(RankerRewards, AllUserRewards);
}

void UUP_Arena_Lobby::SetTextArenaHistoryAvatar()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager = GameInstance->ArenaManager;
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	URScrollView* ScrollView = Cast<URScrollView>(GetWidgetFromName(TEXT("BP_ScrollView_C_0")));
	if (!IsValid(ScrollView))
		return;

	ScrollView->Init();
	ScrollView->ClearScrollItemData();

	URScrollItemData*					scroll_item_data = nullptr;
	UUC_Arena_Battle_Record_Data*		reward_data = nullptr;
	 
	TArray<FARENA_HISTORY>& history = ArenaManager->GetArenaInfo().history;
	int32 history_count = history.Num();
	for (int32 n = 0; n < history_count; ++n)
	{
		scroll_item_data	=	ScrollView->AddScrollItemData(UUC_Arena_Battle_Record_Data::StaticClass());
		reward_data			=	Cast<UUC_Arena_Battle_Record_Data>(scroll_item_data);

		reward_data->SetHistory(history[n].isDefense, history[n].isWin, history[n].deltaPoint, history[n].level, 
			FText::FromString(history[n].nick), 
			FText::FromString(history[n].guild), 
			history[n].power, history[n].heroIDs);
	}

	ScrollView->InvalidateData();
}

void UUP_Arena_Lobby::OnClickInformation(bool isCheck)
{
	if (isCheck == true)
	{
		for (TWeakObjectPtr<UWidgetSwitcher>& switcher : TabButtonRadio)
		{
			UWidgetSwitcher* pSwitcher = switcher.Get();
			if (pSwitcher)
				pSwitcher->SetActiveWidgetIndex(1);
		}
		InfoRadioSwitcher->SetActiveWidgetIndex(0);

		GameInformation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_List->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Arena_Lobby::OnClickHistory(bool isCheck)
{
	if (isCheck == true)
	{
		for (TWeakObjectPtr<UWidgetSwitcher>& switcher : TabButtonRadio)
		{
			UWidgetSwitcher* pSwitcher = switcher.Get();
			if (pSwitcher)
				pSwitcher->SetActiveWidgetIndex(1);
		}
		HistoryRadioSwitcher->SetActiveWidgetIndex(0);

		GameInformation->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(WidgetSwitcher_List))
		{
			WidgetSwitcher_List->SetActiveWidgetIndex(0);
			WidgetSwitcher_List->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UUP_Arena_Lobby::OnClickRanking(bool isCheck)
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager = GameInstance->ArenaManager;
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	if (isCheck == true)
	{
		for (TWeakObjectPtr<UWidgetSwitcher>& switcher : TabButtonRadio)
		{
			UWidgetSwitcher* pSwitcher = switcher.Get();
			if (pSwitcher)
				pSwitcher->SetActiveWidgetIndex(1);
		}
		RankingRadioSwitcher->SetActiveWidgetIndex(0);

		GameInformation->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(WidgetSwitcher_List))
		{
			WidgetSwitcher_List->SetActiveWidgetIndex(1);
			WidgetSwitcher_List->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	UC_Arena_ranking_list->SetRank(ArenaManager->m_RankList);
}

void UUP_Arena_Lobby::Button_ClickDailyBonusAdd()
{
	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("ArenaDailyBonus"), TEXT(""));
}

void UUP_Arena_Lobby::Button_ClickDefenceReward()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager = GameInstance->ArenaManager;

	if (ArenaManager->IsDefenceReward())
		ArenaManager->RequestDefenceReward();
}

void UUP_Arena_Lobby::ShowRejectRequestPopupDelay()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->GetTimerManager().ClearTimer(DelayTimer);

	UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_ItemManage_OptionValueShortage"))),
		ECommonPopupType::VE_OK);
}

void UUP_Arena_Lobby::CountResetRp(bool bResult)
{
	Init();
}

void UUP_Arena_Lobby::CountResetStatusRp(int32 nResetCount)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URInventory*	RInventory = RGameInstance->RInventory;

	auto TableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow(FName(TEXT("ArenaDailyBonus")));
	if (TableInfo == nullptr)
		return;

	if (RInventory->GetArenaDailyBonusToken() == 0 && TableInfo->resetLimitCountPerDay <= nResetCount)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000061")),
			ECommonPopupType::VE_OK);
		return;
	}

	int32 nNeedGemCount = TableInfo->currencyValue * (nResetCount + 1);
	int32 nHaveGemCount = RInventory->GetGem();
	if (nNeedGemCount <= nHaveGemCount)
	{
		if (RInventory->GetArenaDailyBonusToken() > 0)
		{
			UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000062")),
				ECommonPopupType::VE_OK);
		}
		else
		{
			if (IsValid(UC_Arena_charge_popup))
			{
				UC_Arena_charge_popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UC_Arena_charge_popup->SetText(nNeedGemCount, RInventory->GetArenaDailyBonusToken(), true);
			}
		}
	}
	else
		RGameInstance->GetTimerManager().SetTimer(DelayTimer, this, &UUP_Arena_Lobby::ShowRejectRequestPopupDelay, 0.1f);
}
