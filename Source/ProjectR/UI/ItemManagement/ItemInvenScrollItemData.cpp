// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ItemInvenScrollItemData.h"

#include "GlobalIntegrated.h"



#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"
#include "Table/ItemTableInfo.h"
#include "UtilFunctionIntegrated.h"


bool UItemInvenScrollItemData::IsNotify()
{
	if (bCompareEnable == false)
		return false;
	if (RGAMEINSTANCE(this) == nullptr)
		return false;
	if (RGAMEINSTANCE(this)->RInventory == nullptr)
		return false;
	if (RGAMEINSTANCE(this)->NotifyManager == nullptr)
		return false;
		
	FITEM* outSourceItem = nullptr;
	outSourceItem = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemIconInfo.itemUD);
	if (outSourceItem == nullptr)
		return false;

	FItemTableInfo* sourceItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
	if (sourceItemTableInfo == nullptr)
		return false;

	float fSourceValue = RGAMEINSTANCE(this)->NotifyManager->GetItemValue(*(outSourceItem));

	FITEM* outDestItem = nullptr;
	outDestItem = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CompareItemUD);
	if (outDestItem == nullptr)
		return false;

	FItemTableInfo* destItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outDestItem->itemId);
	if (destItemTableInfo == nullptr)
		return false;

	if (sourceItemTableInfo->EquipItemPropertyType == destItemTableInfo->EquipItemPropertyType)
	{
		float fDestValue = RGAMEINSTANCE(this)->NotifyManager->GetItemValue(*(outDestItem));

		if (fSourceValue > fDestValue)
			return true;
	}

	return false;
}

void UItemInvenScrollItemData::SetCalData()
{
	if (bCurHeroEnable == false)
		return;
	
	bNotify = false;
	bCompareEnable = false;
	ItemIconInfo.bCompare = false;

	bool bIsValid = ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->NotifyManager));
	if (bIsValid == false)
		return;
	
	FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(CurHeroUD);
	if (outHero == nullptr)
		return;
	
	FITEM* sourceItem = RGameInstance->RInventory->GetItemDataWithUD(ItemIconInfo.itemUD);
	FItemTableInfo* sourceItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
	bIsValid = (sourceItem && sourceItemTableInfo);
	if (bIsValid == false)
		return;

	if (sourceItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
	{
		bCompareEnable = true;
						
		//bSelectEnable = UUtilFunctionLibrary::IsEnableItem(outHero.heroId, ItemIconInfo.itemId);
		ItemIconInfo.bDisable = !UUtilFunctionLibrary::CanCharacterEquipItem(outHero->heroId, ItemIconInfo.itemId);

		if (!ItemIconInfo.bDisable)
		{
			float fSourceValue = RGameInstance->NotifyManager->GetItemValue(*sourceItem);

			FITEM* destItem = nullptr;
			FItemTableInfo* destItemTableInfo = nullptr;
			for (FString& dest : outHero->equipedItemUDs)
			{
				if (dest.IsEmpty())
					continue;

				FITEM* tempItem = RGameInstance->RInventory->GetItemDataWithUD(dest);
				if (tempItem)
				{
					FItemTableInfo* tempTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(tempItem->itemId);
					if (tempTableInfo)
					{
						if (tempTableInfo->ItemType == EItemTypeEnum::VE_EquipItem && sourceItemTableInfo->EquipPosition == tempTableInfo->EquipPosition)
						{
							destItem = tempItem;
							destItemTableInfo = tempTableInfo;
							break;
						}
					}
				}
			}

			if (destItem && destItemTableInfo)
			{
				ItemIconInfo.bCompare = true;
								
				if (sourceItem->equipedHeroUD.Len() > 0)
				{
					bNotify = false;
				}
				else
				{
					float fDestValue = RGameInstance->NotifyManager->GetItemValue(*destItem);
					bNotify = (fSourceValue > fDestValue);
				}

				float fCompareSource = URInventory::GetItemValue(sourceItem->itemId, sourceItem->enchant, sourceItem->level);
				float fCompareDest = URInventory::GetItemValue(destItem->itemId, destItem->enchant, destItem->level);

				if (fCompareSource > fCompareDest)
				{
					ItemIconInfo.bCompareUp = true;
					ItemIconInfo.CompareValue = fCompareSource - fCompareDest;
				}
				else if (fCompareSource < fCompareDest)
				{
					ItemIconInfo.bCompareUp = false;
					ItemIconInfo.CompareValue = fCompareDest - fCompareSource;
				}
				else
				{
					ItemIconInfo.bCompareUp = false;
					ItemIconInfo.CompareValue = 0;
				}

				if (destItem->itemUD == sourceItem->itemUD)
				{
					bSelectEnable = false;
				}
			}
			else
			{
				if (sourceItem->equipedHeroUD.Len() > 0)
				{
					bNotify = false;
				}
				else
				{
					bNotify = true;
				}								
			}
		}

		if (RGameInstance->RInventory->bOpenBatchRegisterWidget)
			ItemIconInfo.bDisable = false;

	}
}

bool UItemInvenScrollItemData::IsCraftEnable()
{
	if ( IsValid(RGameInstance->UserInfo))
	{
		if (CraftableAccountLevel > 0)
		{
			if (RGameInstance->UserInfo->Avatar.level < CraftableAccountLevel)
			{
				return false;
			}
		}
	}

	return true;
}