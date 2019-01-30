
#include "ProjectR.h"
#include "UC_MissionScrollItem.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"
#include "Global/MissionManager.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/UC_ItemIcon.h"
#include "Table/DailyMissionTableInfo.h"
#include "Global/CampaignManager.h"

void UUC_MissionScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_MissionScrollItem::InvalidateData()
{
	FFloorMissionTable* rowData = UTableManager::GetInstancePtr()->GetFloorMissionTable(FName(*missionData.id));
	if (!rowData)
		return;
	SetMissionText(rowData->MissionTitle_StringKey, rowData->MissionDocument_StringKey);
	SetProgressState(missionData);
	SetRewardIcon(rowData->CurrencyType, missionData.rewardAmt);
}

void UUC_MissionScrollItem::SetMissionText(FName InTitleKey, FName InDescKey)
{
	FText TitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InTitleKey);
	FText DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InDescKey);
	FFloorStageDetail* stageData = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FName(*missionData.stageId));
	if (!stageData)
		return;

	FText StageName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, stageData->IngameSubTitle);
	switch (missionData.cls)
	{
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::TANKER:
		DescText = FText::Format(DescText, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Filter_HeroClass01")), StageName, FText::AsNumber(missionData.goal));
		break;
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::MELEEDEALER:
		DescText = FText::Format(DescText, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Filter_HeroClass02")), StageName, FText::AsNumber(missionData.goal));
		break;
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::RANGEDEALER:
		DescText = FText::Format(DescText, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Filter_HeroClass03")), StageName, FText::AsNumber(missionData.goal));
		break;
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::SUPPORTER:
		DescText = FText::Format(DescText, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Filter_HeroClass05")), StageName, FText::AsNumber(missionData.goal));
		break;
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::MAGICDEALER:
		DescText = FText::Format(DescText, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Filter_HeroClass04")), StageName, FText::AsNumber(missionData.goal));
		break;
	case EFLOOR_MISSION_CLASS_CONDITION_TYPE::NONE:
	{
		if (InDescKey == TEXT("DM_DI010003") || InDescKey == TEXT("DM_DI010004"))
		{
			DescText = FText::Format(DescText, FText::AsNumber(missionData.goal));
		}
		else
		{
			DescText = FText::Format(DescText, StageName, FText::AsNumber(missionData.goal));
		}
		break;
	}
	default:
		break;
	}
	MissionNameText->SetText(TitleText);
	MissionDescText->SetText(DescText);
}

void UUC_MissionScrollItem::SetProgressState(FFLOOR_MISSION missionStatus)
{
	CurrentProgressText->SetText(FText::AsNumber(missionStatus.current));
	MaxProgressText->SetText(FText::AsNumber(missionStatus.goal));

	float fPercent = (float)missionStatus.current / (float)missionStatus.goal;
	MissionProgressBar->SetPercent(fPercent);

	if (1.0f <= fPercent)
	{
		ButtonText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Quest_Desc_7")));

		RequestRewardButton->OnClicked.RemoveDynamic(this, &UUC_MissionScrollItem::OnRequestRewardButtonClick);
		RequestRewardButton->OnClicked.AddDynamic(this, &UUC_MissionScrollItem::OnRequestRewardButtonClick);
	}
	else
	{
		ButtonText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UC_QuestList_ShortCut")));

		RequestRewardButton->OnClicked.RemoveDynamic(this, &UUC_MissionScrollItem::OnDirectLinkButtonClick);
		RequestRewardButton->OnClicked.AddDynamic(this, &UUC_MissionScrollItem::OnDirectLinkButtonClick);
	}
}

void UUC_MissionScrollItem::SetRewardIcon(FName InCurrencyType, int32 InCurrencyValue)
{
	FWalletTableInfo* walletData = UTableManager::GetInstancePtr()->GetWalletRow(InCurrencyType);
	if (!walletData)
		return;
	RewardIcon->Refresh(walletData->ItemIconKey, InCurrencyValue, false, 0);
}

void UUC_MissionScrollItem::OnDirectLinkButtonClick()
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	if (!TableManager)
		return;
	FFloorMissionTable* rowData = TableManager->GetFloorMissionTable(FName(*missionData.id));
	FFloorStageDetail* stageData = TableManager->GetFloorStageDetailRow(FName(*missionData.stageId));
	if (!rowData)
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UCampaignManager* CampaignManager = RGameInstance->CampaignManager;
	UMissionManager* MissionManager = RGameInstance->MissionManager;
	if (!CampaignManager || !MissionManager)
		return;

	MissionManager->StageTargetFloor = FName(*missionData.stageId);
	CampaignManager->TowerStatus.currentFloor = rowData->FloorInfoId;

	if (stageData->PlayType.ToString().Contains(TEXT("S")) || stageData->PlayType.ToString().Contains(TEXT("M")))
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_TowerBossLobby);
	else
		UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
}

void UUC_MissionScrollItem::OnRequestRewardButtonClick()
{
	UPacketFunctionLibrary::TOWER_FLOOR_MISSION_REWARD_RQ(missionData.id);
}