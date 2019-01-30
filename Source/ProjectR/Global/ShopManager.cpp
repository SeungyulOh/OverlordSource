// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ShopManager.h"
#include "RGameInstance.h"
#include "TableManager.h"

#include "Table/ShopProductTableInfo.h"
#include "Table/ShopDisplayGroupTableInfo.h"

#include "UI/RWidgetManager.h"
#include "UI/Shop/FloorShop/UP_FloorShop.h"

#include "Network/PacketFunctionLibrary.h"
#include "UI/Shop/FloorShop/UP_FloorShop_New.h"

#define MAX_DISPLAYGROUP 5

void UShopManager::Initialize(/*URGameInstance* RGameInstance*/)
{ 
//	GameInstance = RGameInstance;
	
	PrepareSellingProducts();
	PrepareShopDisplayGroup();
}

void UShopManager::PrepareSellingProducts()
{
	TArray< FName > ProductIds = UTableManager::GetInstancePtr()->ShopProductTableInfo->GetRowNames();
	
	SellingProducts.Empty();
	ShopProductList.Empty(ProductIds.Num());

	for (FName& productID : ProductIds)
	{
		FShopProductTableInfo* ShopProductTableInfo = UTableManager::GetInstancePtr()->GetShopProductRow(productID);
		if (ShopProductTableInfo)
		{
			if (SellingProducts.Contains(ShopProductTableInfo->displayGroup) == false)
			{
				SellingProducts.Emplace(ShopProductTableInfo->displayGroup);
			}
			SellingProducts[ShopProductTableInfo->displayGroup].Emplace( productID);
			
			FShopProductInfo ShopProductInfo;
			ShopProductInfo.ProductID				= productID;
			ShopProductInfo.ProductType				= ShopProductTableInfo->productType;
			ShopProductInfo.ProductKey				= ShopProductTableInfo->productKey;
			ShopProductInfo.ProductName				= ShopProductTableInfo->productName;
			ShopProductInfo.ProductDesc				= ShopProductTableInfo->productDesc;
			ShopProductInfo.SellCount				= ShopProductTableInfo->amount;
			ShopProductInfo.MaxPurchaseCount		= ShopProductTableInfo->buyCount;
			ShopProductInfo.CurrentPurchaseCount	= 0;// ShopProductTableInfo->buyCount;
			ShopProductInfo.CurrencyType			= ShopProductTableInfo->currencyType;
			ShopProductInfo.Price					= ShopProductTableInfo->price;
			ShopProductInfo.buyType					= ShopProductTableInfo->buyType;

			ShopProductList.Emplace(ShopProductInfo);
		}
	}
}

void UShopManager::PrepareShopDisplayGroup()
{
	TArray< FName > groupIds = UTableManager::GetInstancePtr()->ShopDisplayGroupTableInfo->GetRowNames();

	ShopDisplayGroups.Empty(MAX_DISPLAYGROUP);
	for (FName& displayGroup : groupIds)
	{
		int32 displayGroupIndex = FCString::Atoi(*(displayGroup.ToString()));
		if (ShopDisplayGroups.Contains(displayGroupIndex) == false)
		{
			ShopDisplayGroups.Emplace(displayGroupIndex);
		}
		FShopDisplayGroupTableInfo* ShopDisplayGroupTableInfo = UTableManager::GetInstancePtr()->GetShopDisplayGroupRow(displayGroup);
		ShopDisplayGroups[displayGroupIndex] = ShopDisplayGroupTableInfo;
	}
}

void UShopManager::SetShopUserData(FSHOP_USER_DATA& InShopUserData)
{
	ShopUserData = InShopUserData;
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	GameInstance->TimerManager->ClearTimer(ResetRemainTimeHandler);
	//GameInstance->TimerManager->SetTimer(ResetRemainTimeHandler, this, &UShopManager::UpdateResetRemainTime, 1.0f, true);

	OnShopDataChange.Broadcast();
}


FSHOP_PRODUCT_STATUS UShopManager::GetShopProductStatus(FName InProductID)
{
	FString CompareProductID = InProductID.ToString();
	for (FSHOP_PRODUCT_STATUS& productStatus : ShopUserData.productStatus)
	{
		if (productStatus.productId.Compare(CompareProductID))
			return productStatus;
	}

	return FSHOP_PRODUCT_STATUS();
}


int32 UShopManager::GetShopProductBuyCount(FName InProductID)
{
	FString CompareProductID = InProductID.ToString();
	for (FSHOP_PRODUCT_STATUS& productStatus : ShopUserData.productStatus)
	{
		if (productStatus.productId.Compare(CompareProductID))
			return productStatus.buyCount;
	}

	return 0;
}

FShopProductInfo* UShopManager::GetShopProductInfoPtr(FName InProductID)
{
	for (FShopProductInfo& productInfo : ShopProductList)
	{
		if (productInfo.ProductID == InProductID)
			return &productInfo;
	}

	return nullptr;
}

FShopDisplayGroupTableInfo* UShopManager::GetShopDisplayGroupInfo(int32 InDisplayGroupIdx)
{	
	if (ShopDisplayGroups.Contains(InDisplayGroupIdx))
	{
		return ShopDisplayGroups[InDisplayGroupIdx];
	}
	return nullptr;
}

TArray< FName >* UShopManager::GetShopDisplayList(int32 InDisplayGroupIdx)
{
	if (SellingProducts.Contains(InDisplayGroupIdx))
	{
		return &( SellingProducts[InDisplayGroupIdx] );
	}

	return nullptr;
}


void UShopManager::GetShopDisplayList(int32 InIndex, TArray< FName >& InOutArray)
{
	if (SellingProducts.Contains(InIndex))
	{
		InOutArray = SellingProducts[InIndex];
	}
}


void UShopManager::UpdateResetRemainTime(int32 remainSec)
{
	UUP_FloorShop_New* umg = Cast<UUP_FloorShop_New>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_FloorShop));

	if (umg)
		umg->SetRemainTime(remainSec);
	/*
	for (FSHOP_DISPLAY_GROUP_STATUS& groupStatus : ShopUserData.displayGroupStatus)
	{
		--groupStatus.resetTimer;
	}
	*/
}

void UShopManager::SetHeroShopPanelList(TArray<FHERO_SHOP_PANEL>& InList)
{
	HeroShopPanelList = InList;
	
	HeroShopPanelTimeMap.Empty(HeroShopPanelList.Num());

	for (FHERO_SHOP_PANEL& panel : HeroShopPanelList)
	{
		HeroShopPanelTimeMap.Emplace(panel.panelId, FDateTime::UtcNow());
	}
}

void UShopManager::UpdateHeroShopPanel(FHERO_SHOP_PANEL& InPanel)
{
	for (int32 i = 0; i < HeroShopPanelList.Num(); ++i)
	{
		if (HeroShopPanelList[i].panelId == InPanel.panelId)
		{
			HeroShopPanelList[i] = InPanel;

			if (HeroShopPanelTimeMap.Contains(InPanel.panelId))
			{
				HeroShopPanelTimeMap[InPanel.panelId] = FDateTime::UtcNow();
			}
			break;
		}
	}
}

FDateTime UShopManager::GetShopPanelTime(FString InPanelID)
{
	if (HeroShopPanelTimeMap.Contains(InPanelID))
	{
		return HeroShopPanelTimeMap[InPanelID];
	}

	return FDateTime::UtcNow();
}

void UShopManager::SetFloorShopSlotArrData(TArray<FSHOP_FLOORSHOP_SLOT> dataList)
{

	if (dataList.Num() > 0)
	{
		UUP_FloorShop_New* umg = Cast<UUP_FloorShop_New>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_FloorShop));

		if (umg)
			umg->SetItemList(dataList);
		//FloorShopSlotArr = dataList;
	}

}

TArray<FSHOP_FLOORSHOP_SLOT> UShopManager::GetFloorShopSlotArrData()
{
	return FloorShopSlotArr;
}

void UShopManager::UpdateHeroProduct(FHERO_SHOP_PRODUCT& InProduct)
{
	bool bSeek = false;
	for (FHERO_SHOP_PANEL& panel : HeroShopPanelList)
	{
		for (int32 i = 0; i < panel.productList.Num(); ++i)
		{
			if (panel.productList[i].productId == InProduct.productId)
			{
				panel.productList[i] = InProduct;
				bSeek = true;
				break;
			}
		}

		if(bSeek)
			break;
	}
}

FHERO_SHOP_PANEL* UShopManager::GetHeroShopPanel(FString InPanelID)
{
	for (int32 i = 0; i < HeroShopPanelList.Num(); ++i)
	{
		if (HeroShopPanelList[i].panelId == InPanelID)
		{
			return &HeroShopPanelList[i];
		}
	}

	return nullptr;
}

FHERO_SHOP_PANEL* UShopManager::GetHeroShopPanelByProductID(FString InProductID)
{
	for (FHERO_SHOP_PANEL& panel : HeroShopPanelList)
	{
		for (int32 i = 0; i < panel.productList.Num(); ++i)
		{
			if (panel.productList[i].productId == InProductID)
			{
				return &panel;
			}
		}
	}

	return nullptr;
}

void UShopManager::RequestFloorStoreStatus(uint32 InFloor, bool InIsUseTicket)
{
// 	if (InFloor <= 0)
// 		InFloor = 1;
// 
// 	if (InFloor > 25)
// 		InFloor = 25;
// 
// 	UPacketFunctionLibrary::SHOP_FLOORSTORE_STATUS_RQ(InFloor, InIsUseTicket);
}

void UShopManager::RequestFloorShopItemBuy(int32 slotId)
{
	UPacketFunctionLibrary::SHOP_FLOORSHOP_BUY_RQ(CurrShopID, slotId);
}

void UShopManager::RequestFloorShopSlotOpen(int32 slotId)
{
	UPacketFunctionLibrary::SHOP_FLOORSHOP_OPEN_SLOT_RQ(slotId);
}

void UShopManager::RequestFloorStoreReset(uint32 InFloor, uint32 InStoreType, bool InIsUseGem)
{
// 	UPacketFunctionLibrary::SHOP_FLOORSTORE_RESET_RQ(InFloor, InStoreType, InIsUseGem);
}

// void UShopManager::ResponFloorStoreStatus(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus)
// {
// 	CurFloorStoreStatus = InCurFloorStoreStatus;
// 	UpdateFloorStoreStatus();
// }
// 
// void UShopManager::ResponFloorStoreItemBuy(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus, TArray<FREWARD>& InReward)
// {
// 	CurFloorStoreStatus = InCurFloorStoreStatus;
// 	Reward = InReward;
// 	UpdateFloorStoreItemBuy();
// }
// 
// void UShopManager::ResponFloorStoreReset(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus)
// {
// 	CurFloorStoreStatus = InCurFloorStoreStatus;
// 	UpdateFloorStoreReset();
// }
// 
void UShopManager::UpdateFloorStoreStatus()
{
	URGameInstance*		GameInstace			=	RGAMEINSTANCE(this);
	URWidgetManager*	WidgetManager		=	GameInstace->RWidgetManager;

	checkf(WidgetManager != nullptr, TEXT("UShopManager::UpdateFloorStoreStatus[WidgetManager is Empty]"));

// 	UUP_FloorShop*	Widget = static_cast<UUP_FloorShop*>(WidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_FloorShop));
// 
// 	if (IsValid(Widget))
// 	{
// 		if (CurFloorStoreStatus.closed)
// 		{
// 			Widget->ShopOpenPopupActive();
// 			Widget->SetUI();
// 		}
// 		else
// 		{
// 			Widget->ShopOpenPopupDisable();
// 			Widget->SetUI();
// 		}
// 	}
}

void UShopManager::UpdateFloorStoreItemBuy()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	URWidgetManager*	WidgetManager = GameInstace->RWidgetManager;

	checkf(WidgetManager != nullptr, TEXT("UShopManager::UpdateFloorStoreStatus[WidgetManager is Empty]"));

	UUP_FloorShop*	Widget = static_cast<UUP_FloorShop*>(WidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_FloorShop));

// 	if (IsValid(Widget))
// 	{
// 		if (CurFloorStoreStatus.closed)
// 		{
// 			Widget->ShopOpenPopupActive();
// 			Widget->SetUI();
// 		}
// 		else
// 		{
// 			Widget->ShopOpenPopupDisable();
// 			Widget->SetUI();
// 			Widget->SetRewardItem(Reward);
// 		}
// 	}
}

void UShopManager::UpdateFloorStoreReset()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	URWidgetManager*	WidgetManager = GameInstace->RWidgetManager;

	checkf(WidgetManager != nullptr, TEXT("UShopManager::UpdateFloorStoreStatus[WidgetManager is Empty]"));

	UUP_FloorShop*	Widget = static_cast<UUP_FloorShop*>(WidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_FloorShop));

// 	if (IsValid(Widget))
// 	{
// 		if (CurFloorStoreStatus.closed)
// 		{
// 			Widget->ShopOpenPopupActive();
// 			Widget->SetUI();
// 		}
// 		else
// 		{
// 			Widget->ShopOpenPopupDisable();
// 			Widget->SetUI();
// 		}
// 	}
}
