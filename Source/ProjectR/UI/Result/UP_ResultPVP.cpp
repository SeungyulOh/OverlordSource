// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ResultPVP.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"

#include "UI/Common/ItemBaseScrollItemData.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RewardInfo.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/PlayStateInfo.h"

#include "UtilFunctionIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Result/UC_ResultStatistics.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Result/UC_ResultEffect.h"

#include "Table/LevelGrowth.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "GameState/TowerLobbyState.h"

void UUP_ResultPVP::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_HeroManager))
	{
		Button_HeroManager->OnClicked.RemoveAll(this);
		Button_HeroManager->OnClicked.AddDynamic(this, &UUP_ResultPVP::GotoHeroManager);
	}

	if (IsValid(Button_GotoTowerOutSide))
	{
		Button_GotoTowerOutSide->OnClicked.RemoveAll(this);
		Button_GotoTowerOutSide->OnClicked.AddDynamic(this, &UUP_ResultPVP::GotoTowerOutSide);
	}
	if (IsValid(Button_GotoArenaLobby))
	{
		Button_GotoArenaLobby->OnClicked.RemoveAll(this);
		Button_GotoArenaLobby->OnClicked.AddDynamic(this, &UUP_ResultPVP::GotoArenaLobby);
	}
	if (IsValid(Button_GotoFastMatch))
	{
		Button_GotoFastMatch->OnClicked.RemoveAll(this);
		Button_GotoFastMatch->OnClicked.AddDynamic(this, &UUP_ResultPVP::GotoFastMatch);
	}

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, ItemIcon, TEXT("ItemIcon_0"));

	for (int32 idx = 0; idx < ItemIcon.Num(); idx++)
	{
		if (ItemIcon.IsValidIndex(idx))
			ItemIcon[idx]->SetVisibility(ESlateVisibility::Collapsed);
	}


	SetRankText();
	SetPointText();
	SetResultDisplay();
	SetRankImage();
	SetReward();
	SetTimeText();
	SetAvatar();
	SetRewardItem();
}

void UUP_ResultPVP::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_ResultPVP::GotoHeroManager()
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

void UUP_ResultPVP::GotoTowerOutSide()
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

void UUP_ResultPVP::GotoArenaLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Arena);
		RGameInstance->EventManager->ClearEvent();

		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}

	/*URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager = RGameInstance->ArenaManager;

	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaLobby);*/
}

void UUP_ResultPVP::GotoFastMatch()
{
	URGameInstance*		RGameInstance	=	RGAMEINSTANCE(this);
	UArenaManager*		ArenaManager	=	RGameInstance->ArenaManager;

	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	ArenaManager->RequestTargetInfo();
}

void UUP_ResultPVP::SetRankText()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	FARENA_STATUS Result =  ArenaManager->GetArenaInfoResult();
	FARENA_STATUS BeforeStatus = ArenaManager->m_MyArenaInfo;
	if (IsValid(Text_Ranking))
		Text_Ranking->SetText(FText::AsNumber(Result.rank));

	int32 Rank = Result.rank - BeforeStatus.rank;

	if (Rank == 0)
		WidgetSwitcher_Ranking->SetActiveWidgetIndex(2);
	else if (Rank > 0)
		WidgetSwitcher_Ranking->SetActiveWidgetIndex(1);
	else if (Rank < 0)
	{
		WidgetSwitcher_Ranking->SetActiveWidgetIndex(0);
		Rank = Rank * -1;
	}

	if (IsValid(Text_RankUp))
		Text_RankUp->SetText(FText::Format(FText::FromString(TEXT("(+{0})")), FText::AsNumber(Rank))); //FText::::AsNumber(Rank));
	if (IsValid(Text_RankDown))
		Text_RankDown->SetText(FText::Format(FText::FromString(TEXT("(-{0})")), FText::AsNumber(Rank)));

	if (ArenaManager->IsMyWin())
	{
		honor_ui->SetVisibility(ESlateVisibility::Visible);
		item_reward->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		honor_ui->SetVisibility(ESlateVisibility::Collapsed);
		item_reward->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_ResultPVP::SetPointText()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	FARENA_STATUS Result = ArenaManager->GetArenaInfoResult();
	FARENA_STATUS BeforeStatus = ArenaManager->m_MyArenaInfo;
	if (IsValid(Text_Point))
		Text_Point->SetText(FText::AsNumber(Result.point));

	int32 Point = Result.point - BeforeStatus.point;

	if (Point == 0)
		WidgetSwitcher_Point->SetActiveWidgetIndex(2);
	else if (Point > 0)
		WidgetSwitcher_Point->SetActiveWidgetIndex(0);
	else if (Point < 0)
	{
		WidgetSwitcher_Point->SetActiveWidgetIndex(1);
		Point = Point * -1;
	}

	if (IsValid(Text_PointUp))
		Text_PointUp->SetText(FText::Format(FText::FromString(TEXT("(+{0})")), FText::AsNumber(Point))); //FText::::AsNumber(Rank));
	if (IsValid(Text_PointDown))
		Text_PointDown->SetText(FText::Format(FText::FromString(TEXT("(-{0})")), FText::AsNumber(Point)));

	ArenaManager->m_MyArenaInfo = Result;
}

void UUP_ResultPVP::SetResultDisplay()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;

	if (IsValid(UC_Result_Display))
	{
		if(ArenaManager->IsMyWin())
			UC_Result_Display->PlayAni("Victory");
		else
			UC_Result_Display->PlayAni("Defeated");
	}
}

void UUP_ResultPVP::SetRankImage()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	FARENA_STATUS Result = ArenaManager->GetArenaInfoResult();
	FName	ImageKey;

	switch (Result.rankPosition-1)
	{
		case	0:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_001"));
			break;
		}
		case	1:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_002"));
			break;
		}
		case	2:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_003"));
			break;
		}
		case	3:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_004"));
			break;
		}
		case	4:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_005"));
			break;
		}
		case	5:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_006"));
			break;
		}
		case	6:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_007"));
			break;
		}
		case	7:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_008"));
			break;
		}
		case	8:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_009"));
			break;
		}
		case	9:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_010"));
			break;
		}
		case	10:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_10percent"));
			break;
		}
		case	11:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_20percent"));
			break;
		}
		case	12:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_30percent"));
			break;
		}
		case	13:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_40percent"));
			break;
		}
		case	14:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_50percent"));
			break;
		}
		case	15:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_60percent"));
			break;
		}
		case	16:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_70percent"));
			break;
		}
		case	17:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_80percent"));
			break;
		}
		case	18:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_90percent"));
			break;
		}
		case	19:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_100percent"));
			break;
		}
	}

	Image_Rank->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ImageKey));
}

void UUP_ResultPVP::SetReward()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	FCurrencyPropertyTableInfo* property_table = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(WALLET_PVPTOKEN);
	if (property_table)
	{
		UC_MaterialIcon->Refresh(property_table->ItemIconKey, ArenaManager->GetArenaInfoPVPToken(), false, 0);
	}
}

void UUP_ResultPVP::SetTimeText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	float	ClearTime = GameInstance->PlayStateInfo->GetClearTime();

	if (IsValid(Text_ResultTime))
	{
		int32 nMinute = ClearTime / 60;
		int32 nSecond = ClearTime - (nMinute * 60);

		FString Minute_Str = FString::FromInt(nMinute * 0.1f);
		Minute_Str += FString::FromInt(nMinute % 10);

		FString Second_Str = FString::FromInt(nSecond * 0.1f);
		Second_Str += FString::FromInt(nSecond % 10);

		Minute_Str += FString(":");
		Minute_Str += Second_Str;

		Text_ResultTime->SetText(FText::FromString(Minute_Str));
	}
}

void UUP_ResultPVP::SetAvatar()
{
	URGameInstance*	GameInstance	=	RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager	=	GameInstance->ArenaManager;
	FAVATAR&		PrevAvatar		=	GameInstance->UserInfo->GetAvatarData();

	int32			AvatarExpMax	=	0;
	int32			PrevLevel		=	PrevAvatar.level;
	int32			ResultLevel		=	ArenaManager->GetResultAvatar().level;
	//float			PrevPercent		=	0.0f;
	//float			NextPercent		=	0.0f;

	FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(PrevLevel)));
	if (tableInfo)
	{
		AvatarExpMax = tableInfo->AvatarMaxExp;
	}

	if (PrevLevel <= ResultLevel) //todo : check..
	{
		float PrevPercent = float(PrevAvatar.exp) / float(AvatarExpMax);
		if (ProgressBar_LevelMin)
		{
			ProgressBar_LevelMin->SetPercent(PrevPercent);
		}
		
		if (ProgressBar_LevelMax)
		{
			float NextPercent = float(ArenaManager->GetResultAvatar().exp) / float(AvatarExpMax);
			ProgressBar_LevelMax->SetPercent(NextPercent);
		}
		
		if (IsValid(Text_LevelPercent))
		{
			float rounded = ((int)(PrevPercent * 100 + .5) / 100.0);
			float ExpPercent = float(rounded * 100);
			Text_LevelPercent->SetText(FText::AsNumber(ExpPercent));
		}
	}
	if (IsValid(Text_Level))
	{
		Text_Level->SetText(FText::AsNumber(ResultLevel));
	}

}

void UUP_ResultPVP::SetRewardItem()
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	TArray<FREWARD> Rewards = ArenaManager->GetResultReward();

	int32	Idx = 0;
	for (FREWARD& reward : Rewards)
	{
		if (ItemIcon.IsValidIndex(Idx) == false)
			continue;
		if (reward.rewardId.Compare(WALLET_PVPTOKEN) == 0)
			continue;

		FITEM_ICON_INFO itemiconinfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(reward, itemiconinfo);

		ItemIcon[Idx]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemIcon[Idx]->SetItemIconStructure(itemiconinfo);
		Idx++;
	}
}
