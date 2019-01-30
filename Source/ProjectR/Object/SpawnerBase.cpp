// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"


#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "SpawnerBase.h"

#include "UtilFunctionIntegrated.h"


// Sets default values
ASpawnerBase::ASpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//test infiniteChallengeMode	
	SpawnedMembers.Empty();
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnerBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpawnerBase::CallSpawn()
{
	OnSpawn();
}

void ASpawnerBase::SpawnNextSpawner()
{
	if (NextSpawner.Num() > 0)
	{
		for (auto Spawner : NextSpawner)
		{
			if (IsValid( Spawner ) )
				Spawner->CallSpawn();
		}
	}
}

void ASpawnerBase::CheckDeadEvent(AActor* Target, AActor* FromActor)
{
	if (SpawnedMembers.Contains(Target))
	{
		DeadSpawnedTarget(Target);
		OnDeadSpawnedMemberEvent();
	}
}


bool ASpawnerBase::DeadSpawnedTarget(AActor* TargetActor)
{
	SpawnedMembers.Remove(TargetActor);
	if (SpawnedMembers.Num() == 0)
	{
		IsClear = true;

		SpawnNextSpawner();
	}
	return IsClear;
}

AActor* ASpawnerBase::ExecuteSpawnEnemy(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 Level)
{
// 	UGameObjectManager* objectMgr = UUtilFunctionLibrary::GetGameObjectManagerFromActor(this);
// 	if (IsValid(objectMgr))
// 	{
// 		return objectMgr->SpawnCharacter(SpawnTransform, CharacterID, Category,
// 			KID_NONE, HERO_UD_NONE, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
// 			Level);
// 	}
	
	AActor* SpawnedActor = UObjectFunctionLibrary::SpawnCharacter(SpawnTransform, CharacterID, Category,
			KID_NONE, HERO_UD_NONE, GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
			Level);

	return SpawnedActor;
}

