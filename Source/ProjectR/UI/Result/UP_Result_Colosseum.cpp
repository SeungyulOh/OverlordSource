// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Result_Colosseum.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"

#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "ProjectRGameMode.h"
#include "GameInfo/UserInfo.h"
#include "GameState/PlayState.h"
#include "Network/PacketDataStructures.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/RWidgetManager.h"
#include "UI/Result/UC_Result_Hero.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UI/Common/UP_Popup_Avatar.h"

#include "UI/Control/RScrollView.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Result/UC_Result_HeroNew.h"
#include "UI/Result/UC_ResultStatistics.h"
#include "UI/Result/UC_ResultEffect.h"
#include "UI/Common/UC_Notify.h"
#include "UI/Common/ItemBaseScrollItemData.h"
#include "UtilFunctionIntegrated.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Table/CampaignTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "UI/Campaign/UP_BattleRepeatStatePopup.h"
#include "GameState/TowerLobbyState.h"
#include "UI/UIDefs.h"

#include "UI/Colosseum/UC_Colosseum_Class.h"
#include "UI/Colosseum/UC_Coloseum_RankUp_Splash.h"
#include "Global/CampaignManager.h"

void UUP_Result_Colosseum::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, ItemIcons, TEXT("UC_Item_Icons_C_"));
	if (Button_HeroManager)
	{
		Button_HeroManager->OnClicked.RemoveDynamic(this, &UUP_Result_Colosseum::ClickHeroManager);
		Button_HeroManager->OnClicked.AddDynamic(this, &UUP_Result_Colosseum::ClickHeroManager);
	}

	if (Button_TowerLobby)
	{
		Button_TowerLobby->OnClicked.RemoveDynamic(this, &UUP_Result_Colosseum::ClickTowerLobby);
		Button_TowerLobby->OnClicked.AddDynamic(this, &UUP_Result_Colosseum::ClickTowerLobby);
	}
	if (Button_ColosseumLobby)
	{
		Button_ColosseumLobby->OnClicked.RemoveDynamic(this, &UUP_Result_Colosseum::ClickColosseumLobby);
		Button_ColosseumLobby->OnClicked.AddDynamic(this, &UUP_Result_Colosseum::ClickColosseumLobby);
	}


	if (GameInstance->IsSuccess)
	{
		SetStageNameText();
	}
	else
	{
		if (GameInstance->IsTimeOut)
			SetStageResultTimeOut();
		else
			SetStageResultFail();
	}

	if (GameInstance->CampaignManager->bLevelUp)
	{
		GameInstance->CampaignManager->bLevelUp = false;
		UUP_Popup_Avatar* pAvatar = UUIFunctionLibrary::ShowAvatarLevelUpPopup(true);
		if (pAvatar)
		{
			pAvatar->VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_Result_Colosseum::ShowUnlockContentsPopup);
			pAvatar->VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_Result_Colosseum::ShowUnlockContentsPopup);
		}
	}
	else
	{
		ShowUnlockContentsPopup();
	}

	Init();
}

void UUP_Result_Colosseum::ShowUnlockContentsPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->ContentsUnLockManager->OnShowUnlockNotification();
}

void UUP_Result_Colosseum::NativeDestruct()
{
	if (Button_TowerLobby)
		Button_TowerLobby->OnClicked.RemoveAll(this);
	if (Button_ColosseumLobby)
		Button_ColosseumLobby->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_Result_Colosseum::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUP_Result_Colosseum::Init()
{
	CurrentStatus = RGAMEINSTANCE(this)->ColosseumManager->GetColosseumStatus();
	PrevStatus = RGAMEINSTANCE(this)->ColosseumManager->GetPrevColosseumStatus();

	SetInfoText();
	SetInfoSwitcher();
	SetProgressBar();
	SetRewards();
}

void UUP_Result_Colosseum::SetInfoText()
{
	currentGrade = (CurrentStatus.gradeId - 1) / 4;
	switch (currentGrade)
	{
		case 0:
		{
			GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Tier1")));
			break;
		}
		case 1:
		{
			GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Tier2")));
			break;
		}
		case 2:
		{
			GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Tier3")));
			break;
		}
		case 3:
		{
			GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Tier4")));
			break;
		}
		case 4:
		{
			GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Tier5")));
			break;
		}
	}

	UColosseumManager* CSManager = RGAMEINSTANCE(this)->ColosseumManager;

	if (IsValid(ColosseumTier))
		ColosseumTier->SetColosseumClassIndex(CurrentStatus.gradeId);
	if (IsValid(Text_LeagueName))
		Text_LeagueName->SetText(GradeText);
	if (IsValid(Text_Ranking))
		Text_Ranking->SetText(FText::AsNumber(CurrentStatus.rank));
	if (IsValid(Text_ColosseumPoint))
		Text_ColosseumPoint->SetText(FText::AsNumber(CurrentStatus.point));
	if (IsValid(Text_WinningRecord))
	{
		Text_WinningRecord->SetText(FText::AsNumber(CSManager->CS_SaveData.WinCount));
	}
	if (IsValid(Text_LosingRecord))
		Text_LosingRecord->SetText(FText::AsNumber(CSManager->CS_SaveData.LoseCount));
	if (IsValid(Text_LevelNew))
	{
		Text_LevelNew->SetText(FText::AsNumber(RGAMEINSTANCE(this)->UserInfo->Avatar.level));
	}
	if (CSManager->CS_SaveData.WinningStreakCount > 0)
	{
		FString continuousWinText = FString("(") + FString::FromInt(CSManager->CS_SaveData.WinningStreakCount) + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Colosseum_Lobby_Winningstreak")).ToString() + FString(")");
		RecordChangingSwitcher->SetVisibility(ESlateVisibility::Visible);
		RecordChangingSwitcher->SetActiveWidgetIndex(0);
		Text_WinningStraight->SetText(FText::FromString(continuousWinText));
		
	}
	else
	{
		RecordChangingSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Result_Colosseum::SetInfoSwitcher()
{
	if (IsValid(RankingChangingSwitcher))
	{
		FString rankText;
		if (CurrentStatus.rank < PrevStatus.rank)
		{
			RankingChangingSwitcher->SetActiveWidgetIndex(0);
			rankText = FString("(+") + FString::FromInt(PrevStatus.rank - CurrentStatus.rank) + FString(")");
			Text_RankingRaise->SetText(FText::FromString(rankText));
		}
		else if (CurrentStatus.rank == PrevStatus.rank)
		{
			RankingChangingSwitcher->SetActiveWidgetIndex(2);
		}
		else
		{
			RankingChangingSwitcher->SetActiveWidgetIndex(1);
			rankText = FString("(-") + FString::FromInt(CurrentStatus.rank - PrevStatus.rank) + FString(")");
			Text_RankingFall->SetText(FText::FromString(rankText));
		}
	}
	if (IsValid(ColosseumPointSwitcher))
	{
		FString pointText;
		if (CurrentStatus.point > PrevStatus.point)
		{
			ColosseumPointSwitcher->SetActiveWidgetIndex(0);
			pointText = FString("(+") + FString::FromInt(CurrentStatus.point - PrevStatus.point) + FString(")");
			Text_PointRaise->SetText(FText::FromString(pointText));
		}
		else if (CurrentStatus.point < PrevStatus.point)
		{
			ColosseumPointSwitcher->SetActiveWidgetIndex(1);
			pointText = FString("(-") + FString::FromInt(PrevStatus.point - CurrentStatus.point) + FString(")");
			Text_PointFall->SetText(FText::FromString(pointText));
		}
		else
		{
			ColosseumPointSwitcher->SetActiveWidgetIndex(2);
		}
	}
}

void UUP_Result_Colosseum::SetProgressBar()
{
	float percent = 0.f;
	auto GameInstance = RGAMEINSTANCE(this);
	percent = float(GameInstance->ColosseumManager->GetPrevAvatarExp()) / float(GameInstance->ColosseumManager->GetMaxAvatarExp());
	ProgressBar_LevelMin->SetPercent(percent);
	percent = float(GameInstance->UserInfo->Avatar.exp) / float(GameInstance->UserInfo->AvatarExpMax);
	ProgressBar_LevelMax->SetPercent(percent);
	if (IsValid(Text_LevelPercent))
		Text_LevelPercent->SetText(FText::AsNumber(int32(percent*100.f)));
}

void UUP_Result_Colosseum::SetRewards()
{
	auto GameInstance = RGAMEINSTANCE(this);
	TArray<FREWARD>	rewards = GameInstance->ColosseumManager->m_rewards;

	for (int32 i = 0; i < 3; i++)
	{
		if (i < rewards.Num())
		{
			FITEM_ICON_INFO iconInfo;
			UUIFunctionLibrary::GetRewardIconByRewardType(rewards[i], iconInfo);
			ItemIcons[2 - i]->SetItemIconStructure(iconInfo);
		}
		else
		{
			ItemIcons[2 - i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_Result_Colosseum::OpenRankingChanginSplash()
{
//	if (IsValid(RGAMEINSTANCE(this)))
	{
		int32 prevGrade = RGAMEINSTANCE(this)->ColosseumManager->currentGradeIndex;
		if (currentGrade > prevGrade)
		{
		}
		else if (currentGrade < prevGrade)
		{
		}
	}
}

void UUP_Result_Colosseum::SetStageNameText()
{
	UC_Result_Display->SetState(EResultEffectEnum::VE_Victory);
}

void UUP_Result_Colosseum::SetTimeText()
{

}

void UUP_Result_Colosseum::SetStageResultFail()
{
	UC_Result_Display->SetState(EResultEffectEnum::VE_Defeat);
	//Text_TimeInFail->SetText(FText());
}

void UUP_Result_Colosseum::SetStageResultTimeOut()
{
	UC_Result_Display->SetState(EResultEffectEnum::VE_Defeat);
	//Text_TimeInFail->SetText(FText());
}

void UUP_Result_Colosseum::ClickHeroManager()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_DetailHeros);

		RGameInstance->EventManager->ClearEvent();

		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_Result_Colosseum::ClickTowerLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_MainLobby);
		RGameInstance->EventManager->ClearEvent();

		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_Result_Colosseum::ClickColosseumLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Colosseum);
		RGameInstance->EventManager->ClearEvent();
// 		RGameInstance->ColosseumManager->openColosseumUI.RemoveDynamic(RGameInstance->ColosseumManager, &UColosseumManager::OpenColosseumLobby);
// 		RGameInstance->ColosseumManager->openColosseumUI.AddDynamic(RGameInstance->ColosseumManager, &UColosseumManager::OpenColosseumLobby);
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

