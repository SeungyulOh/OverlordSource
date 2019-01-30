// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_DeckSoulInfo.h"
#include "UI/DeckManagement/UC_Soul_Column.h"

void UUC_DeckSoulInfo::NativeConstruct()
{
	Super::NativeConstruct();
	FindChildWidgetsWithCertainName<UUC_Soul_Column>(this, CrewSouls, TEXT("UC_Soul_Column"));

// 	for (UUC_Soul_Column* soul : CrewSouls)
// 	{
// 		soul->SetSoulInfo(TEXT("None"));
// 	}
}

void UUC_DeckSoulInfo::SetSoulInfo(int32 InIndex, FString InSoulUD)
{
	if (CrewSouls.IsValidIndex(InIndex))
	{
		CrewSouls[InIndex]->SetSoulInfo(InSoulUD);
	}
}
