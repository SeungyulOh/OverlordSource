// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_FloorSelector.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "Global/RGameInstance.h"
#include "Global/CampaignManager.h"
#include "Global/QuestManager.h"
#include "GameInfo/UserInfo.h"

#include "Network/PacketFunctionLibrary.h"


void UUC_FloorSelector::NativeConstruct()
{
	Super::NativeConstruct();

	if (variables.Button_Menu)
	{
		variables.Button_Menu->OnClicked.Clear();
		variables.Button_Menu->OnClicked.AddDynamic(this, &UUC_FloorSelector::OnClick_Menu);
	}

	if (variables.ScrollView)
	{
		variables.ScrollView->Custom_Init();
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FTOWER_STATUS TowerStatus = RGameInstance->CampaignManager->TowerStatus;
	for (size_t i = 0; i < variables.ScrollView->ItemDataList.Num(); ++i)
	{
		URFloorScrollItemData* ItemData = Cast<URFloorScrollItemData>(variables.ScrollView->ItemDataList[i]);
		if (ItemData)
		{
			if (TowerStatus.currentFloor == ItemData->Floor)
			{
				variables.ScrollView->MoveScrollPos(ItemData);
			}
		}
	}

	for (size_t i = 0; i < variables.ScrollView->ScrollItemList.Num(); ++i)
	{
		URFloorScrollItemData* ItemData = Cast<URFloorScrollItemData>(variables.ScrollView->ScrollItemList[i]->ItemData);
		if (ItemData)
		{
			int32 TargetFloor = TowerStatus.currentFloor;//RGameInstance->UserInfo->Avatar.elevatedFloor;
			if (RGameInstance->QuestManager->IsNavigationUsing())
			{
				int32 nbaseIndex = 0;
				int32 nCurrentQuest = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
				int32 nFloor = 0;

				for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
				{
					nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
					if (nbaseIndex >= nCurrentQuest)
						break;
				}
				TargetFloor = nFloor + 1;
			}
			if (TargetFloor == ItemData->Floor)
			{
				URFloorScrollItem* ScrollItem = Cast<URFloorScrollItem>(variables.ScrollView->ScrollItemList[i]);
				if (ScrollItem)
				{
					ScrollItem->Selected();
					OnClick_Menu();
				}
				break;
			}
		}
	}
}

void UUC_FloorSelector::OnClick_Menu()
{
	int32 CurrentIdx = variables.WidgetSwitcher_State->GetActiveWidgetIndex();
	int32 NextIdx = CurrentIdx == 0 ? 1 : 0;

	variables.WidgetSwitcher_State->SetActiveWidgetIndex(NextIdx);
}

void UUC_FloorSelector::ShowFloorSelector(bool bShow)
{
	variables.WidgetSwitcher_State->SetActiveWidgetIndex(bShow ? 1 : 0);
}

void URFloorScrollView::Custom_Init()
{
	if(!ScrollItemList.Num())
	{
		for (int32 i = 0; i < 9; ++i)
		{
			ScrollItemList_AddElement();
		}
	}
	ResetScrollAnimationState();

	/*
	if (!ItemDataList.Num())
	{
		FAVATAR& Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
		for (size_t i = 0; i < Avatar.elevatedFloor; ++i)
		{
			URFloorScrollItemData* ScrollItemData = Cast<URFloorScrollItemData>(AddScrollItemData(URFloorScrollItemData::StaticClass()));

			if (!IsValid(ScrollItemData))
				continue;

			ScrollItemData->Floor = i+1;
		}
	}*/
	
	InvalidateData();
}

void URFloorScrollView::InvalidateData()
{
	Super::InvalidateData();

	for (size_t i = 0; i < ScrollItemList.Num(); ++i)
	{
		URFloorScrollItem* ScrollItem = Cast<URFloorScrollItem>(ScrollItemList[i]);
		if (ScrollItem)
		{
			if (ItemDataList.IsValidIndex(ScrollItem->Count))
			{
				URFloorScrollItemData* targetitemdata = Cast<URFloorScrollItemData>(ItemDataList[ScrollItem->Count]);
				if (targetitemdata)
				{
					FText txt = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_FloorShop_FloorControl")), FText::AsNumber(targetitemdata->Floor));
					ScrollItem->FloorText1->SetText(txt);
					ScrollItem->FloorText2->SetText(txt);
				}
			}
			
		}
	}
}

void URFloorScrollView::ScrollItemList_ChangeItemDataList(int32 dataIndex)
{
	Super::ScrollItemList_ChangeItemDataList(dataIndex);

	for (size_t i = 0; i < ScrollItemList.Num(); ++i)
	{
		URFloorScrollItem* ScrollItem = Cast<URFloorScrollItem>(ScrollItemList[i]);
		if (ScrollItem)
		{
			if (ItemDataList.IsValidIndex(ScrollItem->Count))
			{
				URFloorScrollItemData* targetitemdata = Cast<URFloorScrollItemData>(ItemDataList[ScrollItem->Count]);
				if (targetitemdata)
				{
					FText txt = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_FloorShop_FloorControl")), FText::AsNumber(targetitemdata->Floor));
					ScrollItem->FloorText1->SetText(txt);
					ScrollItem->FloorText2->SetText(txt);

					int32 iFoundIdx = SelectedScrollItemList.Find(ScrollItem->Count);
					if (iFoundIdx != INDEX_NONE)
						ScrollItem->SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					else
						ScrollItem->SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

		}
	}
}

void URFloorScrollView::MoveScrollPos(URScrollItemData* SelectItemData)
{
	int32 FindIdx = -1;
	for (int32 i = 0; i < ItemDataList.Num(); ++i)
	{
		if (IsValid(ItemDataList[i]))
		{
			if (SelectItemData == ItemDataList[i])
			{
				FindIdx = i;
				break;
			}
		}
	}

	float OffSetY = ScrollViewSize.Y * 0.5f - ItemInitSize.Y * 0.5f;

	if (FindIdx != -1)
	{
		FVector2D diff = FVector2D::ZeroVector;
		diff.Y = -ItemInitSize.Y * FindIdx;
		if (FMath::Abs(diff.Y) < OffSetY)
			diff.Y = 0.f;
		else
			diff.Y += OffSetY;

		if (ScrollItemList.Num() > 0)
		{
			CurScrollOffset = 0;
			SetScrollUpdate(ScrollItemList[0], diff);
		}
	}
}

void URFloorScrollItem::Selected()
{
	Super::Selected();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance->QuestManager->IsNavigationUsing())
// 		return;

	/*URFloorScrollItemData* ScrollItemData = Cast<URFloorScrollItemData>(ItemData);
	if (ScrollItemData)
	{
		FAVATAR Avatar = RGameInstance->UserInfo->GetAvatarData();
		if (Avatar.elevatedFloor >= ScrollItemData->Floor)
		{
			for (auto& Element : ParentScrollView->ScrollItemList)
			{
				URFloorScrollItem* ScrollItem = Cast<URFloorScrollItem>(Element);
				if (ScrollItem)
					ScrollItem->SelectedImage->SetVisibility(ESlateVisibility::Collapsed);

			}
			URBaseWidget* Parent = ParentScrollView->GetParentWidget();
			UUC_FloorSelector* FloorSelector = Cast<UUC_FloorSelector>(Parent);
			if (FloorSelector)
				FloorSelector->OnClick_Menu();

			SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (!RGameInstance->QuestManager->IsNavigationUsing())
				UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(ScrollItemData->Floor);
			Cast<URFloorScrollView>(ParentScrollView)->MoveScrollPos(ScrollItemData);
		}
		else
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("Floor_Stage_Move_Confirm_Error_Message"))));
		}
	}*/
}
