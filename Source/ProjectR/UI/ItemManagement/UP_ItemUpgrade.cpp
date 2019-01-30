// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemUpgrade.h"

#include "Table/ItemTableInfo.h"

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
#include "UI/Common/Popup/UC_Popup_Item_UpgradeConfirm.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_GenericSplash.h"
#include "UC_ItemUpgradeResult.h"
#include "SharedConstants/WalletConstants.h"
#include "UC_ItemEnchant.h"
#include "UP_ItemEnchantPopup.h"
#include "Table/ItemGradeTableInfo.h"
#include "UP_GenericScrollview.h"
#include "UIFunctionLibrary.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"

void UUP_ItemUpgrade::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(Button_Upgrade))
		Button_Upgrade->OnClicked.AddDynamic(this, &UUP_ItemUpgrade::OnButtonUpgradeClicked);

	if (IsValid(Button_ShowEquipment))
	{
		Button_ShowEquipment->OnClicked.Clear();
		Button_ShowEquipment->OnClicked.AddDynamic(this, &UUP_ItemUpgrade::OnClick_ShowEquipment);
	}
}

void UUP_ItemUpgrade::NativeDestruct()
{
	if (IsValid(Button_Upgrade))
		Button_Upgrade->OnClicked.RemoveAll(this);

	//UP_Popup_Item_Enchant_New->OnClickOK();
	if (UP_Popup_Item_Enchant_New->GetVisibility() == ESlateVisibility::Visible)
		UP_Popup_Item_Enchant_New->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}

void UUP_ItemUpgrade::InvalidateData()
{
	Super::InvalidateData();
	if (ItemUD.IsEmpty())
		return;

	bool bUpgradePossible = true;

	ItemIcon->RefreshEquipmentIcon(ItemUD);

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;
	check(FItemUpgradeCostInfo::CanUpgrade(ItemInfo->ItemGrade));

	CurrentGrade = ItemInfo->ItemGrade;
	CurrentLevel = ItemInfo->ItemLevel;

	ItemText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

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
	/*Stat Settings*/
	TArray<int32> BasicOptions;
	ItemInfo->GetBasicOptions(BasicOptions);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, BasicOptions);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, BasicOptions);

	InvalidateBasicStats(BasicOptions);
	//InvalidateSubstats(BasicOptions);


	FItemUpgradeCostInfo* UpgradeInfo = FItemUpgradeCostInfo::LookUp(ItemInfo->ItemGrade, ItemInfo->EquipType, ItemInfo->ItemLevel);
	if (!ensure(UpgradeInfo))
		return;

	// Property material
	FWalletTableInfo* MaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(UpgradeInfo->Material_PropertyIndex);
	if (!ensure(MaterialInfo != nullptr))
		return;
	int32 CommonMaterialAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(UpgradeInfo->Material_PropertyIndex);
	
	if (IsValid(UpgradeMaterial))
	{
		int32 amt = FMath::Min<int32>(CommonMaterialAmount, UpgradeInfo->Material_PropertyAmount);

		if (CommonMaterialAmount < UpgradeInfo->Material_PropertyAmount)
			bUpgradePossible = false;

		if (IsValid(Text_CurAmountMaterial))
		{
			Text_CurAmountMaterial->SetText(FText::AsNumber(amt));
		}
		if (IsValid(Text_ReqAmountMaterial))
		{
			Text_ReqAmountMaterial->SetText(FText::AsNumber(UpgradeInfo->Material_PropertyAmount));
		}

		UpgradeMaterial->Refresh(MaterialInfo->ItemIconKey, CommonMaterialAmount, false, 0);
	}
	
	if (IsValid(EquipmentImage))
	{
		if (IsValid(Text_ReqAmountEquipment))
			Text_ReqAmountEquipment->SetText(FText::AsNumber(UpgradeInfo->Material_EquipAmount));

		if (IsValid(Text_CurAmountEquipment))
		{
			UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
			if (GenericScrollview)
			{
				int32 SelectedCount = GenericScrollview->Variables.ScrollView->GetSelectedScrollItem().Num();
				Text_CurAmountEquipment->SetText(FText::AsNumber(SelectedCount));

				if (SelectedCount < UpgradeInfo->Material_EquipAmount)
					bUpgradePossible = false;
			}
		}

		bool bWeapon = FMath::IsWithin(UpgradeInfo->EquipType, EItemEquipTypeEnum::VE_Sword, EItemEquipTypeEnum::VE_Wand);
		bool bArmor = FMath::IsWithin(UpgradeInfo->EquipType, EItemEquipTypeEnum::VE_HeavyArmor, EItemEquipTypeEnum::VE_FabricPants);
		bool bAccessarries = FMath::IsWithin(UpgradeInfo->EquipType, EItemEquipTypeEnum::VE_Necklace, EItemEquipTypeEnum::VE_Ring);
		if (bWeapon)
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(TEXT("Icon_WeaponEmpty")), EquipmentImage);
		if (bArmor)
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(TEXT("Icon_ArmorEmpty")), EquipmentImage);
		if (bAccessarries)
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(TEXT("Icon_NecklaceEmpty")), EquipmentImage);
	}

	// Text_ItemUpgradeDescription
	EItemGradeEnum NextGrade = (EItemGradeEnum)((int32)CurrentGrade + 1);

	FText DescriptionTemplateText = GradeDescriptionText.GetText();
	FFormatNamedArguments GradeArg;
	GradeArg.Add(TEXT("grade"), GetRichGradeText(NextGrade));
	FText DescriptionText = FText::Format(DescriptionTemplateText, GradeArg);
	if(IsValid(Text_ItemUpgradeDescription))
		Text_ItemUpgradeDescription->SetRichText(DescriptionText);

	// grade texts
	if(IsValid(Text_GradeBefore))
		RefreshTextGrade(Text_GradeBefore, CurrentGrade);
	if(IsValid(Text_GradeAfter))
		RefreshTextGrade(Text_GradeAfter, NextGrade);

	// gold
	int32 OwnedGoldAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GOLD);
	HasSufficientGold = OwnedGoldAmount >= UpgradeInfo->Gold;

	// Button_Enchant and gold needed
	if(IsValid(Button_Upgrade))
	{
		Button_Upgrade->SetGoldAmount(UpgradeInfo->Gold);

		Button_Upgrade->SetIsEnabled(bUpgradePossible);
	}
}

void UUP_ItemUpgrade::SetItemUD(FString InItemUD)
{
	ItemUD = InItemUD;
	InvalidateData();
}

void UUP_ItemUpgrade::InvalidateBasicStats(const TArray<int32>& BasicOptions)
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

void UUP_ItemUpgrade::InvalidateSubstats(const TArray<int32>& BasicOptions)
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

void UUP_ItemUpgrade::OnClick_ShowEquipment()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (!GenericScrollview->isInsideViewport())
		{
			GenericScrollview->SetState(EScrollviewType::VE_ITEM_UPGRADE);
			GenericScrollview->Appear(true);
		}
		else
		{
			GenericScrollview->Appear(false);
		}
		
	}
}




void UUP_ItemUpgrade::RefreshTextGrade(UTextBlock* TextBlockGrade, EItemGradeEnum ItemGrade)
{
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemGrade));
	check(ItemGradeInfo != nullptr);

	TextBlockGrade->SetColorAndOpacity(FLinearColor(ItemGradeInfo->Color));
	TextBlockGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemGradeInfo->UIStringKey));
}

FText UUP_ItemUpgrade::GetRichGradeText(EItemGradeEnum ItemGrade)
{
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemGrade));
	check(ItemGradeInfo != nullptr);

	FText GradeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemGradeInfo->UIStringKey);

	return FText::FromString(FString(TEXT("<span color=\"#")) + ItemGradeInfo->Color.ToHex() + TEXT("\">") + GradeText.ToString() + TEXT("</>"));

}

void UUP_ItemUpgrade::OnButtonUpgradeClicked()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	//check(CanUpgrade());
	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	auto ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	UC_Popup_Item_UpgradeConfirm->SetVisibility(ESlateVisibility::Visible);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		TArray<FString> SelectedUDs;
		for (auto& Element : GenericScrollview->Variables.ScrollView->GetSelectedScrollItemData())
		{
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(Element);
			if (itemdata)
				SelectedUDs.Emplace(itemdata->ItemUD);
		}

		UC_Popup_Item_UpgradeConfirm->SetData(ItemUD, SelectedUDs);
		UP_Popup_Item_Enchant_New->beforeItemCombatPower = FItemTableInfo::GetCombatPower(ItemData);
		UP_Popup_Item_Enchant_New->beforeItemGrade = ItemInfo->ItemGrade;
		RGameInstance->HttpClient->OnInventoryItemUpgrade.AddUObject(this, &UUP_ItemUpgrade::OnInventoryItemUpgradeRp);
	}

	
}

void UUP_ItemUpgrade::OnClickClose()
{
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ItemUpgrade);

	UUP_GenericScrollview* UP_GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (UP_GenericScrollview)
	{
		if (UP_GenericScrollview->isInsideViewport())
		{
			UP_GenericScrollview->Appear(false);
		}
	}
}

void UUP_ItemUpgrade::OnInventoryItemUpgradeRp(bool bSuccess, TArray<FREWARD> Rewards, FString NewItemUD)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemUpgrade.RemoveAll(this);

	if (bSuccess)
	{
		// Show BP UP_Splash_ItemUpgradeResult

		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(NewItemUD);
		check(ItemData != nullptr);
		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		check(ItemInfo != nullptr);

		check(IsValid(UP_Popup_Item_Enchant_New));
		UP_Popup_Item_Enchant_New->ItemUD = NewItemUD;
		UP_Popup_Item_Enchant_New->RefreshPopup();

		if (FItemUpgradeCostInfo::CanUpgrade(ItemInfo->ItemGrade))
		{
			ItemUD = NewItemUD;

			// invalidate itself with the new item.
			// reset selection state
			//InvalidateData();

			UUP_HeroManager* HeroManagerPage = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
			if (HeroManagerPage)
			{
				HeroManagerPage->OnClick_HeroIcon(HeroManagerPage->GetCurrentHeroUD());
			}

			UP_Popup_Item_Enchant_New->inInvalidUpgrade.RemoveDynamic(this, &UUP_ItemUpgrade::OnClickClose);
			UP_Popup_Item_Enchant_New->inInvalidUpgrade.AddDynamic(this, &UUP_ItemUpgrade::OnClickClose);
		}
		else
		{
			// exit
			UP_Popup_Item_Enchant_New->inInvalidUpgrade.RemoveDynamic(this, &UUP_ItemUpgrade::OnClickClose);
			UP_Popup_Item_Enchant_New->inInvalidUpgrade.AddDynamic(this, &UUP_ItemUpgrade::OnClickClose);
			//ClosePage();
		}
	}
	else
	{
		// Show BP UP_Splash_ItemUpgradeResult
		auto Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_ItemUpgradeResult);
		check(IsValid(Splash));
		auto SplashContent = Cast<UUC_ItemUpgradeResult>(Splash->GetContentWidget());
		check(IsValid(SplashContent));

		SplashContent->ItemUD = ItemUD;
		SplashContent->InvalidateData();

		// invalidate itself with the new item.
		// reset selection state

		InvalidateData();
	}
}

