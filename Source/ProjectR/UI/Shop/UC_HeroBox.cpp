// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroBox.h"
#include "GlobalIntegrated.h"


#include "Global/ShopManager.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroBox::NativeConstruct()
{	
	Super::NativeConstruct();

	//OnSelectedItem.RemoveDynamic(this, &UUC_HeroBox::OnClickSelected);
	//OnSelectedItem.AddDynamic(this, &UUC_HeroBox::OnClickSelected);

	if (IsValid(Button_Value))
	{
		Button_Value->OnClicked.RemoveDynamic(this, &UUC_HeroBox::OnClick_Buy);
		Button_Value->OnClicked.AddDynamic(this, &UUC_HeroBox::OnClick_Buy);
	}
	if (IsValid(ClickBtn))
	{
		ClickBtn->OnClicked.RemoveDynamic(this, &UUC_HeroBox::OnClick_Buy);
		ClickBtn->OnClicked.AddDynamic(this, &UUC_HeroBox::OnClick_Buy);
	}



	URScrollView* ScrollView = FindParentRBaseWidget<URScrollView>(this);
	if (IsValid(ScrollView))
	{
		ParentScrollView = ScrollView;
	}

	

	bStart = false;
}

void UUC_HeroBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bStart)
	{
		if (IsValid(TimeText) && IsValid(RestTimeProgressBar))
		{
			FDateTime nowTime = FDateTime::UtcNow();
			FTimespan timeSpan = nowTime - CurDateTime;
			float time = timeSpan.GetTotalSeconds();
			if (time >= (float)CurPanel.timer)
			{
				RestTime = 0;
			}
			else
			{
				RestTime = CurPanel.timer - (int32)time;
			}

			FString timeStr;
			UDescriptionFunctionLibrary::GetTimeStrOneParam((float)RestTime, timeStr);

			TimeText->SetText(FText::FromString(timeStr));

			float fValue = 0;
			if (TotalTime > 0)
			{
				fValue = (float)RestTime / (float)TotalTime;
			}

			RestTimeProgressBar->SetPercent(fValue);
		}

		if (CurPanel.productList.Num() > 0)
		{
			FHERO_SHOP_PRODUCT& product = CurPanel.productList[0];

			if (product.price > 0)
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
	}
}

void UUC_HeroBox::SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel)
{
	CurPanel = InPanel;

	bStart = true;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->ShopManager))
	{
		CurDateTime = RGameInstance->ShopManager->GetShopPanelTime(CurPanel.panelId);
	}

	if (IsValid(NormalSwitcher))
	{
		if (CurPanel.panelId == TEXT("SPP000004"))
		{
			NormalSwitcher->SetActiveWidgetIndex(0);
		}
		else
		{
			NormalSwitcher->SetActiveWidgetIndex(1);
		}
	}

	if (CurPanel.productList.Num() > 0)
	{
		FHeroShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetHeroShopProductTableInfo(FName(*CurPanel.productList[0].productId));
		if (ProductTableInfo)
		{
			if (IsValid(BackImage))
			{
				BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ProductTableInfo->ItemSpriteRef));
			}

			int32 hour = ProductTableInfo->limitEndTime / 10000;
			int32 min = (ProductTableInfo->limitEndTime - hour * 10000) / 100;
			int32 sec = (ProductTableInfo->limitEndTime - hour * 10000 - min * 100);
			TotalTime = hour * 3600 + min * 60 + sec;

			if (IsValid(TitleText))
			{
				TitleText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, ProductTableInfo->productName));
			}
		}

		

		if (IsValid(UC_CurrencyDisplay))
		{
			UC_CurrencyDisplay->SetCurrency(FName(*CurPanel.productList[0].currencyType), CurPanel.productList[0].price);
		}

		if (IsValid(FreeSwitcher) && IsValid(PurchaseSwitcher))
		{
			if (CurPanel.productList[0].price > 0)
			{
				FreeSwitcher->SetActiveWidgetIndex(0);		
				PurchaseSwitcher->SetActiveWidgetIndex(1);
			}
			else
			{
				FreeSwitcher->SetActiveWidgetIndex(1);
				PurchaseSwitcher->SetActiveWidgetIndex(0);
			}
		}
	}
}

void UUC_HeroBox::SendBuyPacket(FString InProductID, int32 InBuytype)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroBox::OnBuy);
		RGameInstance->EventManager->OnHeroShopBuyRp.AddDynamic(this, &UUC_HeroBox::OnBuy);
// 		UPacketFunctionLibrary::HERO_SHOP_PRODUCT_BUY_RQ(InProductID, InBuytype);
	}
}

void UUC_HeroBox::SendRefreshPacket()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopListRp.RemoveDynamic(this, &UUC_HeroBox::OnRefresh);
		RGameInstance->EventManager->OnHeroShopListRp.AddDynamic(this, &UUC_HeroBox::OnRefresh);
// 		UPacketFunctionLibrary::HERO_SHOP_PANEL_LIST_RQ(CurPanel.panelId);

		bSendRefreshPacket = true;
	}
}

void UUC_HeroBox::OnBuy()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager) && IsValid(RGameInstance->ShopManager) && IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroBox::OnBuy);

		FHERO_SHOP_PANEL* panel = RGameInstance->ShopManager->GetHeroShopPanel(CurPanel.panelId);
		if (panel)
		{
			SetHeroShopPanel(*panel);
		}

		UUIFunctionLibrary::AddUIEventInt(EUIEventEnum::VE_Shop, 5);
	}
}

void UUC_HeroBox::OnClick_Buy()
{
	OnClickSelected(nullptr);
}

void UUC_HeroBox::OnClickSelected(URScrollItem* InSelectScrollItem)
{
	if (CurPanel.productList.Num() > 0)
	{
		FHERO_SHOP_PRODUCT& product = CurPanel.productList[0];

		if (product.price > 0)
		{
			if (UUtilFunctionLibrary::IsCurrencyHave(GetWorld(), FName(*product.currencyType), product.price))
			{
				SendBuyPacket(product.productId, 1);
			}
		}
		else
		{
			SendBuyPacket(product.productId, 1);
		}		
	}
}

void UUC_HeroBox::OnRefresh()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager) && IsValid(RGameInstance->ShopManager))
	{
		RGameInstance->EventManager->OnHeroShopListRp.RemoveDynamic(this, &UUC_HeroBox::OnRefresh);

		FHERO_SHOP_PANEL* panel = RGameInstance->ShopManager->GetHeroShopPanel(CurPanel.panelId);
		if (panel)
		{
			SetHeroShopPanel(*panel);
		}
	}

	bSendRefreshPacket = false;
}