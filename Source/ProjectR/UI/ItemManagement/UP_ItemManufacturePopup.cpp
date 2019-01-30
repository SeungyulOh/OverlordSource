// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemManufacturePopup.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"

#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Table/SkillTableInfo_V2.h"

#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"



void UUP_ItemManufacturePopup::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	PlayAni(TEXT("FlareAni"),false,-1);

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUP_ItemManufacturePopup::OnClickOK);
		Button_Click->OnClicked.AddDynamic(this, &UUP_ItemManufacturePopup::OnClickOK);
	}
}

void UUP_ItemManufacturePopup::NativeDestruct()
{
	
	Super::NativeDestruct();
}


void UUP_ItemManufacturePopup::SetItem(FITEM item)
{
	// TODO: fixedOptions
// 	this->PlayAni(TEXT("FlareAni"));
// 	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(item.itemId);
// 	if (ItemTableInfo)
// 	{
// 		if (IsValid(EnchantInfoPanel))
// 		{
// 			if (ItemTableInfo->MaxEnchantLevel > 0)
// 			{
// 				EnchantInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 				if (IsValid(EnchantCurText) && IsValid(EnchantMaxText))
// 				{
// 					EnchantCurText->SetText(FText::AsNumber(item.enchant));
// 					EnchantMaxText->SetText(FText::AsNumber(ItemTableInfo->MaxEnchantLevel));
// 
// 					URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 					if ( IsValid(RGameInstance->GameDataManager))
// 					{
// 						EnchantCurText->SetColorAndOpacity(RGameInstance->GameDataManager->GetItemEnchantLevelColor(item.enchant));
// 					}
// 
// 					if (IsValid(EnchantInfoSwitcher))
// 					{
// 						if (item.enchant == ItemTableInfo->MaxEnchantLevel)
// 						{
// 							EnchantInfoSwitcher->SetActiveWidgetIndex(1);
// 						}
// 						else
// 						{
// 							EnchantInfoSwitcher->SetActiveWidgetIndex(0);
// 						}
// 					}
// 				}
// 			}
// 			else
// 			{
// 				EnchantInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}
// 		
// 		if (IsValid(ItemIcon))
// 		{
// 			FITEM_ICON_INFO iconinfo;
// 			UUIFunctionLibrary::GetItemIconInfoWithID(GetWorld(), iconinfo, item.itemId);
// 			
// 			ItemIcon->SetItemIconStructure(iconinfo, false);
// 			ItemIcon->SetVisibleEquipInfo(true);
// 			//ItemIcon->PlayAnimTrack(TEXT("Enchant_Level"));
// 			ItemIcon->InvalidateData();
// 			ItemIcon->PlayAni(TEXT("Enchant_Level"));
// 			
// 		}
// 
// 		if (LocalText_Prefix)
// 		{
// 			if (ItemTableInfo->PassiveSkillOnEquip.IsNone() == false)
// 			{
// 				auto PassiveTable = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(ItemTableInfo->PassiveSkillOnEquip);
// 				auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, PassiveTable->ItemPrefix);
// 				LocalText_Prefix->SetText(String);
// 				LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 			}
// 			else if (ItemTableInfo->ItemSetKey.IsNone() == false)
// 			{
// 				auto ItemSetTable = UTableManager::GetInstancePtr()->GetItemSetEffectRow(ItemTableInfo->ItemSetKey);
// 				auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemSetTable->SetPrefix);
// 				LocalText_Prefix->SetText(String);
// 				LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 			}
// 			else
// 			{
// 				LocalText_Prefix->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 
// 			/*if (Image_NameDot)
// 			{
// 				Image_NameDot->SetVisibility(LocalText_Prefix->GetVisibility());
// 			}*/
// 		}
// 
// 		/*if (Panel_PassiveSkill)
// 		{
// 			if (ItemTableInfo->PassiveSkillOnEquip.IsNone())
// 			{
// 				Panel_PassiveSkill->SetVisibility(ESlateVisibility::Collapsed);
// 
// 				if (LocalText_Prefix)
// 				{
// 					LocalText_Prefix->SetVisibility(ESlateVisibility::Collapsed);
// 				}
// 			}
// 			else
// 			{
// 				Panel_PassiveSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 				auto PassiveTable = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(ItemTableInfo->PassiveSkillOnEquip);
// 				if (LocalText_Prefix)
// 				{
// 					auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, PassiveTable->ItemPrefix);
// 					LocalText_Prefix->SetText(String);
// 					LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 				}
// 
// 				if (Panel_SkillCoolTime)
// 				{
// 					if (PassiveTable->CoolTime > 1)
// 					{
// 						Panel_SkillCoolTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 						if (Text_SkillCoolTime)
// 						{
// 							Text_SkillCoolTime->SetText(FText::AsNumber(PassiveTable->CoolTime));
// 						}
// 					}
// 					else
// 					{
// 						Panel_SkillCoolTime->SetVisibility(ESlateVisibility::Collapsed);
// 					}
// 				}
// 
// 				if (Text_PassiveSkillDesc)
// 				{
// 					Text_PassiveSkillDesc->SetText(UUIFunctionLibrary::GetItemPassiveSkillDescText(ItemTableInfo->PassiveSkillOnEquip));
// 				}
// 			}
// 		}*/
// 
// 		if (RText_PopupDesc)
// 		{
// 			FText ItemName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName);
// 			FFormatOrderedArguments Args;
// 			Args.Emplace(ItemName);
// 			FText PopupDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "Item_04");
// 			RText_PopupDesc->SetRichText(FText::Format(PopupDesc, Args));
// 		}
// 
// 		/*UUC_Item_Icons* itemIcon = FindChildWidget<UUC_Item_Icons>(RootPanel);
// 		if (IsValid(itemIcon))
// 		{
// 			FITEM_ICON_INFO iconinfo;
// 			if (UUIFunctionLibrary::GetItemIconInfoWithID(GetWorld(), iconinfo, item.itemId))
// 			{
// 				itemIcon->SetItemIconStructure(iconinfo);
// 				itemIcon->InvalidateData();
// 			}
// 		}*/
// 
// 		if (IsValid(RootPanel))
// 		{
// 			if (IsValid(ItemTypeDetailSwitcher))
// 			{
// 				switch (ItemTableInfo->ItemType)
// 				{
// 				case EItemTypeEnum::VE_EquipItem:
// 				{
// 					ItemTypeDetailSwitcher->SetActiveWidgetIndex(0);
// 				}
// 				break;
// 				default:
// 				{
// 					ItemTypeDetailSwitcher->SetActiveWidgetIndex(1);
// 
// 					if (IsValid(ItemDetailText))
// 					{
// 						ItemDetailText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Description, ItemTableInfo->Description));
// 					}
// 				}
// 				break;
// 				}
// 			}
// 
// 			UTextBlock* itemDescTextBlock = FindChildWidget<UTextBlock>(RootPanel, TEXT("Text_Desc"));
// 			if (IsValid(itemDescTextBlock))
// 			{
// 				
// 				FString desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName).ToString() + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000002")).ToString();
// 				itemDescTextBlock->SetText(FText::FromString(desc));
// 				
// 			}
// 
// 			UTextBlock* itemNameTextBlock = FindChildWidget<UTextBlock>(RootPanel, TEXT("Text_ItemName"));
// 			if (IsValid(itemNameTextBlock))
// 			{
// 				itemNameTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
// 				itemNameTextBlock->SetColorAndOpacity(UUtilFunctionLibrary::GetItemGradeColor(GetWorld(), item.itemId));
// 			}
// 
// 			UTextBlock* itemNameTextGrade = FindChildWidget<UTextBlock>(RootPanel, TEXT("Text_ItemGrade"));
// 			if (IsValid(itemNameTextGrade))
// 			{
// 				FName textKey = UUtilFunctionLibrary::GetItemGradeText(GetWorld(), item.itemId);
// 				if (!textKey.IsNone())
// 					itemNameTextGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, textKey));
// 
// 				FSlateColor color = UUtilFunctionLibrary::GetItemGradeColor(GetWorld(), item.itemId);
// 				itemNameTextGrade->SetColorAndOpacity(color);
// 			}
// 
// 			UPanelWidget* mainPanel = FindChildWidget<UPanelWidget>(RootPanel, TEXT("CanvasPanel_Main"));
// 			UPanelWidget* subPanel = FindChildWidget<UPanelWidget>(RootPanel, TEXT("CanvasPanel_Sub"));
// 			if (IsValid(mainPanel) && IsValid(subPanel))
// 			{
// 				UTextBlock* FrontTextBlock1 = FindChildWidget<UTextBlock>(mainPanel, TEXT("MainProperty"));
// 				UTextBlock* FrontTextBlockValue1 = FindChildWidget<UTextBlock>(mainPanel, TEXT("MainProperty_Value"));
// 				UTextBlock* FrontTextBlock2 = FindChildWidget<UTextBlock>(subPanel, TEXT("MainProperty"));
// 				UTextBlock* FrontTextBlockValue2 = FindChildWidget<UTextBlock>(subPanel, TEXT("MainProperty_Value"));
// 
// 				if (IsValid(FrontTextBlock1) && IsValid(FrontTextBlockValue1) && IsValid(FrontTextBlock2) && IsValid(FrontTextBlockValue2))
// 				{
// 					FWorkedItemTableInfo WorkedItemTableInfo;
// 					if (URenewal_BattleFunctionLibrary::GetCharacterItemTableWorkedInfo(WorkedItemTableInfo, item.itemId, item.enchantValue, item.enchant) == true)
// 					{
// 						if (ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
// 						{
// 							mainPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 							subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 							switch (ItemTableInfo->EquipItemPropertyType)
// 							{
// 							case EEquipItemPropertyType::VE_PhysicalWeapon:
// 							case EEquipItemPropertyType::VE_MagicWeapon:
// 							{
// 								int32 PhysicalAttackPower = WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption();
// 								int32 MagicAttackPower = WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption();
// 
// 								if (PhysicalAttackPower > 0 && MagicAttackPower <= 0)
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() == ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "PhysicalAttackPower"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption()));
// 
// 									FrontTextBlock2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicAttackPower"));
// 									int32 PropertyValue2 = WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption() * VALUE_TEMP;
// 									FrontTextBlockValue2->SetText(FText::AsNumber(PropertyValue2));
// 								}
// 								else if (PhysicalAttackPower <= 0 && MagicAttackPower > 0)
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() == ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicAttackPower"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption()));
// 
// 									FrontTextBlock2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "PhysicalAttackPower"));
// 									int32 PropertyValue2 = WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption() * VALUE_TEMP;
// 									FrontTextBlockValue2->SetText(FText::AsNumber(PropertyValue2));
// 								}
// 								else
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() == ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "PhysicalAttackPower"));
// 									FrontTextBlock2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicAttackPower"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption()));
// 									FrontTextBlockValue2->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption()));
// 								}
// 							}
// 							break;
// 
// 							case EEquipItemPropertyType::VE_PhysicalGuard:
// 							case EEquipItemPropertyType::VE_MagicGuard:
// 							{
// 								int32 PhysicalGuard = WorkedItemTableInfo.GetEnchantArmorNotAddOption();
// 								int32 MagicGuard = WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption();
// 
// 								if (PhysicalGuard > 0 && MagicGuard <= 0)
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() != ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::Collapsed);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "Armor"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantArmorNotAddOption()));
// 								}
// 								else if (PhysicalGuard <= 0 && MagicGuard > 0)
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() != ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::Collapsed);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicResistance"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption()));
// 								}
// 								else
// 								{
// 									if (IsValid(subPanel) && subPanel->GetVisibility() == ESlateVisibility::Collapsed)
// 										subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 									FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "Armor"));
// 									FrontTextBlock2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MagicResistance"));
// 									FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantArmorNotAddOption()));
// 									FrontTextBlockValue2->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption()));
// 								}
// 							}
// 							break;
// 
// 							case EEquipItemPropertyType::VE_HPGuard:
// 							{
// 								if (IsValid(subPanel) && subPanel->GetVisibility() == ESlateVisibility::Collapsed)
// 									subPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 								FrontTextBlock1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "MaxHP"));
// 								FrontTextBlockValue1->SetText(FText::AsNumber(WorkedItemTableInfo.GetEnchantMaxHPNotAddOption()));
// 
// 								FrontTextBlock2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "DodgeValue"));
// 								int32 PropertyValue2 = WorkedItemTableInfo.GetEnchantMaxHPNotAddOption() * VALUE_TEMP;
// 								FrontTextBlockValue2->SetText(FText::AsNumber(PropertyValue2));
// 							}
// 							break;
// 							}
// 						}
// 						else
// 						{
// 							mainPanel->SetVisibility(ESlateVisibility::Collapsed);
// 							subPanel->SetVisibility(ESlateVisibility::Collapsed);
// 						}
// 					}
// 				}
// 			}
// 
// 			if (IsValid(ItemTypeSwitcher))
// 			{
// 				ItemTypeSwitcher->SetActiveWidgetIndex(0);
// 
// 				if (ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
// 				{
// 					switch (ItemTableInfo->EquipPosition)
// 					{
// 					case EItemEquipPositionEnum::VE_Weapon:
// 					{
// 						//if (!ItemTableInfo->EquipHero.IsNone())
// 						//{
// 						//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
// 						//	if (CharacterTableInfo)
// 						//	{
// 						//		ItemTypeSwitcher->SetActiveWidgetIndex(1);
// 
// 						//		if (IsValid(ItemWeaponText))
// 						//		{
// 						//			ItemWeaponText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
// 						//		}
// 						//	}
// 						//}
// 					}
// 					break;
// 					case EItemEquipPositionEnum::VE_Armor:
// 					case EItemEquipPositionEnum::VE_Pants:
// 					case EItemEquipPositionEnum::VE_Ring:
// 					{
// 					}
// 					break;
// 					}
// 
// 					if (!ItemTableInfo->EquipHero.IsNone())
// 					{
// 						FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
// 						if (CharacterTableInfo)
// 						{
// 							ItemTypeSwitcher->SetActiveWidgetIndex(1);
// 
// 							if (IsValid(ItemWeaponText))
// 							{
// 								ItemWeaponText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
// 							}
// 						}
// 					}
// 				}
// 			}
// 
// 			UPanelWidget* optionPanel1 = FindChildWidget<UPanelWidget>(RootPanel, TEXT("Option_Panel1"));
// 			UPanelWidget* optionPanel2 = FindChildWidget<UPanelWidget>(RootPanel, TEXT("Option_Panel2"));
// 			UPanelWidget* optionPanel3 = FindChildWidget<UPanelWidget>(RootPanel, TEXT("Option_Panel3"));
// 			UPanelWidget* optionPanel4 = FindChildWidget<UPanelWidget>(RootPanel, TEXT("Option_Panel4"));
// 			UPanelWidget* optionPanel5 = FindChildWidget<UPanelWidget>(RootPanel, TEXT("Option_Panel5"));
// 
// 			if (IsValid(optionPanel1) && IsValid(optionPanel2) && IsValid(optionPanel3) && IsValid(optionPanel4))
// 			{
// 				optionPanel1->SetVisibility(ESlateVisibility::Collapsed);
// 				optionPanel2->SetVisibility(ESlateVisibility::Collapsed);
// 				optionPanel3->SetVisibility(ESlateVisibility::Collapsed);
// 				optionPanel4->SetVisibility(ESlateVisibility::Collapsed);
// 				optionPanel5->SetVisibility(ESlateVisibility::Collapsed);
// 
// 				for (int32 i = 0; i < item.options.Num(); ++i)
// 				{
// 					UPanelWidget* TargetPanel = nullptr;
// 					switch (i)
// 					{
// 						case 0: TargetPanel = optionPanel1; break;
// 						case 1: TargetPanel = optionPanel2; break;
// 						case 2: TargetPanel = optionPanel3; break;
// 						case 3: TargetPanel = optionPanel4; break;
// 						case 4: TargetPanel = optionPanel5; break;
// 					}
// 
// 					if (TargetPanel == nullptr)
// 						continue;
// 
// 					TargetPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 					UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(TargetPanel, TEXT("ItemOption_Name"));
// 					if (IsValid(optionFrontTextBlock))
// 					{
// 						FName ItemStringKey = UUIFunctionLibrary::GetOptionName(item.options[i].Option);
// 						optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemStringKey));
// 					}
// 
// 					UTextBlock* optionBackTextBlock = FindChildWidget<UTextBlock>(TargetPanel, TEXT("ItemOption_Value"));
// 					if (IsValid(optionBackTextBlock))
// 					{
// 						optionBackTextBlock->SetText(FText::AsNumber(item.options[i].OptionValue));
// 					}
// 				}
// 
// 				/*FItemGenerateOptionTableInfo* OptionGenerateTableInfo = UTableManager::GetInstancePtr()->GetItemGenerateOptionRow(ItemTableInfo->ItemGenerateOptionKey);
// 				if (OptionGenerateTableInfo)
// 				{
// 					if (OptionGenerateTableInfo->ItemGenerateOption1Value)
// 					{
// 						optionPanel1->SetVisibility(ESlateVisibility::Visible);
// 
// 						UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(optionPanel1, TEXT("ItemOption_Name"));
// 						if (IsValid(optionFrontTextBlock))
// 						{
// 							optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, OptionGenerateTableInfo->OptionNameStringKey1));
// 						}
// 
// 						UTextBlock* optionBackTextBlock = FindChildWidget<UTextBlock>(optionPanel1, TEXT("ItemOption_Value"));
// 						if (IsValid(optionBackTextBlock))
// 						{
// 							optionBackTextBlock->SetText(FText::AsNumber(OptionGenerateTableInfo->ItemGenerateOption1Value));
// 						}
// 					}
// 
// 					if (OptionGenerateTableInfo->ItemGenerateOption2Value)
// 					{
// 						optionPanel2->SetVisibility(ESlateVisibility::Visible);
// 
// 						UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(optionPanel2, TEXT("ItemOption_Name"));
// 						if (IsValid(optionFrontTextBlock))
// 						{
// 							optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, OptionGenerateTableInfo->OptionNameStringKey2));
// 						}
// 
// 						UTextBlock* optionBackTextBlock = FindChildWidget<UTextBlock>(optionPanel2, TEXT("ItemOption_Value"));
// 						if (IsValid(optionBackTextBlock))
// 						{
// 							optionBackTextBlock->SetText(FText::AsNumber(OptionGenerateTableInfo->ItemGenerateOption2Value));
// 						}
// 					}
// 
// 					if (OptionGenerateTableInfo->ItemGenerateOption3Value)
// 					{
// 						optionPanel3->SetVisibility(ESlateVisibility::Visible);
// 
// 						UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(optionPanel3, TEXT("ItemOption_Name"));
// 						if (IsValid(optionFrontTextBlock))
// 						{
// 							optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, OptionGenerateTableInfo->OptionNameStringKey3));
// 						}
// 
// 						UTextBlock* optionBackTextBlock = FindChildWidget<UTextBlock>(optionPanel3, TEXT("ItemOption_Value"));
// 						if (IsValid(optionBackTextBlock))
// 						{
// 							optionBackTextBlock->SetText(FText::AsNumber(OptionGenerateTableInfo->ItemGenerateOption3Value));
// 						}
// 					}
// 
// 					if (!ItemTableInfo->EquipHero.IsNone())
// 					{
// 						if (item.options.Num() > 0)
// 						{
// 							optionPanel4->SetVisibility(ESlateVisibility::Visible);
// 
// 							UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(optionPanel4, TEXT("ItemOption_Name"));
// 							if (IsValid(optionFrontTextBlock))
// 							{
// 								optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, OptionGenerateTableInfo->BonusOptionNameStringKey));
// 							}
// 
// 
// 							UTextBlock* optionBackTextBlock = FindChildWidget<UTextBlock>(optionPanel4, TEXT("ItemOption_Value"));
// 							if (IsValid(optionBackTextBlock))
// 							{
// 								//qsmname
// 								//optionBackTextBlock->SetText(FText::AsNumber(item.options[0].OptionValue));
// 							}
// 						}
// 					}
// 				}*/
// 			}
// 
// 			/*UPanelWidget* skillPanel = FindChildWidget<UPanelWidget>(RootPanel, TEXT("SkillOptionPanel"));
// 			if (IsValid(skillPanel))
// 			{
// 				skillPanel->SetVisibility(ESlateVisibility::Collapsed);
// 
// 				if (!ItemTableInfo->PassiveSkillOnEquip.IsNone())
// 				{
// 					FSkillTableInfo_V2* SkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(ItemTableInfo->PassiveSkillOnEquip);
// 					if (SkillTableInfo)
// 					{
// 						skillPanel->SetVisibility(ESlateVisibility::Visible);
// 
// 						UTextBlock* optionFrontTextBlock = FindChildWidget<UTextBlock>(optionPanel3, TEXT("SkillName"));
// 						if (IsValid(optionFrontTextBlock))
// 						{
// 							optionFrontTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo->SkillName));
// 						}
// 					}
// 				}
// 			}*/
// 		}
// 	}
}

void UUP_ItemManufacturePopup::OnClickOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemCraftResult);
	}
}