// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Colosseum_Result.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"





#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/PlayStateInfo.h"
#include "UI/Result/UC_ResultStatistics.h"


void UUP_Colosseum_Result::NativeConstruct()
{
	Super::NativeConstruct();

// 	if (BattleStatistics)
// 	{
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		
// 		{
// 			UArenaManager* arena_mgr = RGameInstance->ArenaManager;
// 			if (arena_mgr)
// 			{
// 				FColosseumTeamInfo* ColosseumMyTeamInfo = RGameInstance->ColosseumManager->GetColosseumTeamInfoByMyTeamToggle(true);
// 				if (ColosseumMyTeamInfo)
// 				{
// 					BattleStatistics->SetMyIDs(ColosseumMyTeamInfo->UserKID, ColosseumMyTeamInfo->GroupKey);
// 				}
// 
// 				FColosseumTeamInfo* ColosseumOtherTeamInfo = RGameInstance->ColosseumManager->GetColosseumTeamInfoByMyTeamToggle(false);
// 				if (ColosseumOtherTeamInfo)
// 				{
// 					BattleStatistics->SetEnemyID(ColosseumOtherTeamInfo->UserKID, ColosseumOtherTeamInfo->GroupKey);
// 				}
// 
// 				BattleStatistics->InitResultStatistics();
// 			}
// 		}
// 	}
}

void UUP_Colosseum_Result::SetTextClearTime(UTextBlock* minute, UTextBlock* second10, UTextBlock* second1)
{
// 	if (IsValid(minute) && IsValid(second10) && IsValid(second1))
// 	{
// 		auto RGameInstance = RGAMEINSTANCE(this);
// 		if ( IsValid(RGameInstance->PlayStateInfo))
// 		{
// 			float Time = RGameInstance->PlayStateInfo->ClearTime;
// 			if (Time <= 0.0f)
// 			{
// 				Time = 0.0f;
// 			}
// 
// 			int32 min = int32(Time / 60.0f);
// 			int32 sec = int32(Time) - (min * 60);
// 
// 			int32 sec10 = sec / 10;
// 			int32 sec1 = sec - (sec10 * 10);
// 
// 			FText info_text;
// 			info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(min));
// 			minute->SetText(info_text);
// 
// 			info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(sec10));
// 			second10->SetText(info_text);
// 
// 			info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(sec1));
// 			second1->SetText(info_text);
// 		}
// 	}
}

void UUP_Colosseum_Result::SetTextRank(UTextBlock* CurrentRank, UWidgetSwitcher* ChangeRankSwitcher, UTextBlock* UpRank, UTextBlock* DownRank)
{
// 	if (!IsValid(CurrentRank) || !IsValid(ChangeRankSwitcher) || !IsValid(UpRank) || !IsValid(DownRank))
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 
// 	UColosseumManager* ColosseumManager = RGameInstance->ColosseumManager;
// 	if (IsValid(ColosseumManager) == false)
// 		return;
// 
// 	int32 OriginalRank = ColosseumManager->MyColosseumInfo.rank;
// 	int32 ChangeRank = OriginalRank;
// 
// 	if (0 < ColosseumManager->ColosseumPlayResult.colosseumUserChanged.rank)
// 	{
// 		ChangeRank = ColosseumManager->ColosseumPlayResult.colosseumUserChanged.rank;
// 	}
// 
// 	CurrentRank->SetText(FText::FromString(FString::FromInt(ChangeRank)));
// 
// 	int32 DiffRank = 0;
// 	if (OriginalRank > ChangeRank)
// 	{
// 		DiffRank = OriginalRank - ChangeRank;
// 		ChangeRankSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		ChangeRankSwitcher->SetActiveWidgetIndex(0);
// 		UpRank->SetText(FText::FromString(FString::FromInt(DiffRank))); 
// 	}
// 	else if (OriginalRank < ChangeRank)
// 	{
// 		DiffRank = ChangeRank - OriginalRank;
// 		ChangeRankSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		ChangeRankSwitcher->SetActiveWidgetIndex(1);
// 		DownRank->SetText(FText::FromString(FString::FromInt(DiffRank)));
// 	}
// 	else
// 	{
// 		ChangeRankSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		ChangeRankSwitcher->SetActiveWidgetIndex(2);
// 	}
}

void UUP_Colosseum_Result::SetTextMatchRewards(UTextBlock* point, UCanvasPanel* BonusPanel, UTextBlock* BonusCount, UTextBlock* BonusPoint, UTextBlock* sp2_point)
{
// 	if (!IsValid(point) || !IsValid(BonusPanel) || !IsValid(BonusCount) || !IsValid(BonusPoint) || !IsValid(sp2_point))
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 
// 	UColosseumManager* ColosseumManager = RGameInstance->ColosseumManager;
// 	if (IsValid(ColosseumManager) == false)
// 		return;
// 
// 	int32 ColosseumPoint = 0;
// 	int32 ColosseumBonus = 0;
// 	int32 SP2Point = 0;
// 
// // 	FCOLOSSEUM_PLAY_RESULT& PlayResult = ColosseumManager->ColosseumPlayResult;
// // 	for (auto RewardData : PlayResult.rewards)
// // 	{
// // 		if (EREWARD_TYPE::COLOSSEUM_POINT == RewardData.type)
// // 		{
// // 			ColosseumPoint = RewardData.amt;
// // 			ColosseumBonus = ColosseumManager->MyColosseumInfo.contWin * COLOSSEUM_CONT_POINT;
// // 		}
// // 		else if (EREWARD_TYPE::SP2 == RewardData.type)
// // 		{
// // 			SP2Point = RewardData.amt;
// // 		}
// // 	}
// 
// 	point->SetText(FText::Format(FText::FromString("{0}"), FText::AsNumber(ColosseumPoint)));
// 	sp2_point->SetText(FText::Format(FText::FromString("{0}"), FText::AsNumber(SP2Point)));
// 
// 	if (0 < ColosseumPoint && 0 < ColosseumBonus)
// 	{
// 		BonusCount->SetText(FText::FromString(FString::FromInt(ColosseumManager->MyColosseumInfo.contWin)));
// 		BonusPoint->SetText(FText::FromString(FString::FromInt(ColosseumBonus)));
// 		BonusPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 	}
// 	else
// 	{
// 		BonusPanel->SetVisibility(ESlateVisibility::Collapsed);
// 	}
}

void UUP_Colosseum_Result::OnClickEndButton(bool GotoMainLobby)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->EventManager) && IsValid(RGameInstance->RWidgetManager))
// 	{
// 		if (false == GotoMainLobby)
// 		{
// 			UUIFunctionLibrary::AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Colosseum);
// 		}
// 
// 		RGameInstance->EventManager->ClearEvent();
// 		RGameInstance->RWidgetManager->ClearUIPage();
// 		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
// 	}
}

void UUP_Colosseum_Result::SetWinMarkSwitcher(UWidgetSwitcher* widget_switcher)
{
// 	if (IsValid(widget_switcher) == false)
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 
// 	if (IsValid(RGameInstance->ColosseumManager) == false)
// 		return;
// 
// 	if (RGameInstance->ColosseumManager->ColosseumPlayResult.isWin)
// 	{
// 		widget_switcher->SetActiveWidgetIndex(0);
// 	}
// 	else
// 	{
// 		widget_switcher->SetActiveWidgetIndex(1);
// 	}
}

void UUP_Colosseum_Result::GetMatchTargetHeroInfo(TArray<FEQUIPED_HERO>& EquipedHeroList)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 
// 	if (IsValid(RGameInstance->ColosseumManager) == false)
// 		return;
// 	if (IsValid(RGameInstance->OtherHeroManager) == false)
// 		return;
// 
// 	FColosseumTeamInfo* ColosseumTeamInfo = RGameInstance->ColosseumManager->GetColosseumTeamInfoByMyTeamToggle(false);
// 	if (ColosseumTeamInfo == nullptr)
// 		return;
// 
// 	TArray<FEQUIPED_HERO> outOtherHeroList;
// 	if (RGameInstance->OtherHeroManager->GetOtherHeroList(ColosseumTeamInfo->UserKID, outOtherHeroList))
// 	{
// 		for (FColosseumHero ColosseumHero : ColosseumTeamInfo->ColosseumHeroList)
// 		{
// 			for (FEQUIPED_HERO OtherHeroData : outOtherHeroList)
// 			{
// 				if (ColosseumHero.HeroUD.Equals(OtherHeroData.heroUD))
// 				{
// 					EquipedHeroList.Add(OtherHeroData);
// 					break;
// 				}
// 			}
// 		}
// 	}
}