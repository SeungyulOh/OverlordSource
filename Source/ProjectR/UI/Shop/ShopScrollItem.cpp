// Fill out your copyright notice in the Descripticlick page of Project Settings.

#include "ProjectR.h"
#include "ShopScrollItem.h"
#include "UI/RWidgetManager.h"
#include "UI/Shop/ShopScrollData.h"
#include "UI/Shop/ShopListViewControlEventDelegator.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Control/RScrollView.h"
#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

#define NAME_SHOPPRODUCT TEXT("SIT")
#define CASH_ITEMNUMBER 4

void UShopScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UShopScrollItem::OnClick_ShowPurchasePopup);
		Button_Click->OnClicked.AddDynamic(this, &UShopScrollItem::OnClick_ShowPurchasePopup);
	}
}

void UShopScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UShopScrollItem::SetSelected(bool bSelected)
{
	Super::SetSelected(bSelected);
	if(bSelected)
		OnClick_ShowPurchasePopup();
}

bool UShopScrollItem::IsValidData()
{	
	return true;
}

void UShopScrollItem::Clear()
{	
}

void UShopScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	ItemData = item;
	SetShopScrollItem();
}

void UShopScrollItem::SetShopScrollItem(URScrollItemData* InOtherData )
{
	UShopScrollData* ShopScrollData = Cast<UShopScrollData>( InOtherData );
	if( ShopScrollData == nullptr )
		ShopScrollData = Cast< UShopScrollData >(this->ItemData);
	if (ShopScrollData != nullptr)
	{
		ProductID = ShopScrollData->ProductID;
		if (Icon_ProductIcon)
		{		
			Icon_ProductIcon->SetItemIconStructure(ShopScrollData->ItemIconInfo);			
		}

		if (Text_ProductName)
			Text_ProductName->SetText(ShopScrollData->ProductName);

		if (Text_ProductDesc)
			Text_ProductDesc->SetText(ShopScrollData->ProductDesc);

		if (CurrencyDisplay)
		{
			CurrencyDisplay->SetCurrency(ShopScrollData->CurrencyType,
				ShopScrollData->Price,
				ShopScrollData->DisplayGroup == CASH_ITEMNUMBER);
		}
		
		RemainSellCount = ShopScrollData->MaxPurchaseCount - ShopScrollData->CurrentPurchaseCount;

		
		switch ((EShopBuyTypeEnum)ShopScrollData->buyType)
		{
		case EShopBuyTypeEnum::VE_UnLimited:
		{
			if (Switcher_SellingStatus)
				Switcher_SellingStatus->SetActiveWidgetIndex(0);
			if (Switcher_ButtonStatus)
				Switcher_ButtonStatus->SetActiveWidgetIndex(0);
		}
		break;

		case EShopBuyTypeEnum::VE_Limited_Quantity:
		{
			if (Switcher_SellingStatus)
				Switcher_SellingStatus->SetActiveWidgetIndex(1);
			
			if (Switcher_ButtonStatus)
				Switcher_ButtonStatus->SetActiveWidgetIndex(RemainSellCount > 0 ? 0 : 3);

			if (IsValid(Text_PossiblePurchaseCount))
				Text_PossiblePurchaseCount->SetText(FText::AsNumber(RemainSellCount));

			if (IsValid(Text_MaxPurchaseCount))
				Text_MaxPurchaseCount->SetText(FText::AsNumber(ShopScrollData->MaxPurchaseCount));
		}
		break;

		case EShopBuyTypeEnum::VE_Limited_Daily:
		case EShopBuyTypeEnum::VE_Limited_Weekly:
		case EShopBuyTypeEnum::VE_Limited_Monthly:
		{
			if (Switcher_SellingStatus)
				Switcher_SellingStatus->SetActiveWidgetIndex(1);

			if (IsValid(Text_PossiblePurchaseCount))
				Text_PossiblePurchaseCount->SetText(FText::AsNumber(ShopScrollData->MaxPurchaseCount - ShopScrollData->CurrentPurchaseCount));

			if (IsValid(Text_MaxPurchaseCount))
				Text_MaxPurchaseCount->SetText(FText::AsNumber(ShopScrollData->MaxPurchaseCount));
			if (RemainSellCount > 0)
			{
				if (Switcher_ButtonStatus)
					Switcher_ButtonStatus->SetActiveWidgetIndex(0);
			}
			else
			{
				auto RGameInstance = RGAMEINSTANCE(this);
				FSHOP_PRODUCT_STATUS UserProduct = RGameInstance->ShopManager->GetShopProductStatus(ShopScrollData->ProductID);

				if (Switcher_ButtonStatus)
					Switcher_ButtonStatus->SetActiveWidgetIndex(2);

				if (Text_BuyCount)
				{
					Text_BuyCount->SetText(FText::Format(
						UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI,TEXT("UI_Shop_LimitedCount_1")),
						0,//ShopScrollData->MaxPurchaseCount - ShopScrollData->CurrentPurchaseCount,
						ShopScrollData->MaxPurchaseCount));
				}


				int32 d = (int32)(UserProduct.coolTime / 86400);
				int32 h = (int32)(UserProduct.coolTime / 3600) % 24;
				int32 m = (int32)(UserProduct.coolTime / 60) % 60;

				FText day = UUIFunctionLibrary::Get_0n_Number(d);
				FText hour = UUIFunctionLibrary::Get_0n_Number(h);
				FText min = UUIFunctionLibrary::Get_0n_Number(m);
				FText time = FText::Format(FText::FromString("{0}:{1}:{2}"), day, hour, min);

				if (Text_ButtonTime)
				{
					Text_ButtonTime->SetText(time);
				}
				
				if (IsValid(Text_RemainTime))
				{
					FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_LimitedCount_2"));
					Text_RemainTime->SetText(FText::Format(text, time));
				}
			}
		}
		break;
		}
	}


}

void UShopScrollItem::OnClick_ShowPurchasePopup()
{	
	if (ParentScrollView != nullptr)
	{
		if (ParentScrollView->EventDelegator)
		{
			UShopScrollData* ShopScrollData = Cast< UShopScrollData >(this->ItemData);
			if (ShopScrollData == nullptr)
				return;

			if ((EShopBuyTypeEnum)ShopScrollData->buyType != EShopBuyTypeEnum::VE_UnLimited &&
				ShopScrollData->CurrentPurchaseCount >= ShopScrollData->MaxPurchaseCount)
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 				if (!IsValid(RGameInstance))
// 					return;

				FText Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_LimitedCount_4"));
				switch ((EShopBuyTypeEnum)ShopScrollData->buyType)
				{
				case EShopBuyTypeEnum::VE_UnLimited:
					return;
				case EShopBuyTypeEnum::VE_Limited_Quantity:
				{
					Desc = FText::Format(
						FText::FromString(TEXT("{0}\n{1}")), Desc, 
						UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_LimitedCount_6")));
				}
				break;
				case EShopBuyTypeEnum::VE_Limited_Daily:
				case EShopBuyTypeEnum::VE_Limited_Weekly:
				case EShopBuyTypeEnum::VE_Limited_Monthly:
				{
					FSHOP_PRODUCT_STATUS UserProduct = RGameInstance->ShopManager->GetShopProductStatus(ShopScrollData->ProductID);
					FDateTime now = FDateTime::Now();
					int32 d = (int32)(UserProduct.coolTime / 86400);
					int32 h = (int32)(UserProduct.coolTime / 3600) % 24;
					int32 m = (int32)(UserProduct.coolTime / 60) % 60;
					int32 s = (int32)(UserProduct.coolTime % 60);

					FTimespan RemainTime(d,h,m,s);

					now += RemainTime;

					int32 Year, Month, Day;
					now.GetDate(Year, Month, Day);

					FText TimeText = FText::Format(
						UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_LimitedCount_5")), 
						FText::AsDate(now, EDateTimeStyle::Type::Medium),
						FText::FromString(TEXT("00:00")));
					
					Desc = FText::Format(FText::FromString(TEXT("{0}\n{1}")), Desc, TimeText);
				}
				break;
				}
				
				UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), Desc);
				return;
			}

			UShopListViewControlEventDelegator* ShopDelegator = Cast< UShopListViewControlEventDelegator >(ParentScrollView->EventDelegator);
			if (ShopDelegator)
			{				
				ShopDelegator->PurchasePopupRequest.Broadcast( ShopScrollData );
			}
		}
	}
	else
	{		
		RequestPurchaseDelegate.Broadcast(ProductID);
	}

}

FString UShopScrollItem::GetGoogleProductID()
{
	FString Result = ProductID.ToString();
	int32 iIdx = Result.Find(NAME_SHOPPRODUCT);
	Result = Result.RightChop(iIdx);
	return Result;
}

float UShopScrollItem::ComputeCashPrice()
{/*
	FString Locale;
#if PLATFORM_IOS
	Locale = FIOSPlatformMisc::GetDefaultLocale();
#elif PLATFORM_ANDROID
	Locale = FAndroidMisc::GetDefaultLocale();
#else
	Locale = FGenericPlatformMisc::GetDefaultLocale();
#endif

	if (Locale == TEXT("ko"))
	{
		Locale = "KRW";
	}
	else if (Locale == TEXT("en"))
	{
	}
	else if (Locale == TEXT("zh_Hans"))
	{
	}
	else if (Locale == TEXT("zh_Hant"))
	{
	}

	FString Key = ProductID.ToString() + Locale;

	FInAppPurchaseTableInfo* InAppPurchaseTableInfo = UTableManager::GetInstancePtr()->GetInAppPurchaseRow(FName(*Key));
	if (InAppPurchaseTableInfo == nullptr)
		return 0.0f;
	
	

	return InAppPurchaseTableInfo->Price;*/
	return 0.0f;
}
