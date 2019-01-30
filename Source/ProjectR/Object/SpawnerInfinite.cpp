// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CustomStructures/InfiniteSpawnerInfo.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "SpawnerInfinite.h"
#include "UtilFunctionIntegrated.h"


// Sets default values
ASpawnerInfinite::ASpawnerInfinite() : Super()
{ 	
	QueueInfos.Empty();	
}


void ASpawnerInfinite::SpawnTick(float DeltaTime)
{
	TArray<TSharedPtr<SpawnerQueue::FSpawnerQueueInfo>> CheckQueue;
	CheckQueue.Empty();

	bool IsUpdate = false;

	for (auto Itor = QueueInfos.CreateConstIterator(); Itor; ++Itor)
	{
		TSharedPtr<SpawnerQueue::FSpawnerQueueInfo> QueueInfo = *Itor;
		QueueInfo->SpawnElapsedTime += DeltaTime;
		if (QueueInfo->SpawnElapsedTime > QueueInfo->SpawnTime)
		{
			if (IsFormated)
			{
				CheckQueue.Emplace(QueueInfo);
				OnNativeSpawn();
				IsUpdate = true;
			}
			else
			{
				AActor* Character = ExecuteSpawnEnemy(GetTransform(), QueueInfo->CharacterID, ECharacterCategoryTypeEnum::VE_Npc, 1 );
				if (IsValid(Character))
				{
					SpawnedMembers.Emplace(Character);
					//GetWorldTimerManager().ClearTimer(QueueInfo->SpawnDelayHandler);
					CheckQueue.Emplace(QueueInfo);
					OnNativeSpawn();
				}
			}
		}
	}

	//todo : temp...
	for (auto Itor = CheckQueue.CreateConstIterator(); Itor; ++Itor)
	{
		QueueInfos.Remove(*Itor);
	}
	if (IsFormated && IsUpdate)
	{
		OnSpawn();
	}

}

void ASpawnerInfinite::SetSpawnTargetInfo(FInfiniteSpawnerInfo& SpawnInfo)
{
	IsClear				= false;
	
	TSharedPtr<SpawnerQueue::FSpawnerQueueInfo> Info = MakeShareable(new SpawnerQueue::FSpawnerQueueInfo());
	Info->CharacterID		= SpawnInfo.CharacterID;
	Info->SpawnTime			= SpawnInfo.SpawnDelay;
	Info->SpawnElapsedTime	= 0.0f;	
	//GetWorldTimerManager().SetTimer(Info->SpawnDelayHandler, this, &ASpawnerBase::CallSpawn, Info->SpawnTime, false);
	QueueInfos.Emplace(Info);
}

TArray<FName> ASpawnerInfinite::GetSpawnQueueElement()
{
	TArray<FName> SpawnIDs;
	SpawnIDs.Empty();
	for (auto& Itor : QueueInfos)
	{
		SpawnIDs.Emplace(Itor->CharacterID );
	}
	return SpawnIDs;
}
