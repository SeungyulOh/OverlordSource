// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TimeTreasureInfos.h"
#include "GlobalIntegrated.h"

const int32 MaxRewardCount = 5;

TArray<FTimeTreasureTable*> FTimeTreasureTable::LookUp(int32 InFloorIdx)
{
	TArray<FTimeTreasureTable*> TimeTreasureArray;
	TimeTreasureArray.Reserve(MaxRewardCount);

	UTableManager* TableManager = UTableManager::GetInstancePtr();
	if (TableManager)
	{
		TArray<FName> RowNames = TableManager->TimeTreasureTable->GetRowNames();
		for (size_t i = 0; i < RowNames.Num(); ++i)
		{
			FTimeTreasureTable* Element = TableManager->GetTimeTreasureTableRow(RowNames[i]);
			if (Element->FloorInfoId == InFloorIdx)
				TimeTreasureArray.Emplace(Element);

			if (TimeTreasureArray.Num() >= MaxRewardCount)
				break;
		}
	}

	return TimeTreasureArray;
}
