// Fill out your copyright notice in the Descripticlick page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroShopScrollItem.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Control/RScrollView.h"
#include "UI/Shop/UC_HeroBoxsProduct.h"
#include "UI/Shop/UC_HeroLimiteProduct.h"
#include "UI/Shop/UC_HeroSynergyProduct.h"

#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"
#include "UtilFunctionIntegrated.h"

#include "Network/PacketDataStructures.h"

void UUC_HeroShopScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	IsSizeUpdate = true;

	PrevWidgetSize.Left = 0.0f;
	PrevWidgetSize.Right = 0.0f;
	PrevWidgetSize.Top = 0.0f;
	PrevWidgetSize.Bottom = 0.0f;

	if (IsValid(HeroLimiteProduct))
	{
		HeroLimiteProduct->SetRefScrollView(ParentScrollView);
	}

	if (IsValid(HeroSynergyProduct))
	{
		HeroSynergyProduct->SetRefScrollView(ParentScrollView);
	}

	if (IsValid(HeroBoxsProduct))
	{
		HeroBoxsProduct->SetRefScrollView(ParentScrollView);
	}

}

void UUC_HeroShopScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsSizeUpdate)
	{
		UpdateWidgetSize(MyGeometry, true);

		if (PrevWidgetSize.Left != WidgetSize.Left || PrevWidgetSize.Right != WidgetSize.Right || PrevWidgetSize.Top != WidgetSize.Top || PrevWidgetSize.Bottom != WidgetSize.Bottom)
		{
			ChangeSize();
			PrevWidgetSize = WidgetSize;
		}
	}
}

void UUC_HeroShopScrollItem::SetParentScrollView(URScrollView* InScrollView)
{
	Super::SetParentScrollView(InScrollView);
}

void UUC_HeroShopScrollItem::ChangeSize()
{
	FVector2D size;
	size.X = WidgetSize.Right - WidgetSize.Left;
	size.Y = WidgetSize.Bottom - WidgetSize.Top;

	SetSizeChange(size, true);

	if (IsValid(ParentScrollView))
	{
		ParentScrollView->SetSizeChangeUpdate();
	}
}

void UUC_HeroShopScrollItem::InvalidateData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->ShopManager))
	{
		TArray<FHERO_SHOP_PANEL>& panelList = RGameInstance->ShopManager->GetHeroShopPanelList();

		TArray<FHERO_SHOP_PANEL> boxList;

		for (FHERO_SHOP_PANEL& panel : panelList)
		{
			if (panel.panelId == TEXT("SPP000001"))
			{
				bool bExe = false;

				if (panel.productList.Num() > 0)
				{
					bExe = true;
					//if (panel.productList[0].price > 0)
					//{
					//	bExe = true;
					//}
				}

				if (IsValid(HeroLimiteProduct))
				{
					if (bExe)
					{
						HeroLimiteProduct->SetVisibility(ESlateVisibility::Visible);
						HeroLimiteProduct->SetHeroShopPanel(panel);
					}
					else
					{
						HeroLimiteProduct->SetVisibility(ESlateVisibility::Collapsed);
						HeroLimiteProduct->SetDisStart();
					}
				}
			}
			else if (panel.panelId == TEXT("SPP000003"))
			{
				if (panel.synergyId.Len() > 0 && panel.productList.Num() > 0)
				{
					if (IsValid(HeroSynergyProduct))
					{
						HeroSynergyProduct->SetVisibility(ESlateVisibility::Visible);
						HeroSynergyProduct->SetHeroShopPanel(panel);
					}
				}
				else
				{
					if (IsValid(HeroSynergyProduct))
					{
						HeroSynergyProduct->SetVisibility(ESlateVisibility::Collapsed);
						HeroSynergyProduct->SetDisStart();
					}						
				}
			}
			else if (panel.panelId == TEXT("SPP000004"))
			{
				if (panel.productList.Num() > 0)
				{
					boxList.Emplace(panel);
				}				
			}
			else if (panel.panelId == TEXT("SPP000005"))
			{
				if (panel.productList.Num() > 0)
				{
					boxList.Emplace(panel);
				}
			}
		}

		if (boxList.Num() > 0)
		{
			if (IsValid(HeroBoxsProduct))
			{
				HeroBoxsProduct->SetVisibility(ESlateVisibility::Visible);
				HeroBoxsProduct->SetHeroShopPanelList(boxList);
			}
		}
		else
		{
			if (IsValid(HeroBoxsProduct))
			{
				HeroBoxsProduct->SetVisibility(ESlateVisibility::Collapsed);
				HeroBoxsProduct->SetDisStart();
			}
		}
	}
}