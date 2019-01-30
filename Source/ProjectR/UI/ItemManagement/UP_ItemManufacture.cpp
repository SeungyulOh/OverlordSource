// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemManufacture.h"
#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"

#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/ItemManagement/ItemInvenScrollItemData.h"
#include "UI/ItemManagement/UP_ItemManufacturePopup.h"
#include "UI/ItemManagement/Data/RItemInfo_ScrollData.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Component/CheckBox_Radio.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"

#define LOCTEXT_NAMESPACE
#include "Public/Internationalization/Internationalization.h"

void UUP_ItemManufacture::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();
	
	FindChildWidgetsWithCertainName<UCheckBox_Radio>(this, RadioList, TEXT("Checkbox_"));
	FindChildWidgetsWithCertainName<UPanelWidget>(this, ItemIconPanelList, TEXT("VerticalBox_R"));
	FindChildWidgetsWithCertainName<UImage>(this, NotifyImageList, TEXT("Image_Notify_0"));
	FindChildWidgetsWithCertainName<UImage>(this, PlusImageArray, TEXT("plus_"));

	for (UCheckBox_Radio* radio : RadioList)
	{
		if (IsValid(radio))
		{
			radio->OnCheckStateChanged.RemoveDynamic(this, &UUP_ItemManufacture::RadioStateChanged);
			radio->OnCheckStateChanged.AddDynamic(this, &UUP_ItemManufacture::RadioStateChanged);

			UWidgetSwitcher* switcher = FindChildWidget<UWidgetSwitcher>(radio);
			if (IsValid(switcher))
			{
				radio->Switcher = switcher;
			}
		}
	}

	if (IsValid(CheckBoxRadio_Live))
	{
		CheckBoxRadio_Live->OnCheckStateChanged.RemoveDynamic(this, &UUP_ItemManufacture::SortRadioStateChanged);
		CheckBoxRadio_Live->OnCheckStateChanged.AddDynamic(this, &UUP_ItemManufacture::SortRadioStateChanged);
	}
	if (IsValid(CheckBoxRadio_Dev))
	{
		CheckBoxRadio_Dev->OnCheckStateChanged.RemoveDynamic(this, &UUP_ItemManufacture::SortRadioStateChanged);
		CheckBoxRadio_Dev->OnCheckStateChanged.AddDynamic(this, &UUP_ItemManufacture::SortRadioStateChanged);
	}


	if (IsValid(ItemMenu_btn))
	{
		ItemMenu_btn->SetActiveRadioIndex(0);
	}

	RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(BP_ScrollView_Manufacture))
	{		
		BP_ScrollView_Manufacture->OnScrollViewSelectedItemData.Clear();
		BP_ScrollView_Manufacture->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_ItemManufacture::RecipeScrollViewSelectItem);
	}

	if (IsValid(InventoryScrollView))
	{
		InventoryScrollView->OnScrollViewSelectedItemData.Clear();
		InventoryScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_ItemManufacture::InventoryScrollViewSelectItem);
	}

	if (Button_ItemProcess)
	{
		Button_ItemProcess->OnClicked.RemoveDynamic(this, &UUP_ItemManufacture::OnClick_Process);
		Button_ItemProcess->OnClicked.AddDynamic(this, &UUP_ItemManufacture::OnClick_Process);
	}

	if (Button_OK)
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_ItemManufacture::OnClick_PopupOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_ItemManufacture::OnClick_PopupOK);
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_ItemManufacture::OnClick_PopupCancel);
		Button_Cancel->OnClicked.AddDynamic(this, &UUP_ItemManufacture::OnClick_PopupCancel);
	}

	PrevSelectedItemID = FName("");


	RightType = EItemManufactureRightTypeEnum::VE_RecipeType;
	ChangeLeft(EItemManufactureLeftTypeEnum::VE_WeaponType);

}

void UUP_ItemManufacture::InvalidateData()
{
}

void UUP_ItemManufacture::InvalidateManufactItem(FName name)
{
//	CurManufactureItemName = name;
//
//	bManufactureEnable = false;
//
//	FItemManufatureTableInfo* ManufatureTableInfo = UTableManager::GetInstancePtr()->GetItemManufactureRow(name);
//	if (ManufatureTableInfo)
//	{
//		MaterialCount = 0;
//
//		FName itemID = ManufatureTableInfo->displayedItemID;
//
//		if (IsValid(UC_Item_Icons))
//		{
//			FITEM_ICON_INFO iconinfo;
//			if (UUIFunctionLibrary::GetItemIconInfoWithID(GetWorld(), iconinfo, itemID))
//			{
//				UC_Item_Icons->SetItemIconStructure(iconinfo, true);
//				UC_Item_Icons->InvalidateData();
//			}
//		}
//		
//
//		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemID);
//		if (ItemTableInfo)
//		{
//			if (ScrollView_ItemInfo)
//			{
//				//ScrollView_ItemInfo->ScrollItemList_RemoveAllElement(1);
//				ScrollView_ItemInfo->ClearScrollItemData(1);
//				//URScrollItemData* ScrollItemData = ScrollView_ItemInfo->CreateScrollItemData(URItemInfo_ScrollData::StaticClass());
//				//auto ItemInvenScrollItemData = Cast<URItemInfo_ScrollData>(ScrollItemData);
//				URItemInfo_ScrollData* ItemInvenScrollItemData = Cast<URItemInfo_ScrollData>(ScrollView_ItemInfo->AddScrollItemData(URItemInfo_ScrollData::StaticClass()));
//				ItemInvenScrollItemData->ItemId = itemID;
//				ItemInvenScrollItemData->CurValue_Enchant = 0;
//				ItemInvenScrollItemData->CurLevel_Enchant = 0;
//				ItemInvenScrollItemData->CurLevel_Item = 1;
//				ItemInvenScrollItemData->RandOption = ItemTableInfo->RandomOptionCount;
//
//				int32 NumOption = 0;
//				auto FixedOption = UTableManager::GetInstancePtr()->GetItemFixedOptionRow(ItemTableInfo->ItemGenerateOptionKey);
//				if (FixedOption)
//				{
//					NumOption = FixedOption->OptionsLength;
//					if (NumOption > 0)
//					{
//						ItemInvenScrollItemData->FixedOptions.Reserve(NumOption);
//						ItemInvenScrollItemData->FixedOptionValues.Reserve(NumOption);
//						
//						ItemInvenScrollItemData->FixedOptions.Emplace(FixedOption->Option1);
//						ItemInvenScrollItemData->FixedOptionValues.Emplace(FixedOption->Option1BasicValue);
//
//						if (NumOption > 1)
//						{
//							ItemInvenScrollItemData->FixedOptions.Emplace(FixedOption->Option2);
//							ItemInvenScrollItemData->FixedOptionValues.Emplace(FixedOption->Option2BasicValue);
//						}
//
//						if (NumOption > 2)
//						{
//							ItemInvenScrollItemData->FixedOptions.Emplace(FixedOption->Option3);
//							ItemInvenScrollItemData->FixedOptionValues.Emplace(FixedOption->Option3BasicValue);
//						}
//					}
//				}
//
//				//RandOption
//
//				int32 SizeSkill = ((ItemTableInfo->PassiveSkillOnEquip.IsNone()) ? 75 : 0);
//				int32 SizeSetItem = ((ItemTableInfo->ItemSetKey.IsNone()) ? 200 : 0);
//				int32 SizeOption = ((ItemInvenScrollItemData->RandOption == 0 && NumOption == 0) ? 150 : 0);
//				ItemInvenScrollItemData->WidgetSize.X = 0;
//				ItemInvenScrollItemData->WidgetSize.Y = -(SizeSetItem + SizeSkill + SizeOption);
//				//ScrollView_ItemInfo->SetScrollItemData(0, ItemInvenScrollItemData, nullptr);
//				//ScrollView_ItemInfo->AddItemDataList(ItemInvenScrollItemData);
//				
//				ScrollView_ItemInfo->SetIsCustomItemSize(true);
//				ScrollView_ItemInfo->Init();
//				ScrollView_ItemInfo->SetIsCustomItemSize(false);
//				ScrollView_ItemInfo->InvalidateData();
//			}
//
//			if (IsValid(EnchantInfoPanel))
//			{
//				if (ItemTableInfo->MaxEnchantLevel > 0)
//				{
//					EnchantInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//
//					if (IsValid(EnchantMaxText))
//					{
//						EnchantMaxText->SetText(FText::AsNumber(ItemTableInfo->MaxEnchantLevel));
//					}
//				}
//				else
//				{
//					EnchantInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
//				}
//			}
//
//			FText ItemName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName);
//
//			if (RText_PopupDesc)
//			{
//				FFormatOrderedArguments Args;
//				Args.Emplace(ItemName);
//				FText PopupDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "Item_02");
//				RText_PopupDesc->SetRichText(FText::Format(PopupDesc, Args));
//			}
//
//			if (IsValid(TextBlock_ItemNameText))
//			{
//				TextBlock_ItemNameText->SetText(ItemName);
//				TextBlock_ItemNameText->SetColorAndOpacity(UUtilFunctionLibrary::GetItemGradeColor(GetWorld(), itemID));
//			}
//
//			if (IsValid(ItemTypeText))
//			{
//				ItemTypeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				ItemTypeText->SetText(UResourceFunctionLibrary::GetHeroEquipTypeName(ItemTableInfo->EquipType));
//
//				//FName textKey = UUtilFunctionLibrary::GetItemGradeText(GetWorld(), itemID);
//				//if (!textKey.IsNone())
//				//	ItemTypeText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, textKey));
//
//				//FSlateColor color = UUtilFunctionLibrary::GetItemGradeColor(GetWorld(), itemID);
//				//ItemTypeText->SetColorAndOpacity(color);
//			}
//
//			if (IsValid(WidgetSwitcher_ItemType))
//			{
//				switch (ItemTableInfo->ItemType)
//				{
//				case EItemTypeEnum::VE_EquipItem:
//				{
//					WidgetSwitcher_ItemType->SetActiveWidgetIndex(0);
//				}
//				break;
//				default:
//				{
//					WidgetSwitcher_ItemType->SetActiveWidgetIndex(1);
//
//					if (IsValid(TextBlock_Detail))
//					{
//						TextBlock_Detail->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Description, ItemTableInfo->Description));
//					}
//				}
//				break;
//				}
//			}
//
//			if (IsValid(ItemOptionPanel))
//			{
//				if (ItemTableInfo->ItemOptionCount > 0)
//				{
//					ItemOptionPanel->SetVisibility(ESlateVisibility::Visible);
//
//					if (IsValid(ItemOptionCountText))
//					{
//						ItemOptionCountText->SetText(FText::AsNumber(ItemTableInfo->ItemOptionCount));
//					}
//				}
//				else
//				{
//					ItemOptionPanel->SetVisibility(ESlateVisibility::Collapsed);
//				}
//			}
//
//			if (IsValid(ItemHeroOptionPanel))
//			{
//				if (ItemTableInfo->EquipHero.IsNone())
//				{
//					ItemHeroOptionPanel->SetVisibility(ESlateVisibility::Collapsed);
//				}
//				else
//				{
//					ItemHeroOptionPanel->SetVisibility(ESlateVisibility::Visible);
//				}
//			}
//
//			if (IsValid(ItemSkillPanel))
//			{
//				if (ItemTableInfo->PassiveSkillOnEquip.IsNone())
//				{
//					ItemSkillPanel->SetVisibility(ESlateVisibility::Collapsed);
//				}
//				else
//				{
//					ItemSkillPanel->SetVisibility(ESlateVisibility::Visible);
//				}
//			}
//
//			if (LocalText_Prefix)
//			{
//				if (ItemTableInfo->PassiveSkillOnEquip.IsNone() == false)
//				{
//					auto PassiveTable = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(ItemTableInfo->PassiveSkillOnEquip);
//					auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, PassiveTable->ItemPrefix);
//					LocalText_Prefix->SetText(String);
//					LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				}
//				else if (ItemTableInfo->ItemSetKey.IsNone() == false)
//				{
//					auto ItemSetTable = UTableManager::GetInstancePtr()->GetItemSetEffectRow(ItemTableInfo->ItemSetKey);
//					auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemSetTable->SetPrefix);
//					LocalText_Prefix->SetText(String);
//					LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				}
//				else
//				{
//					LocalText_Prefix->SetVisibility(ESlateVisibility::Collapsed);
//				}
//
//				if (Image_NameDot)
//				{
//					Image_NameDot->SetVisibility(LocalText_Prefix->GetVisibility());
//				}
//			}
//
//			
//			FWorkedItemTableInfo WorkedItemTableInfo;
//			if (IsValid(ItemProperty1FrontText) && IsValid(ItemProperty2FrontText) && IsValid(ItemProperty1BackText) && IsValid(ItemProperty2BackText))
//			{
//				if (URenewal_BattleFunctionLibrary::GetCharacterItemTableWorkedInfo(WorkedItemTableInfo, itemID, 0, 0) == true)
//				{
//					if (IsValid(ItemTypeSwitcher))
//					{
//						ItemTypeSwitcher->SetActiveWidgetIndex(0);
//					}
//
//					if (ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
//					{
//						if(IsValid( ItemProperty) )
//							ItemProperty->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//
//						switch (ItemTableInfo->EquipItemPropertyType)
//						{
//						case EEquipItemPropertyType::VE_PhysicalWeapon:
//						case EEquipItemPropertyType::VE_MagicWeapon:
//						{
//							int32 PhysicalAttackPower = WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption();
//							int32 MagicAttackPower = WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption();
//
//							if (PhysicalAttackPower > 0 && MagicAttackPower <= 0)
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::Hidden);
//								}
//
//								ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "PhysicalAttackPower"));
//								ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption()));
//							}
//							else if (PhysicalAttackPower <= 0 && MagicAttackPower > 0)
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::Hidden);
//								}
//
//								ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicAttackPower"));
//								ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption()));
//							}
//							else
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//								}
//								if (ItemProperty1FrontText)
//								{
//									ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "PhysicalAttackPower"));
//								}
//								if (ItemProperty1BackText)
//								{
//									ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption()));
//								}
//
//								if (ItemProperty2FrontText)
//								{
//									ItemProperty2FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicAttackPower"));
//								}
//								
//								if (ItemProperty2BackText)
//								{
//									ItemProperty2BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption()));
//								}
//							}
//						}
//						break;
//
//						case EEquipItemPropertyType::VE_PhysicalGuard:
//						case EEquipItemPropertyType::VE_MagicGuard:
//						{
//							int32 PhysicalGuard = WorkedItemTableInfo.GetEnchantArmorNotAddOption();
//							int32 MagicGuard = WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption();
//
//							if (PhysicalGuard > 0 && MagicGuard <= 0)
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::Hidden);
//								}
//
//								ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "Armor"));
//								ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantArmorNotAddOption()));
//							}
//							else if (PhysicalGuard <= 0 && MagicGuard > 0)
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::Hidden);
//								}
//
//								ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicResistance"));
//								ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption()));
//							}
//							else
//							{
//								if (ItemProperty2Panel)
//								{
//									ItemProperty2Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//								}
//
//								ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "Armor"));
//								if (ItemProperty2FrontText)
//								{
//									ItemProperty2FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicResistance"));
//								}
//								ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantArmorNotAddOption()));
//								if (ItemProperty2BackText)
//								{
//									ItemProperty2BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption()));
//								}
//							}
//						}
//						break;
//
//						case EEquipItemPropertyType::VE_HPGuard:
//						{
//							if (ItemProperty2Panel)
//							{
//								ItemProperty2Panel->SetVisibility(ESlateVisibility::Hidden);
//							}
//
//							ItemProperty1FrontText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MaxHP"));
//							ItemProperty1BackText->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMaxHPNotAddOption()));
//						}
//						break;
//						}
//
//						if (IsValid(ItemTypeSwitcher))
//						{
//							ItemTypeSwitcher->SetActiveWidgetIndex(0);
//
//							if (!WorkedItemTableInfo.Data.EquipHero.IsNone())
//							{
//								FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(WorkedItemTableInfo.Data.EquipHero);
//								if (CharacterTableInfo)
//								{
//									ItemTypeSwitcher->SetActiveWidgetIndex(1);
//
//									if (IsValid(ItemWeaponTypeText))
//									{
//										ItemWeaponTypeText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
//									}
//								}
//							}
//						}
//					}
//					else
//					{
//						if (IsValid(ItemProperty))
//							ItemProperty->SetVisibility(ESlateVisibility::Collapsed);
//					}
//				}
//			}
//		}
//
//		if (IsValid(BP_Text_NeedEnchantGold))
//		{			
//			//ItemPriceText->SetText(FText::Format(FText::FromString("{0:0.0}"), FText::AsNumber(ManufatureTableInfo->GoldCost)));
//			BP_Text_NeedEnchantGold->SetText(FText::AsNumber(ManufatureTableInfo->GoldCost));
//		}
//
//		for (UPanelWidget* panel : ItemIconPanelList)
//		{
//			if (IsValid(panel))
//			{
//				panel->SetVisibility(ESlateVisibility::Collapsed);
//			}
//		}
//
//		int32 haveMaterialCount = 0;
//		if (!ManufatureTableInfo->material1.IsNone())
//		{
//			if (InvalidateMaterial(ManufatureTableInfo->material1, 0, ManufatureTableInfo->material1Count))
//				++haveMaterialCount;
//
//			++MaterialCount;
//		}
//		
//		if (!ManufatureTableInfo->material2.IsNone())
//		{
//			if (InvalidateMaterial(ManufatureTableInfo->material2, 1, ManufatureTableInfo->material2Count))
//				++haveMaterialCount;
//
//			++MaterialCount;
//		}
//
//		if (!ManufatureTableInfo->material3.IsNone())
//		{
//			if (InvalidateMaterial(ManufatureTableInfo->material3, 2, ManufatureTableInfo->material3Count))
//				++haveMaterialCount;
//
//			++MaterialCount;
//		}
//
//		//if (!ManufatureTableInfo->material4.IsNone())
//		//{
//		//	if (InvalidateMaterial(ManufatureTableInfo->material4, 3, ManufatureTableInfo->material4Count))
//		//		++haveMaterialCount;
//
//		//	++MaterialCount;
//		//}
//
//		if (!ManufatureTableInfo->targetHero.IsNone())
//		{
//			FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ManufatureTableInfo->targetHero);
//			if (TableInfo)
//			{
//				if (InvalidateRune(ManufatureTableInfo->targetHero, ManufatureTableInfo->runeCount))
//					++haveMaterialCount;
//
//				++MaterialCount;
//			}
//		}
//
//		if ( IsValid(RGameInstance->UserInfo))
//		{
//			FailCause = EItemManufactureCause::VE_None;
//			FHERO* outData = RGameInstance->RInventory->GetHeroWithID(ManufatureTableInfo->targetHero);
//			if (outData != nullptr)
//			{
//				if (outData->level < ManufatureTableInfo->craftableAccountLevel)
//					//if (RGameInstance->UserInfo->Avatar.level < ManufatureTableInfo->craftableAccountLevel)
//				{
//					bManufactureEnable = false;
//					FailCause = EItemManufactureCause::VE_Level;
//					ShortageLevel = ManufatureTableInfo->craftableAccountLevel;
//
//					if (IsValid(BP_Craft_Disabled))
//					{
//						BP_Craft_Disabled->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//					}
//
//					if (IsValid(TextBlock_Level))
//					{
//						TextBlock_Level->SetText(FText::AsNumber(ManufatureTableInfo->craftableAccountLevel));
//					}
//				}
//			}
//
//			if (FailCause == EItemManufactureCause::VE_None) //todo
//			{
//				if (RGameInstance->RInventory->GetGold() >= ManufatureTableInfo->GoldCost)
//				{
//					if (haveMaterialCount == MaterialCount)
//					{
//						bManufactureEnable = true;
//						FailCause = EItemManufactureCause::VE_None;
//					}
//					else
//					{
//						FailCause = EItemManufactureCause::VE_Material;
//					}
//				}
//				else
//				{
//					ShortageValue = ManufatureTableInfo->GoldCost - RGameInstance->RInventory->GetGold();
//					FailCause = EItemManufactureCause::VE_Gold;
//				}
//			}
//
//			if (IsValid(BP_Craft_Disabled))
//			{
//				BP_Craft_Disabled->SetVisibility(ESlateVisibility::Collapsed);
//			}
//
//			
//		}
//	}
//
//	/*if (IsValid(ItemManufactureBtn))
//	{
//		ItemManufactureBtn->SetIsEnabled(bManufactureEnable);
//	}*/
//
//	OnInvalidateManufactItem();
}

void UUP_ItemManufacture::SelectItemUI(FName ItemID)
{
	URScrollItemData* SeekItem = nullptr;
	for (URScrollItemData* ItemData : BP_ScrollView_Manufacture->ItemDataList)
	{
		UItemInvenScrollItemData* ItemInvenData = Cast<UItemInvenScrollItemData>(ItemData);
		if (IsValid(ItemInvenData))
		{
			if (!ItemInvenData->RecipeId.IsNone() && ItemInvenData->RecipeId == ItemID)
			{
				SeekItem = ItemData;
				break;
			}
		}		
	}

	if (IsValid(SeekItem))
	{
		BP_ScrollView_Manufacture->InvalidateData();
		BP_ScrollView_Manufacture->SetSelectScrollListData(SeekItem);		
	}
}

bool UUP_ItemManufacture::InvalidateRune(FName id, int32 count)
{
	int32 idx = 3;
	if (ItemIconPanelList.IsValidIndex(idx))
	{
		UPanelWidget* panel = ItemIconPanelList[idx];
		if (IsValid(panel))
		{
			panel->SetVisibility(ESlateVisibility::Visible);

			UUC_Item_Icons* itemIcon = FindChildWidget<UUC_Item_Icons>(panel);
			if (IsValid(itemIcon))
			{
				FITEM_ICON_INFO iconinfo;
				if (UUIFunctionLibrary::GetRuneIconByID(GetWorld(),id, iconinfo))
				{
					itemIcon->SetItemIconStructure(iconinfo, false);
					itemIcon->InvalidateData();
				}
			}

			int32 CurCount = UUtilFunctionLibrary::GetCurrentHaveRuneCount(GetWorld(), id);

			UTextBlock* frontText = FindChildWidget<UTextBlock>(panel, TEXT("Text_CurrentMaterial"));
			if (IsValid(frontText))
			{
				frontText->SetText(FText::AsNumber(CurCount));
			}

			UTextBlock* backText = FindChildWidget<UTextBlock>(panel, TEXT("Text_NeedMaterial"));
			if (IsValid(backText))
			{
				backText->SetText(FText::AsNumber(count));
			}

			if (CurCount >= count)
				return true;
		}
	}

	return false;
}

bool UUP_ItemManufacture::InvalidateMaterial(FName name, int32 idx, int32 count)
{
	if (ItemIconPanelList.IsValidIndex(idx))
	{
		UPanelWidget* panel = ItemIconPanelList[idx];
		if (IsValid(panel))
		{
			panel->SetVisibility(ESlateVisibility::Visible);

			UUC_Item_Icons* itemIcon = FindChildWidget<UUC_Item_Icons>(panel);
			if (IsValid(itemIcon))
			{
				FITEM_ICON_INFO iconinfo;
				if (UUIFunctionLibrary::GetItemIconInfoWithID(iconinfo, name))
				{
					itemIcon->SetItemIconStructure(iconinfo);
					itemIcon->InvalidateData();
				}
			}

			int32 CurCount = UUtilFunctionLibrary::GetCurrentHaveItemCount(GetWorld(), name);

			UTextBlock* frontText = FindChildWidget<UTextBlock>(panel, TEXT("Text_CurrentMaterial"));
			if (IsValid(frontText))
			{
				frontText->SetText(FText::AsNumber(CurCount));
			}

			UTextBlock* backText = FindChildWidget<UTextBlock>(panel, TEXT("Text_NeedMaterial"));
			if (IsValid(backText))
			{
				backText->SetText(FText::AsNumber(count));
			}

			if (CurCount >= count)
				return true;
		}
	}

	return false;
}

void UUP_ItemManufacture::ChangeRight(EItemManufactureRightTypeEnum right)
{
	if (RightType == right)
		return;

	RightType = right;

	InvalidateRight();
}

void UUP_ItemManufacture::ChangeRightItem(bool bAll)
{
	bAllItemShow = bAll;

	ChangeLeft(LeftType);
}

void UUP_ItemManufacture::ChangeLeft(EItemManufactureLeftTypeEnum left)
{
	if (LeftType == left)
	{
		InvalidateRight();
		UpdateDetail();
		return;
	}
		

	PrevSelectedItemID = FName("");

	LeftType = left;

	RightType = EItemManufactureRightTypeEnum::VE_RecipeType;

	InvalidateRight();
	UpdateDetail();
}

void UUP_ItemManufacture::UpdateDetail()
{
	if (RightType == EItemManufactureRightTypeEnum::VE_RecipeType)
	{
		bool bDetail = true;
		switch (LeftType)
		{
		case EItemManufactureLeftTypeEnum::VE_WeaponType:
		{
			if (WeaponManufactCount == 0)
				bDetail = false;
		}
		break;
		case EItemManufactureLeftTypeEnum::VE_ArmorType:
		{
			if (ArmorManufactCount == 0)
				bDetail = false;
		}
		break;
		case EItemManufactureLeftTypeEnum::VE_MagicArmorType:
		{
			if (MagicArmorManufactCount == 0)
				bDetail = false;
		}
		break;
		case EItemManufactureLeftTypeEnum::VE_Accessories:
		{
			if (RingManufactCount == 0)
				bDetail = false;
		}
		break;
		case EItemManufactureLeftTypeEnum::VE_Expendables:
		{
			if (MaterialManufactCount == 0)
				bDetail = false;
		}
		break;
		}

		if (IsValid(WidgetSwitcher_Detail))
		{
			WidgetSwitcher_Detail->SetActiveWidgetIndex(bDetail ? 0 : 1);
		}
	}
}

void UUP_ItemManufacture::InvalidateRight()
{
	switch (RightType)
	{
	case EItemManufactureRightTypeEnum::VE_RecipeType:
	{
		WeaponManufactCount = 0;
		ArmorManufactCount = 0;
		MagicArmorManufactCount = 0;
		RingManufactCount = 0;
		MaterialManufactCount = 0;

		int32 WeaponManufactEnableCount = 0;
		int32 ArmorManufactEnableCount = 0;
		int32 MagicArmorManufactEnableCount = 0;
		int32 RingManufactEnableCount = 0;
		int32 MaterialManufactEnableCount = 0;

		if (IsValid(InventoryGroup))
		{
			InventoryGroup->SetActiveRadioIndex(0);
		}

		if (IsValid(InventorySwitcher))
		{
			InventorySwitcher->SetActiveWidgetIndex(1);
		}

		if (IsValid(BP_ScrollView_Manufacture))
		{
			BP_ScrollView_Manufacture->ItemDataList.Reset();

			FName selectCharacterID;

			//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			TArray<FName> stringKeyList = UTableManager::GetInstancePtr()->GetItemManufactureRowNames();
			for (FName& stringKey : stringKeyList)
			{
				FItemManufatureTableInfo* ItemManufatureTableInfo = UTableManager::GetInstancePtr()->GetItemManufactureRow(stringKey);
				if (ItemManufatureTableInfo)
				{
					int32 EnableCount = UUtilFunctionLibrary::GetItemManufactureEnableCount(GetWorld(), stringKey);

					bool bExe = false;
					bool bCheck = true;
					FName itemId = ItemManufatureTableInfo->displayedItemID;

					//FString strItemID = itemId.ToString();
					//if (strItemID.Find(TEXT("NEC")) != -1)
					//{
					//	int32 a;
					//	a = 10;
					//}

					if(!ItemManufatureTableInfo->IsShow)
						continue;

					
// 					
// 					{
// 						if (!ItemManufatureTableInfo->targetHero.IsNone())
// 						{
// 							FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemManufatureTableInfo->targetHero);
// 							if (CharacterTableInfo)
// 							{
// 								if (IsValid(RGameInstance->HeroInventory))
// 								{
// 									FHERO* outData = RGameInstance->HeroInventory->GetInventoryDataWithID(ItemManufatureTableInfo->targetHero);
// 									if (outData != nullptr)
// 									{
// 										if (!outData->activated)
// 										{
// 											continue;
// 										}
// 										else
// 										{
// 											if (!selectCharacterID.IsNone())
// 											{
// 												if (selectCharacterID != ItemManufatureTableInfo->targetHero)
// 													continue;
// 											}
// 
// 											//if (ItemManufatureTableInfo->craftableHeroLevel > 0)
// 											//{
// 											//	if (outData.level < ItemManufatureTableInfo->craftableHeroLevel)
// 											//	{
// 											//		continue;
// 											//	}
// 											//}
// 										}
// 									}
// 									else
// 									{
// 										continue;
// 									}
// 								}
// 							}
// 						}
// 
// 
// 						if (IsValid(RGameInstance->UserInfo))
// 						{
// 							if (ItemManufatureTableInfo->craftableAccountLevel > 0)
// 							{
// 								FHERO* outData = RGameInstance->HeroInventory->GetInventoryDataWithID(ItemManufatureTableInfo->targetHero);
// 								if (outData != nullptr)
// 								{
// 									if (outData->level < ItemManufatureTableInfo->craftableAccountLevel)
// 									//if (RGameInstance->UserInfo->Avatar.level < ItemManufatureTableInfo->craftableAccountLevel)
// 									{
// 										if (bAllItemShow)
// 										{
// 											bCheck = false;
// 										}
// 										else
// 											continue;
// 									}
// 								}
// 							}
// 						}
// 					}

					EItemGradeEnum	itemGrade = EItemGradeEnum::Normal;
					FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(selectCharacterID);
					FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemId);
					if (ItemTableInfo)
					{
						itemGrade = ItemTableInfo->ItemGrade;

						switch (ItemTableInfo->ItemType)
						{
						case EItemTypeEnum::VE_EquipItem:
						{
							switch (ItemTableInfo->EquipPosition)
							{
							case EItemEquipPositionEnum::VE_Weapon:
							{
								if (CharacterTableInfo1)
								{
									if (CharacterTableInfo1->EquipType == ItemTableInfo->EquipType)
									{
										if (LeftType == EItemManufactureLeftTypeEnum::VE_WeaponType)
										{
											bExe = true;
										}

										if (bCheck)
										{
											++WeaponManufactCount;

											if (EnableCount > 0)
											{
												++WeaponManufactEnableCount;
											}
										}
									}
								}
							}
							break;
							case EItemEquipPositionEnum::VE_Armor:
							{
								if (CharacterTableInfo1)
								{									
									if (UUtilFunctionLibrary::IsSameType(CharacterTableInfo1->EquipArmorType, ItemTableInfo->EquipType))
									{
										if (LeftType == EItemManufactureLeftTypeEnum::VE_ArmorType)
										{
											bExe = true;
										}

										if (bCheck)
										{
											++ArmorManufactCount;

											if (EnableCount > 0)
											{
												++ArmorManufactEnableCount;
											}
										}
									}
								}
							}
							break;
							case EItemEquipPositionEnum::VE_Pants:
							{
								if (CharacterTableInfo1)
								{
									if (UUtilFunctionLibrary::IsSameType(CharacterTableInfo1->EquipArmorType, ItemTableInfo->EquipType))
									{
										if (LeftType == EItemManufactureLeftTypeEnum::VE_MagicArmorType)
										{
											bExe = true;
										}

										if (bCheck)
										{
											++MagicArmorManufactCount;

											if (EnableCount > 0)
											{
												++MagicArmorManufactEnableCount;
											}
										}
									}
								}
							}
							break;
							case EItemEquipPositionEnum::VE_Ring:
							{
								if (LeftType == EItemManufactureLeftTypeEnum::VE_Accessories)
								{
									bExe = true;
								}

								if (bCheck)
								{
									++RingManufactCount;

									if (EnableCount > 0)
									{
										++RingManufactEnableCount;
									}
								}
							}
							break;
							}
						}
						break;
						case EItemTypeEnum::VE_MaterialItem:
						{
							if (LeftType == EItemManufactureLeftTypeEnum::VE_Expendables)
							{
								bExe = true;
							}

							if (bCheck)
							{
								++MaterialManufactCount;

								if (EnableCount > 0)
								{
									++MaterialManufactEnableCount;
								}
							}
						}
						break;
						}
					}

					if(!bExe)
						continue;

					UItemInvenScrollItemData* ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(BP_ScrollView_Manufacture->AddScrollItemData(UItemInvenScrollItemData::StaticClass()));
					if (IsValid(ItemInvenScrollItemData))
					{
						FITEM_ICON_INFO iconinfo;
						if (UUIFunctionLibrary::GetItemIconInfoWithID(iconinfo, itemId))
						{
							ItemInvenScrollItemData->ItemIconInfo = iconinfo;
						}

						ItemInvenScrollItemData->bRecipeItem = true;
						ItemInvenScrollItemData->RecipeId = stringKey;
						ItemInvenScrollItemData->ItemGrade = itemGrade;
						ItemInvenScrollItemData->CraftableAccountLevel = ItemManufatureTableInfo->craftableAccountLevel;
						ItemInvenScrollItemData->RGameInstance = RGameInstance;
					}
				}
			}

			BP_ScrollView_Manufacture->ItemDataList.Sort(
				[](URScrollItemData& A, URScrollItemData& B)
			{
				UItemInvenScrollItemData& TargetA = (UItemInvenScrollItemData&)A;
				UItemInvenScrollItemData& TargetB = (UItemInvenScrollItemData&)B;

				int32 ACompare = GetSortNumber(TargetA);
				int32 BCompare = GetSortNumber(TargetB);

				return ACompare > BCompare;
			}
			);
					
			BP_ScrollView_Manufacture->Init();
			BP_ScrollView_Manufacture->InvalidateData();
			if (!PrevSelectedItemID.IsNone())
			{
				SelectItemUI(PrevSelectedItemID);
			}
			else
			{
				BP_ScrollView_Manufacture->SetSelectScrollListIndex(0);
			}

			for (int32 i = 0; i < NotifyImageList.Num(); ++i)
			{
				UImage* notifyImage = NotifyImageList[i];
				if (IsValid(notifyImage))
				{
					switch ((EItemManufactureLeftTypeEnum)i)
					{
					case EItemManufactureLeftTypeEnum::VE_WeaponType:
					{
						if (WeaponManufactEnableCount > 0)
						{
							notifyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
						else
						{
							notifyImage->SetVisibility(ESlateVisibility::Collapsed);
						}
					}
					break;
					case EItemManufactureLeftTypeEnum::VE_ArmorType:
					{
						if (ArmorManufactEnableCount > 0)
						{
							notifyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
						else
						{
							notifyImage->SetVisibility(ESlateVisibility::Collapsed);
						}
					}
					break;
					case EItemManufactureLeftTypeEnum::VE_MagicArmorType:
					{
						if (MagicArmorManufactEnableCount > 0)
						{
							notifyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
						else
						{
							notifyImage->SetVisibility(ESlateVisibility::Collapsed);
						}
					}
					break;
					case EItemManufactureLeftTypeEnum::VE_Accessories:
					{
						if (RingManufactEnableCount > 0)
						{
							notifyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
						else
						{
							notifyImage->SetVisibility(ESlateVisibility::Collapsed);
						}
					}
					break;
					case EItemManufactureLeftTypeEnum::VE_Expendables:
					{
						if (MaterialManufactEnableCount > 0)
						{
							notifyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
						}
						else
						{
							notifyImage->SetVisibility(ESlateVisibility::Collapsed);
						}
					}
					break;
					}
				}
			}

			OnInvalidateRightRadioText();
		}
	}
	break;
	case EItemManufactureRightTypeEnum::VE_InvenType:
	{
		if (IsValid(InventoryGroup))
		{
			InventoryGroup->SetActiveRadioIndex(1);
		}

		if (IsValid(InventorySwitcher))
		{
			InventorySwitcher->SetActiveWidgetIndex(0);
		}

		if (IsValid(InventoryScrollView))
		{
			InventoryScrollView->ItemDataList.Empty();

			//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->RInventory))
			{
				TArray<FString> keyList;
				RGameInstance->RInventory->GetItemKeys(keyList);
				if (IsValid(InvenCountText))
				{
					InvenCountText->SetText(FText::AsNumber(keyList.Num()));
				}

				if (IsValid(InvenCountMaxText))
				{
					InvenCountMaxText->SetText(FText::AsNumber(200));
				}

				TArray<FString> itemUDList;
				//RGameInstance->RInventory->GetItemInvenKeys(itemUDList, EItemInventoryTypeEnum::VE_Manufacture, EItemInvenTypeEnum::VE_ConsumableItem);

				for (auto& ItemUDElem : itemUDList)
				{
					auto ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(InventoryScrollView->AddScrollItemData(UItemInvenScrollItemData::StaticClass()));
					if (!IsValid(ItemInvenScrollItemData))
					{
						continue;
					}

					UUIFunctionLibrary::GetItemIconInfoWithUD(ItemInvenScrollItemData->ItemIconInfo, ItemUDElem);
				}
			}

			InventoryScrollView->Init();
			InventoryScrollView->InvalidateData();
		}
	}
	break;
	}
}

int32 UUP_ItemManufacture::GetSortNumber(UItemInvenScrollItemData& InData)
{
	int32 Compare = 0;

	//FItemManufatureTableInfo* ManuTableInfo = UTableManager::GetInstancePtr()->GetItemManufactureRow(InData.RecipeId);
	//if (ManuTableInfo)
	//{
	//	Compare += ManuTableInfo->craftableAccountLevel * 1000;
	//	Compare += (int32)InData.ItemGrade * 100;

	//	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ManuTableInfo->displayedItemID);
	//	if (ItemTableInfo)
	//	{
	//		Compare += ItemTableInfo->itemQuality;

	//		if (!ItemTableInfo->EquipHero.IsNone())
	//		{
	//			Compare += 30;
	//		}

	//		switch (ItemTableInfo->ItemType)
	//		{
	//		case EItemTypeEnum::VE_EquipItem:
	//		{
	//			switch (ItemTableInfo->EquipPosition)
	//			{
	//			case EItemEquipPositionEnum::VE_Armor:
	//			case EItemEquipPositionEnum::VE_Pants:
	//			{
	//				if (UUIFunctionLibrary::IsEquipTypeArmor(ItemTableInfo->EquipType))
	//				{
	//					Compare += 20;
	//				}
	//			}
	//			break;
	//			}
	//		}
	//		break;
	//		}
	//	}
	//}

	return Compare;
}

void UUP_ItemManufacture::UpdatePlusImage(int32 _count)
{
	for (size_t i = 0; i < PlusImageArray.Num(); ++i)
	{
		if(_count - 2 == i)
			PlusImageArray[i]->SetVisibility(ESlateVisibility::Visible);
		else
			PlusImageArray[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}

void UUP_ItemManufacture::InventoryScrollViewSelectItem(URScrollItemData* SelectItem)
{
	if (IsValid(SelectItem) && IsValid(WidgetSwitcher_Detail) )
	{
		if(SelectItem->bSelected)
			WidgetSwitcher_Detail->SetActiveWidgetIndex(0);
		else
			WidgetSwitcher_Detail->SetActiveWidgetIndex(1);
	}

// 	UItemInvenScrollItemData* ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(SelectItem);
// 	if (IsValid(ItemInvenScrollItemData))
// 	{
// 
// 	}
}

void UUP_ItemManufacture::RecipeScrollViewSelectItem(URScrollItemData* SelectItem)
{
	if (IsValid(SelectItem) && IsValid(WidgetSwitcher_Detail))
	{
		if (SelectItem->bSelected)
			WidgetSwitcher_Detail->SetActiveWidgetIndex(0);
		else
			WidgetSwitcher_Detail->SetActiveWidgetIndex(1);
	}

	UItemInvenScrollItemData* ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(SelectItem);
	if (IsValid(ItemInvenScrollItemData))
	{
		PrevSelectedItemID = ItemInvenScrollItemData->RecipeId;
		InvalidateManufactItem(ItemInvenScrollItemData->RecipeId);		
	}
}

void UUP_ItemManufacture::ChangeHero(FString InHeroUD)
{
	PrevSelectedItemID = TEXT("");
	InvalidateRight();
}

void UUP_ItemManufacture::OnClick_ExcuteManufacture()
{
// 	if (!bManufactureEnable)
// 	{
// 		switch (FailCause)
// 		{
// 			case EItemManufactureCause::VE_Material:
// 			{
// 				auto ShortagePopup = RGameInstance->RWidgetManager->ShowCurrencyShortagePopup(
// 					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Item_03")),
// 					EREWARD_TYPE::ITEM,
// 					0);
// 				ShortagePopup->SetHideCurrency();
// 			}
// 			break;
// 			case EItemManufactureCause::VE_Gold:
// 			{
// 				auto DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroManage_Notify2");
// 				auto DescSubText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Result_gold");
// 				FFormatOrderedArguments Args;
// 				Args.Emplace(DescSubText);
// 				DescText = FText::Format(DescText, Args);
// 
// 				UUIFunctionLibrary::ShowCurrencyShortagePopup(GetWorld(),
// 					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 					DescText,
// 					EREWARD_TYPE::GOLD,
// 					ShortageValue);
// 			}
// 			break;
// 		}
// 
// 
// 		return;
// 	}
// 
// 	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (!RGameInstance->HttpClient->IsValidNetwork())
// 		return;
// 
// 	RGameInstance->HttpClient->OnItemManufactureRpDelegate.Clear();
// 	RGameInstance->HttpClient->OnItemManufactureRpDelegate.AddDynamic(this, &UUP_ItemManufacture::ItemManufactureRp);
// 
// 	if (!CurManufactureItemName.IsNone())
// 	{
// 		UPacketFunctionLibrary::ITEM_MANUFATURE_RQ(this, CurManufactureItemName.ToString(), EIS_CHECKED::CHECKED, 1);
// 	}
}

void UUP_ItemManufacture::OnClick_Shop()
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Shop);
	}
}

void UUP_ItemManufacture::OnInvalidateManufactItem()
{
	UpdatePlusImage(MaterialCount);
}

void UUP_ItemManufacture::ItemManufactureRp(ECRUD CRUD, FITEM ManufacturedItem)
{
	if (CRUD == ECRUD::CRUD_OK)
	{
		if (ManufacturedItem.itemId.IsNone())
		{
			UE_LOG(LogUI, Log, TEXT("ManufacturedItem ID is None"));
			return;
		}

		//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RWidgetManager))
		{
			RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemCraftResult);

			UUserWidget* widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemCraftResult);
			UUP_ItemManufacturePopup* popupWidget = Cast<UUP_ItemManufacturePopup>(widget);
			if (IsValid(popupWidget))
			{
				popupWidget->SetItem(ManufacturedItem);
			}
		}
		
		InvalidateManufactItem(CurManufactureItemName);

		if (IsValid(BP_ScrollView_Manufacture))
		{
			BP_ScrollView_Manufacture->Refresh();
		}
	}
}

void UUP_ItemManufacture::OnClick_Process()
{
	if (FailCause == EItemManufactureCause::VE_Level)
	{
		FFormatOrderedArguments Args;
		Args.Emplace(ShortageLevel);
		FText LevelDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "Item_01");
		
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			FText::Format(LevelDesc, Args));
		return;
	}

	if (Panel_CraftPopup)
	{
		Panel_CraftPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


void UUP_ItemManufacture::OnClick_PopupCancel()
{
	if (Panel_CraftPopup)
	{
		Panel_CraftPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_ItemManufacture::OnClick_PopupOK()
{
	OnClick_ExcuteManufacture();
	OnClick_PopupCancel();
}

void UUP_ItemManufacture::RadioStateChanged(bool isChecked)
{
	for (size_t i =0;i < RadioList.Num(); ++i)
	{
		if (RadioList[i]->IsChecked() && isChecked)
		{
			ChangeLeft(EItemManufactureLeftTypeEnum(i));
			break;
		}
	}
}

void UUP_ItemManufacture::SortRadioStateChanged(bool isChecked)
{
	if (CheckBoxRadio_Live->IsChecked() && isChecked)
		ChangeRightItem(true);
	if (CheckBoxRadio_Dev->IsChecked() && isChecked)
		ChangeRightItem(false);
}
