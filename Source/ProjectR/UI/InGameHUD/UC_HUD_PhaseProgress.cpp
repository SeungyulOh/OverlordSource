// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_PhaseProgress.h"
#include "Global/RGameInstance.h"

#include "UtilFunctionIntegrated.h"

//#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "SharedConstants/GlobalConstants.h"

void UUC_HUD_PhaseProgress::NativeConstruct()
{
	Super::NativeConstruct();

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnSetUpSpawner.RemoveDynamic(this, &UUC_HUD_PhaseProgress::ProgressPhase);
		RGameInstance->EventManager->OnSetUpSpawner.AddDynamic(this, &UUC_HUD_PhaseProgress::ProgressPhase);
	}

	ProgressPhase();
}

void UUC_HUD_PhaseProgress::NativeDestruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnSetUpSpawner.RemoveDynamic(this, &UUC_HUD_PhaseProgress::ProgressPhase);
	}

	Super::NativeDestruct();
}

void UUC_HUD_PhaseProgress::ProgressPhase()
{
	++PhaseValue;

	int32 phase1 = 0;
	int32 phase2 = 0;
	int32 phase3 = 0;

	switch (PhaseValue)
	{
		case 0:
		{
			phase1 = 1;
			phase2 = 0;
			phase3 = 0;
		}
		break;
		case 1:
		{
			phase1 = 2;
			phase2 = 1;
			phase3 = 0;
		}
		break;
		case 2:
		{
			phase1 = 2;
			phase2 = 2;
			phase3 = 1;
		}
		break;
	}

	//Phase;
	if (Switcher_Phase1)
	{
		Switcher_Phase1->SetActiveWidgetIndex(phase1);
	}
	if (Switcher_Phase2)
	{
		Switcher_Phase2->SetActiveWidgetIndex(phase2);
	}
	if (Switcher_Phase3)
	{
		Switcher_Phase3->SetActiveWidgetIndex(phase3);
	}
}