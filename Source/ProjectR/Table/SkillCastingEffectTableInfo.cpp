// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "SkillCastingEffectTableInfo.h"





FSkillCastingEffectTableInfo* FSkillCastingEffectTableInfo::LookUp(EDecalShapeEnum InDecalShape, ESkillDecalColorType InSkillDecalColorType)
{

	TArray<FName> RowNames = UTableManager::GetInstancePtr()->SkillCastingEffectTableInfo->GetRowNames();
	for (FName RowName : RowNames)
	{
		FSkillCastingEffectTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetSkillCastingEffectRow(RowName);
		if (TableInfo->DecalShape == InDecalShape && TableInfo->SkillDecalColorType == InSkillDecalColorType)
		{
			return TableInfo;
		}
	}

	UE_LOG(LogGamePlay, Log, TEXT("FSkillCastingEffectTableInfo::LookUp: failed to find row"));

	return nullptr;
}
