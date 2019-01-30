// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_FloorShop_New.h"
#include "UC_Popup_FloorShop_Item.h"
#include "UI/HeroInfo/UC_Popup_HeroBreakUp_Splash.h"
#include "Network/PacketFunctionLibrary.h"
#include "UC_FloorShopScrollListItem.h"
#include "UI/Control/RScrollView.h"
#include "Global/RGameInstance.h"
#include "Global/ShopManager.h"
#include "Utils/UIFunctionLibrary.h"

void UUP_FloorShop_New::NativeConstruct()
{
	UPacketFunctionLibrary::SHOP_FLOORSHOP_STATUS_RQ();
	Super::NativeConstruct();
	URGameInstance* GameIns = RGAMEINSTANCE(this);
	if(ScrollView_ItemList->ScrollItemList.Num() == 0)
		ScrollView_ItemList->Init_NoClear(FLOORSHOP_ROWNUM);

	GameIns->ShopManager->BuyItemDelegate.AddDynamic(this, &UUP_FloorShop_New::OpenBuyConfirmPopup);
	
	if (cachedListData.Num() == FLOORSHOP_ROWNUM)
		return;

	cachedListData.Reset(FLOORSHOP_ROWNUM);
	for (int32 i = 0; i < FLOORSHOP_ROWNUM; i++)
	{
		URScrollItemData* ItemData = ScrollView_ItemList->CreateScrollItemData(UUC_FloorShopScrollListItemData::StaticClass());
		ItemData->WidgetSize = ScrollView_ItemList->GetItemInitSize();
		cachedListData.Emplace(Cast<UUC_FloorShopScrollListItemData>(ItemData));
	}
}

void UUP_FloorShop_New::NativeDestruct()
{
	URGameInstance* GameIns = RGAMEINSTANCE(this);

	GameIns->ShopManager->BuyItemDelegate.RemoveDynamic(this, &UUP_FloorShop_New::OpenBuyConfirmPopup);

	Super::NativeDestruct();
}

void UUP_FloorShop_New::OpenBuyConfirmPopup(FSHOP_FLOORSHOP_SLOT slotInfo)
{
	Popup_SellItem->SetItemInfo(slotInfo);
	Popup_SellItem->SetVisibility(ESlateVisibility::Visible);
}

void UUP_FloorShop_New::SetItemList(TArray<FSHOP_FLOORSHOP_SLOT> dataList)
{
	ScrollView_ItemList->ClearScrollItemData();
	for (int32 row = 0; row < FLOORSHOP_ROWNUM; row++)
	{
		UUC_FloorShopScrollListItemData* rowData = Cast<UUC_FloorShopScrollListItemData>(cachedListData[row]);
		rowData->floorNum = row * 5 + 1;
		rowData->dataList.Reset(FLOORSHOP_SLOTNUM_INROW);
		for (int32 slot = 0; slot < FLOORSHOP_SLOTNUM_INROW; slot++)
			rowData->dataList.Emplace(dataList[slot + row * FLOORSHOP_SLOTNUM_INROW]);

		ScrollView_ItemList->ItemDataList.Emplace(rowData);
	}

	ScrollView_ItemList->InvalidateData();
}

void UUP_FloorShop_New::SetRemainTime(int32 remainSec)
{
	URGameInstance* GameIns = RGAMEINSTANCE(this);
	m_RemainSec = remainSec;
	GameIns->TimerManager->ClearTimer(shopResetTime);
	GameIns->TimerManager->SetTimer(shopResetTime, this, &UUP_FloorShop_New::RefreshTimerUI, 1.f);

}

void UUP_FloorShop_New::RefreshTimerUI()
{
	int32 iHour = (int32)(m_RemainSec / 3600.0f);
	int32 iMin = (int32)((m_RemainSec - (float)iHour * 3600.0f) / 60.0f);
	int32 iSec = (int32)(m_RemainSec - (float)iHour * 3600.0f - (float)iMin * 60.0f);

// 	FNumberFormattingOptions* numberFormat = new FNumberFormattingOptions();
// 	numberFormat->SetMinimumIntegralDigits(2);
//	UUIFunctionLibrary::Get_0n_Number(iHour);
	Text_ShopTime_Hour->SetText(UUIFunctionLibrary::Get_0n_Number(iHour));//FText::AsNumber(iHour, numberFormat));
	Text_ShopTime_Min->SetText(UUIFunctionLibrary::Get_0n_Number(iMin));// FText::AsNumber(iMin, numberFormat));
	Text_ShopTime_Sec->SetText(UUIFunctionLibrary::Get_0n_Number(iSec));// FText::AsNumber(iSec, numberFormat));

	m_RemainSec--;

	if(m_RemainSec == 0)
		UPacketFunctionLibrary::SHOP_FLOORSHOP_STATUS_RQ();
}
