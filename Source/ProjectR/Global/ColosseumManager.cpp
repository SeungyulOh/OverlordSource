// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ColosseumManager.h"
#include "GlobalIntegrated.h"

#include "Network/PacketFunctionLibrary.h"
#include "SaveObject/RSaveGame.h"


void UColosseumManager::Initialize()
{
	URSaveGame* SaveGame = RGAMEINSTANCE(GEngine)->GetSaveGame();
	if(IsValid(SaveGame))
	{
		CS_SaveData = SaveGame->ColosseumRecord;
	}
}

void UColosseumManager::ShutDown()
{
	URSaveGame* SaveGame = RGAMEINSTANCE(GEngine)->GetSaveGame();
	if (IsValid(SaveGame))
	{
		SaveGame->ColosseumRecord = CS_SaveData;
		SaveGame->Save();
	}
}

void UColosseumManager::RequestColosseumInfo()
{
	UPacketFunctionLibrary::COLOSSEUM_STATUS_RQ();
	UPacketFunctionLibrary::COLOSSEUM_RANKING_LIST_RQ();
}


void UColosseumManager::OpenColosseumLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ColosseumLobby);
	openColosseumUI.RemoveAll(this);
}
