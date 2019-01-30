// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GachaInfos.h"



FGachaInfo* FGachaInfo::Lookup(FName InSummontype)
{
	TArray<FName> GachaRows = UTableManager::GetInstancePtr()->GetShopCharacterSummonRowNames();
	for (FName GachaKey : GachaRows)
	{
		FGachaInfo* GachaTableInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(GachaKey);
		if (GachaTableInfo->SummonType == InSummontype)
			return GachaTableInfo;
	}

	return nullptr;
}
