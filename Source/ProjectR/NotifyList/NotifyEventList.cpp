// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "NotifyEventList.h"

#include "GlobalIntegrated.h"
#include "NotifyEventUnlockContents.h"


UNotifyEvent* UNotifyEventList::AddNotifyEvent(ENotifyEventTypeEnum eventtype)
{
	SetNotifyStateFlag(eventtype);

	switch (eventtype)
	{
	case ENotifyEventTypeEnum::VE_GetNewHero:
	case ENotifyEventTypeEnum::VE_GetNewItem:
	case ENotifyEventTypeEnum::VE_AvatarLevelUp:
	case ENotifyEventTypeEnum::VE_CompleteMission:
	{
		for (UNotifyEvent* notify : NotifyList)
		{
			if (IsValid(notify))
			{
				if (notify->GetNotifyEventType() == eventtype)
				{
					return notify;
				}
			}
		}
	}
	break;
	default:
		break;
	}

	UNotifyEvent* notifyEvent = nullptr;
	switch (eventtype)
	{
	case ENotifyEventTypeEnum::VE_UnlockContents:
		notifyEvent = NewObject< UNotifyEventUnlockContents>((UObject*)GetTransientPackage(), UNotifyEventUnlockContents::StaticClass());
		break;
	default:
		notifyEvent = NewObject< UNotifyEvent>((UObject*)GetTransientPackage(), UNotifyEvent::StaticClass());
		break;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(notifyEvent))
	{
		notifyEvent->EventType = eventtype;
		notifyEvent->RGameInstance = RGameInstance;
		NotifyList.Emplace(notifyEvent);

		OnAddNotify.Broadcast(notifyEvent);
	}

	if ( IsValid(RGameInstance->NotifyManager))
	{
		if (NotifyList.Num() > 0)
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, true);
		}
		else
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, false);
		}
	}

	return notifyEvent;
}

void UNotifyEventList::RemoveNotifyEvent(UNotifyEvent* InNotifyEvent)
{
	if (IsValid(InNotifyEvent))
	{
		if (NotifyList.Contains(InNotifyEvent))
		{			
			NotifyList.Remove(InNotifyEvent);
			OnRemoveNotify.Broadcast(InNotifyEvent);
		}
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyManager))
	{
		if (NotifyList.Num() > 0)
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, true);
		}
		else
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, false);
		}
	}
}

void UNotifyEventList::ClearList()
{
	NotifyList.Empty();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, false);
	}
}

void UNotifyEventList::SetNotifyStateFlag(ENotifyEventTypeEnum InEventType)
{
	ENotifyStateFlagEnum notifyStateFlag = ENotifyStateFlagEnum::BM_None;
	switch (InEventType)
	{
		case ENotifyEventTypeEnum::VE_UnlockContents:
			break;
		case ENotifyEventTypeEnum::VE_GetNewHero:
			notifyStateFlag = ENotifyStateFlagEnum::BM_Hero;
			break;
		case ENotifyEventTypeEnum::VE_GetNewItem:
			notifyStateFlag = ENotifyStateFlagEnum::BM_Item;
			break;
		case ENotifyEventTypeEnum::VE_AvatarLevelUp:
			break;
		case ENotifyEventTypeEnum::VE_DailyQuest:			
			break;
		case ENotifyEventTypeEnum::VE_CompleteMission:
			notifyStateFlag = ENotifyStateFlagEnum::BM_Achievement;
			break;
	}

	if (notifyStateFlag != ENotifyStateFlagEnum::BM_None)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->NotifyManager))
		{
			RGameInstance->NotifyManager->SetNotify(notifyStateFlag, true);
		}
	}
}