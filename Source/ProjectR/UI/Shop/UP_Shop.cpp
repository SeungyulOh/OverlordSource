// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Shop.h"
#include "Global/ShopManager.h"
#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UI/Shop/ShopScrollData.h"
#include "UI/Shop/PurchasePopup.h"
#include "UI/Shop/ShopScrollItem.h"
#include "UI/Shop/ShopListViewControlEventDelegator.h"
#include "UI/Shop/UC_HeroBox.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Particle.h"

const int32 MAXSCROLLITEM = 12;
const int32 MAXSCROLLITEMDATA = 30;

void UUP_Shop::NativeConstruct()
{	
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Hidden);
	InitProductScrollList();

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	//bFirst = true;
	//OnChangeShopData();


	//RGameInstance->TimerManager->SetTimer(UpdateResetRemainTimeHandler, this, &UUP_Shop::UpdateResetRemainTime, 1.0f, true);
	RGameInstance->ShopManager->OnShopDataChange.RemoveDynamic(this, &UUP_Shop::OnChangeShopData);
	RGameInstance->ShopManager->OnShopDataChange.AddDynamic(this, &UUP_Shop::OnChangeShopData);
	UPacketFunctionLibrary::SHOP_USER_DATA_RQ();

	if (UC_Particle)
	{
		UC_Particle->ActivateSelectedParticle();
	}

}


void UUP_Shop::NativeDestruct()
{
	if (ProductListScrollView && ProductListScrollView->EventDelegator)
	{
		UShopListViewControlEventDelegator* EventDelegator = Cast< UShopListViewControlEventDelegator >(ProductListScrollView->EventDelegator);
		if (IsValid(EventDelegator))
		{
			EventDelegator->PurchasePopupRequest.RemoveDynamic(this, &UUP_Shop::ShowPurchasePopup);
		}
	}

	URGameInstance* RGameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (RGameInstance)
	{
		if (RGameInstance->TimerManager)
		{
			if (UpdateResetRemainTimeHandler.IsValid())
			{
				RGameInstance->TimerManager->ClearTimer(UpdateResetRemainTimeHandler);
			}
		}
		if (RGameInstance->ShopManager)
		{
			if (RGameInstance->ShopManager->OnShopDataChange.IsBound())
			{
				RGameInstance->ShopManager->OnShopDataChange.RemoveDynamic(this, &UUP_Shop::OnChangeShopData);
			}
		}
	}

	Super::NativeDestruct();
}

void UUP_Shop::InitProductScrollList()
{
	if (ProductListScrollView)
	{
		ProductListScrollView->SetVisibility(ESlateVisibility::Hidden);

		if (!ProductListScrollView->ScrollItemList.Num())
			ProductListScrollView->Init_NoClear(MAXSCROLLITEM);

		if (!CachedItemDataList.Num())
		{
			CachedItemDataList.Reset(MAXSCROLLITEMDATA);
			FVector2D ItemInitSize = ProductListScrollView->GetItemInitSize();
			for (int32 i = 0; i < MAXSCROLLITEMDATA; ++i)
			{
				UShopScrollData* ItemData = Cast<UShopScrollData>(ProductListScrollView->CreateScrollItemData(UShopScrollData::StaticClass()));
				ItemData->WidgetSize = ItemInitSize;
				CachedItemDataList.Emplace(ItemData);
			}
		}
		
		if(!IsValid(ProductListScrollView->EventDelegator))
			ProductListScrollView->EventDelegator = NewObject< UShopListViewControlEventDelegator >((UObject*)GetTransientPackage(), UShopListViewControlEventDelegator::StaticClass());
		
		if(IsValid(ProductListScrollView->EventDelegator))
		{
			UShopListViewControlEventDelegator* eventdelegate = Cast<UShopListViewControlEventDelegator>(ProductListScrollView->EventDelegator);
			if (eventdelegate)
			{
				eventdelegate->PurchasePopupRequest.Clear();
				eventdelegate->PurchasePopupRequest.AddDynamic(this, &UUP_Shop::ShowPurchasePopup);
			}
		}
	}
	CurrentShopDisplayIndex = -1;
}


void UUP_Shop::SetShopDisplayPage(int32 InDisplayIndex, bool bForceUpdate )
{
	if (!bForceUpdate)
	{
		if (CurrentShopDisplayIndex == InDisplayIndex)
			return;
	}

	CurrentShopDisplayIndex = InDisplayIndex;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UUIFunctionLibrary::RefreshTopbarProperty_InShop(RGameInstance, InDisplayIndex);
	
	/*
	currency update
	*/
	//InvalidateCurrencyDisplay(); //none use
	
	UpdateProductList();
}


void UUP_Shop::UpdateProductList()
{
	if (ProductListScrollView == nullptr)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	auto ShopManager = RGameInstance->ShopManager;
	if (ShopManager == nullptr)
		return;
	
	//ProductListScrollView->ClearScrollItemData();

	TArray< FName > CurrentDisplayListArray;
	ShopManager->GetShopDisplayList(CurrentShopDisplayIndex, CurrentDisplayListArray);
	int32 Max = (CurrentDisplayListArray.Num() > MAXSCROLLITEMDATA ? MAXSCROLLITEMDATA : CurrentDisplayListArray.Num());
	ProductListScrollView->ClearScrollItemData(MAXSCROLLITEMDATA);

	for (int32 i = 0; i < Max; ++i)
	{
		FName TargetID = CurrentDisplayListArray[i];
		FShopProductInfo* productInfo = ShopManager->GetShopProductInfoPtr(TargetID);
		//(*CurrentDisplayList)[i];
		if (productInfo == nullptr)
			continue;

		FShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetShopProductRow(productInfo->ProductID);
		if (ProductTableInfo == nullptr
			|| ProductTableInfo->displayGroup != CurrentShopDisplayIndex)
			continue;

		/*
		Item status
		*/

		UShopScrollData* shopScrollData = CachedItemDataList[i];
		shopScrollData->ProductID = productInfo->ProductID;
		shopScrollData->ProductType = productInfo->ProductType;
		shopScrollData->ProductName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, productInfo->ProductName);
		shopScrollData->ProductDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, productInfo->ProductDesc);
		shopScrollData->CurrentPurchaseCount = ShopManager->GetShopProductBuyCount(TargetID);
		shopScrollData->MaxPurchaseCount = productInfo->MaxPurchaseCount; //todo count code
		shopScrollData->CurrencyType = productInfo->CurrencyType;
		shopScrollData->Price = productInfo->Price;
		shopScrollData->DisplayGroup = ProductTableInfo->displayGroup;
		shopScrollData->buyType = ProductTableInfo->buyType;

		/*
		Icon setting
		*/
		shopScrollData->ItemIconInfo.itemId = ProductTableInfo->DisplayItemID;
		shopScrollData->ItemIconInfo.itemGrade = ProductTableInfo->Grade;
		shopScrollData->ItemIconInfo.count = productInfo->SellCount;
		shopScrollData->ItemIconInfo.ItemIconRef = UUtilFunctionLibrary::GetTexture2DFromAssetID(ProductTableInfo->ItemIconRef);

		ProductListScrollView->ItemDataList.Emplace(shopScrollData);
		//ProductListScrollView->AddItemDataList
	}
	
	ProductListScrollView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ProductListScrollView->InvalidateData();
}


int32 UUP_Shop::GetMaxResetCount(int32 InDisplayIndex)
{
	FShopDisplayGroupTableInfo* ShopDisplayGroupInfo = UTableManager::GetInstancePtr()->GetShopDisplayGroupRow(*(FString::FromInt(InDisplayIndex)));
	if (ShopDisplayGroupInfo)
	{
		return ShopDisplayGroupInfo->resetPrice.Num();
	}
	return 0;
}

void UUP_Shop::InvalidateCurrencyDisplay()
{
	/*auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	for (UUC_CurrencyDisplay* currencyDisplay : CurrencyDisplayList)
	{
		if (currencyDisplay == nullptr)
			continue;
		
		int32 currencyValue = RGameInstance->RInventory->GetCurrency(currencyDisplay->CurrentCurrencyType);
		currencyDisplay->SetQuantity(currencyValue);
	}

	if (CurrencySelectSwitch)
	{
		CurrencySelectSwitch->SetActiveWidgetIndex(CurrentShopDisplayIndex);
	}*/
}



void UUP_Shop::OnClick_Purchase(FName InProductID)
{
	if (PurchasePopupPanel)
		PurchasePopupPanel->SetVisibility(ESlateVisibility::Collapsed);

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	
	FShopProductInfo* ProductInfo = RGameInstance->ShopManager->GetShopProductInfoPtr(InProductID);
	if (ProductInfo)
	{
		if (ProductInfo->CurrencyType == WALLET_CASH)
		{
			OnCashEvent();
			//RGameInstance->BillingManager->Purchase(ProductInfo->ProductID.ToString());
			return;
		}

		int32 userCurrency = RGameInstance->RInventory->GetCurrency(ProductInfo->CurrencyType);
		if( userCurrency < ProductInfo->Price )
		{
			FText caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Fail_Purchase"));
			FText content = UUIFunctionLibrary::GetCurrentyWarningMessage(ProductInfo->CurrencyType);
			UUIFunctionLibrary::ShowCommonPopup(caption, content, ECommonPopupType::VE_OK);
			return;
		}
		UPacketFunctionLibrary::SHOP_BUY_RQ(ProductInfo->ProductID.ToString(), 
			(int32)ProductInfo->ProductType, 
			ProductInfo->SellCount, 
			ProductInfo->CurrencyType.ToString(), 
			ProductInfo->Price);

// 		UPacketFunctionLibrary::SHOP_BUY_RQ(this,
// 			ProductInfo->ProductID.ToString(),
// 			(int32)ProductInfo->ProductType,
// 			ProductInfo->SellCount,
// 			(int32)ProductInfo->CurrencyType,
// 			ProductInfo->Price);
	}
}

void UUP_Shop::OnClick_ResetRequest()
{
	/*auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	FText caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Shop_Fail_Reset")));
	FText content;
	int32 MaxResetCount = GetMaxResetCount(CurrentShopDisplayIndex);
	
	
	FSHOP_DISPLAY_GROUP_STATUS displayGroupStatus = RGameInstance->ShopManager->GetShopDisplayResetStatus(CurrentShopDisplayIndex);
	if (displayGroupStatus.resetCount == MaxResetCount)
	{		
		content = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Shop_Max_Reset_Count")));
		UUIFunctionLibrary::ShowCommonPopup( caption, content, ECommonPopupType::VE_OK );
		return;
	}

	FShopDisplayGroupTableInfo* ShopDisplayGroupTableInfo = RGameInstance->ShopManager->GetShopDisplayGroupInfo(CurrentShopDisplayIndex);
	if (ShopDisplayGroupTableInfo)
	{
		if (ShopDisplayGroupTableInfo->resetPrice.IsValidIndex(displayGroupStatus.resetCount) == false)
			return;

		int32 userCurrenty = RGameInstance->UserInfo->GetUserCurrency(ShopDisplayGroupTableInfo->currencyType);
		if (userCurrenty < ShopDisplayGroupTableInfo->resetPrice[displayGroupStatus.resetCount])
		{
			content = UUIFunctionLibrary::GetCurrentyWarningMessage(ShopDisplayGroupTableInfo->currencyType);
			UUIFunctionLibrary::ShowCommonPopup(caption, content, ECommonPopupType::VE_OK);
			return;
		}
	}
	*/

	
}

void UUP_Shop::OnClick_Close()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

}



void UUP_Shop::ShowPurchasePopup( UShopScrollData* InSelectedData )
{
	if (InSelectedData == nullptr)
		return;

	if (PurchasePopupPanel)
	{
		PurchasePopupPanel->SetVisibility(ESlateVisibility::Visible);
		//if (ScrollItemInPurchasePopup )
		//{			
		//	ScrollItemInPurchasePopup->SetShopScrollItem(InSelectedData);
		//	ScrollItemInPurchasePopup->RequestPurchaseDelegate.RemoveDynamic(this, &UUP_Shop::OnClick_Purchase);
		//	ScrollItemInPurchasePopup->RequestPurchaseDelegate.AddDynamic(this, &UUP_Shop::OnClick_Purchase);			
		//}	

		if (IsValid(PopupItemIcon))
		{
			PopupItemIcon->SetItemIconStructure(InSelectedData->ItemIconInfo);
		}

		if (IsValid(PopupTitleText))
		{
			PopupTitleText->SetText(InSelectedData->ProductName);
		}

		if (IsValid(PopupDescText))
		{
			PopupDescText->SetText(InSelectedData->ProductDesc);
		}

		CurPopupProductID = InSelectedData->ProductID;
	}
}

void UUP_Shop::ClosePurchasePopup()
{
	if (IsValid(PurchasePopupPanel))
	{
		if (PurchasePopupPanel->GetVisibility() != ESlateVisibility::Collapsed)
		{
			PurchasePopupPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void UUP_Shop::OnChangeShopData()
{
	//SetShopDisplayPage(CurrentShopDisplayIndex, true );
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance))
	{
		if (RadioGroup)
		{
			RadioGroup->SetActiveRadioIndex(0);
		}
		SetShopDisplayPage(1, true);

		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		/*if (bFirst)
		{
			bFirst = false;
		}*/
	}
}
