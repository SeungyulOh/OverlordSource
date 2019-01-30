// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "NotifyEvent.h"

#include "GlobalIntegrated.h"
#include "NotifyList/NotifyEventList.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

bool UNotifyEvent::GoShortCuts()
{
	bool bRtContent = false;
	if ( IsValid(RGameInstance->RWidgetManager) && IsValid(RGameInstance->NotifyEventList))
	{
		
		switch (EventType)
		{
		case ENotifyEventTypeEnum::VE_UnlockContents:
			break;
		case ENotifyEventTypeEnum::VE_GetNewHero:
			bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_DetailHeros, 0);
			break;
		case ENotifyEventTypeEnum::VE_GetNewItem:
			bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_ItemManagement, 0);
			break;
		case ENotifyEventTypeEnum::VE_AvatarLevelUp:
			break;
		case ENotifyEventTypeEnum::VE_DailyQuest:
			break;
		case ENotifyEventTypeEnum::VE_CompleteMission:
			bRtContent = UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Mission, 0);
			break;
		}

// 		if (bRtContent)
// 		{
// 			RGameInstance->NotifyEventList->RemoveNotifyEvent(this);
// 		}
	}

	return bRtContent;
}
