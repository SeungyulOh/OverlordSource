// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Result.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"

//

#include "GameInfo/PlayStateInfo.h"

#include "GameInfo/RInventory.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "ProjectRGameMode.h"
#include "GameInfo/UserInfo.h"

#include "GameState/PlayState.h"
#include "GameState/TowerLobbyState.h"

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
#include "UI/Common/UP_Popup_Avatar.h"
#include "UI/UI_Common/UP_RewardPopup.h"
#include "UI/UI_Common/UP_CommonPopup.h"

#include "UI/Result/UC_Result_HeroNew.h"
#include "UI/Result/UC_ResultStatistics.h"
#include "UI/Result/UC_ResultEffect.h"
#include "UI/Result/UC_ResultHeroIcon.h"

#include "UI/InGameHUD/QuestPassingAlertPopup.h"

#include "UI/Common/UC_Notify.h"
#include "UI/Common/ItemBaseScrollItemData.h"

#include "UtilFunctionIntegrated.h"
#include "CustomStructures/HeroSlotInfo.h"

#include "Table/CampaignTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"

#include "UI/Campaign/UP_BattleRepeatStatePopup.h"
#include "DirectionTrackManager.h"

#define SINGLERAID_OFFSET 7
#define MULTIRAID_OFFSET 2

#define NEXTSTAGE_OFFSET 7

#define SINGLERAID_STRINGKEY FName(TEXT("UI_Result_Desc8"))
#define MULTIRAID_STRINGKEY FName(TEXT("UI_Result_Desc9"))

void UUP_Result::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	if (Button_HeroManager)
		Button_HeroManager->OnClicked.AddDynamic(this, &UUP_Result::ClickHeroManager);
	if (Button_MainLobby_01)
		Button_MainLobby_01->OnClicked.AddDynamic(this, &UUP_Result::ClickMainLobby);
	if (Button_MainLobby_02)
		Button_MainLobby_02->OnClicked.AddDynamic(this, &UUP_Result::ClickMainLobby);
	if (Button_StageSelect)
		Button_StageSelect->OnClicked.AddDynamic(this, &UUP_Result::ClickStageSelect);
	if (Button_StageDifficulty)
		Button_StageDifficulty->OnClicked.AddDynamic(this, &UUP_Result::ClickStageDifficulty);
	if (Button_Retry)
		Button_Retry->OnClicked.AddDynamic(this, &UUP_Result::ClickRetry);
	if (Button_Next_01)
		Button_Next_01->OnClicked.AddDynamic(this, &UUP_Result::ClickNextStage);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUP_Result::ClickMainLobby);

	if (GameInstance->IsSuccess == false)
	{
		if(IsValid(WidgetSwitcher_Type))
			WidgetSwitcher_Type->SetActiveWidgetIndex(1);
		if(IsValid(UC_Result_Display))
			UC_Result_Display->SetState(EResultEffectEnum::VE_Defeat);

		if (IsValid(WidgetSwitcher_Button3))
			WidgetSwitcher_Button3->SetVisibility(ESlateVisibility::Collapsed);
		SetStageFailText();
		SetTimeText();

		if (GameInstance->ContentsUnLockManager->IsFirstEnter(EFirstCheckContentsType::FirstStageFail))
		{
			UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)EFirstCheckContentsType::FirstStageFail, 1);

			GameInstance->ContentsUnLockManager->ShowStageFailPopup();
		}
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
			UC_Result_Display->SetState(EResultEffectEnum(int32(EResultEffectEnum::VE_Star1) + int32(StageStar)-1));
		if (IsValid(WidgetSwitcher_Button3))
			WidgetSwitcher_Button3->SetVisibility(ESlateVisibility::Visible);
		SetStageNameText();
		SetTimeText();
		SetAvatarDataText();
		SetReward();
		SetStar();
		SetHeroIcon();
		SetImportantMissionInfo();

		UPlayState* PlayState = Cast<UPlayState>(GameInstance->GetCurrentState());
		if (PlayState)
		{
			PlayState->NextStageTableInfo = FindNextStageInfo();

			if (!PlayState->NextStageTableInfo && Button_Next_01)
				Button_Next_01->SetVisibility(ESlateVisibility::Collapsed);
			else if (PlayState->NextStageTableInfo->PlayType == FName(TEXT("S")))
				LocalizingTextBlock_NextStage->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI , SINGLERAID_STRINGKEY));
			else if (PlayState->NextStageTableInfo->PlayType == FName(TEXT("M")))
				LocalizingTextBlock_NextStage->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, MULTIRAID_STRINGKEY));
		}		

		if(GameInstance->CampaignManager->IsAutoPlay())
		{
			if (TimeHandler_BattleRepeat.IsValid())
				GameInstance->GetTimerManager().ClearTimer(TimeHandler_BattleRepeat);
			GameInstance->GetTimerManager().SetTimer(TimeHandler_BattleRepeat, this, &UUP_Result::ClickRetry, 1, false);
			
		}
	}
	if(!ResultPopupOrderChecker)
	{
		ResultPopupOrderChecker = NewObject<UResultPopupOrderChecker>(this);
		ResultPopupOrderChecker->Initialize();
	}
}

FFloorStageDetail* UUP_Result::FindNextStageInfo()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
	FFloorStageDetail* CurStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[BaseStageInfo->StageIndex]);
	FFloorStageDetail* NextStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[BaseStageInfo->StageIndex + 1]);

	if (CurStageTable->PlayType != NextStageTable->PlayType)
	{
		return UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[BaseStageInfo->StageIndex + NEXTSTAGE_OFFSET]);
	}
	else
		return NextStageTable;
}

void UUP_Result::NativeDestruct()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(GEngine);

	if(GameInstance->IsSuccess)
		UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_VictoryPosition);
	

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

	if(ResultPopupOrderChecker)
	{
		ResultPopupOrderChecker = nullptr;
	}
	

	Super::NativeDestruct();
}

void UUP_Result::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(ResultPopupOrderChecker)
	{
		if (!ResultPopupOrderChecker->IsEmptyQueue())
		{
			if (CurrentShowingPopup == EResultPopupType::None)
			{
				CurrentShowingPopup = ResultPopupOrderChecker->DequeuePopupOrder();
				ShowResultPopup(CurrentShowingPopup);
			}
		}
	}
}

void UUP_Result::SetStageNameText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*		BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, WidgetSwitcher_Star, TEXT("WidgetSwitcher_Star"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_Star, TEXT("Text_Star_0"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_UnStar, TEXT("Text_UnStar_0"));
	FindChildWidgetsWithCertainName<UUC_ResultHeroIcon>(this, ResultHeroIcon, TEXT("HeroIcon_0"));
	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, ItemIcon, TEXT("ItemIcon_0"));
	
	if (BaseStageInfo->bPlayAlone)
	{
		if (Text_Star.IsValidIndex(0))
			Text_Star[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_03")));
		if (Text_Star.IsValidIndex(1))
			Text_Star[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_02")));
		if (Text_Star.IsValidIndex(2))
			Text_Star[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_04")));
	}
	else
	{
		if (Text_Star.IsValidIndex(0))
			Text_Star[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_01")));
		if (Text_Star.IsValidIndex(1))
			Text_Star[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_02")));
		if (Text_Star.IsValidIndex(2))
			Text_Star[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_03")));
	}


	if (Text_UnStar.IsValidIndex(0))
		Text_UnStar[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_01")));
	if (Text_UnStar.IsValidIndex(1))
		Text_UnStar[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_02")));
	if (Text_UnStar.IsValidIndex(2))
		Text_UnStar[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_StarCondition_03")));

	if (IsValid(Text_CampaignName))
	{
		FFloorStageDetail* StageTableInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
		checkf(StageTableInfo != nullptr, TEXT("%s[ StageTableInfo Is Empty ]"), __FUNCTION__);

		FText	StageName = FText::Format(FText::FromString("{0} {1}"), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->IngameSubTitle), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->Title));
		Text_CampaignName->SetText(StageName);
	}
}

void UUP_Result::SetTimeText()
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

void UUP_Result::SetAvatarDataText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	FAVATAR& AvatarData =  GameInstance->UserInfo->GetAvatarData();
	if (IsValid(Text_Level))
		Text_Level->SetText(FText::AsNumber(AvatarData.level));

	if (IsValid(Text_LevelPercent) && AvatarData.level != 0 )
	{
		FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(AvatarData.level)));
		checkf(tableInfo != nullptr, TEXT("%s[ tableInfo Is Empty ] , avatarlevel : %s"), __FUNCTION__ , *FString::FromInt(AvatarData.level));

		float Percent = float(AvatarData.exp) / float(tableInfo->AvatarMaxExp);
		float rounded = ((int)(Percent * 100 + .5) / 100.0);
		float ExpPercent = float(rounded * 100);
		Text_LevelPercent->SetText(FText::AsNumber(ExpPercent));
	}

	float AvatarEXP = static_cast<float>(GameInstance->UserInfo->Avatar.exp) / static_cast<float>(GameInstance->UserInfo->AvatarExpMax);
	
	if (IsValid(ProgressBar_LevelMax))
		ProgressBar_LevelMax->SetPercent(AvatarEXP);
	if (IsValid(ProgressBar_LevelMin))
		ProgressBar_LevelMin->SetPercent(AvatarEXP);
}

void UUP_Result::SetReward()
{
	URGameInstance*		GameInstance		=	RGAMEINSTANCE(this);
	UCampaignManager*	CampaignManager		=	GameInstance->CampaignManager;
	FWalletTableInfo*	WalletInfo			=	nullptr;
	int32				iCount				=	4;
	BP_ScrollView->SetVisibility(ESlateVisibility::Collapsed);
// 	BP_ScrollView->Init();
// 	BP_ScrollView->ClearScrollItemData();
	for (int32 i = 0; i < 5; i++)
	{
		if(IsValid(ItemIcon[i]) && ItemIcon.IsValidIndex(i))
			ItemIcon[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (FREWARD& reward : CampaignManager->m_ResultReward)
	{
		if (iCount < 0)
			break;
		if (reward.type == EREWARD_TYPE::ITEM || reward.type == EREWARD_TYPE::WALLET)
		{
			if (reward.rewardId.Compare(WALLET_GOLD) == 0 )
			{
				if (reward.rewardId.Compare(WALLET_GOLD) == 0)
					GoldAmt += reward.amt;
			}
			else if (reward.rewardId.Compare(WALLET_HERO_RUNE) == 0)
			{
				FITEM_ICON_INFO RewardItemIconInfo;
				UUIFunctionLibrary::GetRewardIconByRewardType(reward, RewardItemIconInfo);
// 				if (IsValid(BP_ScrollView))
// 				{
// 					URScrollItemData*					scroll_item_data = nullptr;
// 					UItemBaseScrollItemData*			reward_data = nullptr;
				if (IsValid(ItemIcon[iCount]) && ItemIcon.IsValidIndex(iCount))
				{
					ItemIcon[iCount]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					ItemIcon[iCount]->SetItemIconStructure(RewardItemIconInfo, false);
				}

// 					scroll_item_data = BP_ScrollView->AddScrollItemData(UItemBaseScrollItemData::StaticClass());
// 					reward_data = Cast<UItemBaseScrollItemData>(scroll_item_data);
//					reward_data->SetData(RewardItemIconInfo);
					iCount--;
//				}
			}
			else
			{
				FITEM_ICON_INFO RewardItemIconInfo;
				UUIFunctionLibrary::GetRewardIconByRewardType(reward, RewardItemIconInfo);

// 				if (IsValid(BP_ScrollView))
// 				{
//					URScrollItemData*					scroll_item_data = nullptr;
//					UItemBaseScrollItemData*			reward_data = nullptr;
				if (IsValid(ItemIcon[iCount]) && ItemIcon.IsValidIndex(iCount))
				{
					ItemIcon[iCount]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					ItemIcon[iCount]->SetItemIconStructure(RewardItemIconInfo, false);
				}
// 					scroll_item_data = BP_ScrollView->AddScrollItemData(UItemBaseScrollItemData::StaticClass());
// 					reward_data = Cast<UItemBaseScrollItemData>(scroll_item_data);
//					reward_data->SetData(RewardItemIconInfo);
				iCount--;
//				}
			}
		}
	}
// 	if (IsValid(BP_ScrollView))
// 		BP_ScrollView->InvalidateData();

	WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WALLET_GOLD);
	checkf(WalletInfo != nullptr, TEXT("UP_RESULT WALLET_GOLD Is Empty"));

	if (IsValid(Text_Point))
		Text_Point->SetText(FText::AsNumber(GoldAmt));

	GameInstance->ContentsUnLockManager->ShowContentsUnlockNotificationPopup();
}

void UUP_Result::SetStar()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
	checkf(FloorStageTable, TEXT("StageID Invalid"));

	for (int32 i = 0; i < 3; i++)
		WidgetSwitcher_Star[i]->SetActiveWidgetIndex(1);

	float	EndTime		=	BaseStageInfo->GetElapsedGameTime();
	int32	DieCount	=	RGAMEINSTANCE(this)->PlayStateInfo->GetHeroDieCount();

	if (BaseStageInfo->bPlayAlone)
	{
		WidgetSwitcher_Star[0]->SetActiveWidgetIndex(0);

		if (EndTime < FloorStageTable->StarCondition1)
			WidgetSwitcher_Star[1]->SetActiveWidgetIndex(0);

		if (EndTime < FloorStageTable->StarCondition2)
			WidgetSwitcher_Star[2]->SetActiveWidgetIndex(0);
	}
	else
	{
		if (EndTime < FloorStageTable->StarCondition1)
			WidgetSwitcher_Star[1]->SetActiveWidgetIndex(0);

		if (DieCount == FloorStageTable->StarCondition2)
			WidgetSwitcher_Star[0]->SetActiveWidgetIndex(0);

		WidgetSwitcher_Star[2]->SetActiveWidgetIndex(0);
	}
}

void UUP_Result::SetHeroIcon()
{
	for (int32 Num = 0; Num < ResultHeroIcon.Num(); Num++)
	{
		if (ResultHeroIcon.IsValidIndex(Num) == false || IsValid(ResultHeroIcon[Num]) == false)
			continue;

		ResultHeroIcon[Num]->SetIcon(Num);
	}
}

void UUP_Result::SetStageFailText()
{
	//set fail text
	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, WidgetSwitcher_Fail, TEXT("WidgetSwitcher_Fail"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_Fail, TEXT("Text_Fail"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_UnFail, TEXT("Text_UnFail"));

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	FFloorStageDetail* StageData = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
	if (StageData->PlayType.ToString().Contains(TEXT("N")) || StageData->PlayType.ToString().Contains(TEXT("H")))
	{
		FName clearCondition = StageData->clearInfo1;
		if (!clearCondition.IsNone())
		{
			//leader die
			Text_Fail[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_01")));
			Text_UnFail[0]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_01")));
			//time out
			Text_Fail[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_02")));
			Text_UnFail[1]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Common_02")));
			if (clearCondition.ToString().Contains(TEXT("3")))
			{
				WidgetSwitcher_Fail[2]->SetVisibility(ESlateVisibility::Visible);
				Text_Fail[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Protect")));
				Text_UnFail[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Protect")));
			}
			else if (clearCondition.ToString().Contains(TEXT("4")))
			{
				WidgetSwitcher_Fail[2]->SetVisibility(ESlateVisibility::Visible);
				Text_Fail[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Survival")));
				Text_UnFail[2]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Clear_Faill_Survival")));
			}
			else
			{
				//3rd condition invisible
				WidgetSwitcher_Fail[2]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	switch (RGAMEINSTANCE(this)->CampaignFailReson)
	{
	case EStageFailReason::VE_LEADER_DIE_FAIL:
		WidgetSwitcher_Fail[0]->SetActiveWidgetIndex(1);
		WidgetSwitcher_Fail[1]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[2]->SetActiveWidgetIndex(0);
		break;
	case EStageFailReason::VE_TIMEOUT_FAIL:
		WidgetSwitcher_Fail[0]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[1]->SetActiveWidgetIndex(1);
		WidgetSwitcher_Fail[2]->SetActiveWidgetIndex(0);
		break;
	case EStageFailReason::VE_ALL_SURVIVAL_FAIL:
	case EStageFailReason::VE_PROTECT_NPC_DIE_FAIL:
		WidgetSwitcher_Fail[0]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[1]->SetActiveWidgetIndex(0);
		WidgetSwitcher_Fail[2]->SetActiveWidgetIndex(1);
		break;
	default:
		break;
	}


	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	float	ClearTime = GameInstance->PlayStateInfo->GetClearTime();
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

void UUP_Result::ClickHeroManager()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		return;

	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->GetCurrentState());
	if (PlayState)
		PlayState->NextStageTableInfo = nullptr;

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

void UUP_Result::ClickMainLobby()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		return;

	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->GetCurrentState());
	if (PlayState)
		PlayState->NextStageTableInfo = nullptr;

	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->EventManager->ClearEvent();
		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_Result::ClickStageSelect()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		return;

	if (!IsValid(RGameInstance->RWidgetManager))
		return;

	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->GetCurrentState());
	if (PlayState)
		PlayState->NextStageTableInfo = nullptr;

	RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Campaign);
	RGameInstance->EventManager->ClearEvent();
	UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
	if (TowerState)
		TowerState->SetIndoorStart(true);
	RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
}

void UUP_Result::ClickStageDifficulty()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		return;

	if (!IsValid(RGameInstance->RWidgetManager))
		return;

	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->GetCurrentState());
	if (PlayState)
		PlayState->NextStageTableInfo = nullptr;

	RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Campaign);
	RGameInstance->EventManager->ClearEvent();
	RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
}

void UUP_Result::ClickRetry()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

// 	if(RGameInstance->IsAutoPlayMode())
// 		RGameInstance->CampaignManager->DecreasePlayCount();

	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->CurrentState);
	PlayState->Retry();
}

void UUP_Result::ClickNextStage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		return;
	
	bool bCanMoveNextStage = true;
	//for tuto temp
	if (bCanMoveNextStage && RGameInstance->ForceTutorialManager->HaveTuto)
	{
		RGameInstance->ForceTutorialManager->HaveTuto = false;
		UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Tutorial_Move_Area_Confirm")), 
			ECommonPopupType::VE_OKCANCEL);
		if (popup)
		{
			popup->OnClickOK.RemoveDynamic(this, &UUP_Result::ClickTutoOK);
			popup->OnClickOK.AddDynamic(this, &UUP_Result::ClickTutoOK);
			popup->OnClickCancel.RemoveDynamic(this, &UUP_Result::ClickTutoCancel);
			popup->OnClickCancel.AddDynamic(this, &UUP_Result::ClickTutoCancel);
		}
		bCanMoveNextStage = false;
	}

	if (bCanMoveNextStage && bCheckQuestPass)
	{
		URWidgetManager* WM = RGameInstance->RWidgetManager;
		WM->AttachUIPage(EUIPageEnum::UIPage_QuestPassingAlertPopup);

		UQuestPassingAlertPopup* QuestPassingAlertPopup = Cast<UQuestPassingAlertPopup>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_QuestPassingAlertPopup));

		if (IsValid(QuestPassingAlertPopup))
		{
			int32 nQuestID = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;

			QuestPassingAlertPopup->SetPopupText(nQuestID);

			QuestPassingAlertPopup->SetVisibility(ESlateVisibility::Visible);

			QuestPassingAlertPopup->Button_OK->OnClicked.RemoveDynamic(this, &UUP_Result::GoNextStage);
			QuestPassingAlertPopup->Button_OK->OnClicked.AddDynamic(this, &UUP_Result::GoNextStage);
			bCanMoveNextStage = false;
		}

	}

	// unlock check
	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->CurrentState);
	if (bCanMoveNextStage && PlayState->NextStageTableInfo->PlayType == FName(TEXT("N")))
	{
		FFloorStageDetail* pNextStageInfo = FindNextStageInfo();
		if (!RGameInstance->ContentsUnLockManager->IsUnlockFloor(pNextStageInfo->FloorInfoId))
		{
			FText TitleText;
			FText DescText;

			RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, TitleText, DescText, pNextStageInfo->FloorInfoId);
			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
			bCanMoveNextStage = false;
		}
	}

	if(bCanMoveNextStage)
		GoNextStage();
}

void UUP_Result::GoNextStage()
{
	bCheckQuestPass = false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UPlayState* PlayState = Cast<UPlayState>(RGameInstance->CurrentState);
	if (PlayState->NextStageTableInfo->PlayType == FName(TEXT("S")) || PlayState->NextStageTableInfo->PlayType == FName(TEXT("M")))
	{
		if (!IsValid(RGameInstance->RWidgetManager))
			return;

		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_TowerBoss);
		RGameInstance->EventManager->ClearEvent();
		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
	else
		PlayState->NextStage();

}

void UUP_Result::ClickTutoOK()
{
	ClickStageSelect();
}

void UUP_Result::ClickTutoCancel()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	GameInstance->ForceTutorialManager->nCurrentTutorialStep++;
	UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)EFirstCheckContentsType::FirstForceTutorial, GameInstance->ForceTutorialManager->nCurrentTutorialStep);
// 	UPacketFunctionLibrary::ROA_TUTORIAL_PROGRESS_RQ(GameInstance->ForceTutorialManager->nCurrentTutorialStep);
	GameInstance->ForceTutorialManager->SettingCurQuest(GameInstance->ForceTutorialManager->nCurrentTutorialStep);
	UPlayState* PlayState = Cast<UPlayState>(GameInstance->CurrentState);
	PlayState->NextStage();
}

void UUP_Result::SetImportantMissionInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nQuestID = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	
	// init
	bCheckQuestPass = false;

	bool bCompleteQuest = RGameInstance->CampaignManager->bChangeQuest;
	
	if (bCompleteQuest)
		nQuestID -= 1;

	FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(nQuestID).ToString()));
	if (pTableData)
	{
		if(!bCompleteQuest)
		{
			UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			if (BaseStageInfo)
			{
				if (BaseStageInfo->StageID == pTableData->FloorStageId)
					bCheckQuestPass = true;
			}
		}
		ImportantMissionPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTableData->TargetStringKey));
		ImportantCurrentText->SetText(FText::AsNumber(!bCompleteQuest ? RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress : pTableData->ConditionGoal));
		ImportantMaxText->SetText(FText::AsNumber(pTableData->ConditionGoal));
	}
	else
	{
		ImportantMissionPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	RGameInstance->CampaignManager->bChangeQuest = false;
}

void UUP_Result::ShowResultPopup(EResultPopupType InType)
{
	switch (InType)
	{
	case EResultPopupType::HeroRewardPopup:
		ShowHeroRewardPopup();
		break;
	case EResultPopupType::QuestRewardPopup:
		ShowQuestRewardPopup();
		break;
	case EResultPopupType::AvatarLevelupPopup:
		ShowAvatarLevelupPopup();
		break;
	case EResultPopupType::UnlockPopup:
		ShowUnlockPopup();
		break;
	default:
		break;
	}
}

void UUP_Result::ShowHeroRewardPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (0 < RGameInstance->CampaignManager->SavedHeroRewards.Num())
	{
		FREWARD reward = RGameInstance->CampaignManager->SavedHeroRewards[0];
		RGameInstance->CampaignManager->SavedHeroRewards.RemoveAt(0);
		FName id = reward.rewardId;

		FCharacterTableInfo* pCharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(id);
		if(pCharacterTableInfo)
		{
			FText Name = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_NotifyQueue_GetNewHero")), FText::AsNumber(pCharacterTableInfo->Grade), 
										UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, pCharacterTableInfo->DisplayName));
			UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(id, Name, FText::GetEmpty());

			if (TimeHandler_HeroRewardDelay.IsValid())
				RGameInstance->GetTimerManager().ClearTimer(TimeHandler_HeroRewardDelay);
			RGameInstance->GetTimerManager().SetTimer(TimeHandler_HeroRewardDelay, this, &UUP_Result::ShowHeroRewardPopup, 2.0f, false);
		}
	}
	else
	{
		OnResultPopupClose();
	}
}

void UUP_Result::ShowQuestRewardPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (0 < RGameInstance->CampaignManager->SavedQuestRewards.Num())
	{
		int32 nPrevQuestId = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId - 1;
		FFloorQuestTableInfo* pTable = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(nPrevQuestId).ToString()));
		
		if (!pTable)
		{
			OnResultPopupClose();
		}
		else
		{
			FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTable->MissionTitleKey);
			UUP_RewardPopup* rewardpopup = UUIFunctionLibrary::ShowQuestRewardsPopup(RGameInstance->CampaignManager->SavedQuestRewards, text);
			if (rewardpopup)
			{
				rewardpopup->VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_Result::OnResultPopupClose);
				rewardpopup->VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_Result::OnResultPopupClose);
			}
		}

		RGameInstance->CampaignManager->SavedQuestRewards.Empty();
	}
	else
	{
		OnResultPopupClose();
	}
}

void UUP_Result::ShowAvatarLevelupPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->CampaignManager->bLevelUp)
	{
		UUP_Popup_Avatar* pAvatar = UUIFunctionLibrary::ShowAvatarLevelUpPopup(true);
		if (pAvatar)
		{
			pAvatar->VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_Result::OnResultPopupClose);
			pAvatar->VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_Result::OnResultPopupClose);
		}
		RGameInstance->CampaignManager->bLevelUp = false;
	}
	else
	{
		OnResultPopupClose();
	}
}

void UUP_Result::ShowUnlockPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->ContentsUnLockManager->ShowUnlockNotificationDelay();

	
	OnResultPopupClose();
}

void UUP_Result::OnResultPopupClose()
{
	CurrentShowingPopup = EResultPopupType::None;
}

void UResultPopupOrderChecker::Initialize()
{
	PopupOrderQue.Empty();

	EnqueuePopupOrder(EResultPopupType::HeroRewardPopup);
	EnqueuePopupOrder(EResultPopupType::QuestRewardPopup);
	EnqueuePopupOrder(EResultPopupType::AvatarLevelupPopup);
	EnqueuePopupOrder(EResultPopupType::UnlockPopup);
}

void UResultPopupOrderChecker::EnqueuePopupOrder(EResultPopupType InType)
{
// 	PopupOrderQue.Enqueue(InType);
	PopupOrderQue.Emplace(InType);
}

EResultPopupType UResultPopupOrderChecker::DequeuePopupOrder()
{
	EResultPopupType retType = EResultPopupType::None;
// 	if (PopupOrderQue.Dequeue(retType))
// 		return retType;

	if (PopupOrderQue.IsValidIndex(0))
	{
		retType = PopupOrderQue[0];
		PopupOrderQue.RemoveAt(0);
	}

	return retType;
}

bool UResultPopupOrderChecker::IsEmptyQueue()
{
// 	return PopupOrderQue.IsEmpty();
	return (PopupOrderQue.Num() == 0);
}
