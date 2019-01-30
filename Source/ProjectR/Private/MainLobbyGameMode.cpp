// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "MainLobbyGameMode.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"

AMainLobbyGameMode::AMainLobbyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->SetTickableWhenPaused(true);
}

void AMainLobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMainLobbyGameMode::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = false;

	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	//if (IsValid(GameInstance))
	{
		bResult = GameInstance->ConsoleCheatManager->ProcessConsoleExec(Cmd, Ar, Executor);
	}

	if (bResult == false)
		bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	return bResult;
}