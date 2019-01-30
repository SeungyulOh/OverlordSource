// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_DashBoard.h"

#include "GlobalIntegrated.h"



//#include "Global/BattleFieldManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/RTSManager.h"

// #define MAX_MemberCount		5
// 
// void UUC_HUD_DashBoard::NativeConstruct()
// {
// 	Super::NativeConstruct();
// 	PrepareWidget();
// 
// 	MyPlayerUK = -1;
// 	SetVisibility(ESlateVisibility::Collapsed);
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 	
// // 	if (!RGameInstance->RealTimeModManager->IsRealTimeConnectMode(EPVP::BG))
// // 		return;
// 	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 		return;
// 
// 	RGameInstance->EventManager->OnModRoomUpdateMedal.RemoveDynamic(this, &UUC_HUD_DashBoard::OnModRoomUpdateMedal);
//  	RGameInstance->EventManager->OnModRoomUpdateMedal.AddDynamic(this, &UUC_HUD_DashBoard::OnModRoomUpdateMedal);
// 
// 	RGameInstance->EventManager->OnShowDashBoard.RemoveDynamic(this, &UUC_HUD_DashBoard::OnShowDashBoard);
// 	RGameInstance->EventManager->OnShowDashBoard.AddDynamic(this, &UUC_HUD_DashBoard::OnShowDashBoard);
// 
// 	OnModRoomUpdateMedal();
// }
// 
// void UUC_HUD_DashBoard::NativeDestruct()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnModRoomUpdateMedal.RemoveDynamic(this, &UUC_HUD_DashBoard::OnModRoomUpdateMedal);
// 	}
// 
// 	Super::NativeDestruct();
// }
// 
// bool UUC_HUD_DashBoard::Initialize()
// {
// 	Super::Initialize();
// 	PrepareWidget();
// 	return true;
// }
// 
// void UUC_HUD_DashBoard::PrepareWidget()
// {
// 	if (IsValid(Button_Close))
// 	{
// 		Button_Close->OnClicked.RemoveDynamic(this, &UUC_HUD_DashBoard::OnClick_Close);
// 		Button_Close->OnClicked.AddDynamic(this, &UUC_HUD_DashBoard::OnClick_Close);
// 	}
// 
// 	//Blue
// 	FindChildWidgetsWithCertainName<UPanelWidget>(this, Panel_BlueTeamRankList, TEXT("HorizontalBox_BlueTeam_"));
// 	FindChildWidgetsWithCertainName<UPanelWidget>(this, Panel_BlueTeamList, TEXT("CanvasPanel_BlueTeamData_"));
// 	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, Swicher_BlueTeamMyDataList, TEXT("WidgetSwitcher_BlueTeam_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamNameList, TEXT("TextBlock_BlueTeamName_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamGuildNameList, TEXT("TextBlock_BlueTeamGuild_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamPointList, TEXT("TextBlock_BlueTeamPoint_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamKillCountList, TEXT("TextBlock_BlueTeamKill_"));
// 
// 	//Red
// 	FindChildWidgetsWithCertainName<UPanelWidget>(this, Panel_RedTeamRankList, TEXT("HorizontalBox_RedTeam_"));
// 	FindChildWidgetsWithCertainName<UPanelWidget>(this, Panel_RedTeamList, TEXT("CanvasPanel_RedTeamData_"));
// 	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, Swicher_RedTeamMyDataList, TEXT("WidgetSwitcher_RedTeam_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_RedTeamNameList, TEXT("TextBlock_RedTeamName_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_RedTeamGuildNameList, TEXT("TextBlock_RedTeamGuild_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_RedTeamPointList, TEXT("TextBlock_RedTeamPoint_"));
// 	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_RedTeamKillCountList, TEXT("TextBlock_RedTeamKill_"));
// }
// 
// 
// void UUC_HUD_DashBoard::OnShowDashBoard()
// {
// 	OnModRoomUpdateMedal();
// 	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// }
// 
// void UUC_HUD_DashBoard::OnModRoomUpdateMedal()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// // 	if (!RGameInstance->RealTimeModManager->IsRealTimeConnectMode(EPVP::BG))
// // 		return;
// 	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 		return;
// // 	int32 MyGroupKey = RGameInstance->GameObjectMgr->GetMyGroupKey();
// 	int32 MyGroupKey = UObjectFunctionLibrary::GetMyGroupKey();
// 	MyPlayerUK = RGameInstance->RealTimeModManager->GetMyPlayActorUK();
// 
// 	TArray<FBattleGroundTeamData>& BattleGroundTeamList = RGameInstance->RealTimeModManager->GetBattleGroundTeamList();
// 
// 	bool IsBlueTeam = true;
// 	for (FBattleGroundTeamData& TeamData : BattleGroundTeamList)
// 	{
// 		// Team Data
// 		int32 TeamKillCount = 0;
// 		for (auto& RankingInfo : TeamData.TeamRanking)
// 		{
// 			TeamKillCount += RankingInfo.kill;
// 		}
// 
// 		if (IsBlueTeam)
// 		{
// 			TextBlock_BlueTeamPouch->SetText(FText::AsNumber(TeamData.TeamMedalCount));
// 			TextBlock_BlueTeamKillCount->SetText(FText::AsNumber(TeamKillCount));
// 		}
// 		else
// 		{
// 			TextBlock_RedTeamPouch->SetText(FText::AsNumber(TeamData.TeamMedalCount));
// 			TextBlock_RedTeamKillCount->SetText(FText::AsNumber(TeamKillCount));
// 		}
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
// 						if (IsBlueTeam)
// 						{
// 							if (0 < MemberData.medal) {
// 								TextBlock_BlueTemMyRanking->SetText(FText::AsNumber(FindRank));
// 							}
// 							else {
// 								TextBlock_BlueTemMyRanking->SetText(FText::FromString(TEXT("-")));
// 							}
// 						}
// 						else
// 						{
// 							if (0 < MemberData.medal) {
// 								TextBlock_RedTemMyRanking->SetText(FText::AsNumber(FindRank));
// 							}
// 							else {
// 								TextBlock_RedTemMyRanking->SetText(FText::FromString(TEXT("-")));
// 							}
// 						}
// 					}
// 				}
// 
// 				if (IsBlueTeam)
// 				{
// 					CanvasPanel_BlueTeam_My->SetVisibility(MyDataVisible);
// 				}
// 				else
// 				{
// 					CanvasPanel_RedTeam_My->SetVisibility(MyDataVisible);
// 				}
// 			}
// 
// 			if (IsBlueTeam)
// 			{
// 				SetDashBoardMember(MemberData, Panel_BlueTeamRankList[CheckIndex], Panel_BlueTeamList[CheckIndex], Swicher_BlueTeamMyDataList[CheckIndex],
// 					Text_BlueTeamNameList[CheckIndex], Text_BlueTeamGuildNameList[CheckIndex], Text_BlueTeamPointList[CheckIndex], Text_BlueTeamKillCountList[CheckIndex]);
// 			}
// 			else
// 			{
// 				SetDashBoardMember(MemberData, Panel_RedTeamRankList[CheckIndex], Panel_RedTeamList[CheckIndex], Swicher_RedTeamMyDataList[CheckIndex],
// 					Text_RedTeamNameList[CheckIndex], Text_RedTeamGuildNameList[CheckIndex], Text_RedTeamPointList[CheckIndex], Text_RedTeamKillCountList[CheckIndex]);
// 			}
// 		}
// 
// 		IsBlueTeam = false;
// 	}
// }
// 
// void UUC_HUD_DashBoard::OnClick_Close()
// {
// 	SetVisibility(ESlateVisibility::Collapsed);
// }
// 
// void UUC_HUD_DashBoard::SetDashBoardMember(OVERLORD::RTS::STRUCT::PLAYER_SCORET& PlayerInfo, UPanelWidget* PanelRankList, UPanelWidget* PanelMember, UWidgetSwitcher* SwicherMyData, UTextBlock* MemberName, UTextBlock* MemberGuild, UTextBlock* MemberPoint, UTextBlock* MemberKillCount)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// 	FString RankNickName;
// 	FString RankGuildName;
// 	MyPlayerUK = RGameInstance->RealTimeModManager->GetMyPlayActorUK();
// 	if (PlayerInfo.actorUK == MyPlayerUK)
// 	{
// 		SwicherMyData->SetActiveWidgetIndex(1);
// 
// 		RankNickName = RGameInstance->UserInfo->Avatar.nick;
// 		RankGuildName = RGameInstance->GuildInfo->MyGuildInfo.name;
// 	}
// 	else
// 	{
// 		SwicherMyData->SetActiveWidgetIndex(0);
// 	}
// 
// // 	FRTS_ACTOR* actor = RGameInstance->RealTimeModManager->Get_RTS_Actor(PlayerInfo.actorUK);
// // 	if (nullptr != actor) {
// // 		RankNickName = actor->nick;
// // 		RankGuildName = actor->subNick;
// // 	}
// 
// 	FUserBattleRecord* record = RGameInstance->BattleRecordManager->GetUserBattleRecord(PlayerInfo.kId);
// 	if (record != nullptr)
// 	{
// 		RankNickName = record->UserNickName;
// 		RankGuildName = record->UserSubNick;
// 	}
// 		
// 
// // 	FBattleFieldUserInfo* BattleUserInfo = RGameInstance->BattleFieldManager->GetBattleFieldUserInfo(PlayerInfo.actorUK);
// // 	if (nullptr != BattleUserInfo)
// // 	{
// // 		RankNickName = BattleUserInfo->NickName;
// // 		RankGuildName = BattleUserInfo->GuildName;
// // 	}
// 	
// 	MemberName->SetText(FText::FromString(*RankNickName));
// 	MemberGuild->SetText(FText::FromString(*RankGuildName));
// 	MemberPoint->SetText(FText::AsNumber(PlayerInfo.medal));
// 	MemberKillCount->SetText(FText::AsNumber(PlayerInfo.kill));
// 
// 	if (PLAYER_UK_NONE == PlayerInfo.actorUK)
// 	{
// 		PanelRankList->SetVisibility(ESlateVisibility::Collapsed);
// 		PanelMember->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// 	else
// 	{
// 		PanelRankList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		PanelMember->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 	}
// }