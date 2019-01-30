// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "IGameState.h"

#include "GlobalIntegrated.h"



void UIGameState::BeginState()
{
}

void UIGameState::ComplateState()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		if (GameInstance)
		{
			GameInstance->OptionManager->EnableDynamicShadow(true);
		}
	}
}