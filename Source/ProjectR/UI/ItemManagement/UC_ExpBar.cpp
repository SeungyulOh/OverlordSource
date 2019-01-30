// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ExpBar.h"

void UUC_ExpBar::InvalidateData()
{
	Super::InvalidateData();

	if (ExpsByLevel.Num() == 0)
		return;

	// Find the residue
	int32 Level;
	int32 Residue;
	GetLevelAndResidue(Level, Residue);

	float CurrentFill;
	if (Level >= ExpsByLevel.Num())
	{
		// Clamp
		CurrentFill = 1.0;
	}
	else
	{
		CurrentFill = ((float)Residue) / (float)ExpsByLevel[Level];
	}

	// Not per cent
	ProgressBar->SetPercent(CurrentFill);

	FText PercentText = FText::FromString(FString::Printf(TEXT("%.1f%%"), CurrentFill * 100.0));
	Text_Value->SetText(PercentText);
}

void UUC_ExpBar::GetLevelAndResidue(int32& OutLevel, int32& OutResidue)
{
	OutLevel = 0;
	OutResidue = CurrentValue;

	for (; OutLevel < ExpsByLevel.Num(); OutLevel++)
	{
		if (OutResidue < ExpsByLevel[OutLevel])
		{
			return;
		}

		OutResidue -= ExpsByLevel[OutLevel];
	}
}
