// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/UIDefs.h"
#include "UP_LobbyNotifyList.h"

#include "GlobalIntegrated.h"

#include "NotifyList/NotifyEventList.h"
#include "NotifyList/NotifyDefs.h"
#include "NotifyList/NotifyEvent.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollView.h"
#include "UI/UI_Lobby/LobbyNotifyScrollItemData.h"
#include "UI/Control/RScrollView.h"

void UUP_LobbyNotifyList::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyEventList))
	{
		RGameInstance->NotifyEventList->OnAddNotify.RemoveDynamic(this, &UUP_LobbyNotifyList::OnAddNotifyEvent);
		RGameInstance->NotifyEventList->OnAddNotify.AddDynamic(this, &UUP_LobbyNotifyList::OnAddNotifyEvent);
		RGameInstance->NotifyEventList->OnRemoveNotify.RemoveDynamic(this, &UUP_LobbyNotifyList::OnRemoveNotifyEvent);
		RGameInstance->NotifyEventList->OnRemoveNotify.AddDynamic(this, &UUP_LobbyNotifyList::OnRemoveNotifyEvent);
	}

	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Notify, false);
	}

	InvalidateData();
}

void UUP_LobbyNotifyList::InvalidateData()
{
	if (IsValid(NotifyScrollView))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->NotifyEventList))
		{
			NotifyScrollView->ItemDataList.Reset();

			for (UNotifyEvent* notify : RGameInstance->NotifyEventList->NotifyList)
			{
				if (IsValid(notify))
				{
					ULobbyNotifyScrollItemData* ItemData = NewObject< ULobbyNotifyScrollItemData >((UObject*)GetTransientPackage(), ULobbyNotifyScrollItemData::StaticClass());
					if (IsValid(ItemData))
					{
						ItemData->NotifyEvent = notify;
						NotifyScrollView->ItemDataList.Emplace(ItemData);
					}					
				}
			}

			NotifyScrollView->Init();
			NotifyScrollView->InvalidateData();
		}
	}
}

void UUP_LobbyNotifyList::OnAddNotifyEvent(UNotifyEvent* notify)
{
	InvalidateData();
}

void UUP_LobbyNotifyList::OnRemoveNotifyEvent(UNotifyEvent* notify)
{
	InvalidateData();
}
