// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroProduct.h"
#include "GlobalIntegrated.h"

#include "Global/ShopManager.h"



#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroProduct::NativeConstruct()
{	
	Super::NativeConstruct();

}

void UUC_HeroProduct::SendRefreshPacket()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopListRp.RemoveDynamic(this, &UUC_HeroProduct::OnRefresh);
		RGameInstance->EventManager->OnHeroShopListRp.AddDynamic(this, &UUC_HeroProduct::OnRefresh);
// 		UPacketFunctionLibrary::HERO_SHOP_PANEL_LIST_RQ(ShopPanel.panelId);

		bSendRefreshPacket = true;
	}
}

void UUC_HeroProduct::SendBuyPacket(FString InProductID, int32 InBuytype)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroProduct::OnBuy);
		RGameInstance->EventManager->OnHeroShopBuyRp.AddDynamic(this, &UUC_HeroProduct::OnBuy);
// 		UPacketFunctionLibrary::HERO_SHOP_PRODUCT_BUY_RQ(InProductID, InBuytype);
	}
}

void UUC_HeroProduct::SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel, bool bRefresh)
{
	ShopPanel = InPanel;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->ShopManager))
	{
		CurDateTime = RGameInstance->ShopManager->GetShopPanelTime(ShopPanel.panelId);
	}

	if(bRefresh)
		RefreshUI();
}

void UUC_HeroProduct::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(RestTimeText))
	{
		FDateTime nowTime = FDateTime::UtcNow();
		FTimespan timeSpan = nowTime - CurDateTime;
		float time = timeSpan.GetTotalSeconds();
		if (time >= (float)ShopPanel.timer)
		{
			RestTime = 0;
		}
		else
		{			
			RestTime = ShopPanel.timer - (int32)time;
		}

		FString timeStr;
		UDescriptionFunctionLibrary::GetTimeStrOneParam((float)RestTime, timeStr);

		RestTimeText->SetText(FText::FromString(timeStr));
	}
}

void UUC_HeroProduct::RefreshUI()
{
	bStart = true;

	if (IsValid(ResetPanel))
	{
		if (ShopPanel.resetRemainCount > 0)
		{
			ResetPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (IsValid(ResetValue))
			{
				ResetValue->SetCurrency(FName(*ShopPanel.resetCurrencyType), ShopPanel.resetPrice);
			}
		}
		else
		{
			ResetPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//FHeroShopPanelTableInfo* HeroShopPanelTableInfo = UTableManager::GetInstancePtr()->GetHeroShopPanelTableInfo(FName(*ShopPanel.panelId));
	//if (HeroShopPanelTableInfo)
	//{
	//	if (IsValid(ResetValue))
	//	{
	//		ResetValue->SetCurrency(HeroShopPanelTableInfo->panelUserResetCurrencyType, HeroShopPanelTableInfo->panelUserResetCurrencyValue);
	//	}
	//}
}

void UUC_HeroProduct::SetRefScrollView(URScrollView* InScrollView)
{
	RefScrollView = InScrollView;
}

void UUC_HeroProduct::ResetBtnClick()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager) && IsValid(RGameInstance->RWidgetManager))
	{
		if (IsValid(ResetValue))
		{
			if (ResetValue->IsHaveCurrency())
			{
				UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_MerchandiseReset")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_ResetMassage2")), ECommonPopupType::VE_OKCANCEL);

				if (IsValid(CommonPopup))
				{
					CommonPopup->OnClickOK.Clear();
					CommonPopup->OnClickOK.AddDynamic(this, &UUC_HeroProduct::OnResetOK);

					if (IsValid(CommonPopup->LocalizingTextBlock_OK))
					{
						CommonPopup->LocalizingTextBlock_OK->CustomControl = true;
						CommonPopup->LocalizingTextBlock_OK->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Reset")));
					}
				}
			}
			else
			{
				ResetValue->ShowMessageBox();
			}
		}
	}
}

void UUC_HeroProduct::OnResetOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopRefreshRp.Clear();
		RGameInstance->EventManager->OnHeroShopRefreshRp.AddDynamic(this, &UUC_HeroProduct::OnResetRefresh);
// 		UPacketFunctionLibrary::HERO_SHOP_PANEL_RESET_RQ(ShopPanel.panelId);
	}
}

void UUC_HeroProduct::OnRefresh()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->ShopManager) && IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopListRp.RemoveDynamic(this, &UUC_HeroProduct::OnRefresh);

		FHERO_SHOP_PANEL* panel = RGameInstance->ShopManager->GetHeroShopPanel(ShopPanel.panelId);
		if (panel)
		{
			SetHeroShopPanel(*panel);
		}
	}

	bSendRefreshPacket = false;
}

void UUC_HeroProduct::OnBuy()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroProduct::OnBuy);
	}
}

void UUC_HeroProduct::OnResetRefresh(FHERO_SHOP_PANEL InPanel)
{
	SetHeroShopPanel(InPanel);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopRefreshRp.Clear();
	}	
}