// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_RealTime_Result.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"


//#include "Global/BattleFieldManager.h"


#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"

#include "UtilFunctionIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UI/Result/UC_ResultStatistics.h"
#include "UI/Result/UC_ResultEffect.h"
#include "UI/Common/UC_Item_Icons.h"

#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"
#include "Network/RTSProtocol/rts.struct_generated.h"

#define MAX_MemberCount		5

void UUP_RealTime_Result::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (IsValid(BattleStatistics))
		{
			int32 MyKID = RGameInstance->HttpClient->GetKID();
// 			int32 MyGroupKey = RGameInstance->GameObjectMgr->GetMyGroupKey();
			int32 MyGroupKey = UObjectFunctionLibrary::GetMyTeamKey();

// 			EPVP RealTimeConnectMode = RGameInstance->RealTimeModManager->GetRealTimeConnectMode();
// 			if (EPVP::BG == RealTimeConnectMode)
// 			if(URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 			{
// 				TArray<FBattleGroundTeamData>& BattleGroundTeamList = RGameInstance->RealTimeModManager->GetBattleGroundTeamList();
// 				for (FBattleGroundTeamData& TeamData : BattleGroundTeamList)
// 				{
// 					if (TeamData.GroupKey == MyGroupKey)
// 					{
// 						BattleStatistics->SetMyIDs(MyKID, MyGroupKey);
// 					}
// 					else
// 					{
// 						BattleStatistics->SetEnemyID(0, TeamData.GroupKey);
// 					}
// 				}
// 			}
			//else if (EPVP::GR == RealTimeConnectMode)
//			else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
			{
				BattleStatistics->SetMyIDs(MyKID, MyGroupKey);
				BattleStatistics->SetEnemyID(0, GroupKey_REALTIME_NPC);
			}

			BattleStatistics->InitResultStatistics();
		}
	}

	RefreshResultState();
	RefreshRewardData();
	RefreshRankData();
}

void UUP_RealTime_Result::NativeDestruct()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 	}
	Super::NativeDestruct();
}

void UUP_RealTime_Result::OnClick_GotoLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		//EPVP RealTimeConnectMode = RGameInstance->RealTimeModManager->GetRealTimeConnectMode();
		//if (EPVP::BG == RealTimeConnectMode)
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
		{
			UUIFunctionLibrary::AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_BattleGround);
		}
		//else if (EPVP::GR == RealTimeConnectMode)
		else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
		{
			UUIFunctionLibrary::AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Guild);
		}

		RGameInstance->EventManager->ClearEvent();
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_RealTime_Result::OnClick_BattleRecord()
{
	if (IsValid(BattleStatistics))
	{
		BattleStatistics->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_RealTime_Result::SetGameEndTime()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString MinText, SecText;
	UDescriptionFunctionLibrary::GetTimeStr(RGameInstance->RealTimeModManager->GetGameTime(), MinText, SecText);
	if (IsValid(TextBlock_Min))
		TextBlock_Min->SetText(FText::FromString(MinText));
	if (IsValid(TextBlock_Sec))
		TextBlock_Sec->SetText(FText::FromString(SecText));
}

void UUP_RealTime_Result::RefreshResultState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
	
	if (IsValid(ResultEffect))
	{
		ResultEffect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		switch (RGameInstance->RealTimeModManager->RealTimePlayResult)
		{
		case OVERLORD::RTS::ENUM::RTSPlayResult::RTSPlayResult_NONE:	// Draw 
			ResultEffect->SetState(EResultEffectEnum::VE_Draw);
			break;
		case OVERLORD::RTS::ENUM::RTSPlayResult::RTSPlayResult_WIN:
			ResultEffect->SetState(EResultEffectEnum::VE_Victory);
			break;
		case OVERLORD::RTS::ENUM::RTSPlayResult::RTSPlayResult_LOSE:
			ResultEffect->SetState(EResultEffectEnum::VE_Defeat);
			break;
		}
	}

	EGamePlayModeTypeEnum GamePlayMode = UUtilFunctionLibrary::GetGamePlayModeEnum();
	if (EGamePlayModeTypeEnum::VE_BattleGround == GamePlayMode)
	{
		for (auto PanelWidget : Panel_GuildRaidHideDataList)
		{
			PanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		if (IsValid(Text_RankChangeName))
		{
			Text_RankChangeName->LocalizingKey = FName("UI_Result_RealTimeContents_0009");
			Text_RankChangeName->ApplyLocalizedText();
		}
		if (IsValid(FirstTitleSwitcher))
		{
			FirstTitleSwitcher->SetActiveWidgetIndex(0);
		}
		if (IsValid(SecondTitleSwitcher))
		{
			SecondTitleSwitcher->SetActiveWidgetIndex(0);
		}
		if (IsValid(Text_RewardDesc))
		{
			Text_RewardDesc->LocalizingKey = FName("UI_Result_RealTimeContents_0018");
			Text_RewardDesc->ApplyLocalizedText();
		}

		SetGameEndTime();
	}
	else if (EGamePlayModeTypeEnum::VE_GuildRaid == GamePlayMode)
	{
		for (auto PanelWidget : Panel_GuildRaidHideDataList)
		{
			PanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IsValid(Text_RankChangeName))
		{
			Text_RankChangeName->LocalizingKey = FName("UI_Result_RealTimeContents_0011");
			Text_RankChangeName->ApplyLocalizedText();
		}
		if (IsValid(FirstTitleSwitcher))
		{
			FirstTitleSwitcher->SetActiveWidgetIndex(1);
		}
		if (IsValid(SecondTitleSwitcher))
		{
			SecondTitleSwitcher->SetActiveWidgetIndex(1);
		}
		if (IsValid(Text_RewardDesc))
		{
			Text_RewardDesc->LocalizingKey = FName("UI_Result_RealTimeContents_0019");
			Text_RewardDesc->ApplyLocalizedText();
		}
	}
}

void UUP_RealTime_Result::RefreshRewardData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (false == IsValid(RGameInstance))
		return;

	TArray<FITEM_ICON_INFO> FirstRewardsItemIconInfo;
	TArray<FREWARD>& PouchRewards = RGameInstance->RealTimeModManager->RealTimeModReward.pouch;
	for (auto& RewardsElem : PouchRewards)
	{
		FITEM_ICON_INFO RewardItemIconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, RewardItemIconInfo);
		FirstRewardsItemIconInfo.Emplace(RewardItemIconInfo);
	}

	if (IsValid(FirstItemSwitcher))
	{
		if (0 < FirstRewardsItemIconInfo.Num())
		{
			FirstItemSwitcher->SetActiveWidgetIndex(0);
		}
		else
		{
			FirstItemSwitcher->SetActiveWidgetIndex(1);
		}
	}
	RefreshRewardIcon(FirstItemIconList, FirstRewardsItemIconInfo);


	TArray<FITEM_ICON_INFO> SecondRewardsItemIconInfo;
	TArray<FREWARD>& RankRewards = RGameInstance->RealTimeModManager->RealTimeModReward.rank;
	for (auto& RewardsElem : RankRewards)
	{
		FITEM_ICON_INFO RewardItemIconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, RewardItemIconInfo);
		SecondRewardsItemIconInfo.Emplace(RewardItemIconInfo);
	}

	if (IsValid(SecondItemSwitcher))
	{
		if (0 < SecondRewardsItemIconInfo.Num())
		{
			SecondItemSwitcher->SetActiveWidgetIndex(0);
		}
		else
		{
			SecondItemSwitcher->SetActiveWidgetIndex(1);
		}
	}
	RefreshRewardIcon(SecondItemIconList, SecondRewardsItemIconInfo);
}

void UUP_RealTime_Result::RefreshRewardIcon(TArray<UUC_Item_Icons*>& ItemIconList, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo)
{
	int32 CheckIndex = 0;
	for (auto ItemIcon : ItemIconList)
	{
		if (!IsValid(ItemIcon))
			continue;

		ESlateVisibility ItemVisibility = ESlateVisibility::Collapsed;
		if (RewardsItemIconInfo.IsValidIndex(CheckIndex))
		{
			ItemVisibility = ESlateVisibility::SelfHitTestInvisible;
			ItemIcon->SetItemIconStructure(RewardsItemIconInfo[CheckIndex]);
		}

		ItemIcon->SetVisibility(ItemVisibility);

		++CheckIndex;
	}
}

void UUP_RealTime_Result::RefreshRankData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	//EPVP RealTimeConnectMode = RGameInstance->RealTimeModManager->GetRealTimeConnectMode();
	//if (EPVP::BG == RealTimeConnectMode)
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
	{
		BattleGroundRankData();
	}
	//else if (EPVP::GR == RealTimeConnectMode)
	else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
	{
		GuildRaidRankData();
	}
}

void UUP_RealTime_Result::BattleGroundRankData()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// // 	int32 MyGroupKey = RGameInstance->GameObjectMgr->GetMyGroupKey();
// 	int32 MyGroupKey = UObjectFunctionLibrary::GetMyGroupKey();
// 
// 	MyPlayerUK = RGameInstance->RealTimeModManager->GetMyPlayActorUK();
// 
// 	TArray<FBattleGroundTeamData>& BattleGroundTeamList = RGameInstance->RealTimeModManager->GetBattleGroundTeamList();
// 	for (FBattleGroundTeamData& TeamData : BattleGroundTeamList)
// 	{
// 		if (TeamData.GroupKey != MyGroupKey)
// 			continue;
// 
// 		bool MyDataRanker = false;
// 		for (int32 CheckIndex = 0; CheckIndex <= MAX_MemberCount; ++CheckIndex)
// 		{
// 			OVERLORD::RTS::STRUCT::PLAYER_SCORET MemberData;
// 
// 			if (CheckIndex < MAX_MemberCount)
// 			{
// 				// Member Data
// 				if (TeamData.TeamRanking.IsValidIndex(CheckIndex))
// 				{
// 					MemberData = TeamData.TeamRanking[CheckIndex];
// 
// 					if (MemberData.actorUK == MyPlayerUK)
// 						MyDataRanker = true;
// 				}
// 			}
// 			else
// 			{
// 				// My Data
// 				ESlateVisibility MyDataVisible = ESlateVisibility::Collapsed;
// 				if (TeamData.GroupKey == MyGroupKey)
// 				{
// 					if (false == MyDataRanker)
// 					{
// 						MyDataVisible = ESlateVisibility::SelfHitTestInvisible;
// 
// 						MemberData.actorUK = MyPlayerUK;
// 
// 						int32 FindRank = 1;
// 						for (auto& FindMyRankData : TeamData.TeamRanking)
// 						{
// 							if (FindMyRankData.actorUK == MyPlayerUK)
// 							{
// 								MemberData = FindMyRankData;
// 								break;
// 							}
// 							++FindRank;
// 						}
// 
// 						if (IsValid(Text_BlueTemMyRanking))
// 						{
// 							if (0 < MemberData.medal) {
// 								Text_BlueTemMyRanking->SetText(FText::AsNumber(FindRank));
// 							}
// 							else {
// 								Text_BlueTemMyRanking->SetText(FText::FromString(TEXT("-")));
// 							}
// 						}
// 					}
// 				}
// 
// 				if (IsValid(Panel_BlueTeamMyData))
// 				{
// 					Panel_BlueTeamMyData->SetVisibility(MyDataVisible);
// 				}
// 			}
// 
// 			BattleGroundMember(MemberData, Panel_BlueTeamRankList[CheckIndex], Panel_BlueTeamList[CheckIndex], Swicher_BlueTeamMyDataList[CheckIndex],
// 				Text_BlueTeamNameList[CheckIndex], Text_BlueTeamGuildNameList[CheckIndex], Text_BlueTeamPointList[CheckIndex], Text_BlueTeamKillCountList[CheckIndex]);
// 		}
// 	}
}

void UUP_RealTime_Result::BattleGroundMember(OVERLORD::RTS::STRUCT::PLAYER_SCORET& PlayerInfo, UPanelWidget* PanelRankList, UPanelWidget* PanelMember, UWidgetSwitcher* SwicherMyData,
	UTextBlock* MemberName, UTextBlock* MemberGuild, UTextBlock* MemberPoint, UTextBlock* MemberKillCount)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
	
	FString RankNickName;
	FString RankGuildName;

	if (PlayerInfo.actorUK == MyPlayerUK)
	{
		SwicherMyData->SetActiveWidgetIndex(1);

		RankNickName = RGameInstance->UserInfo->Avatar.nick;
		RankGuildName = RGameInstance->GuildInfo->MyGuildInfo.name;
	}
	else
	{
		SwicherMyData->SetActiveWidgetIndex(0);
		//FBattleRecord* battle = RGameInstance->BattleRecordManager->GetBattleRecord(PlayerInfo.kId);
		//FRTS_ACTOR* ActorInfo = RGameInstance->RealTimeModManager->Get_RTS_Actor(PlayerInfo.actorUK);
		FUserBattleRecord* battle = RGameInstance->BattleRecordManager->GetUserBattleRecord(PlayerInfo.kId);
		if (nullptr != battle) {
			RankNickName = battle->UserNickName;
			RankGuildName = battle->UserSubNick;
		}
	}

//	FBattleFieldUserInfo* BattleUserInfo = RGameInstance->BattleFieldManager->GetBattleFieldUserInfo(PlayerInfo.actorUK);
// 	if (nullptr != BattleUserInfo)
// 	{
// 		RankNickName = BattleUserInfo->NickName;
// 		RankGuildName = BattleUserInfo->GuildName;
// 	}

	MemberName->SetText(FText::FromString(*RankNickName));
	MemberGuild->SetText(FText::FromString(*RankGuildName));
	MemberPoint->SetText(FText::AsNumber(PlayerInfo.medal));
	MemberKillCount->SetText(FText::AsNumber(PlayerInfo.kill));

	if (PLAYER_UK_NONE == PlayerInfo.actorUK)
	{
		PanelRankList->SetVisibility(ESlateVisibility::Collapsed);
		PanelMember->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PanelRankList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PanelMember->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_RealTime_Result::GuildRaidRankData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	int32 MyKID = RGameInstance->HttpClient->GetKID();
	FString RankGuildName = RGameInstance->GuildInfo->MyGuildInfo.name;

	TArray<FBattleRecord*> DamageRankList;
	RGameInstance->BattleRecordManager->GetDamageMeterRank(DamageRankList);

	bool MyDataRanker = false;
	for (int32 CheckIndex = 0; CheckIndex <= MAX_MemberCount; ++CheckIndex)
	{
		ESlateVisibility DataVisible = ESlateVisibility::Collapsed;
		FBattleRecord* BattleRecord = nullptr;

		if (CheckIndex < MAX_MemberCount)
		{
			if (DamageRankList.IsValidIndex(CheckIndex))
			{
				BattleRecord = DamageRankList[CheckIndex];
				if (BattleRecord)
				{
					DataVisible = ESlateVisibility::SelfHitTestInvisible;
					if (BattleRecord->UserKID == MyKID)
					{
						MyDataRanker = true;
					}
				}
			}
		}
		else
		{
			if (false == MyDataRanker)
			{
				FUserBattleRecord* MyBattleRecord = RGameInstance->BattleRecordManager->GetUserBattleRecord(MyKID);
				if (MyBattleRecord)
				{
					DataVisible = ESlateVisibility::SelfHitTestInvisible;
					BattleRecord = MyBattleRecord->GetTotalRecord();
				}
			}

			if (IsValid(Panel_BlueTeamMyData))
			{
				Panel_BlueTeamMyData->SetVisibility(DataVisible);
			}
		}

		if (BattleRecord)
		{
			if (IsValid(Swicher_BlueTeamMyDataList[CheckIndex]))
			{
				Swicher_BlueTeamMyDataList[CheckIndex]->SetActiveWidgetIndex((BattleRecord->UserKID == MyKID));
			}
			if (IsValid(Text_BlueTeamNameList[CheckIndex]))
			{
				Text_BlueTeamNameList[CheckIndex]->SetText(FText::FromString(*BattleRecord->NickName));
			}
			if (IsValid(Text_BlueTeamGuildNameList[CheckIndex]))
			{
				Text_BlueTeamGuildNameList[CheckIndex]->SetText(FText::FromString(*RankGuildName));
			}
			if (IsValid(Text_BlueTeamPointList[CheckIndex]))
			{
				Text_BlueTeamPointList[CheckIndex]->SetText(FText::AsNumber(BattleRecord->GetAmountOfAttack()));
			}
		}

		if (IsValid(Panel_BlueTeamRankList[CheckIndex]))
		{
			Panel_BlueTeamRankList[CheckIndex]->SetVisibility(DataVisible);
		}

		if (IsValid(Panel_BlueTeamList[CheckIndex]))
		{
			Panel_BlueTeamList[CheckIndex]->SetVisibility(DataVisible);
		}
	}
}