// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_LobbyNotifyScrollItem.h"

#include "GlobalIntegrated.h"

#include "UI/UI_Icon/HeroIconWithButton.h"
#include "UtilFunctionIntegrated.h"
#include "UI/UI_Lobby/LobbyNotifyScrollItemData.h"

#include "NotifyList/NotifyEvent.h"
#include "NotifyList/NotifyEventUnlockContents.h"




void UUC_LobbyNotifyScrollItem::NativeConstruct()
{
	Super::NativeConstruct();	

	if (BP_Btn_Move)
	{
		BP_Btn_Move->OnClicked.RemoveDynamic(this, &UUC_LobbyNotifyScrollItem::Go);
		BP_Btn_Move->OnClicked.AddDynamic(this, &UUC_LobbyNotifyScrollItem::Go);
	}


}

void UUC_LobbyNotifyScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (IsValid(item))
	{
		ULobbyNotifyScrollItemData* notifyItemData = Cast<ULobbyNotifyScrollItemData>(item);
		if (IsValid(notifyItemData))
		{
			if (IsValid(notifyItemData->NotifyEvent))
			{
				//FNotifyQueueTableInfo* notifyTableInfo = GetNotifyQueueTableInfo(notifyItemData->NotifyEvent->GetNotifyEventType());
				//if (notifyTableInfo)
				//{
				//	if (IsValid(TitleTextBlock))
				//	{
				//		int32 test = (int32)notifyItemData->NotifyEvent->GetNotifyEventType();
				//		TitleTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, notifyTableInfo->StringKey));						
				//	}
				//}
			}
		}
	}

	Super::SetScrollItemData(Index, item, ScrollView);
}

void UUC_LobbyNotifyScrollItem::Go()
{
	if (IsValid(ItemData))
	{
		ULobbyNotifyScrollItemData* notifyItemData = Cast<ULobbyNotifyScrollItemData>(ItemData);
		if (IsValid(notifyItemData))
		{
			if (IsValid(notifyItemData->NotifyEvent))
			{
				notifyItemData->NotifyEvent->GoShortCuts();
			}
		}
	}
}

//FNotifyQueueTableInfo* UUC_LobbyNotifyScrollItem::GetNotifyQueueTableInfo(ENotifyEventTypeEnum eventtype)
//{
//	/*TArray<FName> notifyTable = UTableManager::GetInstancePtr()->GetNotifyQueueRowsNames();
//	for (FName id : notifyTable)
//	{
//		FNotifyQueueTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetNotifyQueueRow(id);
//		if (tableInfo)
//		{
//			if (tableInfo->EventType == eventtype)
//				return tableInfo;
//		}
//	}
//	*/
//	return nullptr;
//}