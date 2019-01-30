// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_BagExtend.h"
#include "AssertionMacros.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

#include "Global/TableManager.h"
#include "Global/RGameInstance.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "GameInfo/RInventory.h"

#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UP_GenericScrollview.h"

void UUC_Popup_BagExtend::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.Clear();
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_BagExtend::OnButtonCancelClicked);
	}
	if (Button_Close)
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_BagExtend::OnButtonCancelClicked);
	}
	if (Button_OK)
	{
		Button_OK->OnClicked.Clear();
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_BagExtend::OnButtonOKClicked);
	}

	InvalidateData();
		
}

void UUC_Popup_BagExtend::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);
	if (Button_Close)
		Button_Close->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_Popup_BagExtend::InvalidateData()
{
	// these checks are getting dangerous..
	if (!RGAMEINSTANCE(this)->RInventory->CanBagBeExtendedFurther())
		return;

	int32 BluestoneCost = RGAMEINSTANCE(this)->RInventory->NextBagExtensionBluestoneCost();

	FConstantVariablesTableInfo* ConstantInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("ITEM_INVENTORY_AddRate")));
	if (!ensure(ConstantInfo != nullptr))
		return;

	int32 InventoryAddRate = 0;
	ConstantInfo->GetValue(InventoryAddRate);

	int32 CurrentBagCapacity = RGAMEINSTANCE(this)->RInventory->GetBagCapacity();
	int32 NextBagCapacity = CurrentBagCapacity + InventoryAddRate;

	Text_BagCapacityBefore->SetText(FText::AsNumber(CurrentBagCapacity));
	Text_BagCapacityAfter->SetText(FText::AsNumber(NextBagCapacity));

	Text_BluestoneCost->SetText(FText::AsNumber(BluestoneCost));

	bool CanExtend = HasEnoughBluestones();
	Button_OK->SetIsEnabled(CanExtend);

	// Text_Description
// 	Args.Add(TEXT("bluestone"), FText::AsNumber(BluestoneCost));
// 	Args.Add(TEXT("slot"), FText::AsNumber(InventoryAddRate));
// 	FText FormattedText = FText::Format(DescriptionTemplateText, Args);
	FText DescriptionTemplateText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_InventoryExpansion_PopUp_Message01"));
	Text_Description->SetText(FText::Format(DescriptionTemplateText, BluestoneCost, InventoryAddRate));
}

void UUC_Popup_BagExtend::OnButtonCancelClicked()
{
	ParentPopup->Close();
}

void UUC_Popup_BagExtend::OnButtonOKClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpgrade.AddUObject(this, &UUC_Popup_BagExtend::OnInventoryUpgradeRp);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (!IsValid(GenericScrollview))
		return;

	bool bHeroInventory = GenericScrollview->Variables.State <= EScrollviewType::VE_ITEM_WEAPON;

	int32 InventorySlotTokenCount = bHeroInventory ? RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_HERO_INVENTORY_SLOT_TOKEN) : RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_ITEM_INVENTORY_SLOT_TOKEN);

	UPacketFunctionLibrary::INVENTORY_UPGRADE_RQ(InventorySlotTokenCount + 1, bHeroInventory ? (int32)(EINVENTORY_UPGRADE_TYPE::HERO) : (int32)(EINVENTORY_UPGRADE_TYPE::ITEM));
}

void UUC_Popup_BagExtend::OnInventoryUpgradeRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpgrade.RemoveAll(this);

	if (bSuccess)
	{
		// display UP_Splash_BagExtendResult

		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_BagExtendResult);
		check(Splash);

		if (!RGAMEINSTANCE(this)->RInventory->CanBagBeExtendedFurther())
		{
			ParentPopup->Close();
			return;
		}

		InvalidateData();
		UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (!IsValid(GenericScrollview))
			return;

		GenericScrollview->Renderer.Render();
	}
	else
	{
		ensure(false);
	}
}

bool UUC_Popup_BagExtend::HasEnoughBluestones()
{
	int32 OwnedBluestoneCount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GEM);
	int32 NextCost = RGAMEINSTANCE(this)->RInventory->NextBagExtensionBluestoneCost();
	return OwnedBluestoneCount >= NextCost;
}