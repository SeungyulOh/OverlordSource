

#include "ProjectR.h"
#include "UP_StageSelect.h"

#include "Global/RGameInstance.h"
#include "Global/CampaignManager.h"
#include "Global/TableManager.h"
#include "Global/QuestManager.h"
#include "Global/ContentsUnLockManager.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "Network/PacketFunctionLibrary.h"

#include "UI/UIDefs.h"
#include "UI/UI_Campaign/CampaignStageButton.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UI/UI_Campaign/TowerFloorMapPopup.h"

void UUP_StageSelect::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegates();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UCampaignManager* CM = RGameInstance->CampaignManager;
	bool bHard = (CM->GetStageDifficulty() == EStageDifficultyEnum::Hard);
	//int32 nCurrentFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;
	int32 nCurrentFloor = bHard ? CM->GetCurrentHardFloor() : CM->GetCurrentNormalFloor();
	CM->TowerStatus.currentFloor = nCurrentFloor;

	if (!RGameInstance->ContentsUnLockManager->IsUnlockFloor(nCurrentFloor))
		CM->TowerStatus.currentFloor--;
	

	SetStageUI();

	if (RGameInstance->QuestManager->IsNavigationUsing())
	{
		RGameInstance->QuestManager->EndNavigation();
		int32 nCurrentQuest = CM->TowerStatus.floorQuestStatus.questId;
		int32 nbaseIndex = 0;
		int32 nFloor = 0;

		for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
		{
			nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
			if (nbaseIndex >= nCurrentQuest)
				break;
		}

		if (CM->TowerStatus.currentFloor != nFloor + 1)
			OnClickGotoMissionContents();
	}
}

void UUP_StageSelect::NativeDestruct()
{
	if (IsValid(variables.GotoMissionContentsButton))
		variables.GotoMissionContentsButton->OnClicked.Clear();
	if (IsValid(variables.DownFloorButton))
		variables.DownFloorButton->OnClicked.Clear();
	if (IsValid(variables.UpFloorButton))
		variables.UpFloorButton->OnClicked.Clear();
	if (IsValid(variables.RewardReceiveButton))
		variables.RewardReceiveButton->OnClicked.Clear();
	if (IsValid(variables.FloorSelectButton))
		variables.FloorSelectButton->OnClicked.Clear();
	
	RGAMEINSTANCE(this)->CampaignManager->OnClickFloorShortCut.RemoveDynamic(this, &UUP_StageSelect::OnChangeFloorbyShortCut);

	Super::NativeDestruct();
}

void UUP_StageSelect::BindDelegates()
{
	if (IsValid(variables.GotoMissionContentsButton))
	{
		variables.GotoMissionContentsButton->OnClicked.Clear();
		variables.GotoMissionContentsButton->OnClicked.AddDynamic(this, &UUP_StageSelect::OnClickGotoMissionContents);
	}

	if (IsValid(variables.DownFloorButton))
	{
		variables.DownFloorButton->OnClicked.Clear();
		variables.DownFloorButton->OnClicked.AddDynamic(this, &UUP_StageSelect::OnClickDownStairButton);
	}

	if (IsValid(variables.UpFloorButton))
	{
		variables.UpFloorButton->OnClicked.Clear();
		variables.UpFloorButton->OnClicked.AddDynamic(this, &UUP_StageSelect::OnClickUpStairButton);
	}

	if (IsValid(variables.RewardReceiveButton))
	{
		variables.RewardReceiveButton->OnClicked.Clear();
		variables.RewardReceiveButton->OnClicked.AddDynamic(this, &UUP_StageSelect::OnClickRewardReceiveButton);
	}

	if (IsValid(variables.FloorSelectButton))
	{
		variables.FloorSelectButton->OnClicked.Clear();
		variables.FloorSelectButton->OnClicked.AddDynamic(this, &UUP_StageSelect::OnClickFloorSelectButton);
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->CampaignManager->OnClickFloorShortCut.RemoveDynamic(this, &UUP_StageSelect::OnChangeFloorbyShortCut);
	RGameInstance->CampaignManager->OnClickFloorShortCut.AddDynamic(this, &UUP_StageSelect::OnChangeFloorbyShortCut);
}

void UUP_StageSelect::SetTitleText()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 nCurrentFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;
	if (IsValid(variables.CurrentFloorText))
		variables.CurrentFloorText->SetText(FText::AsNumber(nCurrentFloor));

	FFloorInfo* FloorInfoTable = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(nCurrentFloor)));
	if (FloorInfoTable)
	{
		if (IsValid(variables.FloorTitleText))
			variables.FloorTitleText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorInfoTable->FloorTitle));
	}
}

void UUP_StageSelect::SetDifficulty()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	StageDifficulty = RGameInstance->CampaignManager->GetStageDifficulty();

	switch (StageDifficulty)
	{
	case EStageDifficultyEnum::Normal:
	{
		if (IsValid(variables.ModeSwitcher))
			variables.ModeSwitcher->SetActiveWidgetIndex(0);

		if (variables.TitleBGImage)
			variables.TitleBGImage->SetColorAndOpacity(ColorNormal);
	}
	break;

	case EStageDifficultyEnum::Hard:
	{
		if (IsValid(variables.ModeSwitcher))
			variables.ModeSwitcher->SetActiveWidgetIndex(1);

		if (IsValid(variables.TitleBGImage))
			variables.TitleBGImage->SetColorAndOpacity(ColorHard);
	}
	break;
	default:
		break;
	}
}

void UUP_StageSelect::SetStageButtons()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nCurrentFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;

	for (int32 i = 0; i < variables.StageButtonList.Num(); ++i)
	{
		if (IsValid(variables.StageButtonList[i]))
		{
			UCampaignStageButton* pStage = variables.StageButtonList[i];
			if (pStage)
				pStage->SetData(nCurrentFloor, i, StageDifficulty);
		}
	}

}

void UUP_StageSelect::SetRewardInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	// calculate star count
	FTOWER_STATUS& rTowerStatus = RGameInstance->CampaignManager->TowerStatus;
	const int32 MaxStageCount = 12;
	const int32 MaxStarCount = 18;
	
	int32 nTotalFloorStageStars = 0;
	bool bHard = (StageDifficulty == EStageDifficultyEnum::Hard);
	int32 nFirstStageIdx = ((rTowerStatus.currentFloor - 1) * MaxStageCount);
	if (bHard)
		nFirstStageIdx += 6;

	for (int32 i = 0; i < 6; ++i)
	{
		int32 nStageIndex = nFirstStageIdx + i;
		if (rTowerStatus.clearGrades.IsValidIndex(nStageIndex))
		{
			nTotalFloorStageStars += rTowerStatus.clearGrades[nStageIndex];
		}
	}
	
	if (IsValid(variables.RewardProgressBar))
		variables.RewardProgressBar->SetPercent((float)nTotalFloorStageStars / (float)MaxStarCount);

	if(IsValid(variables.CurrentStarText))
		variables.CurrentStarText->SetText(FText::AsNumber(nTotalFloorStageStars));
	
	FFloorInfo* pFloorInfo = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FText::AsNumber(rTowerStatus.currentFloor).ToString()));
	if (!pFloorInfo)
		return;
	TArray<FName> RewardList;
	TArray<UImage*> RewardImageList;
	FName BonusHeroName = NAME_None;

	RewardList.Empty(2);
	RewardImageList.Empty(2);
	
	RewardImageList.Emplace(variables.RewardImage1);
	RewardImageList.Emplace(variables.RewardImage2);

// 	if (pFloorInfo)
	{
		if (bHard)
		{
			RewardList.Emplace(pFloorInfo->TreasureBoxKeyH1_Pre);
			RewardList.Emplace(pFloorInfo->TreasureBoxKeyH2_Pre);
			BonusHeroName = pFloorInfo->TreasureBoxKeyH2_Add;
		}
		else
		{
			RewardList.Emplace(pFloorInfo->TreasureBoxKeyN1_Pre);
			RewardList.Emplace(pFloorInfo->TreasureBoxKeyN2_Pre);
			BonusHeroName = pFloorInfo->TreasureBoxKeyN2_Add;
		}

		TArray<FITEM_ICON_INFO> ItemIconArray;

		for (int32 i = 0; i < RewardList.Num(); ++i)
		{
			if (RewardList[i] != NAME_None)
			{
				ItemIconArray.Reset();

				UUIFunctionLibrary::GetRewardsIconInfoByPreview(RewardList[i], ItemIconArray);

				if (ItemIconArray.Num() > 0)
				{
					FITEM_ICON_INFO& info = ItemIconArray[0];
					UUIFunctionLibrary::SetImageBrush(info.ItemIconRef, RewardImageList[i]);
				}
			}

		}
		
		if (IsValid(variables.RewardBonus))
		{
			if (BonusHeroName != NAME_None)
			{
				variables.RewardBonus->SetHeroIcon(BonusHeroName.ToString(), 1);
				variables.RewardBonus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				variables.RewardBonus->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

	}

	// set receive button
	{
		bool bEnable = false;
		int32 RewardperFloorCount = 4; // 층별 보상 수 (일반 + 정예)
		int32 nPossibleCount = 9;
		int32 nRewardIndex = (rTowerStatus.currentFloor - 1) * RewardperFloorCount;

		if (bHard)
			nRewardIndex += 2;

		for (int32 i = 0; i < 2; ++i)
		{
			if (nPossibleCount * (1 + i) <= nTotalFloorStageStars)
			{
				if (rTowerStatus.treasureBoxes.IsValidIndex(nRewardIndex + i))
				{
					if (!rTowerStatus.treasureBoxes[nRewardIndex + i])
					{
						bEnable = true;

						if (bHard)
							nFloorRewardIndex = (i == 0) ? pFloorInfo->TreasureBoxKeyH1Idx : pFloorInfo->TreasureBoxKeyH2Idx;
						else
							nFloorRewardIndex = (i == 0) ? pFloorInfo->TreasureBoxKeyN1Idx : pFloorInfo->TreasureBoxKeyN2Idx;

						break;
					}
				}
				else
				{
					bEnable = true;

					if (bHard)
						nFloorRewardIndex = (i == 0) ? pFloorInfo->TreasureBoxKeyH1Idx : pFloorInfo->TreasureBoxKeyH2Idx;
					else
						nFloorRewardIndex = (i == 0) ? pFloorInfo->TreasureBoxKeyN1Idx : pFloorInfo->TreasureBoxKeyN2Idx;

					break;
				}
			}
		}

		if (IsValid(variables.RewardReceiveButton))
			variables.RewardReceiveButton->SetIsEnabled(bEnable);
	}
}

void UUP_StageSelect::SetStageUI()
{
	SetTitleText();
	SetDifficulty();
	SetStageButtons();
	SetRewardInfo();

	SetImportantMissionInfo();
}

void UUP_StageSelect::SetImportantMissionInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId).ToString()));
	if (pTableData)
	{
		variables.ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTableData->TargetStringKey));
		variables.ImportantCurrentText->SetText(FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress));
		variables.ImportantMaxText->SetText(FText::AsNumber(pTableData->ConditionGoal));
		variables.ClearImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		int32 nLastIndex = UTableManager::GetInstancePtr()->GetFloorQuestRowsNames().Num() - 1;
		FFloorQuestTableInfo* pLastQuestData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(nLastIndex).ToString()));
		if (pLastQuestData)
		{
			variables.ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pLastQuestData->TargetStringKey));
			variables.ImportantCurrentText->SetText(FText::AsNumber(pLastQuestData->ConditionGoal));
			variables.ImportantMaxText->SetText(FText::AsNumber(pLastQuestData->ConditionGoal));
			variables.ClearImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UUP_StageSelect::OnClickGotoMissionContents()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 nCurrentQuest = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	if (nCurrentQuest == 0)
		return;
	int32 nCurrentFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;

	int32 nbaseIndex = 0;

	int32 nFloor = 0;

	for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
	{
		nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
		if (nbaseIndex >= nCurrentQuest)
			break;
	}

	if (nCurrentFloor == nFloor + 1)
	{
		int32 nTargetStageIndex = nCurrentQuest - (nFloor * 6) - 1;
		
		if(variables.StageButtonList.IsValidIndex(nTargetStageIndex))
		{
			variables.StageButtonList[nTargetStageIndex]->OnClickEnterButton();
		}
	}
	else
	{
		RGameInstance->CampaignManager->TowerStatus.currentFloor = nFloor + 1;
		SetStageUI();
	}
	//*/
}

void UUP_StageSelect::OnClickDownStairButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if(1 < RGameInstance->CampaignManager->TowerStatus.currentFloor)
	{
		RGameInstance->CampaignManager->TowerStatus.currentFloor--;
		SetStageUI();
	}
}

void UUP_StageSelect::OnClickUpStairButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (25 <= RGameInstance->CampaignManager->TowerStatus.currentFloor)
		return;

	const int32 StageCountperFloor = 12;

	int32 nCurrntFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;
	int32 nNormalIndex = ((nCurrntFloor - 1) * StageCountperFloor) + 6;
	int32 nHardIndex = nNormalIndex + 6;
	int32 nStageIndex = (StageDifficulty == EStageDifficultyEnum::Normal) ? nNormalIndex : nHardIndex;

	if (RGameInstance->CampaignManager->TowerStatus.clearGrades.IsValidIndex(nStageIndex-1))
	{
		if (0 < RGameInstance->CampaignManager->TowerStatus.clearGrades[nStageIndex-1])
		{
			if(RGameInstance->ContentsUnLockManager->IsUnlockFloor(RGameInstance->CampaignManager->TowerStatus.currentFloor + 1))
			{
				RGameInstance->CampaignManager->TowerStatus.currentFloor++;
				SetStageUI();
			}
			else
			{
				FText TitleText;
				FText DescText;

				RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, TitleText, DescText, RGameInstance->CampaignManager->TowerStatus.currentFloor + 1);
				UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
			}
		}
	}
 }

void UUP_StageSelect::OnClickRewardReceiveButton()
{
	UPacketFunctionLibrary::TOWER_TREASURE_BOX_RQ(nFloorRewardIndex);

	variables.RewardReceiveButton->SetIsEnabled(false);
}

void UUP_StageSelect::OnClickFloorSelectButton()
{
	if(IsValid(variables.FloorSelectPopup))
	{
		variables.FloorSelectPopup->SetCurrentFloorInfo();
		variables.FloorSelectPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_StageSelect::OnChangeFloorbyShortCut(int32 InFloor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if(RGameInstance->CampaignManager->TowerStatus.currentFloor != InFloor)
	{
		RGameInstance->CampaignManager->TowerStatus.currentFloor = InFloor;
		SetStageUI();
	}
}
