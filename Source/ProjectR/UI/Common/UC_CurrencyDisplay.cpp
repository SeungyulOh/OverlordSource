// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UC_CurrencyDisplay.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "GameInfo/GuildInfo.h"


void UUC_CurrencyDisplay::NativeConstruct()
{
	Super::NativeConstruct();
	
	//auto RGameInstance = RGAMEINSTANCE(this);
	//if (IsValid(RGameInstance) )
// 	{
// 		InvalidateQuantity();
// 	}
}


void UUC_CurrencyDisplay::NativeDestruct()
{
	/*auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) )
	{
		
	}*/

	Super::NativeDestruct();
}

void UUC_CurrencyDisplay::OnlyTextDisplay()
{
	if (IconType != nullptr)
	{
		IconType->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Text_Quantity)
	{
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UC_QuestList_ShortCut"));
		Text_Quantity->SetText(text);
	}
}

// void UUC_CurrencyDisplay::InvalidateQuantity()
// {
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		int32 Quantity = 0;
// 		switch (CurrentCurrencyType)
// 		{
// 		case EREWARD_TYPE::GOLD:
// 			Quantity = RGameInstance->RInventory->GetGold();
// 			break;
// 		case EREWARD_TYPE::GEM:
// 			Quantity = RGameInstance->RInventory->GetGem();
// 			break;
// 		case EREWARD_TYPE::REDSTONE:
// 			Quantity = RGameInstance->UserInfo->Avatar.redstone;
// 			break;
// 		case EREWARD_TYPE::PVPTOKEN:
// 			Quantity = RGameInstance->UserInfo->Avatar.pvptoken;
// 			break;
// 		case EREWARD_TYPE::BGT1:
// 			Quantity = RGameInstance->UserInfo->Avatar.bgt1;
// 			break;
// 		case EREWARD_TYPE::BGT2:
// 			Quantity = RGameInstance->UserInfo->Avatar.bgt2;
// 			break;
// 		case EREWARD_TYPE::BGT3:
// 			Quantity = RGameInstance->UserInfo->Avatar.bgt3;
// 			break;
// 		default:
// 		{
// 			if(bGuildGold)
// 				Quantity = (RGameInstance->GuildInfo != nullptr) ? RGameInstance->GuildInfo->MyGuildInfo.gold : 0;
// 		}
// 		}
// 		SetCurrency(CurrentCurrencyType, Quantity);
// 	}
// }

void UUC_CurrencyDisplay::SetCurrency(FName InCurrencyType, float InQuantity, bool bInCash, bool bInGuildGold)
{
	CurCurrencyType = InCurrencyType;
	CurValue = InQuantity;
	bGuildGold = bInGuildGold;

	bCash = bInCash;
	if ( IconType != nullptr )
	{
		int32 SwitchIndex = GetIconSwitchIndex(InCurrencyType);
		IconType->SetActiveWidgetIndex(SwitchIndex);
		IconType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	SetQuantity(InQuantity);
}

void UUC_CurrencyDisplay::SetQuantity(float InQuantity)
{
	if (Text_Quantity != nullptr)
	{		
		if (bCash)
		{
			Text_Quantity->SetText(FText::Format(FText::FromString("$ {0}"), FText::AsNumber(InQuantity)));
		}
		else
			Text_Quantity->SetText(FText::AsNumber(InQuantity));
	}
}

void UUC_CurrencyDisplay::SetItemImage(UObject* InBrushResource)
{
	UUIFunctionLibrary::SetImageBrush(InBrushResource, Image_Item);
}

int32 UUC_CurrencyDisplay::GetIconSwitchIndex(FName InCurrencyType)
{
	if (InCurrencyType == WALLET_GOLD)
		return 0;
	else if (InCurrencyType == WALLET_GEM)
		return 1;
	else if (InCurrencyType == WALLET_STAMINA)
		return 11;
	else if (InCurrencyType == WALLET_PVPTOKEN)
		return 3;
	else
		return 0;
// 	switch (InCurrencyType)
// 	{
// 		case EWALLET_TYPE::GOLD:
// 			return 0;
// 		case EWALLET_TYPE::GEM:
// 			return 1;
// // 		case EREWARD_TYPE::REDSTONE:
// // 			return 2;
// // 		case EREWARD_TYPE::PVPTOKEN:
// // 			return 3;
// // 		case EREWARD_TYPE::BGT1:
// // 			return 8;
// // 		case EREWARD_TYPE::BGT2:
// // 			return 9;
// // 		case EREWARD_TYPE::BGT3:
// // 			return 10;
// 		case EWALLET_TYPE::STAMINA:
// 			return 11;
// // 		case EREWARD_TYPE::ITEM:
// // 			return 12;
// // 		default:
// // 		{
// // 			if (bGuildGold)
// // 				return 5;
// // 		}
// 	}

	return 0;
}

bool UUC_CurrencyDisplay::IsHaveCurrency()
{
	return UUtilFunctionLibrary::IsCurrencyHave(GetWorld(), CurCurrencyType, CurValue);
}

void UUC_CurrencyDisplay::ShowMessageBox()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		FText caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Fail_Purchase"));
		FText content = UUIFunctionLibrary::GetCurrentyWarningMessage(CurCurrencyType);
		UUIFunctionLibrary::ShowCommonPopup(caption, content, ECommonPopupType::VE_OK);
	}
}
