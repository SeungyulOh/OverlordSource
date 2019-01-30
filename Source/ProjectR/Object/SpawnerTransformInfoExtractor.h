// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnerTransformInfoExtractor.generated.h"



UCLASS()
class PROJECTR_API ASpawnerTransformInfoExtractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerTransformInfoExtractor();

	UFUNCTION(BlueprintCallable, Category = InfinityMode_Spawner)
	bool		SaveFile( FString InFileName, FString InFileContents);

	UFUNCTION(BlueprintCallable, Category = InfinityMode_Spawner)
	bool		Save();

private:
	void		AddSpawnerTransform(FString& output);
	void		AddWall(FString& output);
	void		AddTrigger(FString& output);
};
