// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MainLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API AMainLobbyGameMode : public AGameModeBase //: public AGameMode
{
	GENERATED_BODY()

private:
	AMainLobbyGameMode(const FObjectInitializer& ObjectInitializer);

	/*
	Override functions..
	*/
	void Tick(float DeltaTime) override;
	bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
};