// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SharedConstants/GlobalConstants.h"
#include "SpawnerBase.generated.h"


UCLASS()
class PROJECTR_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerBase();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;
	
	FORCEINLINE bool IsSuccess() { return IsClear; }

	UFUNCTION(BlueprintCallable, Category = Spawner)
	void CallSpawn();

	UFUNCTION(BlueprintCallable, Category = Spawner)
	void SpawnNextSpawner();
		
	UFUNCTION(BlueprintCallable, Category = Spawner)
	bool DeadSpawnedTarget(AActor* TargetActor);

	AActor* ExecuteSpawnEnemy(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 Level);

	/*
	--------------------------------------------------------------------------------------------
	Blueprint Native Events
	--------------------------------------------------------------------------------------------
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = Spawner)
	void OnSpawn();

	UFUNCTION(BlueprintImplementableEvent, Category = Spawner)
	void OnNativeSpawn();

	

	UFUNCTION(BlueprintImplementableEvent, Category = FormatedSpawn)
	void OnMakeFormated();

	UFUNCTION(BlueprintImplementableEvent, Category = Spawner)
	void OnDeadSpawnedMemberEvent();

	/*
	
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawner)
	TArray<AActor*>	SpawnedMembers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	float							AutoSpawnDelayTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	int32							StageID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	int32							StageSpawnOrder = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	bool							IsClear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	ESpawnerTypeEnum				SpawnerType = ESpawnerTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	bool							HeadSpawner = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TArray< ASpawnerBase* >			NextSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	bool							BossSpawnTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	bool							BossCameraEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	int32							LeaderIndex = -1;

	
	//Temp - Make use only in Formated_GroupBase...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InfinityMode_Spawner)
	bool							IsFormated = false;
	
	void	CheckDeadEvent(AActor* Target, AActor* FromActor);


};
