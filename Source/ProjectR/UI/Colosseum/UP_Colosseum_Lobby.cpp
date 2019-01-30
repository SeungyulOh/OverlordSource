// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Colosseum_Lobby.h"

#include "UI/Colosseum/UC_Colosseum_BattleRecord.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "Network/PacketFunctionLibrary.h"

#include "GlobalIntegrated.h"


void UUP_Colosseum_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Match->OnClicked.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnMatch);
	Button_RewardInfo->OnClicked.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnRewardInfo);
	Button_SeasonReward->OnClicked.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnSeasonReward);
	Radio_ColloseumInfo->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckColloseumInfo);
	Radio_BattleRecord->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckBattleRecord);
	Radio_Ranking->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckRanking);

	Button_Match->OnClicked.AddDynamic(this, &UUP_Colosseum_Lobby::OnMatch);
	Button_RewardInfo->OnClicked.AddDynamic(this, &UUP_Colosseum_Lobby::OnRewardInfo);
	Button_SeasonReward->OnClicked.AddDynamic(this, &UUP_Colosseum_Lobby::OnSeasonReward);
	Radio_ColloseumInfo->OnCheckRadioCheck.AddDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckColloseumInfo);
	Radio_BattleRecord->OnCheckRadioCheck.AddDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckBattleRecord);
	Radio_Ranking->OnCheckRadioCheck.AddDynamic(this, &UUP_Colosseum_Lobby::OnRadioCheckRanking);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->ColosseumManager->ColosseumUI.RemoveDynamic(this, &UUP_Colosseum_Lobby::Init);
	RGameInstance->ColosseumManager->ColosseumUI.AddDynamic(this, &UUP_Colosseum_Lobby::Init);
	RGameInstance->ColosseumManager->RequestColosseumInfo();

	if (Scrollview)
	{
		if (!Scrollview->ScrollItemList.Num())
			Scrollview->Init_NoClear(MAXSCROLLITEM);

		if (!CachedItemDataList.Num())
		{
			CachedItemDataList.Reset(MAXSCROLLITEMDATA);
			for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
			{
				URColosseum_BattleRecord_Data* ItemData = Cast<URColosseum_BattleRecord_Data>(Scrollview->CreateScrollItemData(URColosseum_BattleRecord_Data::StaticClass()));
				ItemData->WidgetSize = Scrollview->GetItemInitSize();
				CachedItemDataList.Emplace(ItemData);
			}
		}
	}

// 	if (RGameInstance->ForceTutorialManager)
// 		RGameInstance->ForceTutorialManager->CheckUnlockTutorial(EFirstCheckContentsType::FirstColosseum);
}

void UUP_Colosseum_Lobby::NativeDestruct()
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//RGameInstance->ColosseumManager->ColosseumUI.RemoveDynamic(this, &UUP_Colosseum_Lobby::Init);

	Super::NativeDestruct();
}

void UUP_Colosseum_Lobby::Init()
{
	ColosseumStatus = RGAMEINSTANCE(GEngine)->ColosseumManager->GetColosseumStatus();
	SetColosseumInfo();
	SetColosseumSeason();
	SetSeasonTime();
}

void UUP_Colosseum_Lobby::OnMatch()
{
	if (ColosseumStatus.seasonEnd)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000020")), ECommonPopupType::VE_OK);
	}
	else
	{
		UUIFunctionLibrary::ShowMatchingWaiting(EPVP::CS);
		RGAMEINSTANCE(GEngine)->RealTimeModManager->EnterColosseum(FName(TEXT("0")));
	}
	
}

void UUP_Colosseum_Lobby::OnRewardInfo()
{
	if (IsValid(RewardInfoPopup))
		RewardInfoPopup->SetVisibility(ESlateVisibility::Visible);
}

void UUP_Colosseum_Lobby::OnSeasonReward()
{
	if (Button_SeasonReward)
	{
		Button_SeasonReward->SetIsEnabled(false);
	}
	UPacketFunctionLibrary::COLOSSEUM_SEASON_DONE_RQ();
}

void UUP_Colosseum_Lobby::OnRadioCheckColloseumInfo(bool InIsCheck)
{
	if (IsValid(WidgetSwitcher_ColloseumInfo))
		WidgetSwitcher_ColloseumInfo->SetActiveWidgetIndex(int32(ECollseumTap::VE_Info));
}

void UUP_Colosseum_Lobby::OnRadioCheckBattleRecord(bool InIsCheck)
{
	if (IsValid(WidgetSwitcher_ColloseumInfo))
		WidgetSwitcher_ColloseumInfo->SetActiveWidgetIndex(int32(ECollseumTap::VE_BattleRecord));

	if (IsValid(Scrollview) == false)
		return;

	Scrollview->ClearScrollItemData();
	URColosseum_BattleRecord_Data*		reward_data			=	nullptr;
	int32 history_count = ColosseumStatus.history.Num();
	int32 Max = (MAXSCROLLITEMDATA > ColosseumStatus.history.Num() ? ColosseumStatus.history.Num() : MAXSCROLLITEMDATA);
	for (int32 i = 0; i < Max; ++i)
	{
		if (CachedItemDataList.IsValidIndex(i))
		{
			reward_data = CachedItemDataList[i];
			reward_data->SetHistory(ColosseumStatus.history[i].isWin, 
				ColosseumStatus.history[i].gradeId, 
				ColosseumStatus.history[i].deltaPoint, 
				ColosseumStatus.history[i].level, 
				FText::FromString(ColosseumStatus.history[i].nick), 
				FText::FromString(ColosseumStatus.history[i].guild), 
				ColosseumStatus.history[i].power, 
				ColosseumStatus.history[i].heroIDs);
			Scrollview->ItemDataList.Emplace(reward_data); //change refresh
		}
	}

	Scrollview->InvalidateData();
}

void UUP_Colosseum_Lobby::OnRadioCheckRanking(bool InIsCheck)
{
	if (IsValid(WidgetSwitcher_ColloseumInfo))
		WidgetSwitcher_ColloseumInfo->SetActiveWidgetIndex(int32(ECollseumTap::VE_Ranking));
	TArray<FCOLOSSEUM_RANK_USER>	ColosseumRanking = RGAMEINSTANCE(GEngine)->ColosseumManager->GetColoseumRanking();
	
	
	if(IsValid(RankingList))
		RankingList->SetRank(ColosseumRanking);
}

void UUP_Colosseum_Lobby::SetColosseumInfo()
{
	int32		ActiveIndex = (ColosseumStatus.gradeId - 1) / 4;
	if (IsValid(Text_UITIer))
	{
		FName LocalizedKey;
		switch (ActiveIndex)
		{
			case 0: LocalizedKey = FName(TEXT("UI_Colosseum_Tier1")); break;
			case 1:	LocalizedKey = FName(TEXT("UI_Colosseum_Tier2")); break;
			case 2: LocalizedKey = FName(TEXT("UI_Colosseum_Tier3")); break;
			case 3: LocalizedKey = FName(TEXT("UI_Colosseum_Tier4")); break;
			case 4: LocalizedKey = FName(TEXT("UI_Colosseum_Tier5")); break;
		}

		FText GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, LocalizedKey);
		Text_UITIer->SetText(GradeText);
	}


	if (IsValid(ColosseumTier))
	{
		ColosseumTier->SetColosseumClassIndex(ColosseumStatus.gradeId);
	}
	if (IsValid(Text_Ranking))
	{
		Text_Ranking->SetText(FText::AsNumber(ColosseumStatus.rank));
	}
	if (IsValid(Text_Point))
	{
		Text_Point->SetText(FText::AsNumber(ColosseumStatus.point));
	}
																											
	FStaminaUseTableInfo*	StaminaUseTable = UTableManager::GetInstancePtr()->GetStaminaUseTableRow(FName("COLLOSSEUM"));
	if (StaminaUseTable)
	{
		if (IsValid(TextBlock_UseStamia))
			TextBlock_UseStamia->SetText(FText::AsNumber(StaminaUseTable->StaminaPVP));
	}


	RGAMEINSTANCE(this)->ColosseumManager->currentGradeIndex = ActiveIndex;
}

void UUP_Colosseum_Lobby::SetColosseumSeason()
{
	UColosseumManager* CSManager = RGAMEINSTANCE(this)->ColosseumManager;
	if (!IsValid(CSManager))
		return;


	if (IsValid(Text_WiningStreak))
		Text_WiningStreak->SetText(FText::AsNumber(CSManager->CS_SaveData.WinningStreakCount));
	if (IsValid(Text_Win))
		Text_Win->SetText(FText::AsNumber(CSManager->CS_SaveData.WinCount));
	if (IsValid(Text_Lose))
		Text_Lose->SetText(FText::AsNumber(CSManager->CS_SaveData.LoseCount));
}

void UUP_Colosseum_Lobby::SetSeasonTime()
{
	if (ColosseumStatus.seasonEnd)
	{
		WidgetSwitcher_SeasonState->SetActiveWidgetIndex(0);

		if (IsValid(Button_SeasonReward))
			Button_SeasonReward->SetIsEnabled(true);
	}
	else
	{
		WidgetSwitcher_SeasonState->SetActiveWidgetIndex(1);

		int32 nRemainTime = ColosseumStatus.seasonLeft;

		int32 Day = 0;
		int32 Hour = 0;

		if (0 < nRemainTime)
		{
			Day = (int32)(nRemainTime / 86400);
			Hour = (int32)(nRemainTime / 3600) % 24;
		}

		FText day_text = UUIFunctionLibrary::Get_0n_Number(Day);
		FText hour_text = UUIFunctionLibrary::Get_0n_Number(Hour);

		if (IsValid(Button_SeasonReward))
			Button_SeasonReward->SetIsEnabled(false);
		
		if (IsValid(Text_SeasonRemainDay))
		{
			Text_SeasonRemainDay->SetText(day_text);
		}
		if (IsValid(Text_SeasonRemainHour))
		{
			Text_SeasonRemainHour->SetText(hour_text);
		}

		if (IsValid(Text_SeasonRemain))
		{
			FText txt = Text_SeasonRemain->GetText();
			txt = FText::Format(txt, Day, Hour);
			Text_SeasonRemain->SetText(txt);
		}

		if (IsValid(ProgressBar_Season))
		{
			float Max = (60 * 60 * 24 * 7); //7day
			float percent = (Max - ColosseumStatus.seasonLeft) / Max;

			ProgressBar_Season->SetPercent(percent);
		}
	}
}
