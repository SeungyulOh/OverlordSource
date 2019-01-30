// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_GuildLobby.h"
#include "Global/RGameInstance.h"
#include "GameInfo/GuildInfo.h"
#include "UP_Guild_Join.h"
#include "UP_Guild_Management.h"



void UUP_GuildLobby::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Switcher))
	{
		if (!RGAMEINSTANCE(this)->GuildInfo->IsGuildExist())
			Switcher->SetActiveWidgetIndex(0);
		else
			Switcher->SetActiveWidgetIndex(1);
	}
}

void UUP_GuildLobby::NativeDestruct()
{
	Super::NativeDestruct();
}
