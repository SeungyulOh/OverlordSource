// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroSynergyProduct.h"
#include "GlobalIntegrated.h"


#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Shop/UC_HeroItem.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroSynergyProduct::NativeConstruct()
{	
	Super::NativeConstruct();
	HeroItemList.Empty(3);
	HeroItemList.Emplace(UC_HeroItem_01);
	HeroItemList.Emplace(UC_HeroItem_02);
	HeroItemList.Emplace(UC_HeroItem_03);
	if (Button_Refresh)
	{
		Button_Refresh->OnClicked.RemoveDynamic(this, &UUC_HeroSynergyProduct::ResetBtnClick);
		Button_Refresh->OnClicked.AddDynamic(this, &UUC_HeroSynergyProduct::ResetBtnClick);
	}
}

void UUC_HeroSynergyProduct::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ShopPanel.synergyId.Len() > 0)
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

void UUC_HeroSynergyProduct::SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel, bool bRefresh)
{
	Super::SetHeroShopPanel(InPanel, bRefresh);

	for (UUC_HeroItem* item : HeroItemList)
	{
		if (IsValid(item))
		{
			item->SetHeroSynergyProduct(this);
		}
	}
}

void UUC_HeroSynergyProduct::SetRefScrollView(URScrollView* InScrollView)
{
	Super::SetRefScrollView(InScrollView);

	for (UUC_HeroItem* item : HeroItemList)
	{
		if (IsValid(item))
		{
			item->SetParentScrollView(InScrollView);
		}
	}
}

void UUC_HeroSynergyProduct::RefreshUI()
{
	Super::RefreshUI();

	if (ShopPanel.synergyId.Len() > 0 && ShopPanel.productList.Num() > 0)
	{
		FHeroShopSynergyTableInfo* SynergyTableInfo = UTableManager::GetInstancePtr()->GetHeroShopSynergyTableInfo(FName(*ShopPanel.synergyId));
		if (SynergyTableInfo)
		{
			if (IsValid(TitleText1))
			{
				TitleText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, SynergyTableInfo->SynergyName));
			}

			FHeroShopProductTableInfo* ProductTableInfo = UTableManager::GetInstancePtr()->GetHeroShopProductTableInfo(SynergyTableInfo->groupMember1);
			if (ProductTableInfo)
			{
				int32 hour = ProductTableInfo->limitEndTime / 10000;
				int32 min = (ProductTableInfo->limitEndTime - hour * 10000) / 100;
				int32 sec = (ProductTableInfo->limitEndTime - hour * 10000 - min * 100);
				TotalTime = hour * 3600 + min * 60 + sec;
			}

			for (UUC_HeroItem* item : HeroItemList)
			{
				if (IsValid(item))
				{
					item->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			for (int32 i = 0; i < ShopPanel.productList.Num(); ++i)
			{
				if (IsValid(HeroItemList[i]))
				{
					HeroItemList[i]->SetVisibility(ESlateVisibility::Visible);
					HeroItemList[i]->SetProductID(ShopPanel.productList[i]);
				}
			}
		}
	}
}