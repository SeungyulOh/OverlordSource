// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_FloorShopScrollListItem.h"
#include "UC_FloorShop_ScrollItem.h"

void UUC_FloorShopScrollListItem::NativeConstruct()
{
	Super::NativeConstruct();
	//FindChildWidget
	FindChildWidgetsWithCertainName<UUC_FloorShop_ScrollItem>(this, itemList, TEXT("UC_FloorShop_ScrollItem_"));
}

void UUC_FloorShopScrollListItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_FloorShopScrollListItem::InvalidateData()
{
	UUC_FloorShopScrollListItemData* listData = Cast<UUC_FloorShopScrollListItemData>(ItemData);

	for (int32 i = 0; i < FLOORSHOP_SLOTNUM_INROW; i++)
	{
		itemList[i]->SetItemInfo(listData->dataList[i]);
	}
	Text_FloorNum->SetText(FText::AsNumber(listData->floorNum));

	if (listData->dataList[0].state == ESHOP_FLOORSHOP_SLOT_STATE::ENABLED_BUY)
		Text_LockDesc->SetVisibility(ESlateVisibility::Collapsed);
	else
		Text_LockDesc->SetVisibility(ESlateVisibility::Visible);
}
