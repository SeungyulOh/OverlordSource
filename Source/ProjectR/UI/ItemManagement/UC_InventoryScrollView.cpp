// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_InventoryScrollView.h"

#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollItem.h"
#include "UI/ItemManagement/ItemInvenScrollItemData.h"


void UUC_InventoryScrollView::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UUC_InventoryScrollView::InventoryDataReCompositionNew(int32 equipPosition)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bIsValid = ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->RInventory));
	if (bIsValid == false)
		return;

	FString heroUD = "";
	//UUP_Item_Management* ItemManagement = FindParentRBaseWidget<UUP_Item_Management>(this);
	//bIsValid = (IsValid(ItemManagement));
	//if (bIsValid == false && bisTesting == false)
	//	return;
	//
	////FString heroUD = ItemManagement->GetCurrentHeroUD();
	//if(bisTesting == false)
	//	heroUD = ItemManagement->GetCurrentHeroUD();
	//else if (bisTesting == true)
	//{
	//	TArray<FString> InHeroUDList;
	//	RGAMEINSTANCE(this)->RInventory->GetHeroUDs(InHeroUDList);
	//	heroUD = InHeroUDList[0];
	//}

	FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(heroUD);
	
	if (outHero != nullptr)
	{
		ClearScrollItemData();

		TArray<FString> ItemUDList;

		RGameInstance->RInventory->GetItemEquipmentKeys(ItemUDList, ItemType, ItemEquipPosition, ItemEquipType);

		for (auto& ItemUDElem : ItemUDList)
		{
			FITEM* itemDest = RGameInstance->RInventory->GetItemDataWithUD(ItemUDElem);
			if (itemDest)
			{
				FItemTableInfo* ItemTableInfoDest = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemDest->itemId);
				if (ItemTableInfoDest)
				{
					bool bExe = false;
					if (ItemTableInfoDest->ItemType == EItemTypeEnum::VE_EquipItem)
					{
						if (ItemTableInfoDest->EquipPosition == (EItemEquipPositionEnum)equipPosition)
						{
							bExe = true;
						}
					}
					else
					{
						bExe = true;
					}

					if (bExe)
					{
						auto ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(AddScrollItemData(UItemInvenScrollItemData::StaticClass()));
						if (!IsValid(ItemInvenScrollItemData))
							continue;
						UUIFunctionLibrary::GetItemIconInfoWithUD(ItemInvenScrollItemData->ItemIconInfo, ItemUDElem);
						ItemInvenScrollItemData->bCompareEnable = false;
						ItemInvenScrollItemData->bCurHeroEnable = true;
						ItemInvenScrollItemData->CurHeroUD = heroUD;
						ItemInvenScrollItemData->RGameInstance = RGameInstance;
						ItemInvenScrollItemData->SetCalData();						
					}
					else
					{
						continue;
					}
				}
			}
		}

		ItemDataList.Sort([&](const URScrollItemData& A, const URScrollItemData& B) {
			URScrollItemData& DataA = const_cast<URScrollItemData&>(A);
			URScrollItemData& DataB = const_cast<URScrollItemData&>(B);

			UItemInvenScrollItemData* ItemDataA = Cast<UItemInvenScrollItemData>(&DataA);
			UItemInvenScrollItemData* ItemDataB = Cast<UItemInvenScrollItemData>(&DataB);
			if (IsValid(ItemDataA) && IsValid(ItemDataB))
			{
				FItemTableInfo* ItemTableA = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataA->ItemIconInfo.itemId);
				FItemTableInfo* ItemTableB = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataB->ItemIconInfo.itemId);

				if (ItemTableA && ItemTableB)
				{
					int32 ALevel = 1;
					int32 BLevel = 1;

					switch (InventorySortStandardType)
					{
					case EInventorySortStandardTypeEnum::VE_Base:
					{
						bool bCheckConsumable = RGameInstance->RInventory->bConsumableWidget;
						
						if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem && ItemTableB->ItemType != EItemTypeEnum::VE_ConsumableItem)
						{
							return bCheckConsumable;
						}
						else if (ItemTableA->ItemType != EItemTypeEnum::VE_ConsumableItem && ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
						{
							return !bCheckConsumable;
						}
						

						if (ItemTableA->ItemType == EItemTypeEnum::VE_EquipItem && ItemTableB->ItemType == EItemTypeEnum::VE_EquipItem)
						{
							FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
							FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);
							if (AItem && BItem)
							{
								int32 IAvalue = 0;
								int32 IBvalue = 0;
								ALevel = AItem->level;
								BLevel = BItem->level;

								if (AItem->equipedHeroUD.Len() > 0)
								{
									if (!ItemDataA->ItemIconInfo.bDisable)
									{
										IAvalue += 1000;
									}											
								}

								if (ItemDataA->ItemIconInfo.bDisable)
								{
									IAvalue -= 1000;
								}

								if (BItem->equipedHeroUD.Len() > 0)
								{
									if (!ItemDataB->ItemIconInfo.bDisable)
									{
										IBvalue += 1000;
									}											
								}

								if (ItemDataB->ItemIconInfo.bDisable)
								{
									IBvalue -= 1000;
								}

								if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								{
									IAvalue += (int32)ItemTableA->ItemGrade * 10;
									IBvalue += (int32)ItemTableB->ItemGrade * 10;

									if (ALevel > BLevel)
									{
										IAvalue += 1;
									}
									else if(ALevel < BLevel)
									{
										IBvalue += 1;
									}
								}
								else
								{
									IAvalue += (10 - (int32)ItemTableA->ItemGrade) * 10;
									IBvalue += (10 - (int32)ItemTableB->ItemGrade) * 10;

									if (ALevel > BLevel)
									{
										IBvalue += 1;
									}
									else if (ALevel < BLevel)
									{
										IAvalue += 1;
									}
								}

								return IAvalue > IBvalue;
							}
						}
						else if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem && ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
						{
							int32 IAvalue = (int32)ItemTableA->ItemGrade;
							int32 IBvalue = (int32)ItemTableB->ItemGrade;

							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return IAvalue > IBvalue;
							else
								return IAvalue < IBvalue;
						}
						else if (ItemTableA->ItemType == EItemTypeEnum::VE_GemItem && ItemTableB->ItemType == EItemTypeEnum::VE_GemItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return ItemTableA->ItemName.ToString() > ItemTableB->ItemName.ToString();
							else
								return ItemTableA->ItemName.ToString() < ItemTableB->ItemName.ToString();
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Grade:
					{
						bool bCheckConsumable = RGameInstance->RInventory->bConsumableWidget;
						//if (bCheckConsumable)
						{
							if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType != EItemTypeEnum::VE_ConsumableItem)
							{
								return bCheckConsumable;
							}
							else if (ItemTableA->ItemType != EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
							{
								return !bCheckConsumable;
							}
						}

						int32 IAvalue = (int32)ItemTableA->ItemGrade;
						int32 IBvalue = (int32)ItemTableB->ItemGrade;

						if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
							return IAvalue > IBvalue;
						else
							return IAvalue < IBvalue;
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Enchant:
					{
						bool bCheckConsumable = RGameInstance->RInventory->bConsumableWidget;
						//if (bCheckConsumable)
						{
							if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType != EItemTypeEnum::VE_ConsumableItem)
							{
								return bCheckConsumable;
							}
							else if (ItemTableA->ItemType != EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
							{
								return !bCheckConsumable;
							}
						}

						FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
						FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);

						if (AItem && BItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return AItem->enchant > BItem->enchant;
							else
								return AItem->enchant < BItem->enchant;
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_ItemType:
					{
						bool bCheckConsumable = RGameInstance->RInventory->bConsumableWidget;
						//if (bCheckConsumable)
						{
							if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType != EItemTypeEnum::VE_ConsumableItem)
							{
								return bCheckConsumable;
							}
							else if (ItemTableA->ItemType != EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
							{
								return !bCheckConsumable;
							}
						}

						if (ItemTableA->ItemType == EItemTypeEnum::VE_EquipItem && ItemTableB->ItemType == EItemTypeEnum::VE_EquipItem)
						{
							int32 IAvalue = 0;
							int32 IBvalue = 0;

							/*if (!ItemTableA->EquipHero.IsNone())
							{
								IAvalue = 1;
							}

							if (!ItemTableB->EquipHero.IsNone())
							{
								IBvalue = 1;
							}*/

							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return IAvalue > IBvalue;
							else
								return IAvalue < IBvalue;
						}
						else if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem && ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
						{
							int32 IAvalue = (int32)ItemTableA->ItemGrade;
							int32 IBvalue = (int32)ItemTableB->ItemGrade;

							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return IAvalue > IBvalue;
							else
								return IAvalue < IBvalue;
						}
						else if (ItemTableA->ItemType == EItemTypeEnum::VE_GemItem && ItemTableB->ItemType == EItemTypeEnum::VE_GemItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return ItemTableA->ItemName.ToString() > ItemTableB->ItemName.ToString();
							else
								return ItemTableA->ItemName.ToString() < ItemTableB->ItemName.ToString();
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Level:
					{
						bool bCheckConsumable = RGameInstance->RInventory->bConsumableWidget;
						//if (bCheckConsumable)
						{
							if (ItemTableA->ItemType == EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType != EItemTypeEnum::VE_ConsumableItem)
							{
								return bCheckConsumable;
							}
							else if (ItemTableA->ItemType != EItemTypeEnum::VE_ConsumableItem
								&& ItemTableB->ItemType == EItemTypeEnum::VE_ConsumableItem)
							{
								return !bCheckConsumable;
							}
						}

						FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
						FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);

						if (AItem && BItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return AItem->level > BItem->level;
							else
								return AItem->level < BItem->level;
						}
					}
					break;
					}

					float AValue = GetItemValue(ItemDataA->ItemIconInfo.itemId, ItemDataA->ItemIconInfo.enchant, ALevel);
					float BValue = GetItemValue(ItemDataB->ItemIconInfo.itemId, ItemDataB->ItemIconInfo.enchant, BLevel);

					return AValue > BValue;
				}
			}

			return false;
		});

		InvalidateData();
	}
}

void UUC_InventoryScrollView::InventoryDataReComposition(FString itemUD, FString heroUD, int32 equipPosition)
{
	ClearScrollItemData();

	TArray<FString> ItemUDList;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory))
	{
/*		switch (ItemInvenType)
		{
		case EItemInvenTypeEnum::VE_WeaponItem:
		case EItemInvenTypeEnum::VE_ArmorItem:
		case EItemInvenTypeEnum::VE_AccessoriesItem:
		case EItemInvenTypeEnum::VE_JewelryItem:
		case EItemInvenTypeEnum::VE_ExpendablesItem:
		case EItemInvenTypeEnum::VE_GemItem:
		{
			RGameInstance->RInventory->GetItemInvenKeys(ItemUDList, InventoryType, ItemInvenType);
		}
		break;
		default:
		{
			RGameInstance->RInventory->GetItemEquipmentKeys(ItemUDList, InventoryType, ItemType, ItemEquipPosition, ItemEquipType);
		}
		break;
		}	*/	

		for (auto& ItemUDElem : ItemUDList)
		{
			bool bExe = false;
			if (itemUD.Len() > 0 && ItemUDElem != itemUD)
			{
				FITEM* itemSource = RGameInstance->RInventory->GetItemDataWithUD(itemUD);
				FITEM* itemDest = RGameInstance->RInventory->GetItemDataWithUD(ItemUDElem);
				if (itemSource && itemDest)
				{
					FItemTableInfo* ItemTableInfoSource = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemSource->itemId);
					FItemTableInfo* ItemTableInfoDest = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemDest->itemId);
					if (ItemTableInfoSource && ItemTableInfoDest)
					{
						if (ItemTableInfoSource->EquipPosition == ItemTableInfoDest->EquipPosition)
						{
							if (ItemTableInfoSource->EquipPosition == EItemEquipPositionEnum::VE_Weapon)
							{
								if (ItemTableInfoSource->EquipType == ItemTableInfoDest->EquipType)
								{
									bExe = true;

									// TODO: item table
									//if (!ItemTableInfoDest->EquipHero.IsNone())
									//{
									//	bExe = false;

									//	FHERO* heroInfo = RGameInstance->RInventory->GetHeroWithUD(heroUD);
									//	if (heroInfo)
									//	{
									//		if (heroInfo->heroId == ItemTableInfoDest->EquipHero)
									//		{
									//			bExe = true;
									//		}
									//	}
									//}
								}
							}
							else
							{
								bExe = true;

								switch (ItemTableInfoSource->EquipPosition)
								{
								case EItemEquipPositionEnum::VE_Armor:
								case EItemEquipPositionEnum::VE_Pants:
								{
									if (UUIFunctionLibrary::IsEquipTypeArmor(ItemTableInfoSource->EquipType))
									{
										bExe = ItemTableInfoSource->EquipType == ItemTableInfoDest->EquipType;
									}
								}
								break;
								}
							}
						}
					}
				}					
			}
			else
			{
				if (heroUD.Len() > 0)
				{
					FHERO* hero =  RGameInstance->RInventory->GetHeroWithUD(heroUD);
					FITEM* itemDest = RGameInstance->RInventory->GetItemDataWithUD(ItemUDElem);
					if (hero && itemDest)
					{
						FCharacterTableInfo* characterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
						FItemTableInfo* ItemTableInfoDest = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemDest->itemId);
						if (characterTableInfo && ItemTableInfoDest)
						{
							if (equipPosition != -1)
							{
								EItemEquipPositionEnum equipPositionEnum = (EItemEquipPositionEnum)equipPosition;
								if (ItemTableInfoDest->EquipPosition == equipPositionEnum)
								{
									if (equipPositionEnum == EItemEquipPositionEnum::VE_Weapon)
									{
										if (ItemTableInfoDest->EquipType == characterTableInfo->EquipType)
										{
											bExe = true;

											// TODO: item table
											//if (!ItemTableInfoDest->EquipHero.IsNone())
											//{
											//	bExe = false;

											//	if (hero->heroId == ItemTableInfoDest->EquipHero)
											//	{
											//		bExe = true;
											//	}
											//}

										}
									}
									else
									{
										bExe = true;

										switch (ItemTableInfoDest->EquipPosition)
										{
										case EItemEquipPositionEnum::VE_Armor:
										case EItemEquipPositionEnum::VE_Pants:
										{
											bExe = UUIFunctionLibrary::IsEquipTypeArmor(ItemTableInfoDest->EquipType);
										}
										break;
										}
									}
								}
							}							
						}
					}
				}
				else
				{
					bExe = true;
				}				
			}

			if(!bExe)
				continue;

			{
				auto ItemInvenScrollItemData = Cast<UItemInvenScrollItemData>(AddScrollItemData(UItemInvenScrollItemData::StaticClass()));
				if (!IsValid(ItemInvenScrollItemData))
				{
					continue;
				}
							

				for (FString& disud : DisableItemList)
				{
					if (ItemUDElem == disud)
						ItemInvenScrollItemData->bSelectEnable = false;
				}

				UUIFunctionLibrary::GetItemIconInfoWithUD(ItemInvenScrollItemData->ItemIconInfo, ItemUDElem);

				ItemInvenScrollItemData->bCompareEnable = false;

				if (itemUD.Len() > 0 && ItemUDElem != itemUD)
				{
					ItemInvenScrollItemData->bCompareEnable = true;
					ItemInvenScrollItemData->CompareItemUD = itemUD;
					ItemInvenScrollItemData->RGameInstance = RGameInstance;
					
					ItemInvenScrollItemData->ItemIconInfo.bCompare = true;

					FITEM* itemSource = RGameInstance->RInventory->GetItemDataWithUD(itemUD);
					FITEM* itemDest = RGameInstance->RInventory->GetItemDataWithUD(ItemUDElem);
					if (itemSource && itemDest)
					{
						FItemTableInfo* ItemTableSource = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemSource->itemId);
						FItemTableInfo* ItemTableDest = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemDest->itemId);
						if (ItemTableSource && ItemTableDest)
						{
							float SourceValue = GetItemValue(itemSource->itemId, itemSource->enchant);
							float DestValue = GetItemValue(itemDest->itemId, itemDest->enchant);

							if (SourceValue > DestValue)
							{
								ItemInvenScrollItemData->ItemIconInfo.bCompareUp = false;
								ItemInvenScrollItemData->ItemIconInfo.CompareValue = SourceValue - DestValue;
							}
							else if (SourceValue < DestValue)
							{
								ItemInvenScrollItemData->ItemIconInfo.bCompareUp = true;
								ItemInvenScrollItemData->ItemIconInfo.CompareValue = DestValue - SourceValue;
							}
							else
							{
								ItemInvenScrollItemData->ItemIconInfo.CompareValue = 0;
							}
						}
					}					
				}
			}

		}


		ItemDataList.Sort([&](const URScrollItemData& A, const URScrollItemData& B) {
			URScrollItemData& DataA = const_cast<URScrollItemData&>(A);
			URScrollItemData& DataB = const_cast<URScrollItemData&>(B);

			UItemInvenScrollItemData* ItemDataA = Cast<UItemInvenScrollItemData>(&DataA);
			UItemInvenScrollItemData* ItemDataB = Cast<UItemInvenScrollItemData>(&DataB);
			if (IsValid(ItemDataA) && IsValid(ItemDataB))
			{
				FItemTableInfo* ItemTableA = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataA->ItemIconInfo.itemId);
				FItemTableInfo* ItemTableB = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataB->ItemIconInfo.itemId);

				if (ItemTableA && ItemTableB)
				{
					int32 ALevel = 1;
					int32 BLevel = 1;
					switch (InventorySortStandardType)
					{
					case EInventorySortStandardTypeEnum::VE_Base:
					{
						if (ItemTableA->ItemType == EItemTypeEnum::VE_EquipItem && ItemTableB->ItemType == EItemTypeEnum::VE_EquipItem)
						{
							FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
							FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);
							if (AItem && BItem)
							{
								int32 IAvalue = 0;
								int32 IBvalue = 0;
								ALevel = AItem->level;
								BLevel = BItem->level;

								if (AItem->equipedHeroUD.Len() > 0)
								{
									IAvalue += 1000;
								}

								IAvalue += (int32)ItemTableA->ItemGrade;

								if (BItem->equipedHeroUD.Len() > 0)
								{
									IBvalue += 1000;
								}

								IBvalue += (int32)ItemTableB->ItemGrade;

								if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
									return IAvalue > IBvalue;
								else
									return IAvalue < IBvalue;
							}
						}
						else if (ItemTableA->ItemType == EItemTypeEnum::VE_GemItem && ItemTableB->ItemType == EItemTypeEnum::VE_GemItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return ItemTableA->ItemName.ToString() > ItemTableB->ItemName.ToString();
							else
								return ItemTableA->ItemName.ToString() < ItemTableB->ItemName.ToString();
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Grade:
					{
						int32 IAvalue = (int32)ItemTableA->ItemGrade;
						int32 IBvalue = (int32)ItemTableB->ItemGrade;

						if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
							return IAvalue > IBvalue;
						else
							return IAvalue < IBvalue;
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Enchant:
					{
						FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
						FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);

						if (AItem && BItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return AItem->enchant > BItem->enchant;
							else
								return AItem->enchant < BItem->enchant;
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_ItemType:
					{
						if (ItemTableA->ItemType == EItemTypeEnum::VE_EquipItem && ItemTableB->ItemType == EItemTypeEnum::VE_EquipItem)
						{
							int32 IAvalue = 0;
							int32 IBvalue = 0;

							// TODO: item table
							//if (!ItemTableA->EquipHero.IsNone())
							//{
							//	IAvalue = 1;
							//}

							//if (!ItemTableB->EquipHero.IsNone())
							//{
							//	IBvalue = 1;
							//}

							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return IAvalue > IBvalue;
							else
								return IAvalue < IBvalue;
						}
						else if(ItemTableA->ItemType == EItemTypeEnum::VE_GemItem && ItemTableB->ItemType == EItemTypeEnum::VE_GemItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return ItemTableA->ItemName.ToString() > ItemTableB->ItemName.ToString();
							else
								return ItemTableA->ItemName.ToString() < ItemTableB->ItemName.ToString();
						}
					}
					break;
					case EInventorySortStandardTypeEnum::VE_Level:
					{
						FITEM* AItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataA->ItemIconInfo.itemUD);
						FITEM* BItem = RGameInstance->RInventory->GetItemDataWithUD(ItemDataB->ItemIconInfo.itemUD);

						if (AItem && BItem)
						{
							if (InventorySortType == EInventorySortTypeEnum::VE_Descending)
								return AItem->level > BItem->level;
							else
								return AItem->level < BItem->level;
						}
					}
					break;
					}

					float AValue = GetItemValue(ItemDataA->ItemIconInfo.itemId, ItemDataA->ItemIconInfo.enchant, ALevel);
					float BValue = GetItemValue(ItemDataB->ItemIconInfo.itemId, ItemDataB->ItemIconInfo.enchant, BLevel);

					return AValue > BValue;
				}
			}

			return false;
		});

		//if (ItemDataList.Num() > 0 && (itemUD.Len() > 0 || heroUD.Len() > 0))
		//{
		//}
		//else
		//{
		//	RefreshSort();
		//}

		//RefreshSort();

		InvalidateData();	

	}
}

float UUC_InventoryScrollView::GetItemValue(FName tableKey, int32 enchantLevel, int32 InItemLevel /*=1*/)
{
	// FWorkedItemTableInfo no more
	//float rtValue = 0.0f;
	//FItemTableInfo* ItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(tableKey);
	//if (ItemTable)
	//{
	//	FWorkedItemTableInfo WorkedItemTableInfo;
	//	if (URenewal_BattleFunctionLibrary::GetCharacterItemTableWorkedInfo(WorkedItemTableInfo, tableKey, enchantLevel, InItemLevel))
	//	{
	//		switch (ItemTable->EquipItemPropertyType)
	//		{
	//		case EEquipItemPropertyType::VE_PhysicalWeapon:
	//			rtValue = WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption();
	//			break;
	//		case EEquipItemPropertyType::VE_MagicWeapon:
	//			rtValue = WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption();
	//			break;
	//		case EEquipItemPropertyType::VE_PhysicalGuard:
	//			rtValue = WorkedItemTableInfo.GetEnchantArmorNotAddOption();
	//			break;
	//		case EEquipItemPropertyType::VE_MagicGuard:
	//			rtValue = WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption();
	//			break;
	//		case EEquipItemPropertyType::VE_HPGuard:
	//			rtValue = WorkedItemTableInfo.GetEnchantMaxHPNotAddOption();
	//			break;
	//		}
	//	}
	//}

	return 0.0;
}

void UUC_InventoryScrollView::SelectItemUI(FString itemUD)
{
	URScrollItemData* SeekItem = nullptr;
	for (URScrollItemData* ItemData : ItemDataList)
	{
		UItemInvenScrollItemData* ItemInvenData = Cast<UItemInvenScrollItemData>(ItemData);
		if (IsValid(ItemInvenData))
		{
			if (!ItemInvenData->ItemIconInfo.itemUD.IsEmpty() && ItemInvenData->ItemIconInfo.itemUD == itemUD)
			{
				SeekItem = ItemData;
				break;
			}
		}
	}

	if (IsValid(SeekItem))
	{
		ItemDataList.Remove(SeekItem);
		ItemDataList.Insert(SeekItem, 0);

		InvalidateData();

		SetSelectScrollListIndex(0);
	}
}

void UUC_InventoryScrollView::SelectItems(TArray<FString>& itemUDs)
{
	for (URScrollItem* scrollitem : ScrollItemList)
	{
		if (scrollitem->IsSelected == true)
		{
			Super::SetSelected(scrollitem);
		}
	}

	for (auto& Element : itemUDs)
	{
		SelectItem(Element);
	}
}

void UUC_InventoryScrollView::SelectItem(FString itemUD)
{
	bool bFind = false;
	for (URScrollItem* scrollitem : ScrollItemList)
	{
		if (IsValid(scrollitem) && IsValid(scrollitem->ItemData))
		{
			UItemInvenScrollItemData* ItemInvenData = Cast<UItemInvenScrollItemData>(scrollitem->ItemData);
			if (IsValid(ItemInvenData))
			{
				if (ItemInvenData->ItemIconInfo.itemUD == itemUD)
				{
					SetSelected(scrollitem);
					bFind = true;
					break;
				}
			}
		}
	}

	if (!bFind)
	{
		for (URScrollItemData* scrollitemData : ItemDataList)
		{
			if (IsValid(scrollitemData))
			{
				UItemInvenScrollItemData* ItemInvenData = Cast<UItemInvenScrollItemData>(scrollitemData);
				if (IsValid(ItemInvenData))
				{
					if (ItemInvenData->ItemIconInfo.itemUD == itemUD)
					{
						SetSelectScrollListData(scrollitemData);
						break;
					}
				}
			}
		}
	}
}

void UUC_InventoryScrollView::UpdateTargetItem(const FString& InItemKey)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RInventory))
		return;

	auto TargetItemScrollItem = FindItemWithItemUD(InItemKey);
	if (IsValid(TargetItemScrollItem) == false)
		return;

	FITEM* TargetItemInvenData = nullptr;
	TargetItemInvenData	=	RGameInstance->RInventory->GetItemDataWithUD(InItemKey);
	if (TargetItemInvenData == nullptr)
		return;

	FITEM_ICON_INFO TargetItemIconInfo;
	UUIFunctionLibrary::GetItemIconInfoWithUD(TargetItemIconInfo, TargetItemInvenData->itemUD);
	
	auto TargetScrollItemData = Cast<UItemInvenScrollItemData>(TargetItemScrollItem->ItemData);
	if (!IsValid(TargetScrollItemData))
		return;

	TargetScrollItemData->ItemIconInfo = TargetItemIconInfo;
	this->UpdateTargetScrollItem(TargetItemScrollItem);
}

URScrollItem* UUC_InventoryScrollView::FindItemWithItemUD(const FString& InItemUD)
{
	UItemInvenScrollItemData*	TargetScrollItemData = nullptr;
	URScrollItem*				TargetScrollItem = nullptr;
	for (auto& ScrollItemElem : ScrollItemList)
	{
		TargetScrollItemData = Cast<UItemInvenScrollItemData>(ScrollItemElem->ItemData);
		if (TargetScrollItemData == nullptr)
			continue;

		if (TargetScrollItemData->ItemIconInfo.itemUD.Equals(InItemUD) == true)
		{
			TargetScrollItem = ScrollItemElem;

			break;
		}
	}

	return TargetScrollItem;
}


void UUC_InventoryScrollView::SelectItemByItemUD(const FString& InItemUD)
{
	auto FindTargetItem = FindItemWithItemUD(InItemUD);
	if (!IsValid(FindTargetItem))
		return;

	this->SetSelectScrollListData(FindTargetItem->ItemData);
}

void UUC_InventoryScrollView::SetSelected(URScrollItem* InItem)
{ 
	// TODO: it's ItemManagement that has to hook to SetSelected event, not the other way around

	//if (!InItem->ItemData->bSelectEnable)
	//{
	//	UUP_Item_Management* ItemManagement = FindParentRBaseWidget<UUP_Item_Management>(this);
	//	if (IsValid(ItemManagement))
	//	{
	//		ItemManagement->SetDetail(InItem->ItemData);
	//	}

	//	return;
	//}		

	if (InItem->IsSelected)
	{
		// Check if can select another
		// TODO: RScrollView should keep track of the number of items currently selected
		Super::SetSelected(InItem);
	}
	else
	{
		// Unselect it 
		Super::SetSelected(InItem);
	}
}

void UUC_InventoryScrollView::AddDisAbleItem(FString ud)
{
	DisableItemList.Emplace(ud);
}

void UUC_InventoryScrollView::ClearDisAbleItem()
{
	DisableItemList.Empty();
}
