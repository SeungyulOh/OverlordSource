// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ContentsType.h"


#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


//

#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"
#include "GameState/TowerLobbyState.h"


void UUC_ContentsType::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_ContentsType::OnClickShortCut);
		Button_OK->OnClicked.AddDynamic(this, &UUC_ContentsType::OnClickShortCut);
	}
}

void UUC_ContentsType::OnClickShortCut()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	EUIMenuIDEnum eMenuID = EUIMenuIDEnum::VE_None;

	switch (eContentsType)
	{
	case EGamePlayModeTypeEnum::VE_MainLobby:		eMenuID = EUIMenuIDEnum::VE_MainLobby;			break;
	case EGamePlayModeTypeEnum::VE_Campaign:
	{
		RGameInstance->CampaignManager->SetStageDifficulty(EStageDifficultyEnum::Normal);
		eMenuID = EUIMenuIDEnum::VE_Campaign;

	}break;
	
	case EGamePlayModeTypeEnum::VE_CampaignHard:	
	{
		RGameInstance->CampaignManager->SetStageDifficulty(EStageDifficultyEnum::Hard);
		eMenuID = EUIMenuIDEnum::VE_Campaign;
	}break;

	case EGamePlayModeTypeEnum::VE_Arena:			
	{
		eMenuID = EUIMenuIDEnum::VE_Arena;

		if(RGameInstance->RInventory->IsEmptyDefenceDeck())
			RGameInstance->ArenaManager->SetDefenceSetting(true);
	}break;
	case EGamePlayModeTypeEnum::VE_Colosseum:		eMenuID = EUIMenuIDEnum::VE_Colosseum;			break;
// 	case EGamePlayModeTypeEnum::VE_RaidKillBoss:	
// 		eMenuID = EUIMenuIDEnum::VE_AkhanRaid;			
// 		if (IsValid(RGameInstance->RaidManager))
// 			RGameInstance->RaidManager->SetSelectedRaidListIndex(0);
// 		break;

	case EGamePlayModeTypeEnum::VE_BattleField:		eMenuID = EUIMenuIDEnum::VE_BattleField;		break;
	case EGamePlayModeTypeEnum::VE_BattleGround:	eMenuID = EUIMenuIDEnum::VE_BattleGround;		break;
	case EGamePlayModeTypeEnum::VE_GuildRaid:		eMenuID = EUIMenuIDEnum::VE_Guild;				break;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:	
	{
		eMenuID = EUIMenuIDEnum::VE_MirrorDungeon;
		if(RGameInstance->RInventory->IsEmptyDefenceDeck())
			RGameInstance->MirrorDungeonManager->SetMirrorDefenceSetting(true);

	}break;
	case EGamePlayModeTypeEnum::VE_HeroEpisode:		eMenuID = EUIMenuIDEnum::VE_HeroRuneSelect;		break;
// 	case EGamePlayModeTypeEnum::VE_Challenge:		eMenuID = EUIMenuIDEnum::VE_WeekDungeon;		break;
	case EGamePlayModeTypeEnum::VE_WeekDungeon:		eMenuID = EUIMenuIDEnum::VE_WeekDungeon;		break;
// 	case EGamePlayModeTypeEnum::VE_PlayTest:		eMenuID = EUIMenuIDEnum::VE_PlayTest;			break;
	case EGamePlayModeTypeEnum::VE_FloorStore:		eMenuID = EUIMenuIDEnum::VE_FloorShop;			break;
	case EGamePlayModeTypeEnum::VE_Guild:			eMenuID = EUIMenuIDEnum::VE_Guild;				break;
	case EGamePlayModeTypeEnum::VE_TimeTreasure:	eMenuID = EUIMenuIDEnum::VE_TimeTreasure;		break;
	case EGamePlayModeTypeEnum::VE_HeroManagement:	eMenuID = EUIMenuIDEnum::VE_HeroInventory;		break;

	case EGamePlayModeTypeEnum::VE_SingleRaid:
	{
		RGameInstance->RaidManager->TodayType = RGameInstance->GetRaidDayType();
		eMenuID = EUIMenuIDEnum::VE_SingleRaid;
	}
	break;

	case EGamePlayModeTypeEnum::VE_MultiRaid:
	{
		ERaidDayType eType = ERaidDayType::WEEK;//RGameInstance->GetRaidDayType();
		eMenuID = EUIMenuIDEnum::VE_SingleRaid;
	}
	break;
	

	default:
		break;
	}

	UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
	if (TowerState)
		TowerState->SetIndoorStart(true);

	RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)eMenuID);

	RGameInstance->EventManager->ClearEvent();
	RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);

	if (IsValid(RGameInstance->ContentsUnLockManager))
	{
		RGameInstance->ContentsUnLockManager->CloseContentsUnlockNotification();
	}
}