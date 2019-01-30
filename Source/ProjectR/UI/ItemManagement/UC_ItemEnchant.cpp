// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemEnchant.h"

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
#include "UC_StatChange.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_ItemEnchantResult_NewDesign.h"
#include "UP_GenericSplash.h"
#include "UC_ItemEnchantResult.h"
#include "SharedConstants/WalletConstants.h"
#include "UI/Common/UC_StatHeroLevelUp.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"

void UUC_ItemEnchant::NativeConstruct()
{
	Super::NativeConstruct();

	//ParentPopup->Text_PopupTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Enchant_000000")));
	//ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_ItemEnchant::OnButtonCloseClicked);

	Button_Enchant->OnClicked.AddDynamic(this, &UUC_ItemEnchant::OnButtonEnchantClicked);

	// Hook up delegates
	NormalEnchantMaterial->OnButtonToggled.AddUObject(this, &UUC_ItemEnchant::OnEnchantMaterialToggled, false);
}

void UUC_ItemEnchant::NativeDestruct()
{
	NormalEnchantMaterial->OnButtonToggled.RemoveAll(this);

	if (IsValid(Button_Enchant))
		Button_Enchant->OnClicked.RemoveAll(this);
	//ParentPopup->Button_Close->OnClicked.RemoveDynamic(this, &UUC_ItemEnchant::OnButtonCloseClicked);

	Super::NativeDestruct();
}

void UUC_ItemEnchant::InvalidateData()
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

	if (IsValid(EquipmentSwitcher))
	{
		if (ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Weapon)
			EquipmentSwitcher->SetActiveWidgetIndex(0);
		else if (ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Armor ||
			ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Pants)
		{
			EquipmentSwitcher->SetActiveWidgetIndex(1);
		}
		else if (ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Necklace ||
			ItemInfo->EquipPosition == EItemEquipPositionEnum::VE_Ring)
		{
			EquipmentSwitcher->SetActiveWidgetIndex(2);
		}
	}
	


	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	/*ItemLevel*/
	if (IsValid(Text_ItemLevel))
	{
		FString str = FString::FromInt(ItemData->level) + TEXT(" / ") + FString::FromInt(ItemInfo->MaxLevel);
		Text_ItemLevel->SetText(FText::FromString(str));
	}
	if (IsValid(Text_Enchant))
	{
		FString str = TEXT("+ ") + FString::FromInt(ItemData->enchant);
		Text_Enchant->SetText(FText::FromString(str));
	}
	if (IsValid(Text_CurrentEnchant))
	{
		FString str = TEXT("+ ") + FString::FromInt(ItemData->enchant);
		Text_CurrentEnchant->SetText(FText::FromString(str));
	}
	if (IsValid(Text_NextEnchant))
	{
		int32 NextEnchant = ItemData->enchant + 1;
		if (NextEnchant >= ItemInfo->MaxEnchantLevel)
		{
			Text_NextEnchant->SetText(FText::FromString(TEXT("Max")));
		}
		else
		{
			NextEnchant = FMath::Min<int32>(NextEnchant, ItemInfo->MaxEnchantLevel);
			FString str = TEXT("+ ") + FString::FromInt(NextEnchant);
			Text_NextEnchant->SetText(FText::FromString(str));
		}
	}
		
	/*Stat Settings*/
	TArray<int32> BasicOptions;
	ItemInfo->GetBasicOptions(BasicOptions);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, BasicOptions);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, BasicOptions);

	InvalidateBasicStats(BasicOptions);
	//InvalidateSubstats(BasicOptions);


	// stat changes
	InvalidateStatChanges(ItemInfo, ItemData);

	//// materials
	FItemEnchantTableInfo* EnchantInfo = FItemEnchantTableInfo::LookUp(ItemInfo->EquipPosition, ItemInfo->ItemGrade, ItemData->enchant);
	if (!ensure(EnchantInfo != nullptr))
		return;

	// regular enchant material
	FWalletTableInfo* CommonMaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(EnchantInfo->CommonMaterial);
	if (!ensure(CommonMaterialInfo != nullptr))
		return;
	int32 CommonMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(EnchantInfo->CommonMaterial);
	
	NormalEnchantMaterial->MaterialIconKey = CommonMaterialInfo->ItemIconKey;
	NormalEnchantMaterial->MaterialHaveAmount = CommonMaterialAmount;
	NormalEnchantMaterial->MaterialNeedAmount = EnchantInfo->CommonMaterialCount;

	NormalEnchantMaterial->InvalidateData();

	if (IsValid(Text_CurrentAmount1))
	{
		int32 amt = FMath::Min<int32>(CommonMaterialAmount, EnchantInfo->CommonMaterialCount);
		Text_CurrentAmount1->SetText(FText::AsNumber(amt));
	}
	if (IsValid(Text_ReqAmount1))
	{
		Text_ReqAmount1->SetText(FText::AsNumber(EnchantInfo->CommonMaterialCount));
	}
	// special enchant material

	
	

	// protection material
	
	if (EnchantInfo->ProtectionMaterial.IsNone())
	{
		// ?
		UE_LOG(LogUI, Log, TEXT("UUC_ItemEnchant::InvalidateData: EnchantInfo->ProtectionMaterial IsNone"));
		if (IsValid(ProtectionEnchantMaterial))
		{
			ProtectionEnchantMaterial->IsDisabled = true;
			ProtectionEnchantMaterial->InvalidateData();
		}
	}
	else
	{
		FWalletTableInfo* ProtectionMaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(EnchantInfo->ProtectionMaterial);
		if (!ensure(ProtectionMaterialInfo != nullptr))
			return;
		int32 ProtectionMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(EnchantInfo->ProtectionMaterial);

		if (IsValid(ProtectionEnchantMaterial))
		{
			ProtectionEnchantMaterial->MaterialIconKey = ProtectionMaterialInfo->ItemIconKey;
			ProtectionEnchantMaterial->MaterialHaveAmount = ProtectionMaterialAmount;
			ProtectionEnchantMaterial->MaterialNeedAmount = EnchantInfo->ProtectionMaterialCount;

			// (design) EnchantInfo->ProtectionMaterial is rendered disabled for enchant level 0/when the success rate is 100%
			ProtectionEnchantMaterial->IsDisabled = EnchantInfo->ProtectionMaterial.IsNone(); // per mil

			if (EnchantInfo->ProtectionMaterialCount == 0)	
				ProtectionEnchantMaterial->IsDisabled = true;

			ProtectionEnchantMaterial->InvalidateData();

			if (ProtectionEnchantMaterial->IsDisabled == false)
			{
				if (IsValid(NoramlVerticalBox))
					NoramlVerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if (IsValid(ProtectVerticalBox))
					ProtectVerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				if (IsValid(Text_CurrentAmount2))
				{
					int32 amt = FMath::Min<int32>(ProtectionMaterialAmount, EnchantInfo->ProtectionMaterialCount);
					Text_CurrentAmount2->SetText(FText::AsNumber(amt));
				}
				if (IsValid(Text_ReqAmount2))
				{
					Text_ReqAmount2->SetText(FText::AsNumber(EnchantInfo->ProtectionMaterialCount));
				}
			}
			else
			{
				if (IsValid(NoramlVerticalBox))
					NoramlVerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if (IsValid(ProtectVerticalBox))
					ProtectVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
			}

			
		}
	}

	// success probability
	FText SuccessProbText = FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(((float)EnchantInfo->SuccessProbability) / 10.0)));
	if(IsValid(Text_EnchantSuccessProbability))
		Text_EnchantSuccessProbability->SetText(SuccessProbText);

	// Button_Enchant and gold needed
	//Text_EnchantGoldCost->SetText(FText::AsNumber(EnchantInfo->EnchantGoldCost));
	EnchantGoldCost = EnchantInfo->EnchantGoldCost;
	
	if(IsValid(Button_Enchant))
		Button_Enchant->SetGoldAmount(EnchantInfo->EnchantGoldCost);

	InvalidateButtonEnchant();
}

void UUC_ItemEnchant::InvalidateBasicStats(const TArray<int32>& BasicOptions)
{
	for (UUC_ItemInfo_ItemStat* ItemStat : BasicStats)
	{
		ItemStat->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i <= (int32)EItemStatOption::MagicResistance; i++)
	{
		if (BasicOptions[i] == 0)
			continue; // No option

		if (!ensure(WidgetIndex < BasicStats.Num()))
			break;

		BasicStats[WidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BasicStats[WidgetIndex]->Refresh((EItemStatOption)i, BasicOptions[i], false);

		WidgetIndex++;
	}
}

void UUC_ItemEnchant::InvalidateSubstats(const TArray<int32>& BasicOptions)
{
	for (UUC_ItemInfo_ItemStat* ItemStat : Substats)
	{
		ItemStat->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = (int32)EItemStatOption::AttackRange; i < (int32)EItemStatOption::Max; i++)
	{
		if (BasicOptions[i] == 0)
			continue; // No option

		if (!ensure(WidgetIndex <= Substats.Num()))
			break;

		Substats[WidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Substats[WidgetIndex]->Refresh((EItemStatOption)i, BasicOptions[i], false);

		WidgetIndex++;
	}
}

void UUC_ItemEnchant::SetItemUD(FString InItemUD)
{
	ItemUD = InItemUD;
	InvalidateData();
}




void UUC_ItemEnchant::InvalidateButtonEnchant()
{
	Button_Enchant->SetDisabled(!CanEnchant());
}

void UUC_ItemEnchant::OnButtonEnchantClicked()
{
	check(CanEnchant());

	const FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	FItemEnchantTableInfo* EnchantInfo = FItemEnchantTableInfo::LookUp(ItemInfo->EquipPosition, ItemInfo->ItemGrade, ItemData->enchant);
	if (!ensure(EnchantInfo != nullptr))
		return;

	bool IsUsingProtection = !EnchantInfo->ProtectionMaterial.IsNone();

	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemEnchant.AddUObject(this, &UUC_ItemEnchant::OnInventoryItemEnchantRp, *ItemData);

	UPacketFunctionLibrary::INVENTORY_ITEM_ENCHANT_RQ(ItemUD, false, IsUsingProtection);
}

void UUC_ItemEnchant::OnInventoryItemEnchantRp(bool bSuccess, int32 affectedEnchant, FITEM ItemDataBefore)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemEnchant.RemoveAll(this);

	UE_LOG(LogUI, Log, TEXT("UUC_ItemEnchant::OnInventoryItemEnchantRp %d %d"), (int32)bSuccess, affectedEnchant);

	if (bSuccess)
	{
		const FITEM* ItemDataAfterEnchant = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
		const FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataAfterEnchant->itemId);

		// Show BP UP_Splash_ItemEnchantResult
		
		auto Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_ItemEnchantResult);
		check(IsValid(Splash));
		auto SplashContent = Cast<UUC_ItemEnchantResult>(Splash->GetContentWidget());
		check(IsValid(SplashContent));

		SplashContent->ItemUD = ItemUD;
		SplashContent->ItemDataBefore = ItemDataBefore;

		SplashContent->InvalidateData();

		UUP_HeroManager* HeroManagerPage = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
		if (HeroManagerPage)
		{
			HeroManagerPage->OnClick_HeroIcon(HeroManagerPage->GetCurrentHeroUD());
		}

		// Return to parent if no further enchant is possible for the item
// 		bool ReturnToParent = ItemDataAfterEnchant->enchantLocked || ItemInfo->MaxEnchantLevel == ItemDataAfterEnchant->enchant;
// 		if (ReturnToParent)
// 		{
// 			check(RGAMEINSTANCE(this)->RWidgetManager);
// 
// 			auto TopBar = Cast<UTopBar>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
// 			if (!ensure(IsValid(TopBar)))
// 				return;
// 
// 			TopBar->PrevButton();
// 		}
// 		else
// 		{
// 			InvalidateData();
// 		}
		InvalidateData();
	}
	else
	{
		check(RGAMEINSTANCE(this)->RWidgetManager);

		auto TopBar = Cast<UTopBar>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
		if (!ensure(IsValid(TopBar)))
			return;

		TopBar->PrevButton();
	}
}

void UUC_ItemEnchant::OnEnchantMaterialToggled(bool IsOn, bool IsSpecial)
{
	// Radio Group Toggle
	if (IsOn)
	{
		if (IsSpecial && NormalEnchantMaterial->GetIsSelected())
		{
			NormalEnchantMaterial->SetSelect(false);
		}
		
	}

	InvalidateButtonEnchant();
}

void UUC_ItemEnchant::OnClickClose()
{
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ItemEnchant);
}

void UUC_ItemEnchant::RefreshStatChanges(TArray<UUC_StatHeroLevelUp*>& InBasicStatChanges, TArray<UUC_StatHeroLevelUp*>& InSubstatChanges, TArray<int32>& BasicOptionsBefore, TArray<int32>& BasicOptionsAfter, bool bMaxLevel)
{
	for (UUC_StatHeroLevelUp* StatDiff : InBasicStatChanges)
	{
		StatDiff->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i <= (int32)EItemStatOption::MagicResistance; i++)
	{
		// Do not show stat that didn't change
		if (BasicOptionsBefore[i] == 0 && BasicOptionsAfter[i] == 0)
			continue;

		if (!InBasicStatChanges.IsValidIndex(WidgetIndex))
			break;

		FName ItemStatOptionStringKey = UUIFunctionLibrary::ItemStatOptionToStringKey((EItemStatOption)i);

		InBasicStatChanges[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);
		InBasicStatChanges[WidgetIndex]->SetStat(FString::FromInt(BasicOptionsBefore[i]), FString::FromInt(BasicOptionsAfter[i]), ItemStatOptionStringKey.ToString(), FString::FromInt(BasicOptionsAfter[i] - BasicOptionsBefore[i]) , bMaxLevel);

		WidgetIndex++;
	}

	/*for (UUC_StatHeroLevelUp* StatDiff : InSubstatChanges)
	{
		StatDiff->SetVisibility(ESlateVisibility::Collapsed);
	}

	WidgetIndex = 0;
	for (int32 i = (int32)EItemStatOption::MaxHP; i < (int32)EItemStatOption::Max; i++)
	{
		if (BasicOptionsBefore[i] == 0 && BasicOptionsAfter[i] == 0)
			continue;

		if (!InSubstatChanges.IsValidIndex(WidgetIndex))
			break;

		FName ItemStatOptionStringKey = UUIFunctionLibrary::ItemStatOptionToStringKey((EItemStatOption)i);

		InSubstatChanges[WidgetIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);
		InSubstatChanges[WidgetIndex]->SetStat(FString::FromInt(BasicOptionsBefore[i]), FString::FromInt(BasicOptionsAfter[i]), ItemStatOptionStringKey.ToString(), FString::FromInt(BasicOptionsAfter[i] - BasicOptionsBefore[i]));

		WidgetIndex++;
	}*/
}

void UUC_ItemEnchant::InvalidateStatChanges(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	// current enchant level and next
	// (design) Only show changes for the item's Basic Options
	// (design) worked out only for Item Level and Enchant. (no additional options)

	TArray<int32> CurrentBasicOptions;
	ItemInfo->GetBasicOptions(CurrentBasicOptions);
	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, CurrentBasicOptions);

	TArray<int32> EnchantedBasicOptions = CurrentBasicOptions;

	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, CurrentBasicOptions);

	FITEM EnchantedItemData = *ItemData;
	EnchantedItemData.enchant += 1;

	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, &EnchantedItemData, EnchantedBasicOptions);

	// stat changes
	bool bMaxLevel = ItemInfo->MaxEnchantLevel <= EnchantedItemData.enchant;
	RefreshStatChanges(BasicStatChanges, SubstatChanges, CurrentBasicOptions, EnchantedBasicOptions , bMaxLevel);
}

bool UUC_ItemEnchant::CanEnchant()
{
	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemData->enchant >= ItemInfo->MaxEnchantLevel)
		return false;

	FItemEnchantTableInfo* EnchantInfo = FItemEnchantTableInfo::LookUp(ItemInfo->EquipPosition, ItemInfo->ItemGrade, ItemData->enchant);
	if (!ensure(EnchantInfo != nullptr))
		return false;

	bool CanEnchant = false;
	if (EnchantInfo->ProtectionMaterial.IsNone())
	{
		int32 CommonMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(EnchantInfo->CommonMaterial);
		if (EnchantInfo->CommonMaterialCount <= CommonMaterialAmount)
			CanEnchant = true;
	}
	else
	{
		int32 ProtectionMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(EnchantInfo->ProtectionMaterial);
		int32 CommonMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(EnchantInfo->CommonMaterial);
		if (EnchantInfo->ProtectionMaterialCount <= ProtectionMaterialAmount && EnchantInfo->CommonMaterialCount <= CommonMaterialAmount)
			CanEnchant = true;
	}


	int32 OwnedGoldAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GOLD);
	return CanEnchant && OwnedGoldAmount >= EnchantGoldCost;

}

void UUC_ItemEnchant_Button::NativeConstruct()
{
	Super::NativeConstruct();

	switch (ButtonType)
	{
		case EItemButtonType::ItemEnchantType:
			Text_ButtonType_Enable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_EnchantButton")));
			Text_ButtonType_Disable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_EnchantButton")));
			break;
		case EItemButtonType::ItemLevelUpType:
			Text_ButtonType_Enable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_LVUpButton")));
			Text_ButtonType_Disable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_LVUpButton")));
			break;
		case EItemButtonType::ItemUpgradeType:
			Text_ButtonType_Enable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_UpgradeButton")));
			Text_ButtonType_Disable->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroManagement_HeroInfo_UpgradeButton")));
			break;
	}
}

void UUC_ItemEnchant_Button::SetGoldAmount(int32 Amount)
{
	SetTextGoldAmount(FText::AsNumber(Amount));
}
