// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Campaign_Tower.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/MonsterScrollItemData.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"
#include "GlobalIntegrated.h"
#include "UI/Campaign/UC_StageButton.h"
#include "UI/UI_Campaign/UP_CampaignTower_Scrollview.h"
#include "UI/Common/ItemBaseScrollItemData.h"
#include "UI/Common/UC_Particle.h"
#include "UI/HeroManagement/UP_HeroInventory.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "Algo/Reverse.h"
#include "WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "Components/WidgetComponent.h"
#include "Global/RGameInstance.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Lobby/IndoorLobbyStateActor.h"

#include "UI/Campaign/UC_FloorSelector.h"
#include "UI/UI_Common/UP_CommonPopup.h"

#define ENABLE 0
#define DISABLE 1
#define MAXSTAGE_FLOOR 14
#define MAXREWARDCNT 4
#define MAXSTAR 39
#define TOTAL_FLOOR 25

void UUP_Campaign_Tower::NativeConstruct()
{
	Super::NativeConstruct();

	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

	if (!NormalStageButtonArray.Num())
	{
		FindChildWidgetsWithCertainName<UUC_StageButton>(this, NormalStageButtonArray, TEXT("NormalStageButton"));
		Algo::Reverse(NormalStageButtonArray);
	}
	if (!EliteStageButtonArray.Num())
	{
		FindChildWidgetsWithCertainName<UUC_StageButton>(this, EliteStageButtonArray, TEXT("EliteStageButton"));
		Algo::Reverse(EliteStageButtonArray);
	}
	

	PlayAni(TEXT("LeftRightBottom"));
	
	FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();

	if (IsValid(TowerStageInfo))
	{
		TowerStageInfo->UP_Campaign_Tower = this;
		TowerStageInfo->Set_ScrollItem_Pair();
	}

	if (IsValid(TreasureButton))
	{
		TreasureButton->OnClicked.Clear();
		TreasureButton->OnClicked.AddDynamic(this, &UUP_Campaign_Tower::OnClick_Treasure);
	}

	UCampaignManager* CampaignMgr = RGAMEINSTANCE(this)->CampaignManager;
	checkf(CampaignMgr, TEXT("CampaignMgr null"));

	checkf(RGAMEINSTANCE(this), TEXT("GameInst Null"));
	checkf(RGAMEINSTANCE(this)->EventManager, TEXT("EventManager Null"));
	RGAMEINSTANCE(this)->EventManager->OnUpdateCampaignUIPage.Clear();
	RGAMEINSTANCE(this)->EventManager->OnUpdateCampaignUIPage.AddDynamic(this, &UUP_Campaign_Tower::Render_OnFloorChanged);

	RGAMEINSTANCE(this)->MissionManager->ReceiveGetFloorMission.RemoveDynamic(this, &UUP_Campaign_Tower::SetFloorMissionButton);
	RGAMEINSTANCE(this)->MissionManager->ReceiveGetFloorMission.AddDynamic(this, &UUP_Campaign_Tower::SetFloorMissionButton);

	GotoMissionContentsButton->OnClicked.RemoveDynamic(this, &UUP_Campaign_Tower::OnClickGotoMissionContents);
	GotoMissionContentsButton->OnClicked.AddDynamic(this, &UUP_Campaign_Tower::OnClickGotoMissionContents);

	Button_FloorMission->OnClicked.RemoveDynamic(this, &UUP_Campaign_Tower::OnClickFloorMissionButton);
	Button_FloorMission->OnClicked.AddDynamic(this, &UUP_Campaign_Tower::OnClickFloorMissionButton);

	CurrentFloor = TowerStatus.currentFloor;
	SetCurrentFloor(TowerStatus.currentFloor);
	Invalidate();
	SetFloorMissionButton();
	SetImportantMissionInfo();
}

void UUP_Campaign_Tower::Invalidate()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	/*Stagebutton*/
	QuestImageOnStageBtnReset();
	Init_NormalStageButton();
	Init_EliteStageButton();
	if (GameInstance->MissionManager->StageTargetFloor != NAME_None)
	{
		FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(GameInstance->MissionManager->StageTargetFloor);
		bool bCanpass = false;
		for (int32 i = 0; i < NormalStageButtonArray.Num(); i++)
		{
			if (NormalStageButtonArray[i]->GetStageIdx() == FloorStageTable->StageIdx)
			{
				SetSelectTargetStageButton(i);
				bCanpass = true;
				break;
			}
		}

		for (int32 i = 0; i < EliteStageButtonArray.Num(); i++)
		{
			if (bCanpass)
				break;
			if (EliteStageButtonArray[i]->GetStageIdx() == FloorStageTable->StageIdx)
			{
				EliteStageButtonArray[i]->SetSelected(false);
				EliteStageButtonArray[i]->OnButtonClick();
				break;
			}
		}
		GameInstance->MissionManager->StageTargetFloor = NAME_None;
	}
	
	/*end*/

	/*star*/
	iCurrentStarNum = 0;
	FTOWER_STATUS TowerStatus = GameInstance->CampaignManager->TowerStatus;
	for (size_t i = (CurrentFloor -1) * MAXSTAGE_FLOOR ; i< (CurrentFloor - 1) * MAXSTAGE_FLOOR + MAXSTAGE_FLOOR - 1; ++i)
	{
		if(TowerStatus.clearGrades.IsValidIndex(i))
			iCurrentStarNum += TowerStatus.clearGrades[i];
	}	
	CurStarCnt->SetText(FText::FromString(FString::FromInt(iCurrentStarNum)));
	/*end*/

	/*BossCard*/
	if (BossCard)
	{
		bool isAvailableRaid = BossCard->Initialize_FloorBossCard(CurrentFloor);
		if (isAvailableRaid)
		{
			BossCard->Button_HeroCard->OnClicked.RemoveDynamic(this, &UUP_Campaign_Tower::OnClick_HeroCard);
			BossCard->Button_HeroCard->OnClicked.AddDynamic(this, &UUP_Campaign_Tower::OnClick_HeroCard);
		}
	}

	
	/*end*/

	/*treasure box*/
	SetTreasureBox();
	/*end*/

	/*Title*/
	FFloorInfo* FloorInfoTable = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(CurrentFloor)));
	if (FloorInfoTable)
	{
		FString TitleStr = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorInfoTable->FloorTitle).ToString() + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorInfoTable->FloorSubTilte).ToString();

		if (FloorSelector)
		{
			FloorSelector->variables.FloorTitleText1->SetText(FText::FromString(TitleStr));
			FloorSelector->variables.FloorTitleText2->SetText(FText::FromString(TitleStr));
			FloorSelector->ShowFloorSelector(false);
		}
		//TextFloorBoss->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorInfoTable->FloorBoss));
	}
	
	/*end*/
	
}

void UUP_Campaign_Tower::SetTreasureBox()
{
	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

	TreasureStateArray.Empty();
	int32 PossibleCnt = 0;
	if (iCurrentStarNum < 36)
		PossibleCnt = (iCurrentStarNum / 9);
	else if (iCurrentStarNum == 36)
		PossibleCnt = MAXREWARDCNT - 1;
	else if (iCurrentStarNum == MAXSTAR)
		PossibleCnt = MAXREWARDCNT;

	int32 PossibleRewardCnt = 0;
	for (int32 i = 0; i < MAXREWARDCNT; ++i)
	{
		if (i < PossibleCnt)
		{
			if (TowerStatus.treasureBoxes.IsValidIndex((CurrentFloor - 1)*MAXREWARDCNT + i) && TowerStatus.treasureBoxes[(CurrentFloor - 1) * MAXREWARDCNT + i] == true)
				TreasureStateArray.Emplace(ETreasureState::TREASURE_ACQUIRED);
			else
			{
				TreasureStateArray.Emplace(ETreasureState::TREASURE_POSSIBLE);
				++PossibleRewardCnt;
			}

		}
		else
			TreasureStateArray.Emplace(ETreasureState::TREASURE_IMPOSSIBLE);
	}

	int32 TargetTreasureImgIdx = 0;
	int32 iTargetStar = 0;
	for (int32 i = 0; i < MAXREWARDCNT; ++i)
	{
		if (TreasureStateArray[i] == ETreasureState::TREASURE_IMPOSSIBLE)
		{
			TargetTreasureImgIdx = i * 2;
			iTargetStar = (i + 1) * 9 + (i == 3 ? 3 : 0);
			TargetStarCnt->SetText(FText::AsNumber(iTargetStar));
			break;
		}
		else if (TreasureStateArray[i] == ETreasureState::TREASURE_POSSIBLE)
		{
			TargetTreasureImgIdx = i * 2 + 1;
			if (i < MAXREWARDCNT - 1)
			{
				iTargetStar = (i + 1) * 9;
				TargetStarCnt->SetText(FText::AsNumber(iTargetStar));
			}
			else
			{
				iTargetStar = MAXSTAR;
				TargetStarCnt->SetText(FText::AsNumber(iTargetStar));
			}
			break;
		}
		if (i == 3 && TreasureStateArray[i] == ETreasureState::TREASURE_ACQUIRED)
		{
			TargetTreasureImgIdx = 8;
			iTargetStar = MAXSTAR;
			TargetStarCnt->SetText(FText::AsNumber(iTargetStar));
		}
	}

	TArray<UPaperSprite*> TexArray = RGAMEINSTANCE(this)->CampaignManager->TresureboxTexArray;
	if (TreasureButton && TexArray.IsValidIndex(TargetTreasureImgIdx))
	{
		TreasureButton->WidgetStyle.Normal.SetResourceObject(TexArray[TargetTreasureImgIdx]);
		TreasureButton->WidgetStyle.Hovered.SetResourceObject(TexArray[TargetTreasureImgIdx]);
		TreasureButton->WidgetStyle.Pressed.SetResourceObject(TexArray[TargetTreasureImgIdx]);
	}

	if (ProgressBar_Star)
	{
		float percent = FMath::Clamp<float>((float)iCurrentStarNum / (float)iTargetStar, 0.f, 1.f);
		ProgressBar_Star->SetPercent(percent);
	}

	if (Text_PossibleRewardCount)
	{
		Text_PossibleRewardCount->SetText(FText::AsNumber(PossibleRewardCnt));
	}

}

void UUP_Campaign_Tower::SetFloorMissionButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->MissionManager->GetMissionStatus().floor == 0)
		WidgetSwitcher_FloorMission->SetActiveWidgetIndex(1);
	else
		WidgetSwitcher_FloorMission->SetActiveWidgetIndex(0);
}

void UUP_Campaign_Tower::OnClickFloorMissionButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->MissionManager->GetMissionStatus().floor == 0)
	{
		FText desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000022"));
		desc = FText::Format(desc, FText::AsNumber(CurrentFloor));
		UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")), desc, ECommonPopupType::VE_OKCANCEL);
		popup->Button_OK->OnClicked.RemoveDynamic(this, &UUP_Campaign_Tower::RequestGetFloorMission);
		popup->Button_OK->OnClicked.AddDynamic(this, &UUP_Campaign_Tower::RequestGetFloorMission);
	}
	else
		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MissionSystem);
}

void UUP_Campaign_Tower::RequestGetFloorMission()
{
	UPacketFunctionLibrary::TOWER_FLOOR_MISSION_ACTIVE_RQ(CurrentFloor);
}

void UUP_Campaign_Tower::Render_OnFloorChanged(int32 Floor)
{
	PlayAni(TEXT("LeftRightBottom"));
	CurrentFloor = Floor;

	Invalidate();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->QuestManager->IsNavigationUsing())
	{
		RGameInstance->QuestManager->EndNavigation();

		OnClickGotoMissionContents();
	}
}

void UUP_Campaign_Tower::QuestImageOnStageBtnReset()
{
	for (int32 i = 0; i < NormalStageButtonArray.Num(); i++)
		NormalStageButtonArray[i]->Quest_Image_Normal->SetVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < EliteStageButtonArray.Num(); i++)
		EliteStageButtonArray[i]->Quest_Image_Hard->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_Campaign_Tower::Init_NormalStageButton()
{
	URGameInstance* GameInst = RGAMEINSTANCE(this);
	checkf(GameInst, TEXT("GameInst null!"));
	checkf(GameInst->CampaignManager, TEXT("CampaignManager null!"));

	int32 iSelectedIdx = 0;

	FTOWER_STATUS TowerStatus = GameInst->CampaignManager->TowerStatus;

	for (size_t i = 0; i < NormalStageButtonArray.Num(); ++i)
	{
		NormalStageButtonArray[i]->UP_Campaign_Tower = this;

		int32 iCurrentFloor = CurrentFloor;
		if (iCurrentFloor <= 0)
			continue;

		int32 iCurrentStageIdx = (iCurrentFloor - 1) * MAXSTAGE_FLOOR + i;
		
		checkf(FloorStageIDs.IsValidIndex(iCurrentStageIdx), TEXT("FloorStageID is not valid in table"));
		FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[iCurrentStageIdx]);

		int32 ReqStamina = FloorStageTable->Stamina;
		NormalStageButtonArray[i]->Text_Stamina->SetText(FText::FromString(FString::FromInt(ReqStamina)));

		FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
		/*if (CurrentFloor > Avatar.elevatedFloor)
		{
			EStageStateType eClearType = EStageStateType::VE_Locked;
			NormalStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			continue;
		}*/

		TArray<int32> StageClearArray = TowerStatus.clearGrades; // RGAMEINSTANCE(this)->CampaignManager->GetStageClearInfo();
		if (StageClearArray.IsValidIndex(iCurrentStageIdx) && StageClearArray[iCurrentStageIdx] != 0)
		{
			EStageStateType eClearType = EStageStateType(StageClearArray[iCurrentStageIdx]);
			NormalStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
		}
		else
		{
			FString OpenKeyStr = FloorStageTable->OpenKey.ToString();
			if (FloorStageTable->OpenKey.IsNone() || 
				(StageClearArray.IsValidIndex(FCString::Atoi(*OpenKeyStr)) && StageClearArray[FCString::Atoi(*OpenKeyStr)] != 0))
			{
				EStageStateType eClearType = EStageStateType::VE_NotPlayed;
				NormalStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			}
			else
			{
				EStageStateType eClearType = EStageStateType::VE_Locked;
				NormalStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			}
		}
		if (NormalStageButtonArray[i]->GetStageState() == EStageStateType::VE_NotPlayed)
		{
			NormalStageButtonArray[i]->SetSelected(false);
			NormalStageButtonArray[i]->OnButtonClick();
		}
		//set mission image on button
		FFLOOR_MISSION_STATUS floorMissionStatus = GameInst->MissionManager->GetMissionStatus();
		if (floorMissionStatus.floor == CurrentFloor)
		{
			for (int32 missionNum = 0; missionNum < floorMissionStatus.missions.Num(); missionNum++)
			{
				if (FloorStageIDs[iCurrentStageIdx] == FName(*floorMissionStatus.missions[missionNum].stageId))
					NormalStageButtonArray[i]->Quest_Image_Normal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}

void UUP_Campaign_Tower::Init_EliteStageButton()
{
	URGameInstance* GameInst = RGAMEINSTANCE(this);
	checkf(GameInst, TEXT("GameInst null!"));
	checkf(RGAMEINSTANCE(this)->CampaignManager, TEXT("CampaignManager null!"));
	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

	for (size_t i = 0; i < EliteStageButtonArray.Num(); ++i)
	{
		EliteStageButtonArray[i]->UP_Campaign_Tower = this;
		EliteStageButtonArray[i]->SetEliteMode();
		//EliteStageButtonArray[i]->Image_Selected->SetColorAndOpacity(FLinearColor(1.f, 0.f, 1.f));

		int32 iCurrentFloor = CurrentFloor;
		if (iCurrentFloor <= 0)
			continue;

		int32 iCurrentStageIdx = (iCurrentFloor - 1) * MAXSTAGE_FLOOR + i + 6;
		FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[iCurrentStageIdx]);

		int32 ReqStamina = FloorStageTable->Stamina;
		EliteStageButtonArray[i]->Text_Stamina_Elite->SetText(FText::FromString(FString::FromInt(ReqStamina)));

		FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
		/*if (CurrentFloor > Avatar.elevatedFloor)
		{
			EStageStateType eClearType = EStageStateType::VE_Locked;
			EliteStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			continue;
		}*/

		TArray<int32> StageClearArray = TowerStatus.clearGrades;// GameInst->CampaignManager->GetStageClearInfo();
		if (StageClearArray.IsValidIndex(iCurrentStageIdx) && StageClearArray[iCurrentStageIdx] != 0)
		{
			EStageStateType eClearType = EStageStateType(StageClearArray[iCurrentStageIdx]);
			EliteStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
		}
		else
		{
			FString OpenKeyStr = FloorStageTable->OpenKey.ToString();
			if (FloorStageTable->OpenKey.IsNone() || 
				(StageClearArray.IsValidIndex(FCString::Atoi(*OpenKeyStr)) && StageClearArray[FCString::Atoi(*OpenKeyStr)] != 0))
			{
				EStageStateType eClearType = EStageStateType::VE_NotPlayed;
				EliteStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			}
			else
			{
				EStageStateType eClearType = EStageStateType::VE_Locked;
				EliteStageButtonArray[i]->SetStageState(iCurrentStageIdx, eClearType);
			}
		}
		//set mission image on button
		FFLOOR_MISSION_STATUS floorMissionStatus = GameInst->MissionManager->GetMissionStatus();
		if (floorMissionStatus.floor == CurrentFloor)
		{
			for (int32 missionNum = 0; missionNum < floorMissionStatus.missions.Num(); missionNum++)
			{
				if (FloorStageIDs[iCurrentStageIdx] == FName(*floorMissionStatus.missions[missionNum].stageId))
					EliteStageButtonArray[i]->Quest_Image_Hard->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}



void UUP_Campaign_Tower::Toggle_ControlMode(bool isEliteMode)
{
	TowerStageInfo->SetSwitcherControlMode(isEliteMode);
}

void UUP_Campaign_Tower::OnClick_Treasure()
{
	

	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;
	int32 iCurrentFloor = CurrentFloor;
	FFloorInfo* FloorInfoTable = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(iCurrentFloor)));
	if (FloorInfoTable)
	{
		for (int32 i = 0; i < TreasureStateArray.Num(); ++i)
		{
			if (TreasureStateArray[i] == ETreasureState::TREASURE_POSSIBLE)
			{
				int32 treasureIdx = 0;
// 				if (i == 0)
// 					treasureIdx = FloorInfoTable->TreasureBoxKey1Idx;
// 				else if(i == 1)
// 					treasureIdx = FloorInfoTable->TreasureBoxKey2Idx;
// 				else if (i == 2)
// 					treasureIdx = FloorInfoTable->TreasureBoxKey3Idx;
// 				else
// 					treasureIdx = FloorInfoTable->TreasureBoxKey4Idx;
					
				UPacketFunctionLibrary::TOWER_TREASURE_BOX_RQ(treasureIdx);
				break;
			}
		}
		
	}
}


void UUP_Campaign_Tower::OnClick_HeroCard()
{
	URGameInstance*		RGameInstance		=	RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager		=	RGameInstance->RWidgetManager;

	checkf(RWidgetManager != nullptr, TEXT("[%s]WidgetManager is Empty"), __FUNCTION__);

	if (!RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_SingleRaid))
	{
#if WITH_EDITOR
		if (RGameInstance->GameEnvironmentVar.bCheckContentsUnlock)
#endif // WITH_EDITOR
		{
			FText TitleText;
			FText DescText;
			
			RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_SingleRaid, TitleText, DescText);
			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);

			return;
		}
	}

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_TowerBoss);
}

void UUP_Campaign_Tower::SetImportantMissionInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId).ToString()));
	if (pTableData)
	{
		ImportantMissionOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTableData->TargetStringKey));
		ImportantCurrentText->SetText(FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress));
		ImportantMaxText->SetText(FText::AsNumber(pTableData->ConditionGoal));
	}
	else
	{
		ImportantMissionOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Campaign_Tower::OnClickGotoMissionContents()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nbaseIndex = 0;
	int32 nCurrentQuest = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	int32 nFloor = 0;

	for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
	{
		nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
		if (nbaseIndex >= nCurrentQuest)
			break;
	}

	if(CurrentFloor == nFloor + 1)
	{
		int32 nTargetStageIndex = nCurrentQuest - (nFloor * 6) - 1;
		SetSelectTargetStageButton(nTargetStageIndex);
	}
	else
	{
		/*UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(nFloor + 1);*/
		RGameInstance->QuestManager->UseNavigation();
	}

// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
}

void UUP_Campaign_Tower::SetSelectTargetStageButton(int32 InTargetStageIndex)
{
	if (NormalStageButtonArray.IsValidIndex(InTargetStageIndex))
	{
		NormalStageButtonArray[InTargetStageIndex]->SetSelected(false);
		NormalStageButtonArray[InTargetStageIndex]->OnButtonClick();
	}
}

void UUC_TowerStageInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(CheckBox_Monster))
	{
		CheckBox_Monster->OnCheckStateChanged.Clear();
		CheckBox_Monster->OnCheckStateChanged.AddDynamic(this, &UUC_TowerStageInfo::OnClick_MonsterTab);
	}

	if (IsValid(CheckBox_Item))
	{
		CheckBox_Item->OnCheckStateChanged.Clear();
		CheckBox_Item->OnCheckStateChanged.AddDynamic(this, &UUC_TowerStageInfo::OnClick_ItemTab);
	}

	RGameInst = RGAMEINSTANCE(this);
}

void UUC_TowerStageInfo::OnClick_MonsterTab(bool isChecked)
{
	if (WidgetSwitcher_Monster && WidgetSwitcher_Item)
	{
		WidgetSwitcher_Monster->SetActiveWidgetIndex(ENABLE);
		WidgetSwitcher_Item->SetActiveWidgetIndex(DISABLE);
		ScrollView_Monster->SetVisibility(ESlateVisibility::Visible);
		ScrollView_Item->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_TowerStageInfo::OnClick_ItemTab(bool isChecked)
{
	if (WidgetSwitcher_Monster && WidgetSwitcher_Item)
	{
		WidgetSwitcher_Monster->SetActiveWidgetIndex(DISABLE);
		WidgetSwitcher_Item->SetActiveWidgetIndex(ENABLE);
		ScrollView_Monster->SetVisibility(ESlateVisibility::Collapsed);
		ScrollView_Item->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUC_TowerStageInfo::Set_ScrollItem_Pair()
{
	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(UP_Campaign_Tower->FloorStageIDs[UP_Campaign_Tower->CurrentSelectedStageIdx]);

	if (ScrollView_Monster)
	{
		TArray<FName> MonsterArray = FloorStageTable->MonsterIDList;
		if (MonsterArray.Num() > 0)
		{
			if (ScrollView_Monster->ScrollItemList.Num() == 0)
				ScrollView_Monster->Init_NoClear(7);

			if (ScrollView_Monster->ItemDataList.Num() == 0)
			{
				for (size_t i = 0; i < MonsterArray.Num(); ++i)
				{
					UMonsterScrollItemData* ScrollItemData = Cast<UMonsterScrollItemData>(ScrollView_Monster->AddScrollItemData(UMonsterScrollItemData::StaticClass()));

					if (!IsValid(ScrollItemData))
						continue;

					ScrollItemData->NPCID = MonsterArray[i];
					ScrollItemData->MonsterLevel = 1;
					ScrollItemData->IsBossMonster = UUIFunctionLibrary::IsBossNpc(MonsterArray[i]);
				}
			}
		}

		ScrollView_Monster->InvalidateData();
	}

	if (ScrollView_Item)
	{
		TArray<FITEM_ICON_INFO> ItemIconArray;
		UUIFunctionLibrary::GetRewardsIconInfoByPreview(FloorStageTable->ClearRewardTableKey, ItemIconArray);

		if (ItemIconArray.Num() > 0)
		{
			if (ScrollView_Item->ScrollItemList.Num() == 0)
				ScrollView_Item->Init_NoClear(7);

			if (ScrollView_Item->ItemDataList.Num() == 0)
			{
				for (size_t i = 0; i < ItemIconArray.Num(); ++i)
				{
					UItemBaseScrollItemData* ScrollItemData = Cast<UItemBaseScrollItemData>(ScrollView_Item->AddScrollItemData(UItemBaseScrollItemData::StaticClass()));

					if (!IsValid(ScrollItemData))
						continue;

					ScrollItemData->ItemIconInfo = ItemIconArray[i];
				}
			}
		}

		ScrollView_Item->InvalidateData();
	}
}

void UUC_TowerStageInfo::SetSwitcherControlMode(bool isEliteMode)
{
	if (WidgetSwitcher_ControlMode)
		WidgetSwitcher_ControlMode->SetActiveWidgetIndex(isEliteMode);
	if (WidgetSwitcher_ControlMode2)
		WidgetSwitcher_ControlMode2->SetActiveWidgetIndex(!isEliteMode);
}

void UUC_TowerStageInfo::Invalidate_ScrollView()
{
	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(UP_Campaign_Tower->FloorStageIDs[UP_Campaign_Tower->CurrentSelectedStageIdx]);

	if (ScrollView_Monster)
	{
		TArray<FName> MonsterArray = FloorStageTable->MonsterIDList;
		TArray<URScrollItemData*>& ItemDataArray = ScrollView_Monster->GetScrollItemAll();

		for (size_t i = 0; i < MonsterArray.Num(); ++i)
		{
			UMonsterScrollItemData* ScrollItemData = nullptr;
			if(i < ItemDataArray.Num() && ItemDataArray.IsValidIndex(i))
				ScrollItemData = Cast<UMonsterScrollItemData>(ItemDataArray[i]);
			else
				ScrollItemData = Cast<UMonsterScrollItemData>(ScrollView_Monster->AddScrollItemData(UMonsterScrollItemData::StaticClass()));

			if (!IsValid(ScrollItemData))
				continue;

			FCharacterTableInfo* NPCInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(MonsterArray[i]);
			if (NPCInfo)
			{
				ScrollItemData->NPCID = MonsterArray[i];
				ScrollItemData->MonsterLevel = NPCInfo->Level;
				ScrollItemData->IsBossMonster = UUIFunctionLibrary::IsBossNpc(MonsterArray[i]);
			}
		}
		ScrollView_Monster->InvalidateData();
	}

	if (ScrollView_Item)
	{
		TArray<FITEM_ICON_INFO> ItemIconArray;
		UUIFunctionLibrary::GetRewardsIconInfoByPreview(FloorStageTable->ClearRewardTableKey, ItemIconArray);

		TArray<URScrollItemData*>& ItemDataArray = ScrollView_Item->GetScrollItemAll();

		for (size_t i = 0; i < ItemIconArray.Num(); ++i)
		{
			UItemBaseScrollItemData* ScrollItemData = nullptr;
			if(i < ItemDataArray.Num() && ItemDataArray.IsValidIndex(i))
				ScrollItemData = Cast<UItemBaseScrollItemData>(ItemDataArray[i]);
			else
				ScrollItemData = Cast<UItemBaseScrollItemData>(ScrollView_Item->AddScrollItemData(UItemBaseScrollItemData::StaticClass()));

			if (!IsValid(ScrollItemData))
				continue;

			ScrollItemData->ItemIconInfo = ItemIconArray[i];
		}
		ScrollView_Item->InvalidateData();
	}
}

void UUC_TowerFloorSelect::NativeConstruct()
{
	Super::NativeConstruct();


	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Button_Drag->Slot);
	ButtonSizeY = CanvasPanelSlot->GetSize().Y;
	float PosY = CanvasPanelSlot->GetPosition().Y;
	float PosX = CanvasPanelSlot->GetPosition().X;
	
	Height_1Floor = ButtonSizeY / TOTAL_FLOOR;

	UCanvasPanelSlot* FullCanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
	LeftTopPos = FullCanvasPanelSlot->GetPosition().Y;
	FirstPosY = LeftTopPos + PosY;
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	FirstPosX = viewportSize.X/ViewportScale + FullCanvasPanelSlot->GetPosition().X + PosX;


	APlayerController* PC = UGameplayStatics::GetPlayerController(RGAMEINSTANCE(this)->GetWorld(), 0);
	if (PC)
	{
		PC->InputComponent->TouchBindings.Empty();
		PC->InputComponent->BindTouch(IE_Pressed, this, &UUC_TowerFloorSelect::CallBack_TouchBegin);
		PC->InputComponent->BindTouch(IE_Repeat, this, &UUC_TowerFloorSelect::CallBack_TouchOver);
		PC->InputComponent->BindTouch(IE_Released, this, &UUC_TowerFloorSelect::CallBack_TouchReleased);
	}
}




void UUC_TowerFloorSelect::CallBack_TouchBegin(ETouchIndex::Type TouchIndex, FVector Location)
{
	MousePos = *(FVector2D*)&Location;
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	MousePos /= ViewportScale;

//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("viewportsize X: ") + FString::FromInt(viewportSize.X) + TEXT(", ") + TEXT("viewportsize Y: ") + FString::FromInt(viewportSize.Y));
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("ViewportScale X: ") + FString::SanitizeFloat(ViewportScale));

	if (MousePos.X < FirstPosX || MousePos.Y < FirstPosY)
		return;
	if (!isDragState)
		isDragState = true;

	if (Overlay_Floor)
		Overlay_Floor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Update_FloorInfo();

}

void UUC_TowerFloorSelect::CallBack_TouchOver(ETouchIndex::Type TouchIndex, FVector Location)
{
	if (!isDragState)
		return;

	MousePos = *(FVector2D*)&Location;
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	MousePos /= ViewportScale;

	Update_FloorInfo();
}

void UUC_TowerFloorSelect::CallBack_TouchReleased(ETouchIndex::Type TouchIndex, FVector Location)
{
	if (!isDragState)
		return;
	else
		isDragState = false;

	MousePos = *(FVector2D*)&Location;
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	MousePos /= ViewportScale;

	if (Overlay_Floor)
		Overlay_Floor->SetVisibility(ESlateVisibility::Collapsed);

	FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
	UCampaignManager* CampaignMgr = RGAMEINSTANCE(this)->CampaignManager;
	if (IsValid(CampaignMgr))
	{
		int32 iTargetFloor = Calculate_CurrentFloorIdx();
		/*if (iTargetFloor <= Avatar.elevatedFloor)
		{
			UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(iTargetFloor);
			UUP_CampaignTower_Scrollview* TowerWidget = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidgetT<UUP_CampaignTower_Scrollview>(EWidgetBluePrintEnum::WBP_Campaign);
			if (IsValid(TowerWidget))
			{
				TowerWidget->MoveScroll_ToTargetFloor(iTargetFloor);
			}
		}
		else*/
		{
			UUP_CampaignTower_Scrollview* TowerWidget = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidgetT<UUP_CampaignTower_Scrollview>(EWidgetBluePrintEnum::WBP_Campaign);
			if (IsValid(TowerWidget))
			{
				TowerWidget->CanvasPanel_Message->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				TowerWidget->PlayAni(TEXT("Ani_UpStairFailed"));
			}

		}
	}

}

int32 UUC_TowerFloorSelect::Calculate_CurrentFloorIdx()
{
	

	TargetTextFloorPos = FMath::Clamp<float>(MousePos.Y - FirstPosY, 0 , ButtonSizeY);
	
	return FMath::Clamp<int32>(TOTAL_FLOOR - (MousePos.Y - FirstPosY) / Height_1Floor + 1, 1, TOTAL_FLOOR);
}

void UUC_TowerFloorSelect::Update_FloorInfo()
{
	FString str = FString::FromInt(Calculate_CurrentFloorIdx()) + TEXT("F");
	if (TextexpFloor)
		TextexpFloor->SetText(FText::FromString(str));

	if (Overlay_Floor)
	{
		FVector2D NewPos = Overlay_Floor->RenderTransform.Translation;
		NewPos.Y = TargetTextFloorPos;
		Overlay_Floor->SetRenderTranslation(NewPos);
	}
}
