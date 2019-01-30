// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_ItemManufactureIcons.h"

#include "UI/Common/RewardsScrollItemData.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/ItemManagement/ItemInvenScrollItemData.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"

#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"

void UUC_ItemManufactureIcons::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UItemInvenScrollItemData* InvenItemData = Cast<UItemInvenScrollItemData>(item);
	if (IsValid(InvenItemData))
	{
		if (IsValid(Switcher_Lock))
		{
			if (InvenItemData->IsCraftEnable())
			{
				Switcher_Lock->SetActiveWidgetIndex(0);
				if (IsValid(Text_Number))
				{
					//Text_Number->SetVisibility(ESlateVisibility::Collapsed);

					if (InvenItemData->bRecipeItem)
					{
						int32 EnableCount = UUtilFunctionLibrary::GetItemManufactureEnableCount(GetWorld(), InvenItemData->RecipeId);

						//Text_Number->SetVisibility(ESlateVisibility::Visible);
						FText text = FText::Format(FText::FromString("[{0}]"), FText::AsNumber(EnableCount));
						Text_Number->SetText(text);
					}
				}
			}
			else
			{
				Switcher_Lock->SetActiveWidgetIndex(1);
				if (IsValid(Text_Level))
				{
					Text_Level->SetText(FText::AsNumber(InvenItemData->CraftableAccountLevel));
				}
			}
		}

		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InvenItemData->ItemIconInfo.itemId);
		if (ItemTableInfo)
		{
			if (IsValid(ItemImage))
			{
				ItemImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ItemTableInfo->ItemIcon));
			}

			if (IsValid(TypeText))
			{
				TypeText->SetVisibility(ESlateVisibility::Collapsed);

				//if (!ItemTableInfo->EquipHero.IsNone())
				//{
				//	FCharacterTableInfo* CharTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
				//	if (CharTableInfo)
				//	{
				//		TypeText->SetVisibility(ESlateVisibility::Visible);
				//		FText text = FText::Format(FText::FromString("({0})"), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharTableInfo->DisplayName));
				//		TypeText->SetText(text);
				//	}
				//}
			}

			// No item prefix in 2018 Reboot
			//if (LocalText_Prefix)
			//{
			//	if (ItemTableInfo->PassiveSkillOnEquip.IsNone() == false)
			//	{
			//		auto PassiveTable = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(ItemTableInfo->PassiveSkillOnEquip);
			//		auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, PassiveTable->ItemPrefix);
			//		LocalText_Prefix->SetText(String);
			//		LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//	}
			//	else if (ItemTableInfo->ItemSetKey.IsNone() == false)
			//	{
			//		auto ItemSetTable = UTableManager::GetInstancePtr()->GetItemSetEffectRow(ItemTableInfo->ItemSetKey);
			//		auto String = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemSetTable->SetPrefix);
			//		LocalText_Prefix->SetText(String);
			//		LocalText_Prefix->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//	}
			//	else
			//	{
			//		LocalText_Prefix->SetVisibility(ESlateVisibility::Collapsed);
			//	}

			//	/*if (Image_NameDot)
			//	{
			//		Image_NameDot->SetVisibility(LocalText_Prefix->GetVisibility());
			//	}*/
			//}

			if (IsValid(TypeSwitcher))
			{
				TypeSwitcher->SetActiveWidgetIndex(0);

				if (ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
				{
					switch (ItemTableInfo->EquipPosition)
					{
					case EItemEquipPositionEnum::VE_Weapon:
					{
						//if (!ItemTableInfo->EquipHero.IsNone())
						//{
						//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
						//	if (CharacterTableInfo)
						//	{
						//		TypeSwitcher->SetActiveWidgetIndex(1);

						//		if (IsValid(WeaponText))
						//		{
						//			WeaponText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
						//		}
						//	}
						//}
					}
					break;
					case EItemEquipPositionEnum::VE_Armor:
					case EItemEquipPositionEnum::VE_Pants:
					case EItemEquipPositionEnum::VE_Ring:
					{
					}
					break;
					}

					//if (!ItemTableInfo->EquipHero.IsNone())
					//{
					//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
					//	if (CharacterTableInfo)
					//	{
					//		TypeSwitcher->SetActiveWidgetIndex(1);

					//		if (IsValid(WeaponText))
					//		{
					//			WeaponText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
					//		}
					//	}
					//}
				}
			}

			if (IsValid(Text_Detail))
			{
				Text_Detail->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
				Text_Detail->SetColorAndOpacity(UUtilFunctionLibrary::GetItemGradeColor(InvenItemData->ItemIconInfo.itemId));
			}

			// No-gems
			//if (IsValid(GemsPanel))
			//{
			//	UPanelWidget* panelWidget = Cast<UPanelWidget>(GemsPanel);
			//	if (IsValid(panelWidget))
			//	{
			//		for (int32 i = 0; i < panelWidget->GetChildrenCount(); ++i)
			//		{
			//			UWidget* child = panelWidget->GetChildAt(i);
			//			if (IsValid(child))
			//			{
			//				if (i < ItemTableInfo->itemJewelSlotCount)
			//				{
			//					child->SetVisibility(ESlateVisibility::Visible);
			//				}
			//				else
			//				{
			//					child->SetVisibility(ESlateVisibility::Collapsed);
			//				}
			//			}
			//		}
			//	}
			//}

			if (Item_Grade_Image)
			{
				Item_Grade_Image->SetBrushFromTexture(UUtilFunctionLibrary::GetItemGradeIconImg(InvenItemData->ItemIconInfo.itemId));
			}

			if (GradeText)
			{
				FName textKey = UUtilFunctionLibrary::GetItemGradeText(InvenItemData->ItemIconInfo.itemId);
				if (!textKey.IsNone())
					GradeText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, textKey));

				FSlateColor color = UUtilFunctionLibrary::GetItemGradeColor(InvenItemData->ItemIconInfo.itemId);
				GradeText->SetColorAndOpacity(color);
			}

			if (Item_Private_Text)
			{
				Item_Private_Text->SetVisibility(ESlateVisibility::Hidden);

				if (ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
				{
					switch (ItemTableInfo->EquipPosition)
					{
					case EItemEquipPositionEnum::VE_Weapon:
					{
						//if (!ItemTableInfo->EquipHero.IsNone())
						//{
						//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
						//	if (CharacterTableInfo)
						//	{
						//		Item_Private_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						//		Item_Private_Text->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
						//	}
						//}
					}
					break;
					case EItemEquipPositionEnum::VE_Armor:
					case EItemEquipPositionEnum::VE_Pants:
					case EItemEquipPositionEnum::VE_Ring:
					{
					}
					break;
					}

					//if (!ItemTableInfo->EquipHero.IsNone())
					//{
					//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
					//	if (CharacterTableInfo)
					//	{
					//		Item_Private_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					//		Item_Private_Text->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
					//	}
					//}
				}
			}
		}
	}

	Super::SetScrollItemData(Index, item, ScrollView);
}

void UUC_ItemManufactureIcons::RefreshIcon(const FITEM_ICON_INFO& Item_Icon_Info, bool IsLargeIcon)
{
	RefreshItemIcon(Item_Icon_Info, IsLargeIcon);
}

void UUC_ItemManufactureIcons::SetSelected(bool bSelected)
{
	if (bSelected)
		selected->SetVisibility(ESlateVisibility::Visible);
	else
		selected->SetVisibility(ESlateVisibility::Collapsed);
}

