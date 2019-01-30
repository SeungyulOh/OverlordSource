// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UpdateService/Auth/AuthState.h"
#include "ReadyToGoLobbyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UReadyToGoLobbyState : public UAuthState
{
	GENERATED_BODY()

public:
	void OnEnter() override;
	void OnLeave() override;

	void ReadyToGoToLobby();

	UFUNCTION()
	void GoToLobby();

	/// Login procedure. Will be cleaned up.
	/*
	==================================================================================================================
	Delegate
	==================================================================================================================
	*/
	UFUNCTION()
	void AvatarGameDataRp();

	UFUNCTION()
	void OnEndIntroMovie();
private:
	
};
