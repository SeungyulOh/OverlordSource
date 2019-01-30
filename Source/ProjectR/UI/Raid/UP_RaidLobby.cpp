

#include "ProjectR.h"
#include "UP_RaidLobby.h"

#include "Global/RGameInstance.h"
#include "Global/RaidManager.h"
#include "Global/CampaignManager.h"
#include "Global/RealTimeModManager.h"
#include "Global/ContentsUnLockManager.h"

#include "GameInfo/RInventory.h"

#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/GamePlayBluePrintFunction.h"
#include "UP_Raid_tower_boss.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/RWidgetManager.h"

void UUP_RaidLobby::BindDelegate()
{
	if(variables.StageButtons.IsValidIndex(0) && IsValid(variables.StageButtons[0]))
		variables.StageButtons[0]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickSelectBoss0);
	if(variables.StageButtons.IsValidIndex(1) && IsValid(variables.StageButtons[1]))
		variables.StageButtons[1]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickSelectBoss1);
	if(variables.StageButtons.IsValidIndex(2) && IsValid(variables.StageButtons[2]))
		variables.StageButtons[2]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickSelectBoss2);

	if (variables.DifficultyButtons.IsValidIndex(0) && IsValid(variables.DifficultyButtons[0]))
		variables.DifficultyButtons[0]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickEasy);
	if (variables.DifficultyButtons.IsValidIndex(1) && IsValid(variables.DifficultyButtons[1]))
		variables.DifficultyButtons[1]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickNormal);
	if (variables.DifficultyButtons.IsValidIndex(2) && IsValid(variables.DifficultyButtons[2]))
		variables.DifficultyButtons[2]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickHard);
	if (variables.DifficultyButtons.IsValidIndex(3) && IsValid(variables.DifficultyButtons[3]))
		variables.DifficultyButtons[3]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickVeryHard);

	if (variables.DifficultyLockButtons.IsValidIndex(0) && IsValid(variables.DifficultyLockButtons[0]))
		variables.DifficultyLockButtons[0]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickLockEasy);
	if (variables.DifficultyLockButtons.IsValidIndex(1) && IsValid(variables.DifficultyLockButtons[1]))
		variables.DifficultyLockButtons[1]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickLockNormal);
	if (variables.DifficultyLockButtons.IsValidIndex(2) && IsValid(variables.DifficultyLockButtons[2]))
		variables.DifficultyLockButtons[2]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickLockHard);
	if (variables.DifficultyLockButtons.IsValidIndex(3) && IsValid(variables.DifficultyLockButtons[3]))
		variables.DifficultyLockButtons[3]->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickLockVeryHard);

	if (IsValid(variables.EnterButton))
		variables.EnterButton->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickEnter);
	if (IsValid(variables.JoinRoomButton))
		variables.JoinRoomButton->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickJoinRoom);
	if (IsValid(variables.CreateRoomButton))
		variables.CreateRoomButton->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickCreateRoom);
	if (IsValid(variables.QuickEnterButton))
		variables.QuickEnterButton->OnClicked.AddDynamic(this, &UUP_RaidLobby::OnClickQuickEnter);
}

void UUP_RaidLobby::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegate();

	SelectedBossIndex = 0;
	SelectedDifficulty = ERaidDifficultyType::EASY;

	SetRaidUI();

	// first select
	OnClickSelectBoss0();
	OnClickEasy();
}


void UUP_RaidLobby::NativeDestruct()
{
	if (variables.StageButtons.IsValidIndex(0) && IsValid(variables.StageButtons[0]))
		variables.StageButtons[0]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickSelectBoss0);
	if (variables.StageButtons.IsValidIndex(1) && IsValid(variables.StageButtons[1]))
		variables.StageButtons[1]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickSelectBoss1);
	if (variables.StageButtons.IsValidIndex(2) && IsValid(variables.StageButtons[2]))
		variables.StageButtons[2]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickSelectBoss2);

	if (variables.DifficultyButtons.IsValidIndex(0) && IsValid(variables.DifficultyButtons[0]))
		variables.DifficultyButtons[0]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickEasy);
	if (variables.DifficultyButtons.IsValidIndex(1) && IsValid(variables.DifficultyButtons[1]))
		variables.DifficultyButtons[1]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickNormal);
	if (variables.DifficultyButtons.IsValidIndex(2) && IsValid(variables.DifficultyButtons[2]))
		variables.DifficultyButtons[2]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickHard);
	if (variables.DifficultyButtons.IsValidIndex(3) && IsValid(variables.DifficultyButtons[3]))
		variables.DifficultyButtons[3]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickVeryHard);

	if (variables.DifficultyLockButtons.IsValidIndex(0) && IsValid(variables.DifficultyLockButtons[0]))
		variables.DifficultyLockButtons[0]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickLockEasy);
	if (variables.DifficultyLockButtons.IsValidIndex(1) && IsValid(variables.DifficultyLockButtons[1]))
		variables.DifficultyLockButtons[1]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickLockNormal);
	if (variables.DifficultyLockButtons.IsValidIndex(2) && IsValid(variables.DifficultyLockButtons[2]))
		variables.DifficultyLockButtons[2]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickLockHard);
	if (variables.DifficultyLockButtons.IsValidIndex(3) && IsValid(variables.DifficultyLockButtons[3]))
		variables.DifficultyLockButtons[3]->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickLockVeryHard);

	if (IsValid(variables.EnterButton))
		variables.EnterButton->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickEnter);
	if (IsValid(variables.JoinRoomButton))
		variables.JoinRoomButton->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickJoinRoom);
	if (IsValid(variables.CreateRoomButton))
		variables.CreateRoomButton->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickCreateRoom);
	if (IsValid(variables.QuickEnterButton))
		variables.QuickEnterButton->OnClicked.RemoveDynamic(this, &UUP_RaidLobby::OnClickQuickEnter);

	Super::NativeDestruct();
}

int32 UUP_RaidLobby::ConversionDifficultyTypetoInteger(ERaidDifficultyType InType)
{
	int32 nReturnValue = 0;
	switch (InType)
	{
	case ERaidDifficultyType::EASY:		nReturnValue = 0; break;
	case ERaidDifficultyType::NORMAL:	nReturnValue = 1; break;
	case ERaidDifficultyType::HARD:		nReturnValue = 2; break;
	case ERaidDifficultyType::VERYHARD:	nReturnValue = 3; break;
	default: break;
	}

	return nReturnValue;
}

void UUP_RaidLobby::RefreshRaidInfoUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 nDifficultyIndex = ConversionDifficultyTypetoInteger(SelectedDifficulty);
	TArray<FRaidStageTableInfo>& rRaidList = RGameInstance->RaidManager->RaidStageList[RGameInstance->RaidManager->TodayType];
	int32 nIndex = (SelectedBossIndex * 4) + nDifficultyIndex;

	if (rRaidList.IsValidIndex(nIndex))
	{
		FRaidStageTableInfo& rData = rRaidList[nIndex];
		BossKey = rData.RaidStageID;

// 		int32 nOpenKey = FCString::Atoi(*rData.OpenKey.ToString());
// 		if (RGameInstance->CampaignManager->TowerStatus.raidClearGrades.IsValidIndex(nOpenKey))
		{
			if (IsValid(variables.ParticipantCountText))
			{
				bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);
				FName TextKey = bMulti ? FName(TEXT("UI_Multi_EntryCount_String")) : FName(TEXT("UI_Single_EntryCount_String"));
				FText participantText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TextKey);

				variables.ParticipantCountText->SetText(participantText);
			}
			if (IsValid(variables.AttributeText))
				variables.AttributeText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, rData.AttributeTypeString));
			if (IsValid(variables.RecommendPowerText))
				variables.RecommendPowerText->SetText(FText::AsNumber(rData.RecommendPowerValue));

			TArray<FITEM_ICON_INFO> ItemIconArray;
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(rData.RewardPreviewKey, ItemIconArray);

			for (int32 i = 0; i < variables.RewardItems.Num(); ++i)
				variables.RewardItems[i]->SetVisibility(ESlateVisibility::Collapsed);

			for (int32 rewardIndex = 0; rewardIndex < ItemIconArray.Num(); ++rewardIndex)
			{
				if (variables.RewardItems.IsValidIndex(rewardIndex) && IsValid(variables.RewardItems[rewardIndex]))
				{
					variables.RewardItems[rewardIndex]->SetItemIconStructure(ItemIconArray[rewardIndex]);
					variables.RewardItems[rewardIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}
}
void UUP_RaidLobby::SetRaidUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if(RGameInstance->RaidManager->RaidStageList.Contains(RGameInstance->RaidManager->TodayType))
	{
		TArray<FRaidStageTableInfo>& rRaidList = RGameInstance->RaidManager->RaidStageList[RGameInstance->RaidManager->TodayType];

		// boss image
		for (int32 i = 0; i < variables.BossImages.Num(); ++i)
		{
			if(!IsValid(variables.BossImages[i]))
				continue;
			
			int32 nBossTableIndex = i * 4;
			if(rRaidList.IsValidIndex(nBossTableIndex))
			{
				FRaidStageTableInfo& rData = rRaidList[nBossTableIndex];
				UUIFunctionLibrary::SetImageBrushFromTable(rData.BossImage, variables.BossImages[i]);
			}
		}

		bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);

		if (IsValid(variables.ButtonSwitcher))
			variables.ButtonSwitcher->SetActiveWidgetIndex(bMulti ? 1 : 0);

		
		if (IsValid(variables.RemainTryCount))
		{
			int32 nRemainTicketCount = RGameInstance->RInventory->GetS_TOWERBOSS_F1();
			variables.RemainTryCount->SetText(FText::AsNumber(nRemainTicketCount));
		}

		if (IsValid(variables.RewardText))
		{
			FText DayOfWeekText = GetDayofWeekText(RGameInstance->RaidManager->TodayType);
			FWalletTableInfo* walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(rRaidList[0].WalletID);
			FText itemname = FText();
			if (walletInfo)
			{
				itemname = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, walletInfo->CommonStringKey);
			}
			if (!DayOfWeekText.IsEmpty())
			{
				FText rewardtext = FText();

				rewardtext = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Raid_RewardInfo_String"));
				rewardtext = FText::Format(rewardtext, DayOfWeekText, itemname);
				variables.RewardText->SetText(rewardtext);
			}
			else
				variables.RewardText->SetText(FText());
		}
		RefreshRaidInfoUI();
	}
}

FText UUP_RaidLobby::GetDayofWeekText(ERaidDayType InDayType)
{
	FName returnTextKey = NAME_None;
	switch (InDayType)
	{
	case ERaidDayType::MON:
		returnTextKey = TEXT("Day_Monday");
		break;
	case ERaidDayType::TUE:
		returnTextKey = TEXT("Day_Tuesday");
		break;
	case ERaidDayType::WED:
		returnTextKey = TEXT("Day_Wednesday");
		break;
	case ERaidDayType::THU:
		returnTextKey = TEXT("Day_Thursday");
		break;
	case ERaidDayType::FRI:
		returnTextKey = TEXT("Day_Friday");
		break;
	case ERaidDayType::SAT:
		returnTextKey = TEXT("Day_Saturday");
		break;
	case ERaidDayType::SUN:
		returnTextKey = TEXT("Day_Sunday");
		break;

// 	case ERaidDayType::WEEK:
	default:	break;
	}

	return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, returnTextKey);
}

void UUP_RaidLobby::SelectBoss(int32 InBossIndex)
{
	const int32 MaxStageCount = 3;
	if (variables.FocusingImages.Num() < MaxStageCount)
		return;

	// select image
	for (int32 i = 0; i < variables.FocusingImages.Num(); ++i)
	{
		variables.FocusingImages[i]->SetVisibility(ESlateVisibility::Collapsed);
		variables.DisableImages[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (variables.FocusingImages.IsValidIndex(InBossIndex))
	{
		variables.FocusingImages[InBossIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		variables.DisableImages[InBossIndex]->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	SelectedBossIndex = InBossIndex;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	RefreshRaidInfoUI();
}

void UUP_RaidLobby::SelectDiffiCulty(ERaidDifficultyType InDifficultyType)
{
	const int32 MaxDifficultyCount = 4;
	if (variables.DifficultyButtonSwitchers.Num() < MaxDifficultyCount)
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FRaidStageTableInfo>& rRaidList = RGameInstance->RaidManager->RaidStageList[RGameInstance->RaidManager->TodayType];
	
	bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);
	
	for (int32 i = 0; i < variables.DifficultyButtonSwitchers.Num(); ++i)
	{
		int32 nActive = 0;
		if(i > 0)
		{
			// difficulty unlock level check
			if(!RGameInstance->ContentsUnLockManager->IsUnlockRaidDifficulty(!bMulti, i + 1))
			{

				nActive = 2;
				/* 이전 난이도 클리어시 다음난이도 언락 조건체크..
				int32 nCheckIndex = (SelectedBossIndex * 4) + i;
				int32 nOpenkey = rRaidList.IsValidIndex(nCheckIndex) ? FCString::Atoi(*rRaidList[nCheckIndex].OpenKey.ToString()) : -1;

				// pre stage clear check
				if (RGameInstance->CampaignManager->TowerStatus.raidClearGrades.IsValidIndex(nOpenkey) &&
					(0 < RGameInstance->CampaignManager->TowerStatus.raidClearGrades[nOpenkey]))
				{
					nActive = 0;
				}
				//*/
			}
		}
		
		variables.DifficultyButtonSwitchers[i]->SetActiveWidgetIndex(nActive);
	}
	int32 nSwitcherIndex = 0;
	switch (InDifficultyType)
	{
	case ERaidDifficultyType::EASY:		nSwitcherIndex = 0; break;
	case ERaidDifficultyType::NORMAL:	nSwitcherIndex = 1; break;
	case ERaidDifficultyType::HARD:		nSwitcherIndex = 2; break;
	case ERaidDifficultyType::VERYHARD:	nSwitcherIndex = 3; break;
	}

	if (variables.DifficultyButtonSwitchers.IsValidIndex(nSwitcherIndex) && variables.DifficultyButtonSwitchers[nSwitcherIndex])
		variables.DifficultyButtonSwitchers[nSwitcherIndex]->SetActiveWidgetIndex(1);

	SelectedDifficulty = InDifficultyType;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RefreshRaidInfoUI();
}

void UUP_RaidLobby::OnClickEnter()
{
	/*
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	TArray<FRaidStageTableInfo>& rRaidList = RGameInstance->RaidManager->RaidStageList[RGameInstance->RaidManager->TodayType];
	int32 nDifficultyIndex = ConversionDifficultyTypetoInteger(SelectedDifficulty);
	int32 nIndex = (SelectedBossIndex * 4) + nDifficultyIndex;

	if (rRaidList.IsValidIndex(nIndex))
	{
		FRaidStageTableInfo& rData = rRaidList[nIndex];
		
		UBaseStageInfo* CurrentStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		//checkf(CurrentStageInfo, TEXT("CurrentStageInfo is Null"));

		CurrentStageInfo->ClearRewardTableKey = rData.RewardPreviewKey;
		CurrentStageInfo->MapTableKey = rData.MapTableKey;
//		CurrentStageInfo->FloorInfoId = FName(*FString::FromInt(FloorStageTable->FloorInfoId));
		CurrentStageInfo->StageIndex = rData.StageIdx;//FloorStageTable->StageIdx;
		CurrentStageInfo->StageID = BossKey;//FloorStageIDs[nStageIndex];

		if (rData.PlayMode.ToString().Contains(TEXT("M")))
		{
			CurrentStageInfo->bPlayAlone = true;
		}
		else
		{
			CurrentStageInfo->bPlayAlone = false;
		}

		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_DeckSetting);
	}
	
	//*/


	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = BossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(BossKey.ToString());
}

void UUP_RaidLobby::OnClickJoinRoom()
{
	if (variables.UC_party_keypad->GetVisibility() == ESlateVisibility::Collapsed)
	{
		variables.UC_party_keypad->Init();
		variables.UC_party_keypad->OnEnterPartyNumber.RemoveDynamic(this, &UUP_RaidLobby::OnEnterPartyNumber);
		variables.UC_party_keypad->OnEnterPartyNumber.AddDynamic(this, &UUP_RaidLobby::OnEnterPartyNumber);
		variables.UC_party_keypad->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_RaidLobby::OnClickCreateRoom()
{

}

void UUP_RaidLobby::OnClickQuickEnter()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = BossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(BossKey.ToString());
}

void UUP_RaidLobby::OnEnterPartyNumber(FString PartyID)
{
	if (PartyID.IsEmpty())
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RealTimeModManager->JoinNumberString = PartyID;
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = BossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(BossKey.ToString());
}

void UUP_RaidLobby::OnClickSelectBoss0()
{
	SelectBoss(0);
}

void UUP_RaidLobby::OnClickSelectBoss1()
{
	SelectBoss(1);
}

void UUP_RaidLobby::OnClickSelectBoss2()
{
	SelectBoss(2);
}

void UUP_RaidLobby::OnClickEasy()
{
	SelectDiffiCulty(ERaidDifficultyType::EASY);
}

void UUP_RaidLobby::OnClickNormal()
{
	SelectDiffiCulty(ERaidDifficultyType::NORMAL);
}

void UUP_RaidLobby::OnClickHard()
{
	SelectDiffiCulty(ERaidDifficultyType::HARD);
}

void UUP_RaidLobby::OnClickVeryHard()
{
	SelectDiffiCulty(ERaidDifficultyType::VERYHARD);
}

void UUP_RaidLobby::OnClickLockEasy()
{
}

void UUP_RaidLobby::OnClickLockNormal()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);
	EGamePlayModeTypeEnum type = bMulti ? EGamePlayModeTypeEnum::VE_MultiRaid : EGamePlayModeTypeEnum::VE_SingleRaid;

	FText TitleText;
	FText DescText;

	RGameInstance->ContentsUnLockManager->GetUnlockReasonText(type, TitleText, DescText, 2);
	UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
}

void UUP_RaidLobby::OnClickLockHard()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);
	EGamePlayModeTypeEnum type = bMulti ? EGamePlayModeTypeEnum::VE_MultiRaid : EGamePlayModeTypeEnum::VE_SingleRaid;

	FText TitleText;
	FText DescText;

	RGameInstance->ContentsUnLockManager->GetUnlockReasonText(type, TitleText, DescText, 3);
	UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
}

void UUP_RaidLobby::OnClickLockVeryHard()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bMulti = (RGameInstance->RaidManager->TodayType == ERaidDayType::WEEK);
	EGamePlayModeTypeEnum type = bMulti ? EGamePlayModeTypeEnum::VE_MultiRaid : EGamePlayModeTypeEnum::VE_SingleRaid;

	FText TitleText;
	FText DescText;

	RGameInstance->ContentsUnLockManager->GetUnlockReasonText(type, TitleText, DescText, 4);
	UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
}

