// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PurchasePopup.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Shop/ShopScrollData.h"


void UPurchasePopup::InvalidateData()
{
	if (ProductData == nullptr)
		return;
	
	if (ProductData != nullptr)
	{
		if (Icon_ProductIcon)
		{		
			Icon_ProductIcon->SetItemIconStructure(ProductData->ItemIconInfo);			
		}

		if (Text_ProductName)
			Text_ProductName->SetText(ProductData->ProductName);

		if (Text_ProductDesc)
			Text_ProductDesc->SetText(ProductData->ProductDesc);

// 		if (CurrencyDisplay)
// 			CurrencyDisplay->SetCurrency(ProductData->CurrencyType, ProductData->Price);
	}

}

