// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroLimiteProduct.h"
#include "GlobalIntegrated.h"


#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Shop/UC_HeroShopScrollItem.h"
#include "UI/Shop/UP_HeroShop.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroLimiteProduct::NativeConstruct()
{	
	Super::NativeConstruct();

	if (ClickBtn)
	{
		ClickBtn->OnClicked.RemoveDynamic(this, &UUC_HeroLimiteProduct::Buy);
		ClickBtn->OnClicked.AddDynamic(this, &UUC_HeroLimiteProduct::Buy);
	}
	if (Button_Refresh)
	{
		Button_Refresh->OnClicked.RemoveDynamic(this, &UUC_HeroLimiteProduct::ResetBtnClick);
		Button_Refresh->OnClicked.AddDynamic(this, &UUC_HeroLimiteProduct::ResetBtnClick);
	}

}

void UUC_HeroLimiteProduct::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(RestTimeProgress))
	{
		if (ShopPanel.productList.Num() > 0)
		{
			if (TotalTime > 0)
			{
				RestTimeProgress->SetPercent((float)RestTime / (float)TotalTime);
			}
			else
			{
				RestTimeProgress->SetPercent(0.0f);
			}
		}
	}

	if (bStart)
	{
		if (RestTime <= 0.0f)
		{
			if (!bSendRefreshPacket)
			{
				SendRefreshPacket();
			}
		}
	}
}

void UUC_HeroLimiteProduct::RefreshUI()
{
	Super::RefreshUI();

	if (ShopPanel.productList.Num() > 0)
	{
		FHERO_SHOP_PRODUCT shopproduct = ShopPanel.productList[0];

		if (IsValid(LockPanel))
		{
			if (ShopPanel.productList[0].remainQuantity > 0)
			{
				LockPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				LockPanel->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (IsValid(CurrencyValue))
		{
			CurrencyValue->SetCurrency(FName(*shopproduct.currencyType), shopproduct.price);
		}

		if (IsValid(ClickBtn))
		{
			//ClickBtn->SetIsEnabled(UUtilFunctionLibrary::IsCurrencyHave(GetWorld(), shopproduct.currencyType, shopproduct.price));
			if (ShopPanel.productList[0].remainQuantity > 0)
			{
				ClickBtn->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ClickBtn->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		FHeroShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetHeroShopProductTableInfo(FName(*shopproduct.productId));
		if (ProductTableInfo)
		{
			if (IsValid(TitleText1))
			{
				TitleText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, ProductTableInfo->productName));
			}

			if (IsValid(DescText))
			{
				DescText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, ProductTableInfo->productDesc));
			}

			int32 hour = ProductTableInfo->limitEndTime / 10000;
			int32 min = (ProductTableInfo->limitEndTime - hour * 10000) / 100;
			int32 sec = (ProductTableInfo->limitEndTime - hour * 10000 - min * 100);
			TotalTime = hour * 3600 + min * 60 + sec;

			if (IsValid(BackImage))
			{
				BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ProductTableInfo->ItemSpriteRef));
			}
		}
	}
}

void UUC_HeroLimiteProduct::Buy()
{
	if (ShopPanel.productList.Num() > 0)
	{
		FHERO_SHOP_PRODUCT shopproduct = ShopPanel.productList[0];

		UUP_HeroShop* heroShop = FindParentRBaseWidget<UUP_HeroShop>(this);
		if (IsValid(heroShop))
		{
			//heroShop->BuyLimite(shopproduct);
		}

		//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		//if ( IsValid(RGameInstance->EventManager))
		//{
		//	RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroLimiteProduct::OnBuy);
		//	RGameInstance->EventManager->OnHeroShopBuyRp.AddDynamic(this, &UUC_HeroLimiteProduct::OnBuy);
		//	UPacketFunctionLibrary::HERO_SHOP_PRODUCT_BUY_RQ(RGameInstance, shopproduct.productId, 1);
		//}
	}
}

void UUC_HeroLimiteProduct::OnBuy()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroLimiteProduct::OnBuy);
	}

	OnRefresh();

	if (ShopPanel.productList.Num() > 0)
	{
		if (ShopPanel.productList[0].price < 0)
		{
			UUC_HeroShopScrollItem* ShopScrollItem = FindParentRBaseWidget<UUC_HeroShopScrollItem>(this);
			if (IsValid(ShopScrollItem))
			{
				ShopScrollItem->InvalidateData();
			}
		}
	}
}