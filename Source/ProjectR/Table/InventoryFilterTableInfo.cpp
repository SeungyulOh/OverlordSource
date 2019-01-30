// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ContentsUITableInfo.h"
#include "InventoryFilterTableInfo.h"
#include "Global/TableManager.h"




bool FInventoryFilterTableInfo::LookUp(TArray<FItemFilterRow>& OutRows, EItemInventoryEnum ItemInventoryEnum)
{
	// Assuming rows are returned in order as the sheet

	TArray<FName> RowNames = UTableManager::GetInstancePtr()->InventoryFilterTableInfo->GetRowNames();
	for (FName RowName : RowNames)
	{
		FInventoryFilterTableInfo* InventoryFilterInfo = UTableManager::GetInstancePtr()->GetInventoryFilterRow(RowName);

		if (InventoryFilterInfo->Inventory == ItemInventoryEnum)
		{
			FItemFilterRow Row;
			Row.Category = InventoryFilterInfo->Category;
			Row.Category_UIStringKey = InventoryFilterInfo->Category_UIString;
			
			if (!InventoryFilterInfo->Factor01.IsEmpty())
			{
				Row.Factors.Emplace(InventoryFilterInfo->Factor01);
				Row.FactorUIStrings.Emplace(InventoryFilterInfo->Factor01_UIString);
			}
			if (!InventoryFilterInfo->Factor02.IsEmpty())
			{
				Row.Factors.Emplace(InventoryFilterInfo->Factor02);
				Row.FactorUIStrings.Emplace(InventoryFilterInfo->Factor02_UIString);
			}
			if (!InventoryFilterInfo->Factor03.IsEmpty())
			{
				Row.Factors.Emplace(InventoryFilterInfo->Factor03);
				Row.FactorUIStrings.Emplace(InventoryFilterInfo->Factor03_UIString);
			}
			if (!InventoryFilterInfo->Factor04.IsEmpty())
			{
				Row.Factors.Emplace(InventoryFilterInfo->Factor04);
				Row.FactorUIStrings.Emplace(InventoryFilterInfo->Factor04_UIString);
			}
			if (!InventoryFilterInfo->Factor05.IsEmpty())
			{
				Row.Factors.Emplace(InventoryFilterInfo->Factor05);
				Row.FactorUIStrings.Emplace(InventoryFilterInfo->Factor05_UIString);
			}

			OutRows.Emplace(Row);
		}
	}
	
	return true;
}
