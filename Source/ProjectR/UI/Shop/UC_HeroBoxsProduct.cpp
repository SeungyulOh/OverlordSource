// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroBoxsProduct.h"
#include "GlobalIntegrated.h"

#include "Global/ShopManager.h"

#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/Shop/UC_HeroBox.h"
#include "Table/HeroShopTableInfo.h"


void UUC_HeroBoxsProduct::NativeConstruct()
{	
	Super::NativeConstruct();
	BoxList.Empty(2);
	BoxList.Emplace(UC_HeroBox_01);
	BoxList.Emplace(UC_HeroBox_02);
}

void UUC_HeroBoxsProduct::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_HeroBoxsProduct::SetHeroShopPanelList(TArray<FHERO_SHOP_PANEL>& InPanelList)
{
	ShopPanelList = InPanelList;

	if (ShopPanelList.Num() > 0)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->ShopManager))
		{
			CurDateTime = RGameInstance->ShopManager->GetShopPanelTime(ShopPanelList[0].panelId);
		}
	}

	RefreshUI();
}

void UUC_HeroBoxsProduct::RefreshUI()
{
	Super::RefreshUI();

	for (UUC_HeroBox* box : BoxList)
	{
		if (IsValid(box))
		{
			box->SetVisibility(ESlateVisibility::Collapsed);
		}
	}	

	for (int32 i = 0; i < ShopPanelList.Num(); ++i)
	{
		if (BoxList.IsValidIndex(i) && IsValid(BoxList[i]))
		{
			BoxList[i]->SetVisibility(ESlateVisibility::Visible);
			BoxList[i]->SetHeroShopPanel(ShopPanelList[i]);
		}
	}
}

void UUC_HeroBoxsProduct::OnRefresh()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->ShopManager) && IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnHeroShopListRp.RemoveDynamic(this, &UUC_HeroProduct::OnRefresh);

		for (int32 i = 0; i < ShopPanelList.Num(); ++i)
		{
			FHERO_SHOP_PANEL* panel = RGameInstance->ShopManager->GetHeroShopPanel(ShopPanelList[i].panelId);
			if (panel)
			{
				ShopPanelList[i] = *panel;
			}
		}

		if (ShopPanelList.Num() > 0)
		{
			CurDateTime = RGameInstance->ShopManager->GetShopPanelTime(ShopPanelList[0].panelId);
		}

		RefreshUI();
	}

	bSendRefreshPacket = false;
}

void UUC_HeroBoxsProduct::SetRefScrollView(URScrollView* InScrollView)
{
	Super::SetRefScrollView(InScrollView);

	for (UUC_HeroBox* box : BoxList)
	{
		if (IsValid(box))
		{
			box->SetParentScrollView(InScrollView);
		}
	}
}