// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroItem.h"
#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Shop/UC_HeroSynergyProduct.h"
#include "UI/Shop/UP_HeroShop.h"
#include "Table/HeroShopTableInfo.h"
#include "Table/CharacterTableInfo.h"


void UUC_HeroItem::NativeConstruct()
{	
	Super::NativeConstruct();
	TypePanelWidgetList.Empty(2);
	TypePanelWidgetList.Emplace(HorizontalBox_2);
	TypePanelWidgetList.Emplace(CurrencyPanelWidget);
	switch (CurHeroItemType)
	{
	case EUIHeroItemTypeEnum::VE_Shop:
	{
		OnSelectedItem.RemoveDynamic(this, &UUC_HeroItem::OnClickSelected);
		OnSelectedItem.AddDynamic(this, &UUC_HeroItem::OnClickSelected);
	}
	break;
	case  EUIHeroItemTypeEnum::VE_Display:
	{
		for (UPanelWidget* widget : TypePanelWidgetList)
		{
			if (IsValid(widget))
			{
				widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		if (IsValid(DisablePanel))
		{
			DisablePanel->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(DownPanelWidget))
		{
			DownPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	break;
	}
}

void UUC_HeroItem::SetHeroSynergyProduct(UUC_HeroSynergyProduct* InProduct)
{
	SynergyProduct = InProduct;
}

void UUC_HeroItem::SetCharacterID(FName InCharacterID)
{
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InCharacterID);
	if (CharacterTableInfo)
	{
		if (IsValid(JobImage))
		{
			UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
			UUIFunctionLibrary::SetImageBrush(Sprite, JobImage);
		}

		if (IsValid(BackImage))
		{
			FString text = TEXT("Campaign_BG_Hero_") + CharacterTableInfo->DisplayName.ToString();
			BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
		}

		if (IsValid(FrontHeroText))
		{
			FrontHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
		}

		if (IsValid(BackHeroText))
		{
			BackHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}
	}
}

void UUC_HeroItem::SetProductID(FHERO_SHOP_PRODUCT& InProduct)
{
	CurProduct = InProduct;	

	Refresh();
}

void UUC_HeroItem::Refresh()
{
	if (IsValid(DisablePanel))
	{
		DisablePanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	FHeroShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetHeroShopProductTableInfo(FName(*CurProduct.productId));
	if (ProductTableInfo)
	{
		bBuyEnable = true;

		if (CurProduct.remainQuantity > 0)
		{
			if (IsValid(CurrencyPanelWidget))
			{
				if(CurrencyPanelWidget->GetVisibility() == ESlateVisibility::Collapsed)
					CurrencyPanelWidget->SetVisibility(ESlateVisibility::Visible);
			}

// 			if (IsValid(CurrencyValue))
// 			{
// 				CurrencyValue->SetCurrency(ProductTableInfo->currencyType, CurProduct.price);
// 			}
		}
		else
		{
			bBuyEnable = false;

			if (IsValid(CurrencyPanelWidget))
			{
				CurrencyPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}


		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ProductTableInfo->HeroKey);
		if (CharacterTableInfo)
		{
			if (IsValid(JobImage))
			{
				UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
				UUIFunctionLibrary::SetImageBrush(Sprite, JobImage);
			}

			if (IsValid(BackImage))
			{
				FString text = TEXT("Campaign_BG_Hero_") + CharacterTableInfo->DisplayName.ToString();
				BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
			}

			if (IsValid(FrontHeroText))
			{
				FrontHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
			}

			if (IsValid(BackHeroText))
			{
				BackHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
			}
		}

		if (IsValid(FrontText))
		{
			FrontText->SetText(FText::AsNumber(CurProduct.remainQuantity));
		}

		if (IsValid(BackText))
		{
			BackText->SetText(FText::AsNumber(CurProduct.maxQuantity));
		}

		if (IsValid(DisablePanel))
		{
			if (bBuyEnable)
			{
				DisablePanel->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				DisablePanel->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UUC_HeroItem::SendBuyPacket(FString InProductID, int32 InBuytype)
{
	if (!bBuyEnable)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroItem::OnBuy);
		RGameInstance->EventManager->OnHeroShopBuyRp.AddDynamic(this, &UUC_HeroItem::OnBuy);
// 		UPacketFunctionLibrary::HERO_SHOP_PRODUCT_BUY_RQ(InProductID, InBuytype);
	}
}

void UUC_HeroItem::OnBuy()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopBuyRp.RemoveDynamic(this, &UUC_HeroItem::OnBuy);
	}

	if (IsValid(SynergyProduct))
	{
		SynergyProduct->OnRefresh();
	}
}

void UUC_HeroItem::OnClickSelected(URScrollItem* InSelectScrollItem)
{
	if (bBuyEnable)
	{
		//if (UUtilFunctionLibrary::IsCurrencyHave(GetWorld(), CurProduct.currencyType, CurProduct.price))
		{
			UUP_HeroShop* HeroShop = FindParentRBaseWidget<UUP_HeroShop>(this);
			if (IsValid(HeroShop))
			{
				//HeroShop->BuySynergy(CurProduct);
			}
		}
	}
}