// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_Item_Filter.h"

void UUC_Popup_Item_Filter::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Popup_Item_Filter::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Popup_Item_Filter::CheckBoxReset(TArray<UCheckBox*> list)
{
	for (auto& checkBox : list)
	{
		checkBox->SetIsChecked(false);
	}
}

void UUC_Popup_Item_Filter::FilteringAllConditions(bool isChecked)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->RInventory->AttackTypeFilter.Reset();
	RGameInstance->RInventory->itemGradeFilter.Reset();
	RGameInstance->RInventory->itemTierFilter.Reset();
	RGameInstance->RInventory->EquipTypeFilter.Reset();
	RGameInstance->RInventory->EnchantStateFilter.Reset();
	RGameInstance->RInventory->UseStateFilter.Reset();

	if (CheckBox_Normal->IsChecked())
		RGameInstance->RInventory->itemGradeFilter.AddUnique(EItemGradeEnum::Normal);
	if (CheckBox_Rare->IsChecked())
		RGameInstance->RInventory->itemGradeFilter.AddUnique(EItemGradeEnum::Rare);
	if (CheckBox_Epic->IsChecked())
		RGameInstance->RInventory->itemGradeFilter.AddUnique(EItemGradeEnum::Epic);
	if (CheckBox_Unique->IsChecked())
		RGameInstance->RInventory->itemGradeFilter.AddUnique(EItemGradeEnum::Unique);
	if (CheckBox_Tower->IsChecked())
		RGameInstance->RInventory->itemGradeFilter.AddUnique(EItemGradeEnum::Tower);

	if (CheckBox_Using->IsChecked())
		RGameInstance->RInventory->UseStateFilter.AddUnique(EItemUseStateEnum::Using);
	if (CheckBox_Locking->IsChecked())
		RGameInstance->RInventory->UseStateFilter.AddUnique(EItemUseStateEnum::Locking);

	if (CheckBox_T1->IsChecked())
		RGameInstance->RInventory->itemTierFilter.AddUnique(1);
	if (CheckBox_T2->IsChecked())
		RGameInstance->RInventory->itemTierFilter.AddUnique(2);
	if (CheckBox_T3->IsChecked())
		RGameInstance->RInventory->itemTierFilter.AddUnique(3);
	if (CheckBox_T4->IsChecked())
		RGameInstance->RInventory->itemTierFilter.AddUnique(4);
}

void UUC_Popup_Item_Filter_Total::NativeConstruct()
{
	RGAMEINSTANCE(this)->RInventory->CloseFilterPopup.AddDynamic(this, &UUC_Popup_Item_Filter_Total::Close);
}

void UUC_Popup_Item_Filter_Total::NativeDestruct()
{
	RGAMEINSTANCE(this)->RInventory->CloseFilterPopup.RemoveDynamic(this, &UUC_Popup_Item_Filter_Total::Close);
}

void UUC_Popup_Item_Filter_Total::CheckBoxReset(int32 tab)
{
	switch (tab)
	{
	case 1:
		UC_Popup_Item_Filter_Weapon->Reset();
		break;
	case 2:
		UC_Popup_Item_Filter_Armor->Reset();
		break;
	case 3:
		UC_Popup_Item_Filter_Accessories->Reset();
		break;
	default:
		break;
	}
}

void UUC_Popup_Item_Filter_Total::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	CloseFilter.Broadcast(true);
}

void UUC_Popup_Item_Filter_Weapon::NativeConstruct()
{
	Super::NativeConstruct();
	
	CheckBoxList.Reset();
	FindChildWidgetsWithCertainName<UCheckBox>(this, CheckBoxList, TEXT("CheckBox_"));

	Button_Reset->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::OnClickButton_Reset);
	Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::OnClickButton_Close);

	CheckBox_Tanker->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MeleeDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_RangeDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MagicDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Supporter->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Normal->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Rune_Equiped->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_NoEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_HaveEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MaxEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_BlcokEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Sword->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Dagger->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Mace->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Spear->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Bow->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_CrossBow->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Gun->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Staff->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Orb->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
}

void UUC_Popup_Item_Filter_Weapon::NativeDestruct()
{
	Button_Reset->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::OnClickButton_Reset);
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::OnClickButton_Close);

	CheckBox_Tanker->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MeleeDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_RangeDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MagicDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Supporter->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Normal->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Rune_Equiped->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_NoEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_HaveEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_MaxEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_BlcokEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Sword->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Dagger->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Mace->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Spear->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Bow->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_CrossBow->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Gun->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Staff->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);
	CheckBox_Orb->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Weapon::FilteringAllConditions);

	Super::NativeDestruct();
}

void UUC_Popup_Item_Filter_Weapon::Reset()
{
	CheckBoxReset(CheckBoxList);
}

void UUC_Popup_Item_Filter_Weapon::OnClickButton_Close()
{
	RGAMEINSTANCE(this)->RInventory->CloseFilterPopup.Broadcast();
}

void UUC_Popup_Item_Filter_Weapon::OnClickButton_Reset()
{
	Reset();
	FilteringAllConditions(true);
}

void UUC_Popup_Item_Filter_Weapon::FilteringAllConditions(bool isChecked)
{
	Super::FilteringAllConditions(isChecked);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (CheckBox_Tanker->IsChecked())
		RGameInstance->RInventory->AttackTypeFilter.AddUnique(EAttackTypeEnum::VE_Melee);
	if (CheckBox_MeleeDealer->IsChecked())
		RGameInstance->RInventory->AttackTypeFilter.AddUnique(EAttackTypeEnum::VE_Melee);
	if (CheckBox_RangeDealer->IsChecked())
		RGameInstance->RInventory->AttackTypeFilter.AddUnique(EAttackTypeEnum::VE_Range);
	if (CheckBox_MagicDealer->IsChecked())
		RGameInstance->RInventory->AttackTypeFilter.AddUnique(EAttackTypeEnum::VE_Magic);
	if (CheckBox_Supporter->IsChecked())
		RGameInstance->RInventory->AttackTypeFilter.AddUnique(EAttackTypeEnum::VE_Magic);

	if (CheckBox_Rune_Equiped->IsChecked())
		RGameInstance->RInventory->UseStateFilter.AddUnique(EItemUseStateEnum::EquipedRune);

	if (CheckBox_NoEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::NoEnchant);
	if (CheckBox_HaveEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::HaveEnchant);
	if (CheckBox_MaxEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::MaxEnchant);
	if (CheckBox_BlcokEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::CantEnchant);

	if (CheckBox_Sword->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Sword);
	if (CheckBox_Dagger->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Dagger);
	if (CheckBox_Mace->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Hammer);
	if (CheckBox_Spear->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Rod);
	if (CheckBox_Bow->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Bow);
	if (CheckBox_CrossBow->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Crossbow);
	if (CheckBox_Gun->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Gun);
	if (CheckBox_Staff->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Staff);
	if (CheckBox_Orb->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Magic);

	RGameInstance->RInventory->OnClickInvenFilterButton.Broadcast();
}

void UUC_Popup_Item_Filter_Armor::NativeConstruct()
{
	Super::NativeConstruct();

	CheckBoxList.Reset();
	FindChildWidgetsWithCertainName<UCheckBox>(this, CheckBoxList, TEXT("CheckBox_"));

	Button_Reset->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::OnClickButton_Reset);
	Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::OnClickButton_Close);

	CheckBox_Normal->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Rune_Equiped->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_NoEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HaveEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_MaxEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_BlcokEnchant->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HeavyArmorTop->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_LightArmorTop->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_ClothesTop->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HeavyArmorPants->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_LightArmorPants->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_ClothesPants->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
}

void UUC_Popup_Item_Filter_Armor::NativeDestruct()
{
	Button_Reset->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::OnClickButton_Reset);
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::OnClickButton_Close);

	CheckBox_Normal->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_Rune_Equiped->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_NoEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HaveEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_MaxEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_BlcokEnchant->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HeavyArmorTop->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_LightArmorTop->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_ClothesTop->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_HeavyArmorPants->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_LightArmorPants->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);
	CheckBox_ClothesPants->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Armor::FilteringAllConditions);

	Super::NativeDestruct();
}

void UUC_Popup_Item_Filter_Armor::Reset()
{
	CheckBoxReset(CheckBoxList);
}

void UUC_Popup_Item_Filter_Armor::OnClickButton_Close()
{
	RGAMEINSTANCE(this)->RInventory->CloseFilterPopup.Broadcast();
}

void UUC_Popup_Item_Filter_Armor::OnClickButton_Reset()
{
	Reset();
	FilteringAllConditions(true);
}

void UUC_Popup_Item_Filter_Armor::FilteringAllConditions(bool isChecked)
{
	Super::FilteringAllConditions(isChecked);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (CheckBox_Rune_Equiped->IsChecked())
		RGameInstance->RInventory->UseStateFilter.AddUnique(EItemUseStateEnum::EquipedRune);

	if (CheckBox_NoEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::NoEnchant);
	if (CheckBox_HaveEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::HaveEnchant);
	if (CheckBox_MaxEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::MaxEnchant);
	if (CheckBox_BlcokEnchant->IsChecked())
		RGameInstance->RInventory->EnchantStateFilter.AddUnique(EItemEnchantStateEnum::CantEnchant);

	if (CheckBox_HeavyArmorTop->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_HeavyArmor);
	if (CheckBox_LightArmorTop->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_LightArmor);
	if (CheckBox_ClothesTop->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_FabricArmor);
	if (CheckBox_HeavyArmorPants->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_HeavyPants);
	if (CheckBox_LightArmorPants->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_LightPants);
	if (CheckBox_ClothesPants->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_FabricPants);

	RGameInstance->RInventory->OnClickInvenFilterButton.Broadcast();
}

void UUC_Popup_Item_Filter_Accessories::NativeConstruct()
{
	Super::NativeConstruct();

	CheckBoxList.Reset();
	FindChildWidgetsWithCertainName<UCheckBox>(this, CheckBoxList, TEXT("CheckBox_"));

	Button_Reset->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::OnClickButton_Reset);
	Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::OnClickButton_Close);

	CheckBox_Normal->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Necklace->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Ring->OnCheckStateChanged.AddDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
}

void UUC_Popup_Item_Filter_Accessories::NativeDestruct()
{
	Button_Reset->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Accessories::OnClickButton_Reset);
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_Item_Filter_Accessories::OnClickButton_Close);

	CheckBox_Normal->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Rare->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Epic->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Unique->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Tower->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T1->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T2->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T3->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_T4->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Using->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Locking->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter::FilteringAllConditions);
	CheckBox_Necklace->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);
	CheckBox_Ring->OnCheckStateChanged.RemoveDynamic(this, &UUC_Popup_Item_Filter_Accessories::FilteringAllConditions);

	Super::NativeDestruct();
}

void UUC_Popup_Item_Filter_Accessories::Reset()
{
	CheckBoxReset(CheckBoxList);
}

void UUC_Popup_Item_Filter_Accessories::OnClickButton_Close()
{
	RGAMEINSTANCE(this)->RInventory->CloseFilterPopup.Broadcast();
}

void UUC_Popup_Item_Filter_Accessories::OnClickButton_Reset()
{
	Reset();
	FilteringAllConditions(true);
}

void UUC_Popup_Item_Filter_Accessories::FilteringAllConditions(bool isChecked)
{
	Super::FilteringAllConditions(isChecked);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (CheckBox_Necklace->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Necklace);
	if (CheckBox_Ring->IsChecked())
		RGameInstance->RInventory->EquipTypeFilter.AddUnique(EItemEquipTypeEnum::VE_Ring);

	RGameInstance->RInventory->OnClickInvenFilterButton.Broadcast();
}
