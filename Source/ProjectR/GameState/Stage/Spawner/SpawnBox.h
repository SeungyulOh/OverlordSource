// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "Engine/TriggerBox.h"

#include "Global/GridMapManager.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "Object/SpawnerTransform.h"

#include "SpawnBox.generated.h"


UCLASS()
class PROJECTR_API ASpawnBox : public ATriggerBox
{
	GENERATED_BODY()
public:
	ASpawnBox(const FObjectInitializer& ObjectInitializer);
	~ASpawnBox();

};