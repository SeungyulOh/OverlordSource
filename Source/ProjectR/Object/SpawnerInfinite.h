// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnerBase.h"
#include "CustomStructures/InfiniteSpawnerInfo.h"
#include "SpawnerInfinite.generated.h"

namespace SpawnerQueue
{
	USTRUCT()
	struct PROJECTR_API FSpawnerQueueInfo
	{
		FName			CharacterID;
		float			SpawnTime;
		float			SpawnElapsedTime;
		FTimerHandle	SpawnDelayHandler;
	};
}


UCLASS()
class PROJECTR_API ASpawnerInfinite : public ASpawnerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerInfinite();
	
	
	void SpawnTick(float DeltaTime);

	void SetSpawnTargetInfo(FInfiniteSpawnerInfo& SpawnInfo);
		

		

protected:
	UFUNCTION(BlueprintCallable, Category = InfinityMode_Spawner)
	TArray<FName> GetSpawnQueueElement();

private:

	TArray<TSharedPtr<SpawnerQueue::FSpawnerQueueInfo>>		QueueInfos;	
};
