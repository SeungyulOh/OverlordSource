// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroPackage.h"
#include "GlobalIntegrated.h"

#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Shop/UP_HeroShop.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroPackage::NativeConstruct()
{	
	Super::NativeConstruct();
	PackageTabSwitcher.Empty(3);
	PackageTabSwitcher.Emplace(WidgetSwitcher_0);
	PackageTabSwitcher.Emplace(WidgetSwitcher_1);
	PackageTabSwitcher.Emplace(WidgetSwitcher_2);
	RadioButtonList.Empty(3);
	RadioButtonList.Emplace(CheckBox_Radio1);
	RadioButtonList.Emplace(CheckBox_Radio2);
	RadioButtonList.Emplace(CheckBox_Radio3);
	SelectMenuNumber = 0;

	for (auto Elem : RadioButtonList)
	{
		if (Elem == nullptr)
			continue;

		Elem->OnCheckRadioCheck.RemoveDynamic(this, &UUC_HeroPackage::ChangePackage);
		Elem->OnCheckRadioCheck.AddDynamic(this, &UUC_HeroPackage::ChangePackage);
	}

	if (IsValid(ClickBtn))
	{
		ClickBtn->OnClicked.RemoveDynamic(this, &UUC_HeroPackage::Buy);
		ClickBtn->OnClicked.AddDynamic(this, &UUC_HeroPackage::Buy);
	}
	if (IsValid(Button_Refresh))
	{
		Button_Refresh->OnClicked.RemoveDynamic(this, &UUC_HeroPackage::ResetBtnClick);
		Button_Refresh->OnClicked.AddDynamic(this, &UUC_HeroPackage::ResetBtnClick);
	}
}

void UUC_HeroPackage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UUC_HeroPackage::RefreshUI()
{
	Super::RefreshUI();

	CurSelectIdx = -1;

	ChangePackage(true);
}

void UUC_HeroPackage::ChangePackage(bool isCheck)
{
	if (RadioButtonList[SelectMenuNumber]->IsChecked())
		RadioButtonList[SelectMenuNumber]->SetIsChecked(false);

	int iNumber = -1;

	for (int i = 0; i < 3; i++)
	{
		if (RadioButtonList[i]->IsChecked())
			iNumber = i;
	}

	if (iNumber == -1)
		iNumber = SelectMenuNumber;
	else
		SelectMenuNumber = iNumber;

	RadioButtonList[SelectMenuNumber]->SetIsChecked(true);

	if (CurSelectIdx == SelectMenuNumber)
		return;

	CurSelectIdx = SelectMenuNumber;
	
	for (int i = 0; i < PackageTabSwitcher.Num(); ++i)
	{
		if (PackageTabSwitcher.IsValidIndex(i))
		{
			PackageTabSwitcher[i]->SetActiveWidgetIndex((i == SelectMenuNumber) ? 0 : 1);
		}
	}

	RefreshPackage();
}

void UUC_HeroPackage::RefreshPackage()
{
	if (ShopPanel.productList.IsValidIndex(CurSelectIdx))
	{
		FHERO_SHOP_PRODUCT& product = ShopPanel.productList[CurSelectIdx];
		FHeroShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetHeroShopProductTableInfo(FName(*product.productId));
		if (ProductTableInfo)
		{
			if (IsValid(CurrencyDisplay))
			{
				CurrencyDisplay->SetCurrency(FName(*product.currencyType), product.price);
			}

			if (IsValid(ClickBtn))
			{
				if (product.remainQuantity <= 0)
				{
					ClickBtn->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					ClickBtn->SetVisibility(ESlateVisibility::Visible);
					//ClickBtn->SetIsEnabled(UUtilFunctionLibrary::IsCurrencyHave(GetWorld(), product.currencyType, product.price));
				}				
			}

			if (IsValid(BuyComplatePanel))
			{
				if (product.remainQuantity <= 0)
				{
					BuyComplatePanel->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					BuyComplatePanel->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			if (IsValid(BackImage))
			{
				BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ProductTableInfo->ItemSpriteRef));
			}

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
		}
	}
}

void UUC_HeroPackage::Buy()
{
	if (ShopPanel.productList.IsValidIndex(CurSelectIdx))
	{
		UUP_HeroShop* heroShop = FindParentRBaseWidget<UUP_HeroShop>(this);
		if (IsValid(heroShop))
		{
			//heroShop->BuyPackage(ShopPanel.productList[CurSelectIdx]);
		}
	}
}

void UUC_HeroPackage::OnResetRefresh(FHERO_SHOP_PANEL InPanel)
{
	Super::OnResetRefresh(InPanel);
	RefreshPackage();
}