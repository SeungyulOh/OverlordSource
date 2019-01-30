// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemOptionReset.h"

#include "Table/ItemTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"


#include "GameInfo/RInventory.h"

#include "UC_GenericPopup.h"
#include "UC_HeroItemInfo.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UI/UI_Common/UP_RewardPopup.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_GenericSplash.h"
#include "UC_ItemUpgradeResult.h"
#include "SharedConstants/WalletConstants.h"
#include "UC_ItemEnchant.h"
#include "Table/ItemGradeTableInfo.h"

void UUP_ItemOptionReset::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Upgrade->OnClicked.AddDynamic(this, &UUP_ItemOptionReset::OnButtonUpgradeClicked);
}

void UUP_ItemOptionReset::NativeDestruct()
{
	if (IsValid(Button_Upgrade))
		Button_Upgrade->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_ItemOptionReset::InvalidateData()
{
	Super::InvalidateData();

	if (ItemUD.IsEmpty())
		return;

	ItemIcon->RefreshEquipmentIcon(ItemUD);

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;
	check(FItemUpgradeCostInfo::CanResetOption(ItemInfo->ItemGrade));

	CurrentGrade = ItemInfo->ItemGrade;

	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	FItemUpgradeCostInfo* UpgradeInfo = FItemUpgradeCostInfo::LookUp(ItemInfo->ItemGrade, ItemInfo->EquipType, ItemInfo->ItemLevel);
	if (!ensure(UpgradeInfo))
		return;

	// Property material
	FWalletTableInfo* MaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(UpgradeInfo->Material_PropertyIndex);
	if (!ensure(MaterialInfo != nullptr))
		return;
	int32 CommonMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(UpgradeInfo->Material_PropertyIndex);
	
	OptionResetMaterial->MaterialIconKey = MaterialInfo->ItemIconKey;
	OptionResetMaterial->MaterialHaveAmount = CommonMaterialAmount;
	OptionResetMaterial->MaterialNeedAmount = UpgradeInfo->Material_PropertyAmount;

	OptionResetMaterial->InvalidateData();
	// hacky
	OptionResetMaterial->Button_Material->SetIsEnabled(false);

	// Current Additional Stats
	AdditionalStats->Refresh(ItemData);

	// gold
	int32 OwnedGoldAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GOLD);
	HasSufficientGold = OwnedGoldAmount >= UpgradeInfo->Gold;

	// Button_Enchant and gold needed
	//Text_EnchantGoldCost->SetText(FText::AsNumber(EnchantInfo->EnchantGoldCost));
	Button_Upgrade->SetGoldAmount(UpgradeInfo->Gold);
	InvalidateOnEquipmentMaterialSelected();
}

void UUP_ItemOptionReset::SetItemUD(FString InItemUD)
{
	ItemUD = InItemUD;
}

bool UUP_ItemOptionReset::IsInventoryIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	// (design)
	// * only show equipment items of the same grade as the selected one
	// * do not show current item

	if (ItemInfo->ItemGrade != CurrentGrade)
		return false;

	if (ItemData->itemUD == ItemUD)
		return false;

	return true;
}

void UUP_ItemOptionReset::OnInventoryEquipmentItemSelected(FString SelectedItemUD)
{
	// select and toggle goodness

	bool ShouldInvalidate = false;

	if (EquipmentMaterial1_ItemUD == SelectedItemUD)
	{
		EquipmentMaterial1_ItemUD.Empty();

		ShouldInvalidate = true;
	}
	else if (EquipmentMaterial2_ItemUD == SelectedItemUD)
	{
		EquipmentMaterial2_ItemUD.Empty();

		ShouldInvalidate = true;
	}
	else
	{
		if (EquipmentMaterial1_ItemUD.IsEmpty())
		{
			EquipmentMaterial1_ItemUD = SelectedItemUD;

			ShouldInvalidate = true;
		}
		else if (EquipmentMaterial2_ItemUD.IsEmpty())
		{
			EquipmentMaterial2_ItemUD = SelectedItemUD;

			ShouldInvalidate = true;
		}
	}

	if (ShouldInvalidate)
	{
		check(IsValid(ItemInventoryPage));

		InvalidateOnEquipmentMaterialSelected();

		ItemInventoryPage->InvalidateData();
	}
}

void UUP_ItemOptionReset::UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData)
{
	// (design) (bug list)
	// * locked items are disabled
	// * currently equipped items are disabled

	Super::UpdateScrollItemData_EquipmentItem(ScrollItemData);

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ScrollItemData->ItemUD);
	check(ItemData != nullptr);

	ScrollItemData->bDisabled = ItemData->locked || !ItemData->equipedHeroUD.IsEmpty();

	// selected
	ScrollItemData->IsSelected = ScrollItemData->ItemUD == EquipmentMaterial1_ItemUD || ScrollItemData->ItemUD == EquipmentMaterial2_ItemUD;
}

void UUP_ItemOptionReset::InvalidateOnEquipmentMaterialSelected()
{
	// Equipment material icons
	if (EquipmentMaterial1_ItemUD.IsEmpty())
	{
		Icon_EquipmentMaterial1->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Icon_EquipmentMaterial1->SetVisibility(ESlateVisibility::HitTestInvisible);

		Icon_EquipmentMaterial1->RefreshEquipmentIcon(EquipmentMaterial1_ItemUD);
	}

	if (EquipmentMaterial2_ItemUD.IsEmpty())
	{
		Icon_EquipmentMaterial2->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Icon_EquipmentMaterial2->SetVisibility(ESlateVisibility::HitTestInvisible);

		Icon_EquipmentMaterial2->RefreshEquipmentIcon(EquipmentMaterial2_ItemUD);
	}

	// Button
	Button_Upgrade->SetDisabled(!CanUpgrade());
}

bool UUP_ItemOptionReset::CanUpgrade()
{
	return !EquipmentMaterial1_ItemUD.IsEmpty()
		&& !EquipmentMaterial2_ItemUD.IsEmpty()
		&& OptionResetMaterial->MaterialHaveAmount >= OptionResetMaterial->MaterialNeedAmount // eh
		&& HasSufficientGold;
}

void UUP_ItemOptionReset::OnButtonUpgradeClicked()
{
	check(CanUpgrade());
	
	const FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	check(ItemData != nullptr);

	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemOptionReset.AddUObject(this, &UUP_ItemOptionReset::OnInventoryItemUpgradeRp, *ItemData);

	UPacketFunctionLibrary::INVENTORY_ITEM_OPTION_RESET_RQ(ItemUD, { EquipmentMaterial1_ItemUD, EquipmentMaterial2_ItemUD });
}

void UUP_ItemOptionReset::OnInventoryItemUpgradeRp(bool bSuccess, FITEM ItemDataBefore)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemOptionReset.RemoveAll(this);

	if (bSuccess)
	{
		// Show BP UP_Splash_ItemOptionResetResult

		auto Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_ItemOptionResetResult);
		check(IsValid(Splash));
		auto SplashContent = Cast<UUC_ItemOptionResetResult>(Splash->GetContentWidget());
		check(IsValid(SplashContent));

		SplashContent->ItemUD = ItemUD;
		SplashContent->ItemDataBefore = ItemDataBefore;

		SplashContent->InvalidateData();

		// invalidate itself
		// reset selection state
		EquipmentMaterial1_ItemUD.Empty();
		EquipmentMaterial2_ItemUD.Empty();

		InvalidateData();

		ItemInventoryPage->bInventoryUpdated = true;
		ItemInventoryPage->InvalidateData();
	}
	else
	{
		// ?
		ClosePage();
	}
}

void UUP_ItemOptionReset::OnButtonEquipmentMaterialClicked(int32 WhichOne)
{
	if (!ensure(WhichOne == 0 || WhichOne == 1))
		return;

	check(IsValid(ItemInventoryPage));

	if (WhichOne == 0 && !EquipmentMaterial1_ItemUD.IsEmpty())
	{
		EquipmentMaterial1_ItemUD.Empty();

		InvalidateOnEquipmentMaterialSelected();

		ItemInventoryPage->InvalidateData();
	}
	else if (WhichOne == 1 && !EquipmentMaterial2_ItemUD.IsEmpty())
	{
		EquipmentMaterial2_ItemUD.Empty();

		InvalidateOnEquipmentMaterialSelected();

		ItemInventoryPage->InvalidateData();
	}
}

void UUC_ItemOptionResetResult::InvalidateData()
{
	Super::InvalidateData();

	if (!ensure(!ItemUD.IsEmpty()))
		return;

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	ItemIcon->RefreshEquipmentIcon(ItemUD);
	ItemIcon->Shine();
	
	// stat changes
	AdditionalStatsBefore->Refresh(&ItemDataBefore);
	AdditionalStatsAfter->Refresh(ItemData);
}

void UUC_ItemOptionReset_AdditionalStats::Refresh(const FITEM* ItemData)
{
	ensure(AdditionalStats.Num() >= 4);

	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	for (int32 i = 0; i < AdditionalStats.Num(); i++)
	{
		EItemStatOption ItemStatOption = EItemStatOption::PhysicalAttackPower;
		int32 Value = 0;

		if (ItemInfo->GetOneAdditionalOption(ItemData, i, ItemStatOption, Value))
		{
			AdditionalStats[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			AdditionalStats[i]->Refresh(ItemStatOption, Value, true);
		}
		else
		{
			AdditionalStats[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
