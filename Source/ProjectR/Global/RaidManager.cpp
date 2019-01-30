
#include "ProjectR.h"

#include "RaidManager.h"
#include "Global/TableManager.h"


void URaidManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
	SetTableData();
}

void URaidManager::SetTableData()
{
	TArray<FName> RowNames = UTableManager::GetInstancePtr()->GetRaidStageRowNames();

	RaidStageList.Empty();
	for (const FName& TableKey : RowNames)
	{ 
		FRaidStageTableInfo* pData = UTableManager::GetInstancePtr()->GetRaidStageTableRow(TableKey);
		if (pData)
		{
			if (!RaidStageList.Contains(pData->Day))
				RaidStageList.Emplace(pData->Day);
			pData->RaidStageID = TableKey;
			RaidStageList[pData->Day].Emplace(*pData);
		}
	}
}
