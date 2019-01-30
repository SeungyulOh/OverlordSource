// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "StageSpawnInfo.h"



void FStageSpawnInfo::Init()
{
	HasBossSpawner = false;

	SpawnerInfoTable.Reserve(Spawners.Num());
	for (auto& SpawnerInfo : Spawners)
	{
		switch (SpawnerInfo.SpawnerType)
		{
			case ESpawnerTypeEnum::VE_BossSpawner:
			case ESpawnerTypeEnum::VE_RaidBossSpawner:
			{
				HasBossSpawner = true;
				BossSpawnerInfo = SpawnerInfo;
			}
			break;
			case ESpawnerTypeEnum::VE_ProtectionTargetObject:
			case ESpawnerTypeEnum::VE_ProtectTarget:
			{
				HasProtectedObject = true;
			}
			break;
		
		}
		
		SpawnerInfoTable.Emplace(SpawnerInfo.SpawnerID, SpawnerInfo);
	}
}

FSpawnerInfo* FStageSpawnInfo::GetSpanwerInfo(int32 spawnerID)
{
	if (SpawnerInfoTable.Contains(spawnerID))
		return &SpawnerInfoTable[spawnerID];
	
	return nullptr;
}

void FStageGroupEvent::AddClearSpawn(int32 idx)
{
	for (int32 seek : GroupList)
	{
		if (seek == idx)
		{
			if (!ClearGroupList.Contains(idx))
			{
				ClearGroupList.Emplace(idx, idx);
			}
		}
	}
}

bool FStageGroupEvent::IsClearEvent()
{
	if (GroupList.Num() == 0)
		return false;

	if (GroupList.Num() == ClearGroupList.Num())
		return true;

	return false;
}
