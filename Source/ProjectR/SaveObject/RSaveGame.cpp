// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RSaveGame.h"
#include "Kismet/GameplayStatics.h"

#define NAME_SAVESLOT TEXT("SAVEDATA")

URSaveGame::URSaveGame()
	: SaveSlotName(NAME_SAVESLOT)
	, UserIndex(0)
{
}

void URSaveGame::Save()
{
	UGameplayStatics::SaveGameToSlot(this, SaveSlotName, UserIndex);
}

URSaveGame* URSaveGame::Load()
{
	return Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
}

void FColosseumSaveData::Clear()
{
	WinCount = 0;
	WinningStreakCount = 0;
	LoseCount = 0;
}
