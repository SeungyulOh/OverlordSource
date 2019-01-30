// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_RaidBossSpawn.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_RaidBossSpawn : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;


	UFUNCTION(BlueprintImplementableEvent, Category = UUP_RaidBossSpawn)
	void BP_ShowName(EGameModeSequenceTypeEnum GameModeSequenceType, bool Show);
};