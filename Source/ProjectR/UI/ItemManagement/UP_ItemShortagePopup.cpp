// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemShortagePopup.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/Common/UC_CurrencyDisplay.h"

void UUP_ItemShortagePopup::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	PlayAni(TEXT("Appear"));

	if (Button_Store)
	{
		Button_Store->OnClicked.RemoveDynamic(this, &UUP_ItemShortagePopup::OnClickShop);
		Button_Store->OnClicked.AddDynamic(this, &UUP_ItemShortagePopup::OnClickShop);
	}

	if (Button_OK)
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_ItemShortagePopup::OnClickOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_ItemShortagePopup::OnClickOK);
	}

	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUP_ItemShortagePopup::OnClickOK);
		Button_Close->OnClicked.AddDynamic(this, &UUP_ItemShortagePopup::OnClickOK);
	}
}

void UUP_ItemShortagePopup::NativeDestruct()
{
	IsClickCustom = false;

	if (IsHideCurrency && IsValid(LocalCurrency))
	{
		LocalCurrency->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		IsHideCurrency = false;
	}

	Super::NativeDestruct();
}

void UUP_ItemShortagePopup::OnClickOK()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	IsClickCustom = false;
	/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemEnchant);
	}*/

	if (IsHideCurrency && IsValid(LocalCurrency))
	{
		LocalCurrency->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		IsHideCurrency = false;
	}
}

void UUP_ItemShortagePopup::OnClickShop()
{
	if (IsClickCustom)
	{
		OnClickOK();
		return;
	}

	OnClickOK();

	int32 iDirectIndex = 0;
	switch (RewardType) //todo number
	{
		case EREWARD_TYPE::ITEM:	iDirectIndex = 2;	break; 
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Shop);
	}
}

void UUP_ItemShortagePopup::SetNecessaryGold(int32 InGold)
{
	//NecessaryGold = Gold;

//	if (IsValid(Text_NecessaryGold))
	if (IsValid(LocalCurrency))
	{
		LocalCurrency->SetCurrency(WALLET_GOLD, InGold);
// 		FText txtGold = FText::AsNumber(InGold);
// 		Text_NecessaryGold->SetText(txtGold);
	}
}

void UUP_ItemShortagePopup::SetNecessaryGem(int32 InGold)
{
	if (IsValid(LocalCurrency))
	{
		LocalCurrency->SetCurrency(WALLET_GEM, InGold);
		// 		FText txtGold = FText::AsNumber(InGold);
		// 		Text_NecessaryGold->SetText(txtGold);
	}
}

void UUP_ItemShortagePopup::SetCurrency(EREWARD_TYPE InType, int32 InValue)
{
// 	RewardType = InType;
// 	if (IsValid(LocalCurrency))
// 	{
// 		LocalCurrency->SetCurrency(InType, InValue);
// 	}
}

void UUP_ItemShortagePopup::SetChangeTextBlock(FName InCaption, FName InDesc)
{
	//default
	//caption - UI_Enchant_000001
	//desc - Error_Message_000000
	if (LocalText_Caption)
	{
		LocalText_Caption->LocalizingKey = InCaption;
	}

	if (LocalText_Desc)
	{
		LocalText_Desc->LocalizingKey = InDesc;
	}
}

void UUP_ItemShortagePopup::SetItem(UObject* InBrushResource, int32 InValue)
{
// 	if (IsValid(LocalCurrency))
// 	{
// 		LocalCurrency->SetCurrency(EWALLET_TYPE::ITEM, InValue);
// 		LocalCurrency->SetItemImage(InBrushResource);
// 	}
}

void UUP_ItemShortagePopup::SetPopup(FText InCaption, FText InDesc, EREWARD_TYPE InType, int32 InValue)
{
	if (IsValid(LocalText_Caption))
	{
		LocalText_Caption->SetText(InCaption);
	}

	if (IsValid(LocalText_Desc))
	{
		LocalText_Desc->SetText(InDesc);
	}

	SetCurrency(InType, InValue);
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUP_ItemShortagePopup::SetHideCurrency()
{
	if (IsValid(LocalCurrency))
	{
		LocalCurrency->SetVisibility(ESlateVisibility::Hidden);
	}
	IsHideCurrency = true;
}