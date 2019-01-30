// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_WalletItemInfo.h"

#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"



#include "SharedConstants/GlobalConstants.h"
#include "UI/ItemManagement/Item/HeroItemOptions_ScrollItemData.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UC_GenericPopup.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

#include "UI/Component/LocalizingTextBlock.h"
#include "UtilFunctionIntegrated.h"

#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "UI/ItemManagement/UC_HeroItemInfo.h"


void UUC_WalletItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ParentPopup))
	{
		//ParentPopup->Text_PopupTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Info_000000")));
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_WalletItemInfo::OnButtonCloseClicked);
	}

	//Button_Rune_Combine->OnClicked.AddDynamic(this, &UUC_WalletItemInfo::OnButtonRuneCombineClicked);

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_WalletItemInfo::OnButtonCloseClicked);
	}
}

void UUC_WalletItemInfo::NativeDestruct()
{
	if (IsValid(ParentPopup))
	{
		if (IsValid(ParentPopup->Button_Close))
			ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}

	if (IsValid(Button_Rune_Combine))
		Button_Rune_Combine->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_WalletItemInfo::InvalidateData()
{
	Super::InvalidateData();

	if (!ensure(!WalletKey.IsNone()))
		return;
	
	FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
	if (!ensure(WalletInfo != nullptr))
		return;

	check(IsValid(RGAMEINSTANCE(this)->RInventory));
	int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WalletKey);

	ItemIcon->RefreshWalletIcon(WalletKey, WalletInfo, OwnedAmount);

	switch (WalletInfo->PropertyType)
	{
	case EWalletPropertyType::HeroPiece:
		Switcher_Item->SetActiveWidgetIndex(1);
		InvalidateHeroPiece(WalletInfo);
		break;
	case EWalletPropertyType::Rune:
		Switcher_Item->SetActiveWidgetIndex(2);
		InvalidateRune(WalletInfo);
		break;
	default:
		Switcher_Item->SetActiveWidgetIndex(0);
		InvalidateMaterial(WalletInfo);
		break;
	}
}

void UUC_WalletItemInfo::OnButtonCloseClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_WalletItemInfo::OnButtonRuneCombineClicked()
{
	// Open BP UP_Page_RuneCombine

	// pass ItemUD as UIEvent thing
	UUIFunctionLibrary::AddUIEventName(EUIEventEnum::VE_RuneInventoryPage, WalletKey);
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_RuneCombine);

// 	UUserWidget*	Widget = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Page_RuneCombine);
// 
// 	if (Widget)
// 	{
// 		auto	RuneCombine = Cast<UUP_ItemInventoryPage>(Widget);
// 
// 		if (RuneCombine)
// 		{
// 			auto	PageContent = Cast<UUP_ItemInventoryPage_ContentBase>(RuneCombine->GetContentWidget());
// 			if (PageContent)
// 				PageContent->SetWalletKey(WalletKey);
// 		}
// 	}
}

void UUC_WalletItemInfo::InvalidateHeroPiece(const FWalletTableInfo* WalletInfo)
{
	FText NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletInfo->CommonStringKey);
	Text_HeroPiece_Name->SetText(NameText);

	// description <- WalletInfo->Tooltip
	FText DescriptionText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, WalletInfo->Tooltip);
	Text_HeroPiece_Description->SetText(DescriptionText);

	// usage <- WalletInfo->Tooltip2
	FText UsageText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, WalletInfo->Tooltip2);
	Text_HeroPiece_Usage->SetText(UsageText);

	// TODO: use hero piece server API pending
	Button_HeroPiece_Use->SetDisabled(true);
}

void UUC_WalletItemInfo::InvalidateRune(const FWalletTableInfo* WalletInfo)
{
	FText NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletInfo->CommonStringKey);
	Text_Rune_Name->SetText(NameText);

	// Rune Stats

	int32 RuneGrade = FRuneTable::WalletKeyToRuneGrade(WalletKey);
	check(RuneGrade != INDEX_NONE);
	FRuneTable* RuneTable = FRuneTable::LookUp(RuneGrade);
	check(RuneTable != nullptr);

	// Weapon
	{
		int		StatValue = RuneTable->GetAdditionalOptionValue(EItemEquipPositionEnum::VE_Weapon);
		FText	StatFText = RuneTable->GetAdditionalOptionFText(EItemEquipPositionEnum::VE_Weapon);

		EItemStatOption ItemStat = RuneTable->GetAdditionalOptionStat(EItemEquipPositionEnum::VE_Weapon);
		FText StatValueText = UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(ItemStat, StatValue);

		Text_Rune_WeaponStatName->SetText(StatFText);
		Text_Rune_WeaponStatValue->SetText(StatValueText);
	}

	// Armor
	{
		int		StatValue = RuneTable->GetAdditionalOptionValue(EItemEquipPositionEnum::VE_Armor);
		FText	StatFText = RuneTable->GetAdditionalOptionFText(EItemEquipPositionEnum::VE_Armor);

		EItemStatOption ItemStat = RuneTable->GetAdditionalOptionStat(EItemEquipPositionEnum::VE_Armor);
		FText StatValueText = UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(ItemStat, StatValue);

		Text_Rune_DefenseStatName->SetText(StatFText);
		Text_Rune_DefenseStatValue->SetText(StatValueText);
	}

	// description <- WalletInfo->Tooltip2
	FText DescriptionText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, WalletInfo->Tooltip2);
	Text_Rune_Description->SetText(DescriptionText);

	Button_Rune_Combine->SetVisibility(RuneGrade != LastRuneGrade ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UUC_WalletItemInfo::InvalidateMaterial(const FWalletTableInfo* WalletInfo)
{
	// name
	FText NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletInfo->CommonStringKey);
	Text_Material_Name->SetText(NameText);

	// Description <- WalletInfo->Tooltip
	FText DescriptionText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, WalletInfo->Tooltip);
	Text_Material_Description->SetText(DescriptionText);

	// usage <- WalletInfo->Tooltip2
	FText UsageText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, WalletInfo->Tooltip2);
	Text_Material_Usage->SetText(UsageText);

	// TODO: ??
	Button_Material_Use->SetDisabled(true);
}
