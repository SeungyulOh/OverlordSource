// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_GuildRaidMain.h"

#include "GlobalIntegrated.h"

#include "GameInfo/GuildInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_ScrollItemData.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_SeasonInfo.h"
#include "UI/Guild/GuildRaid/UC_GuildRaidSeasonReward.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_Setting.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_MemberData.h"
#include "UI/Common/MonsterScrollItemData.h"
#include "UI/Common/RewardsScrollItemData.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_StageButton.h"

void UUP_GuildRaidMain::NativeConstruct()
{
	Super::NativeConstruct();
// 	Init();

}

void UUP_GuildRaidMain::NativeDestruct()
{
// 	if(IsValid(RewardInfoScrollView))
// 	{
// 		RewardInfoScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectRewardInfoScrollItem);
// 	}
// 
// 	if(IsValid(GetableRewardScrollView))
// 	{
// 		GetableRewardScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectGetableRewardScrollItem);
// 	}
// 
// 	if(IsValid(MonsterScrollView))
// 	{
// 		MonsterScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectApperMonsterScrollItem);
// 	}
// 	if (IsValid(MemberHistoryScrollView))
// 	{
// 		MemberHistoryScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectMemberHistoryScrollItem);
// 	}
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->EventManager)
// 	{
// 		RGameInstance->EventManager->OnRefreshGuildRaidInfo.RemoveDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidUI);	
// 		RGameInstance->EventManager->OnRefreshGuildRaidHistory.RemoveDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidHistory);
// 		RGameInstance->EventManager->OnRefreshGuildRaidRealServerInfo.RemoveDynamic(this, &UUP_GuildRaidMain::OnReceiveRealServerInfo);
// 		RGameInstance->EventManager->OnSelectDifficulty.RemoveDynamic(this, &UUP_GuildRaidMain::OnClick_Difficulty);
// 	}
	
	Super::NativeDestruct();
}

void UUP_GuildRaidMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

// 	SeasonTimeTick(InDeltaTime);
// 	OpenRaidTimeTick(InDeltaTime);
}

// void UUP_GuildRaidMain::SeasonTimeTick(float InDeltaTime)
// {
// 	if (bSeasonTimeCount)
// 	{
// 		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
// // 		nCurrentSeasonOutTime += InDeltaTime;
// 
// 		if (CurrentTime.GetTicks() - nCurrentSeasonOutTime >= FTimespan::FromSeconds(1.0f).GetTicks())
// 		{
// 			nCurrentSeasonOutTime = CurrentTime.GetTicks();
// 
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			if (RGameInstance->GuildInfo == nullptr)
// 				return;
// 			int32& nRemainTime = RGameInstance->GuildInfo->RemainTime;
// 			int Hour = 0;
// 			int Min = 0;
// 			int Sec = 0;
// 
// 			--nRemainTime;
// 
// 			if (nRemainTime > 0)
// 			{
// 				Hour = nRemainTime / 3600;
// 				Min = (nRemainTime % 3600) / 60;
// 				Sec = nRemainTime % 60;
// 			}
// 
// 			bool bSeasonEnd = ((Hour == 0) && (Min == 0) && (Sec == 0));
// 			if (bSeasonEnd)
// 			{
// 				bSeasonTimeCount = false;
// 				UPacketFunctionLibrary::GUILD_RAID_STATUS_RQ();
// 			}
// 
// 			SetSeasonOutTimeText(Hour, Min, Sec);
// 		}
// 	}
// }
// 
// void UUP_GuildRaidMain::OpenRaidTimeTick(float InDeltaTime)
// {
// 	if (bOpenRaidTimeCount)
// 	{
// 		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
// 		//fCurrentOpenRaidTime += InDeltaTime;
// 		if (CurrentTime.GetTicks() - nCurrentOpenRaidTime >= FTimespan::FromSeconds(1.0f).GetTicks())
// 		{
// 			nCurrentOpenRaidTime = CurrentTime.GetTicks();
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			if (RGameInstance->GuildInfo == nullptr)
// 				return;
// 			if (RGameInstance->GuildInfo->OpenRaid.isOpen == false)
// 			{
// 				SetOpenRaidTimeText(0, 0);
// 				return;
// 			}
// 			int32& nRemainTime = RGameInstance->GuildInfo->OpenRaid.left;
// 			int Min = 0;
// 			int Sec = 0;
// 
// 			--nRemainTime;
// 
// 			if (nRemainTime > 0)
// 			{
// 				Min = (nRemainTime % 3600) / 60;
// 				Sec = nRemainTime % 60;
// 			}
// 
// 			bool bCloseRaid = (nRemainTime <= 0);
// 			if (bCloseRaid)
// 			{
// 				bOpenRaidTimeCount = false;
// 				RGameInstance->GuildInfo->OpenRaid.isOpen = false;
// 				UPacketFunctionLibrary::GUILD_RAID_STATUS_RQ();
// 			}
// 			
// 			SetOpenRaidTimeText(Min, Sec);
// 		}
// 	}
// 
// }
// 
// void UUP_GuildRaidMain::Init()
// {
// 	if (IsValid(MemberHistoryScrollView))
// 	{
// 		MemberHistoryScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectMemberHistoryScrollItem);
// 		MemberHistoryScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_GuildRaidMain::OnSelectMemberHistoryScrollItem);
// 	}
// 	if (IsValid(RewardInfoScrollView))
// 	{
// 		RewardInfoScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectRewardInfoScrollItem);
// 		RewardInfoScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_GuildRaidMain::OnSelectRewardInfoScrollItem);
// 	}
// 	if (IsValid(GetableRewardScrollView))
// 	{
// 		GetableRewardScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectGetableRewardScrollItem);
// 		GetableRewardScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_GuildRaidMain::OnSelectGetableRewardScrollItem);
// 	}
// 	if (IsValid(MonsterScrollView))
// 	{
// 		MonsterScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_GuildRaidMain::OnSelectApperMonsterScrollItem);
// 		MonsterScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_GuildRaidMain::OnSelectApperMonsterScrollItem);
// 	}
// 	
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->EventManager)
// 	{
// 		RGameInstance->EventManager->OnRefreshGuildRaidInfo.RemoveDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidUI);
// 		RGameInstance->EventManager->OnRefreshGuildRaidInfo.AddDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidUI);
// 
// 		RGameInstance->EventManager->OnRefreshGuildRaidRealServerInfo.RemoveDynamic(this, &UUP_GuildRaidMain::OnReceiveRealServerInfo);
// 		RGameInstance->EventManager->OnRefreshGuildRaidRealServerInfo.AddDynamic(this, &UUP_GuildRaidMain::OnReceiveRealServerInfo);
// 	
// 		RGameInstance->EventManager->OnSelectDifficulty.RemoveDynamic(this, &UUP_GuildRaidMain::OnClick_Difficulty);
// 		RGameInstance->EventManager->OnSelectDifficulty.AddDynamic(this, &UUP_GuildRaidMain::OnClick_Difficulty);
// 
// 		RGameInstance->EventManager->OnRefreshGuildRaidHistory.RemoveDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidHistory);
// 		RGameInstance->EventManager->OnRefreshGuildRaidHistory.AddDynamic(this, &UUP_GuildRaidMain::OnRefreshRaidHistory);
// 	}
// 
// 	if (RaidSeasonReward)
// 	{
// 		RaidSeasonReward->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// 
// 	if (IsValid(MemberHistoryCountMax))
// 	{
// 		MemberHistoryCountMax->SetText(FText::AsNumber(50));
// 	}
// 
// 	SetGuildRaidStageInfos(RGameInstance->GuildInfo->RaidStatusInfos);
// 	CurrentRaidHistory = RGameInstance->GuildInfo->RaidHistorys;
// 	RaidBossType = ERaidBossTypeEnum::VE_RaidBoss_1;
// 	BossTypeIndex = static_cast<int32>(ERaidBossTypeEnum::VE_RaidBoss_1);
// 
// 	if (RGameInstance->GuildInfo->OpenRaid.isOpen)
// 	{
// 		bOpenRaidTimeCount = true;
// 		int32 modId = FCString::Atoi(*RGameInstance->GuildInfo->OpenRaid.modId);
// 		RaidBossType = static_cast<ERaidBossTypeEnum>((modId - 1) / 6);
// 		BossTypeIndex = ((modId - 1) / 6);
// 	}
// 
// 	SetRaidBossTab(BossTypeIndex);
// 
// 	SetStageButton(true);
// 	SetRaidBossStatus();
// 	SetOpenRaidTimeText(0, 0);
// 	
// 	UPacketFunctionLibrary::GUILD_RAID_STATUS_RQ();
// 	if (RaidBossInfo.IsValidIndex(BossTypeIndex))
// 	{
// 		int nBossLevelIndex = GetModID();
// 		UPacketFunctionLibrary::GUILD_RAID_CHALLENGE_RQ(FString::FromInt(nBossLevelIndex));
// 	}
// }
// 
// void UUP_GuildRaidMain::SetGuildRaidStageInfos(TArray<FGUILD_RAID>& rRaidInfos)
// {
// 	RaidBossInfo.Empty();
// 	RaidBossInfo = rRaidInfos;
// 	//*
// 	if (RaidBossInfo.Num() != 3)
// 	{
// 		int bCheck[3] = {1,7,13};
// 		for (FGUILD_RAID& info : RaidBossInfo)
// 		{
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				if ((FCString::Atoi(*info.modId) - info.grade + 1) == bCheck[i])
// 					bCheck[i] = 0;
// 			}
// 		}
// 
// 		for (int i = 0; i < 3; ++i)
// 		{
// 			if(bCheck[i] != 0)
// 			{
// 				FGUILD_RAID AllCompleteLevelBossData;
// 				AllCompleteLevelBossData.modId = FText::Format(FText::FromString("{0}"), FText::AsNumber(bCheck[i])).ToString();
// 				AllCompleteLevelBossData.grade = -1;
// 				AllCompleteLevelBossData.isOpen = false;
// 				AllCompleteLevelBossData.left = -1;
// 				AllCompleteLevelBossData.members = 0;
// 				RaidBossInfo.EmplaceAt(i, AllCompleteLevelBossData);
// 			}
// 		}
// 		
// 	}
// 	//*/
// }
// void UUP_GuildRaidMain::SetStageButton(bool bInit /*= false*/)
// {
// 	int32 index = 0;
// 	for (auto& StageButton : Button_Stages)
// 	{
// 		if(bInit)
// 			StageButton->InitButton(index + 1);
// 
// 		if (index == RaidBossDifficultyIndex)
// 		{
// 			StageButton->SelectStage(true);
// 		}
// 		else
// 		{
// 			StageButton->SelectStage(false);
// 		}
// 
// 		++index;
// 	}
// }
// 
// void UUP_GuildRaidMain::SetRaidBossTab(int32 InBossIndex)
// {
// 	if (IsValid(BossRadioGroup))
// 	{
// 		BossRadioGroup->SetActiveRadioIndex(InBossIndex);
// 	}
// 
// 	for (int i = 0; i < BossStatus.Num(); ++i)
// 	{
// 		UWidgetSwitcher* tab = BossStatus[i];
// 		tab->SetActiveWidgetIndex(RaidBossInfo[i].isOpen ? 0 : 1);
// 	}
// 
// 	int32 nModId = 0;
// 	if(RaidBossInfo[InBossIndex].grade > 0)
// 		nModId = FCString::Atoi(*RaidBossInfo[InBossIndex].modId);
// 
// 	RaidBossDifficultyIndex = (nModId % 6) - 1;
// 
// 
// 	SetRaidBossInfo(InBossIndex);
// 	SetStageButton();
// 	SetBossThumnail(RaidBossType);
// 
// 	InvalidateSeasonInfo();
// 	InvalidateScrollView();
// 	InvalidateRaidSetting();
// 
// 	SetMemberHistory();
// }
// 
// void UUP_GuildRaidMain::SetRaidBossInfo(int32 InActiveBossType)
// {
// 	if (RaidBossInfo.IsValidIndex(InActiveBossType) == false)
// 		return;
// 
// 	FGUILD_RAID& info = RaidBossInfo[InActiveBossType];
// 	int32 SelectedmodID = GetModID();
// 	int32 CurrentmodID = FCString::Atoi(*info.modId);
// 	if (IsAbleSummonBoss(CurrentmodID) == false)
// 	{
// 		if (IsValid(LocDisableText))
// 		{
// 			FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_GuildRaid_0067");
// 			int32 nLimitLevel = GetBossSummonLimitLevel(InActiveBossType);
// 			LocDisableText->SetText(FText::Format(text, FText::AsNumber(nLimitLevel)));
// 		}
// 		if (IsValid(EnterButtonSwitcher))
// 			EnterButtonSwitcher->SetActiveWidgetIndex(2);
// 	}
// 	else if (SelectedmodID != CurrentmodID)
// 	{
// 		if (IsValid(LocDisableText))
// 		{
// 			if(SelectedmodID < CurrentmodID)	// complete
// 				LocDisableText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_GuildRaid_0062"));
// 			else
// 				LocDisableText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_GuildRaid_0063"));
// 		}
// 
// 		if (IsValid(EnterButtonSwitcher))
// 			EnterButtonSwitcher->SetActiveWidgetIndex(2);
// 
// 	}
// 	else // SelectedmodID == CurrentmodID
// 	{
// 		if (info.isOpen)
// 		{
// 			if (IsValid(EnterButtonSwitcher))
// 				EnterButtonSwitcher->SetActiveWidgetIndex(0);
// 		}
// 		else
// 		{
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			
// 			{
// 				if (RGameInstance->GuildInfo &&
// 					(RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MASTER ||
// 						RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::ELDER))
// 				{
// 					if (IsValid(EnterButtonSwitcher))
// 						EnterButtonSwitcher->SetActiveWidgetIndex(1);
// 				}
// 				else
// 				{
// 					if (IsValid(EnterButtonSwitcher))
// 						EnterButtonSwitcher->SetActiveWidgetIndex(0);
// 				}
// 			}
// 		}
// 	}
// 
// 
// }
// 
// bool UUP_GuildRaidMain::IsAbleSummonBoss(int32 InModID)
// {
// 	int32 nBossGrade = InModID / 6;
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->GuildInfo == nullptr)
// 		return false;
// 
// 	const FGUILD_INFO& GuildInfo = RGameInstance->GuildInfo->MyGuildInfo;
// 
// 	switch (nBossGrade)
// 	{
// 	case 0:
// 		return (BOSS_SUMMON_LIMIT_LEVEL1 <= GuildInfo.level);
// 	case 1:
// 		return (BOSS_SUMMON_LIMIT_LEVEL2 <= GuildInfo.level);
// 	case 2:
// 		return (BOSS_SUMMON_LIMIT_LEVEL3 <= GuildInfo.level);
// 	default:
// 		break;
// 	}
// 	return false;
// }
// 
// int32 UUP_GuildRaidMain::GetBossSummonLimitLevel(int32 InBossType)
// {
// 	if (InBossType == 0)
// 		return BOSS_SUMMON_LIMIT_LEVEL1;
// 	else if (InBossType == 1)
// 		return BOSS_SUMMON_LIMIT_LEVEL2;
// 	else if (InBossType == 2)
// 		return BOSS_SUMMON_LIMIT_LEVEL3;
// 	else
// 		return 0;
// }
// 
// void UUP_GuildRaidMain::SetUIbySeasonStatus(bool bSeasonEnd)
// {
// 	for (UWidget* pWidget : SeasonOut_TimeControls)
// 	{
// 		pWidget->SetVisibility(bSeasonEnd ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
// 	}
// }
// 
// void UUP_GuildRaidMain::SetMemberHistory()
// {
// 	if (IsValid(MemberHistoryCount))
// 	{
// 		MemberHistoryCount->SetText(FText::AsNumber(CurrentRaidHistory.Num()));
// 	}
// }
// 
// void UUP_GuildRaidMain::ShowSeasonRewardPopup(int32 RewardIndex, bool NextRewardIndex)
// {
// 	if (IsValid(RaidSeasonReward))
// 	{
// 		RaidSeasonReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		RaidSeasonReward->Init(RewardIndex, NextRewardIndex);
// 	}
// }
// 
// void UUP_GuildRaidMain::OnSelectRewardInfoScrollItem(URScrollItemData* selectItem)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// }
// 
// void UUP_GuildRaidMain::OnSelectGetableRewardScrollItem(URScrollItemData* selectItem)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// }
// 
// void UUP_GuildRaidMain::OnSelectApperMonsterScrollItem(URScrollItemData* selectItem)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// }
// 
// void UUP_GuildRaidMain::OnSelectMemberHistoryScrollItem(URScrollItemData* selectItem)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// }
// 
// void UUP_GuildRaidMain::OnRefreshRaidUI()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->GuildInfo)
// 	{
// 		SetGuildRaidStageInfos(RGameInstance->GuildInfo->RaidStatusInfos);
// 		if(RGameInstance->GuildInfo->OpenRaid.isOpen)
// 			bOpenRaidTimeCount = true;
// 
// 		SetRaidBossStatus();
// 	}
// 
// 	SetRaidBossTab(BossTypeIndex);
// 	InvalidateSeasonInfo();
// 	InvalidateScrollView();
// 	InvalidateRaidSetting();
// 	
// // 	if (RaidBossInfo.IsValidIndex(BossTypeIndex))
// // 	{
// // 		int nBossLevelIndex = GetModID();
// // 		UPacketFunctionLibrary::GUILD_RAID_CHALLENGE_RQ(this, FString::FromInt(nBossLevelIndex));
// // 	}
// }
// 
// int32 UUP_GuildRaidMain::GetModID()
// {
// 	switch (RaidBossType)
// 	{
// 	case ERaidBossTypeEnum::VE_RaidBoss_1:
// 		return 1 + RaidBossDifficultyIndex;
// 	case ERaidBossTypeEnum::VE_RaidBoss_2:
// 		return 7 + RaidBossDifficultyIndex;
// 	case ERaidBossTypeEnum::VE_RaidBoss_3:
// 		return 13 + RaidBossDifficultyIndex;
// 	default:
// 		break;
// 	}
// 
// 	return 0;
// }
// 
// void UUP_GuildRaidMain::OnRefreshRaidHistory()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		SetGuildRaidStageInfos(RGameInstance->GuildInfo->RaidStatusInfos);
// 		CurrentRaidHistory = RGameInstance->GuildInfo->RaidHistorys;
// 	}
// 	InvalidateSeasonInfo();
// 	SetMemberHistory();
// 	InvalidateScrollView();
// }
// 
// void UUP_GuildRaidMain::OnReceiveRealServerInfo()
// {
// #ifdef WITH_EDITOR
// 	UE_LOG(LogUI, Log, TEXT("Callback UUP_GuildRaidMain::OnReceiveRealServerInfo"));
// #endif // WITH_EDITOR
// 
// }
// 
// void UUP_GuildRaidMain::InvalidateScrollView()
// {
// 	//InvalidateRaidListScrollView();
// 	InvalidateMemberHistoryScrollView();
// 	InvalidateRewardInfoScrollView();
// 	InvalidateGetableRewardScrollView();
// 	InvalidateMonsterScrollView();
// }
// 
// void UUP_GuildRaidMain::InvalidateMemberHistoryScrollView()
// {
// 	if (RaidBossInfo.IsValidIndex(BossTypeIndex) == false)
// 		return;
// 
// 	FGUILD_RAID& raidInfo = RaidBossInfo[BossTypeIndex];
// 
// 	if(IsValid(MemberHistorySwitcher))
// 		MemberHistorySwitcher->SetActiveWidgetIndex((CurrentRaidHistory.Num() > 0) ? 1 : 0);
// 
// 	if (IsValid(MemberHistoryScrollView) == false)
// 		return;
// 
// 	if (CurrentRaidHistory.Num() > 0)
// 	{
// 		MemberHistoryScrollView->Init();
// 		MemberHistoryScrollView->ClearScrollItemData();
// 
// 		for (FGUILD_RAID_CHALLENGE& historyInfo : CurrentRaidHistory)
// 		{
// 			UUC_GuildRaid_MemberData* MemberScrollData = Cast<UUC_GuildRaid_MemberData>(MemberHistoryScrollView->AddScrollItemData(UUC_GuildRaid_MemberData::StaticClass()));
// 
// 			if (MemberScrollData)
// 			{
// 				MemberScrollData->SetMemberData(FName(*historyInfo.nick), historyInfo.trial, historyInfo.spawn, historyInfo.leftSec);
// 			}
// 		}
// 		MemberHistoryScrollView->InvalidateData();
// 	}
// }
// 
// void UUP_GuildRaidMain::InvalidateRewardInfoScrollView()
// {
// 	if (IsValid(RewardInfoScrollView) == false)
// 		return;
// 
// 	if (RewardInfoScrollView)
// 	{
// 		RewardInfoScrollView->Init();
// 		RewardInfoScrollView->ClearScrollItemData();
// 
// // 		RewardIndex = RaidBossDifficultyIndex;
// 		if (RaidBossInfo.IsValidIndex(BossTypeIndex) == false)
// 			return;
// 
// 		FGUILD_RAID& info = RaidBossInfo[BossTypeIndex];
// 		int nBossLevelIndex = GetModID();
// 		FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*FString::FromInt(nBossLevelIndex)));
// 
// 		if (RaidTableInfo)
// 		{
// 			TArray<FITEM_ICON_INFO> RewardItemIcons;
// 
// 			UUIFunctionLibrary::GetRewardsIconInfoByPreview(RaidTableInfo->ClearRewardKey, RewardItemIcons);
// 
// 			if (RewardItemIcons.Num() > 0)
// 			{
// 				for (FITEM_ICON_INFO iconInfo : RewardItemIcons)
// 				{
// 					URewardsScrollItemData* RewardItemData = Cast<URewardsScrollItemData>(RewardInfoScrollView->AddScrollItemData(URewardsScrollItemData::StaticClass()));
// 					if (IsValid(RewardItemData))
// 					{
// 						RewardItemData->SetData(iconInfo);
// 					}
// 				}
// 			}
// 		}
// 		RewardInfoScrollView->InvalidateData();
// 	}
// 
// }
// 
// void UUP_GuildRaidMain::InvalidateGetableRewardScrollView()
// {
// 	if (IsValid(GetableRewardScrollView) == false)
// 		return;
// 
// 	if (GetableRewardScrollView)
// 	{
// 		GetableRewardScrollView->Init();
// 		GetableRewardScrollView->ClearScrollItemData();
// 
// 		if (RaidBossInfo.IsValidIndex(BossTypeIndex) == false)
// 			return;
// 
// 		FGUILD_RAID& info = RaidBossInfo[BossTypeIndex];
// 		int nBossLevelIndex = GetModID();
// 		FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*FString::FromInt(nBossLevelIndex)));
// 
// 		if (RaidTableInfo)
// 		{
// 			TArray<FITEM_ICON_INFO> RewardItemIcons[3];
// 
// 			UUIFunctionLibrary::GetRewardsIconInfoByPreview(RaidTableInfo->RewardKey1, RewardItemIcons[0]);
// 			UUIFunctionLibrary::GetRewardsIconInfoByPreview(RaidTableInfo->RewardKey2, RewardItemIcons[1]);
// 			UUIFunctionLibrary::GetRewardsIconInfoByPreview(RaidTableInfo->RewardKey3, RewardItemIcons[2]);
// 
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				if (RewardItemIcons[i].Num() > 0)
// 				{
// 					for (FITEM_ICON_INFO iconInfo : RewardItemIcons[i])
// 					{
// 						URewardsScrollItemData* RewardItemData = Cast<URewardsScrollItemData>(GetableRewardScrollView->AddScrollItemData(URewardsScrollItemData::StaticClass()));
// 						if (IsValid(RewardItemData))
// 						{
// 							RewardItemData->SetData(iconInfo);
// 						}
// 					}
// 				}
// 			}
// 		}
// 		GetableRewardScrollView->InvalidateData();
// 	}
// }
// 
// void UUP_GuildRaidMain::InvalidateMonsterScrollView()
// {
// 	if (IsValid(MonsterScrollView) == false)
// 		return;
// 	//*
// 	TArray<FName> StageNPCID;
// 	StageNPCID.Empty();
// 
// 	MonsterScrollView->Init();
// 	MonsterScrollView->ClearScrollItemData();
// 
// 	int32 MonsterLevel = 0;
// 
// 	FGUILD_RAID& info = RaidBossInfo[BossTypeIndex];
// 	int nBossLevelIndex = GetModID();
// 	FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*FString::FromInt(nBossLevelIndex)));
// 
// 	if (RaidTableInfo)
// 	{
// 		StageNPCID = RaidTableInfo->MonsterIDList;
// 		MonsterLevel = RaidTableInfo->PlayLevel;
// 	}
// 
// 	if (StageNPCID.Num() > 0)
// 	{
// 		for (const auto& NPCIDElem : StageNPCID)
// 		{
// 			auto ScrollItemData = MonsterScrollView->AddScrollItemData(UMonsterScrollItemData::StaticClass());
// 			UMonsterScrollItemData* MonsterScrollItemData = Cast<UMonsterScrollItemData>(ScrollItemData);
// 
// 			if (IsValid(MonsterScrollItemData) == false)
// 				continue;
// 
// 			MonsterScrollItemData->NPCID = NPCIDElem;
// 			MonsterScrollItemData->MonsterLevel = MonsterLevel;
// 
// 		}
// 	}
// 
// 	MonsterScrollView->InvalidateData();
// 	//*/
// }
// 
// void UUP_GuildRaidMain::InvalidateSeasonInfo()
// {
// 	if (IsValid(RaidSeasonInfo))
// 	{
// 		RaidSeasonInfo->Init(this);
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	// new
// 
// 	auto& info = RaidBossInfo[BossTypeIndex];
// 
// 	FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*info.modId));
// 	if (RaidTableInfo)
// 	{
// 		if (IsValid(RaidBossNameText))
// 		{
// 			RaidBossNameText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, RaidTableInfo->MapNameUIStringKey));
// 		}
// 	}
// 
// 	int Hour = 0;
// 	int Min = 0;
// 	int Sec = 0;
// 	int nRemainTime = 0;
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		if (RGameInstance->GuildInfo != nullptr)
// 		{
// 			nRemainTime = RGameInstance->GuildInfo->RemainTime;
// 			bSeasonTimeCount = true;
// 		}
// 	}
// 
// 	if (nRemainTime > 0)
// 	{
// 		Hour = nRemainTime / 3600;
// 		Min = (nRemainTime % 3600) / 60;
// 		Sec = nRemainTime % 60;
// 	}
// 
// 	SetSeasonOutTimeText(Hour, Min, Sec);
// 
// 	if (IsValid(AttackTeamLeaderNameText))
// 	{
// 		AttackTeamLeaderNameText->SetText(FText::FromString(info.captain));
// 	}
// }
// 
// void UUP_GuildRaidMain::InvalidateRaidSetting()
// {
// // 	if (IsValid(RaidSetting))
// // 	{
// // 		RaidSetting->SetVisibility(ESlateVisibility::Collapsed);	
// // 	}
// 	if (IsValid(RaidSettingPanel))
// 	{
// 		RaidSettingPanel->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// }
// 
// bool UUP_GuildRaidMain::CheckEnoughPoints(FName ModID)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return false;
// 
// 	FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(ModID);
// 
// 	if (RaidTableInfo)
// 	{
// 		if( RGameInstance->GuildInfo->MyGuildInfo.point >= RaidTableInfo->OpenCost)
// 			return true;		
// 	}
// 
// 	return false;
// }
// 
// void UUP_GuildRaidMain::OnClickChallengeButton()
// {
// 	if (RaidBossInfo.IsValidIndex(BossTypeIndex) == false)
// 		return;
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	bool bOpen = RaidBossInfo[BossTypeIndex].isOpen;
// 
// 	if (!bOpen)
// 	{
// 		if( RGameInstance->RWidgetManager)
// 		{
// 			UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 				FText::FromString(TEXT("Closed"))/*UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("closed"))*/,
// 				ECommonPopupType::VE_OK);
// 		}
// 	}
// 	else
// 	{
// // 		if (IsValid(RGameInstance) == false)
// // 			return;
// 
// 		if(RGameInstance->GuildInfo && RGameInstance->GuildInfo->OpenRaid.isOpen)
// 		{
// 			int32 OpenmodID = FCString::Atoi(*RGameInstance->GuildInfo->OpenRaid.modId);
// 			int nBossLevelIndex = GetModID();
// 
// 			if(OpenmodID == nBossLevelIndex)
// 				UPacketFunctionLibrary::GUILD_RAID_DSN_RQ(FString::FromInt(nBossLevelIndex));
// 			else
// 			{
// 				if (RGameInstance->RWidgetManager)
// 				{
// 					UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 						UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 						FText::FromString(TEXT("level not open. try different level")),
// 						ECommonPopupType::VE_OK);
// 				}
// 			}
// 		}
// 	}
// }
// 
// void UUP_GuildRaidMain::OnClickSummonBossButton()
// {
// 	if (IsValid(RaidSetting))
// 	{
// 		//uint8 bossIndex = static_cast<uint8>(RaidBossType);
// 		
// 		if (RaidBossInfo.IsValidIndex(BossTypeIndex))
// 		{
// 			int nBossLevelIndex = GetModID();//FCString::Atoi(*info.modId) + RaidBossDifficultyIndex;
// 			FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*FString::FromInt(nBossLevelIndex)));
// 			if (RaidTableInfo != nullptr && IsValid(RaidSettingPanel))
// 			{
// 				RaidSettingPanel->SetVisibility(ESlateVisibility::Visible);
// // 				int nBossLevelIndex = FCString::Atoi(*RaidBossInfo[BossTypeIndex].modId) + RaidBossDifficultyIndex;
// 
// 				//RaidBossInfo[BossTypeIndex].modId = FString::FromInt(nBossLevelIndex);
// 				FGUILD_RAID copydata = RaidBossInfo[BossTypeIndex];
// 				copydata.modId = FString::FromInt(nBossLevelIndex);
// 				RaidSetting->Init(copydata, RaidTableInfo->OpenCost, RaidSettingPanel);
// 			}
// 		}
// 	}
// }
// 
// void UUP_GuildRaidMain::OnClickBossInfoButton()
// {
// #ifdef WITH_EDITOR
// 	UE_LOG(LogUI, Log, TEXT("called UUP_GuildRaidMain::OnClickBossInfoButton"));
// #endif // WITH_EDITOR
// }
// 
// void UUP_GuildRaidMain::OnClickRaidBossRadio(ERaidBossTypeEnum InType)
// {
// 	if (RaidBossType == InType)
// 		return;
// 
// 	RaidBossType = InType;
// 	BossTypeIndex = static_cast<int32>(InType);
// 	
// 	// info
// 	SetRaidBossTab(BossTypeIndex);
// 
// 	if (RaidBossInfo.IsValidIndex(BossTypeIndex))
// 	{
// 		int nBossLevelIndex = GetModID();//FCString::Atoi(*RaidBossInfo[BossTypeIndex].modId) + RaidBossDifficultyIndex;
// 		UPacketFunctionLibrary::GUILD_RAID_CHALLENGE_RQ(FString::FromInt(nBossLevelIndex));
// 	}
// 	// memberlist
// 	// reward, monster list
// 	//InvalidateScrollView();
// }
// 
// void UUP_GuildRaidMain::OnClick_Difficulty(int32 InDifficulty)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// 
// 	RaidBossDifficultyIndex = InDifficulty;
// 	
// 	RGameInstance->GuildInfo->RaidBossIndex = BossTypeIndex;
// 	
// 	SetStageButton();
// 	SetRaidBossInfo(BossTypeIndex);
// 
// 	if(RaidBossInfo.IsValidIndex(BossTypeIndex))
// 	{
// 		int nBossLevelIndex = GetModID();//FCString::Atoi(*RaidBossInfo[BossTypeIndex].modId) + RaidBossDifficultyIndex;
// 		UPacketFunctionLibrary::GUILD_RAID_CHALLENGE_RQ(FString::FromInt(nBossLevelIndex));
// 	}
// 		
// 	// reward, monster list
// // 	InvalidateSeasonInfo();
// // 	InvalidateScrollView();
// // 	InvalidateRaidSetting();
// }
// 
// void UUP_GuildRaidMain::SetSeasonOutTimeText(int32 InHour, int32 InMin, int32 InSec)
// {
// 	bool bSeasonEnd = ((InHour == 0) && (InMin == 0) && (InSec == 0));
// 
// 	SetUIbySeasonStatus(bSeasonEnd);
// 
// 	if(bSeasonEnd == false)
// 	{
// 		if (IsValid(SeasonOut_RemainHours))
// 		{
// 			SeasonOut_RemainHours->SetText(FText::AsNumber(InHour));
// 		}
// 		if (IsValid(SeasonOut_RemainMinutes))
// 		{
// 			SeasonOut_RemainMinutes->SetText(FText::AsNumber(InMin));
// 		}
// 		if (IsValid(SeasonOut_RemainSeconds))
// 		{
// 			SeasonOut_RemainSeconds->SetText(FText::AsNumber(InSec));
// 		}
// 	}
// }
// 
// void UUP_GuildRaidMain::SetOpenRaidTimeText(int32 InMin, int32 InSec)
// {
// 	if (IsValid(OpenRaidTime_Min))
// 	{
// 		FString text = FText::AsNumber(InMin).ToString();
// 		if (InMin < 10)
// 			text = TEXT("0") + text;
// 		
// 		OpenRaidTime_Min->SetText(FText::FromString(text));
// 	}
// 	if (IsValid(OpenRaidTime_Sec))
// 	{
// 		FString text = FText::AsNumber(InSec).ToString();
// 		if (InSec < 10)
// 			text = TEXT("0") + text;
// 		OpenRaidTime_Sec->SetText(FText::FromString(text));
// 	}
// }
// 
// void UUP_GuildRaidMain::SetBossThumnail(ERaidBossTypeEnum InBossType)
// {
// 	if (IsValid(BossImage) == false)
// 		return;
// 
// 	FName textureName = "";
// 	switch (InBossType)
// 	{
// 	case ERaidBossTypeEnum::VE_RaidBoss_1:
// 		textureName = FName("PT_GuildRaid_Raphael");
// 		break;
// 	case ERaidBossTypeEnum::VE_RaidBoss_2:
// 		textureName = FName("PT_GuildRaid_Arphenia");
// 		break;
// 	case ERaidBossTypeEnum::VE_RaidBoss_3:
// 		textureName = FName("PT_GuildRaid_Veimon");
// 		break;
// 	default:
// 		break;
// 	}
// 	
// 	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(textureName), BossImage);
// }
// 
// void UUP_GuildRaidMain::SetRaidBossStatus()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->GuildInfo == nullptr)
// 		return;
// 
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		RaidProgressBar[i]->SetActiveWidgetIndex(0);	// 0 : live
// 		RaidBossStatus[i]->SetActiveWidgetIndex(0);		// 0 : live
// 	}
// 
// 	if(RGameInstance->GuildInfo->OpenRaid.isOpen)
// 	{
// 		for (int i = 0; i < RGameInstance->GuildInfo->OpenRaid.boss.Num(); ++i)
// 		{
// 			RaidProgressBar[i]->SetActiveWidgetIndex(1);	// 1 : dead
// 			RaidBossStatus[i]->SetActiveWidgetIndex(1);		// 1 : dead
// 		}
// 	}
// }