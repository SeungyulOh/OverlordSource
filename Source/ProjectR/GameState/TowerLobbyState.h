
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EngineMinimal.h"
#include "UI/UIDefs.h"
#include "GameState/IGameState.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "TowerLobbyState.generated.h"

/**
 * The only purpose of this class is to bridge between TowerLobbyGameMode and RGameInstance
 * Functionality related to lobby are in TowerLobbyGameMode as it is required by design that there is no loading scene between lobby contents
 *
 * Has responsibility to allow TowerLobbyGameMode to be somewhat functional in the PIE without RGameInstance being setup through the login process.
 */
UCLASS()
class PROJECTR_API UTowerLobbyState : public UIGameState
{
	GENERATED_BODY()
	
public:
	UTowerLobbyState();

	void Enter() override;
	void Leave() override;

	// meaningless
	void BeginState() override;

	void Refresh() override;

	UFUNCTION()
	void OnStreamingComplete();

	UFUNCTION()
	void OnGameModeInitializationFinished();

	void ChangeLobbyState(ETowerLobbyStateEnum InLobbyState, ETowerLobbySubstateEnum InSubstate);

	UFUNCTION()
	void OnFirstEnterSublevelFinished();

	//// Interface for content

	ATowerLobbyGameMode* GetTowerLobbyGameMode();

	/*
	Content transition helpers
	*/
	void CampaignInTransition();

	UFUNCTION()
	void TowerStatusResponse();

	void SetVisibilityAll(bool IsVisible);

	DECLARE_DELEGATE(FOnLobbyStateUpdated);
	FOnLobbyStateUpdated OnLobbyStateUpdated;
	void SetIsEntry(bool entry);
	FORCEINLINE bool IsEntryState() { return IsEntry; }
	FORCEINLINE void SetIndoorStart(bool flag) { bIndoorStart = flag; }
	FORCEINLINE bool IsIndoorStart() { return bIndoorStart; }
private:
	//void UpdateLobbyState(EUIMenuIDEnum Content);
	
	UFUNCTION()
	void OnLobbyStateChanged();

	/*
	Cached references
	*/
	UPROPERTY()
	TWeakObjectPtr<ATowerLobbyGameMode> TowerLobbyGameMode;

	// Dirty. Fix pls.
	bool IsEntry = true;
	bool bIndoorStart = false;
};
