// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Result_ForRTS.h"

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

#include "UI/Campaign/UP_BattleRepeatStatePopup.h"
#include "GameState/TowerLobbyState.h"


void UUP_Result_ForRTS::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	UUIFunctionLibrary::AttachHUDPage(GetWorld(), false);
	UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, Reward_Icons, TEXT("UC_Item_Icons_C_"));
	if (Button_HeroManager)
		Button_HeroManager->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickHeroManager);
	if (Button_MainLobby_01)
		Button_MainLobby_01->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickMainLobby);
	if (Button_MainLobby_02)
		Button_MainLobby_02->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickMainLobby);
	if (Button_StageSelect)
		Button_StageSelect->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickStageSelect);
	if (Button_StageDifficulty)
		Button_StageDifficulty->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickStageDifficulty);
	if (Button_Retry)
		Button_Retry->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickRetry);
	if (Button_Next_01)
		Button_Next_01->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickNextStage);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUP_Result_ForRTS::ClickMainLobby);

	if (IsValid(WidgetSwitcher_Button1))
		WidgetSwitcher_Button1->SetVisibility(ESlateVisibility::Visible);
	if (IsValid(WidgetSwitcher_Button2))
		WidgetSwitcher_Button2->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(WidgetSwitcher_Button3))
		WidgetSwitcher_Button3->SetVisibility(ESlateVisibility::Collapsed);

	if (GameInstance->IsSuccess == false)
	{
		if (IsValid(WidgetSwitcher_Type))
			WidgetSwitcher_Type->SetActiveWidgetIndex(1);
		if (IsValid(UC_Result_Display))
			UC_Result_Display->SetState(EResultEffectEnum::VE_Defeat);

		if (IsValid(WidgetSwitcher_Button3))
			WidgetSwitcher_Button3->SetVisibility(ESlateVisibility::Collapsed);
		SetStageFailText();
		WidgetSwitcher_Button1->SetActiveWidgetIndex(1);
	}
	else
	{
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
		if (RGameMode == nullptr)
			return;
		EPlayStageStarEnum StageStar = RGameMode->StarGrade;

		if (IsValid(WidgetSwitcher_Type))
			WidgetSwitcher_Type->SetActiveWidgetIndex(0);
		if (IsValid(UC_Result_Display))
			UC_Result_Display->SetState(EResultEffectEnum::VE_Victory);
		SetStageNameText();
		SetTimeText();
		SetAvatarDataText();
		SetReward();
		SetStar();
		WidgetSwitcher_Button1->SetActiveWidgetIndex(1);
	}
	PlayAni(TEXT("BG"));
	PlayAni(TEXT("Display_Ani"));
}

void UUP_Result_ForRTS::NativeDestruct()
{
	if (Button_OK)
		Button_OK->OnClicked.RemoveAll(this);
	if (Button_Next_01)
		Button_Next_01->OnClicked.RemoveAll(this);
	if (Button_Retry)
		Button_Retry->OnClicked.RemoveAll(this);
	if (Button_StageDifficulty)
		Button_StageDifficulty->OnClicked.RemoveAll(this);
	if (Button_StageSelect)
		Button_StageSelect->OnClicked.RemoveAll(this);
	if (Button_MainLobby_02)
		Button_MainLobby_02->OnClicked.RemoveAll(this);
	if (Button_MainLobby_01)
		Button_MainLobby_01->OnClicked.RemoveAll(this);
	if (Button_HeroManager)
		Button_HeroManager->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_Result_ForRTS::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// void UUP_Result_ForRTS::SetStageNameText()
// {
// 	WidgetSwitcher_Type->SetActiveWidgetIndex(0);
// 	UC_Result_Display->SetState(EResultEffectEnum::VE_Victory);
// }
// 
// void UUP_Result_ForRTS::SetTimeText()
// {
// 
// }

void UUP_Result_ForRTS::ClickHeroManager()
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

void UUP_Result_ForRTS::ClickMainLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->EventManager->ClearEvent();
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_Result_ForRTS::ClickStageSelect()
{

}

void UUP_Result_ForRTS::ClickStageDifficulty()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RaidManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_SingleRaid);
		RGameInstance->EventManager->ClearEvent();
		if (RGameInstance->CurrentGameMode != EGamePlayModeTypeEnum::VE_Arena)
			RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_Result_ForRTS::ClickRetry()
{

}

void UUP_Result_ForRTS::ClickNextStage()
{

}

bool UUP_Result_ForRTS::IsEnoughStamina()
{
	return true;
}

void UUP_Result_ForRTS::SetStageNameText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, WidgetSwitcher_Star, TEXT("WidgetSwitcher_Star"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_Star, TEXT("Text_Star_0"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_UnStar, TEXT("Text_UnStar_0"));

	if (Text_Star.IsValidIndex(0))
		Text_Star[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_01")));
	if (Text_Star.IsValidIndex(1))
		Text_Star[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_02")));
	if (Text_Star.IsValidIndex(2))
		Text_Star[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_03")));

	if (Text_UnStar.IsValidIndex(0))
		Text_UnStar[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_01")));
	if (Text_UnStar.IsValidIndex(1))
		Text_UnStar[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_02")));
	if (Text_UnStar.IsValidIndex(2))
		Text_UnStar[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_03")));

	if (IsValid(Text_CampaignName))
	{
// 		FFloorStageDetail* StageTableInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
// 		checkf(StageTableInfo != nullptr, TEXT("%s[ StageTableInfo Is Empty ]"), __FUNCTION__);
// 
// 		FText	StageName = FText::Format(FText::FromString("{0} {1}"), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->IngameSubTitle), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->Title));
		FRaidStageTableInfo* RaidbossTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);
		if(RaidbossTable)
		{
			FName DifficultyNameKey = NAME_None;
			switch (RaidbossTable->Difficulty)
			{
			case ERaidDifficultyType::EASY:
				DifficultyNameKey = FName(*FString(TEXT("Raid_Difficulty_Easy")));
				break;
			case ERaidDifficultyType::NORMAL:
				DifficultyNameKey = FName(*FString(TEXT("Raid_Difficulty_Normal")));
				break;
			case ERaidDifficultyType::HARD:
				DifficultyNameKey = FName(*FString(TEXT("Raid_Difficulty_Hard")));
				break;
			case ERaidDifficultyType::VERYHARD:
				DifficultyNameKey = FName(*FString(TEXT("Raid_Difficulty_Hell")));
				break;
			}
			
			FText difficulty = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, DifficultyNameKey);
			FText name = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, RaidbossTable->Name);
			FText RaidTitleText = FText::Format(FText::FromString(TEXT("[{0}] {1}")), difficulty, name);
			
			Text_CampaignName->SetText(RaidTitleText);
		}
	}
}

void UUP_Result_ForRTS::SetTimeText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	float	ClearTime = GameInstance->PlayStateInfo->GetClearTime();

	if (IsValid(Text_Time))
	{
		int32 nMinute = ClearTime / 60;
		int32 nSecond = ClearTime - (nMinute * 60);

		FString Minute_Str = FString::FromInt(nMinute * 0.1f);
		Minute_Str += FString::FromInt(nMinute % 10);

		FString Second_Str = FString::FromInt(nSecond * 0.1f);
		Second_Str += FString::FromInt(nSecond % 10);

		Minute_Str += FString(":");
		Minute_Str += Second_Str;

		Text_Time->SetText(FText::FromString(Minute_Str));
	}
}

void UUP_Result_ForRTS::SetAvatarDataText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	FAVATAR AvatarData = GameInstance->UserInfo->GetAvatarData();
	if (IsValid(Text_Level))
		Text_Level->SetText(FText::AsNumber(AvatarData.level));

	if (IsValid(Text_LevelPercent) && AvatarData.level != 0)
	{
		FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(AvatarData.level)));
		checkf(tableInfo != nullptr, TEXT("%s[ tableInfo Is Empty ] , avatarlevel : %s"), __FUNCTION__, *FString::FromInt(AvatarData.level));

		float Percent = float(AvatarData.exp) / float(tableInfo->AvatarMaxExp);
		//float ExpPercent = float(Percent * 100);

		Text_LevelPercent->SetText(FText::AsPercent(Percent));
	}
	float AvatarEXP = static_cast<float>(GameInstance->UserInfo->Avatar.exp) / static_cast<float>(GameInstance->UserInfo->AvatarExpMax);
	if (IsValid(ProgressBar_LevelMax))
		ProgressBar_LevelMax->SetPercent(AvatarEXP);
	if (IsValid(ProgressBar_LevelMin))
		ProgressBar_LevelMin->SetPercent(AvatarEXP);
}

void UUP_Result_ForRTS::SetReward()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	auto RTSManager = GameInstance->RealTimeModManager;

	FWalletTableInfo* WalletInfo = nullptr;

// 	BP_ScrollView->Init();
// 	BP_ScrollView->ClearScrollItemData();
	TArray<FREWARD> rewards = RTSManager->GetRTSResults();
	int32				iCount = 4;
	for (FREWARD& reward : rewards)
	{
		if (iCount < 0)
			break;
		if (reward.type == EREWARD_TYPE::ITEM || reward.type == EREWARD_TYPE::WALLET)
		{
			if (reward.rewardId.Compare(WALLET_GOLD) == 0 || reward.rewardId.Compare(WALLET_HERO_RUNE) == 0)
			{
				if (reward.rewardId.Compare(WALLET_GOLD) == 0)
					GoldAmt += reward.amt;
				else if (reward.rewardId.Compare(WALLET_HERO_RUNE) == 0)
					HeroRuneAmt += reward.amt;
			}
			else
			{
				FITEM_ICON_INFO RewardItemIconInfo;
				UUIFunctionLibrary::GetRewardIconByRewardType(reward, RewardItemIconInfo);
				if (IsValid(Reward_Icons[iCount]) && Reward_Icons.IsValidIndex(iCount))
				{
					Reward_Icons[iCount]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Reward_Icons[iCount]->SetItemIconStructure(RewardItemIconInfo, false);
				}
				iCount--;
// 				if (IsValid(BP_ScrollView))
// 				{
// 					URScrollItemData*					scroll_item_data = nullptr;
// 					UItemBaseScrollItemData*			reward_data = nullptr;
// 
// 					scroll_item_data = BP_ScrollView->AddScrollItemData(UItemBaseScrollItemData::StaticClass());
// 					reward_data = Cast<UItemBaseScrollItemData>(scroll_item_data);
// 					reward_data->SetData(RewardItemIconInfo);
// 				}
			}
		}
	}
// 	if (IsValid(BP_ScrollView))
// 		BP_ScrollView->InvalidateData();

	WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WALLET_GOLD);
	checkf(WalletInfo != nullptr, TEXT("UP_RESULT WALLET_GOLD Is Empty"));

	if (IsValid(UC_Item_Icons_Money) && IsValid(Text_Gold))
	{
		if (GoldAmt == 0)
		{
			UC_Item_Icons_Money->SetVisibility(ESlateVisibility::Collapsed);
			Text_Gold->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			UC_Item_Icons_Money->SetVisibility(ESlateVisibility::Visible);
			Text_Gold->SetVisibility(ESlateVisibility::Visible);
		}

		UC_Item_Icons_Money->Refresh(WalletInfo->ItemIconKey, GoldAmt, false, 0);
		Text_Gold->SetText(FText::Format(FText::FromString("{0}{1}"), FText::AsNumber(GoldAmt), FText::FromString("x")));
	}

	WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WALLET_HERO_RUNE);
	checkf(WalletInfo != nullptr, TEXT("UP_RESULT WALLET_HERO_RUNE Is Empty"));

	if (IsValid(UC_Item_Icons_Hero) && IsValid(Text_HeroRune))
	{
		if (HeroRuneAmt == 0)
		{
			UC_Item_Icons_Hero->SetVisibility(ESlateVisibility::Collapsed);
			Text_HeroRune->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			UC_Item_Icons_Hero->SetVisibility(ESlateVisibility::Visible);
			Text_HeroRune->SetVisibility(ESlateVisibility::Visible);

		}
		UC_Item_Icons_Hero->Refresh(WalletInfo->ItemIconKey, HeroRuneAmt, false, 0);
		Text_HeroRune->SetText(FText::Format(FText::FromString("{0}{1}"), FText::AsNumber(HeroRuneAmt), FText::FromString("x")));
	}
	GameInstance->ContentsUnLockManager->ShowContentsUnlockNotificationPopup();
}

void UUP_Result_ForRTS::SetStar()
{
	//temp
	for (int32 i = 0; i < 3; i++)
		WidgetSwitcher_Star[i]->SetVisibility(ESlateVisibility::Collapsed);
// 	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 	float	EndTime = BaseStageInfo->GetElapsedGameTime();
// 	int32	DieCount = RGAMEINSTANCE(this)->PlayStateInfo->GetHeroDieCount();
// 	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
// 	{
// 		FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
// 		checkf(FloorStageTable, TEXT("StageID Invalid"));
// 		if (EndTime < FloorStageTable->StarCondition1)
// 			WidgetSwitcher_Star[1]->SetActiveWidgetIndex(0);
// 	}
// 	for (int32 i = 0; i < 3; i++)
// 		WidgetSwitcher_Star[i]->SetActiveWidgetIndex(1);
// 	if (DieCount == 0)
// 		WidgetSwitcher_Star[0]->SetActiveWidgetIndex(0);
// 
// 	WidgetSwitcher_Star[2]->SetActiveWidgetIndex(0);
}

void UUP_Result_ForRTS::SetStageFailText()
{
	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, WidgetSwitcher_Fail, TEXT("WidgetSwitcher_Fail"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_Fail, TEXT("Text_Fail"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_UnFail, TEXT("Text_UnFail"));

	//leader die
	Text_Fail[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_01")));
	Text_UnFail[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_01")));
	//time out
	Text_Fail[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_02")));
	Text_UnFail[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_02")));
	//
	WidgetSwitcher_Fail[2]->SetVisibility(ESlateVisibility::Collapsed);

	if (RGAMEINSTANCE(this)->CampaignFailReson == EStageFailReason::VE_TIMEOUT_FAIL)
	{
		WidgetSwitcher_Fail[0]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[1]->SetActiveWidgetIndex(1);
		WidgetSwitcher_Fail[2]->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSwitcher_Fail[0]->SetActiveWidgetIndex(1);
		WidgetSwitcher_Fail[1]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[2]->SetActiveWidgetIndex(0);
	}
// 
// 	if (Text_Fail.IsValidIndex(0))
// 		Text_Fail[0]->SetText(FText::FromString(TEXT("Clear timeout")));
// 	if (Text_Fail.IsValidIndex(1))
// 		Text_Fail[1]->SetText(FText::FromString(TEXT("Hero survival")));
// 	if (Text_Fail.IsValidIndex(2))
// 		Text_Fail[2]->SetText(FText::FromString(TEXT("Hero survival")));
// 
// 	if (Text_UnFail.IsValidIndex(0))
// 		Text_UnFail[0]->SetText(FText::FromString(TEXT("Clear timeout")));
// 	if (Text_UnFail.IsValidIndex(1))
// 		Text_UnFail[1]->SetText(FText::FromString(TEXT("Hero survival")));
// 	if (Text_UnFail.IsValidIndex(2))
// 		Text_UnFail[2]->SetText(FText::FromString(TEXT("Hero survival")));


	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	float	ClearTime = GameInstance->PlayStateInfo->GetClearTime();
	//	float	RemainningTime = GameInstance->PlayStateInfo->GetRemainingTime();
	ULocalizingTextBlock* FailTimeTextBlock;
	FailTimeTextBlock = Cast<ULocalizingTextBlock>(GetWidgetFromName(TEXT("LocalizingTextBlock_21")));
	if (IsValid(FailTimeTextBlock))
	{
		int32 nMinute = ClearTime / 60;
		int32 nSecond = ClearTime - (nMinute * 60);

		FString Minute_Str = FString::FromInt(nMinute * 0.1f);
		Minute_Str += FString::FromInt(nMinute % 10);

		FString Second_Str = FString::FromInt(nSecond * 0.1f);
		Second_Str += FString::FromInt(nSecond % 10);

		Minute_Str += FString(":");
		Minute_Str += Second_Str;

		FailTimeTextBlock->SetText(FText::FromString(Minute_Str));
	}
}