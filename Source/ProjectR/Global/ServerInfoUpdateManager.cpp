// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ServerInfoUpdateManager.h"
#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"



void UServerInfoUpdateManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UServerInfoUpdateManager::SetTimer);
	RGameInstance->EventManager->OnUpdateUserInfo.AddDynamic(this, &UServerInfoUpdateManager::SetTimer);
}


void UServerInfoUpdateManager::SetTimer()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	auto World = RGameInstance->GetWorld();
	if (World == nullptr)
		return;

	World->GetTimerManager().ClearTimer(AvatarInfoEventTimeHandle);
	World->GetTimerManager().SetTimer(AvatarInfoEventTimeHandle, this, &UServerInfoUpdateManager::RefreshAvatarInfo, GetAvatarInfoInterval, true);
}

void UServerInfoUpdateManager::RefreshAvatarInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->RInventory->GetStamina() < RGameInstance->UserInfo->StaminaMax)
	{
		//++(RGameInstance->UserInfo->Avatar.stamina);
	}

	if (IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnUpdateUserInfo.Broadcast();
	}
}

void UServerInfoUpdateManager::BeginDestroy()
{
	//RGameInstance->EventManager->OnUpdateUserInfo.Clear();

	Super::BeginDestroy();
}