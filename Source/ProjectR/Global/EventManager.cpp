// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EventManager.h"
#include "RGameInstance.h"
#include "GameInfo/UserInfo.h"
#include "Network/PacketFunctionLibrary.h"

void UEventManager::ClearEvent()
{
	OnBossSpawn.Clear();
	OnGameMode_ReadyToStart.Clear();
	OnGameMode_ReadyToResult.Clear();
	OnRaidNotify.Clear();
	OnTargetChange.Clear();
}

void UEventManager::Initialize(void)
{

}