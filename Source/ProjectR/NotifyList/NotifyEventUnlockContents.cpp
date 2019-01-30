// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "NotifyEventUnlockContents.h"

#include "GlobalIntegrated.h"

#include "NotifyList/NotifyEventList.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "Table/ContentsUnlockTableInfo.h"



void UNotifyEventUnlockContents::SetUnlockKey(FName key)
{
	UnlockTableKey = key;
}

bool UNotifyEventUnlockContents::GoShortCuts()
{
	bool bRtContent = false;
	if ( IsValid(RGameInstance->NotifyEventList))
	{
		FContentsUnlockTableInfo* UnlockTableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(UnlockTableKey);
		if (UnlockTableInfo)
		{
			switch (UnlockTableInfo->ContentType)
			{
			case EGamePlayModeTypeEnum::VE_Campaign:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign, 0);
				break;
			case EGamePlayModeTypeEnum::VE_Arena:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Arena, 0);
				break;
			case EGamePlayModeTypeEnum::VE_Challenge:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_WeekDungeon, 0);
				break;
			case EGamePlayModeTypeEnum::VE_Colosseum:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Colosseum, 0);
				break;
			case EGamePlayModeTypeEnum::VE_RaidKillBoss:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Raid_List, 0);
				break;
			case EGamePlayModeTypeEnum::VE_BattleField:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_BattleField, 0);
				break;
			case EGamePlayModeTypeEnum::VE_BattleGround:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_BattleGround, 0);
				break;
			case EGamePlayModeTypeEnum::VE_MirrorDungeon:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_MirrorDungeon, 0);
				break;
			case EGamePlayModeTypeEnum::VE_TimeTreasure:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_TimeTreasure, 0);
				break;
			case EGamePlayModeTypeEnum::VE_FloorStore:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_FloorShop, 0);
				break;
			case EGamePlayModeTypeEnum::VE_Guild:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Guild, 0);
				break;
			case EGamePlayModeTypeEnum::VE_SingleRaid:
				bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_SingleRaid, 0);
				break;
			}
		}
	}

// 	if (bRtContent)
// 	{
// 		RGameInstance->NotifyEventList->RemoveNotifyEvent(this);
// 	}

	return bRtContent;
}