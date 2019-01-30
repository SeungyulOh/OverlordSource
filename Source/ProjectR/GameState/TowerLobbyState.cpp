// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "UtilFunctionIntegrated.h"
#include "TowerLobbyState.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "UI/RWidgetManager.h"
#include "Network/PacketFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"

//

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "UI/UIDefs.h"

UTowerLobbyState::UTowerLobbyState()
{
	StateType = EGameStateEnum::VE_Lobby;
}

void UTowerLobbyState::Enter()
{
	Super::Enter();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->GameEnvironmentVar.LobbyIsNight = UUtilFunctionLibrary::IsNightingWithRealTime();
	
	if (RGameInstance->GameEnvironmentVar.LobbyIsNight)
		UMapFunctionLibrary::LoadLevel(TEXT("/Game/Maps/Lobby/Lobby/Lobby_Night_P"));
	else
		UMapFunctionLibrary::LoadLevel(TEXT("/Game/Maps/Lobby/Lobby/Lobby_Day_P"));

	//RGameInstance->LoadingSceneManager->ShowLoadingScene(EGamePlayModeTypeEnum::VE_Campaign);
}

void UTowerLobbyState::Leave()
{
	TowerLobbyGameMode.Reset();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	// TODO: managers please clean up after themselves
		
	USoundManager::GetInstancePtr()->StopBGM();
		

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.RemoveObjectAll();
		pGameMode->ObjectHelper.AllObjectVisible = true;
	}

	RGameInstance->RWidgetManager->SetStackEvent(EUIStackEventEnum::VE_None);
	Super::Leave();
}

void UTowerLobbyState::BeginState()
{
	Super::BeginState();
}

void UTowerLobbyState::OnStreamingComplete()
{
	UE_LOG(LogLobby, Log, TEXT("ULobbyTowerState::OnStreamingComplete: success to get game mode"));
}

void UTowerLobbyState::ChangeLobbyState(ETowerLobbyStateEnum InLobbyState, ETowerLobbySubstateEnum InSubstate)
{
	if (TowerLobbyGameMode.IsValid() == false)
		return;

	TowerLobbyGameMode->OnStateChanged.AddUObject(this, &UTowerLobbyState::OnLobbyStateChanged);

	// entry transition hijack
	if (IsEntry)
	{
		TowerLobbyGameMode->ChangeState(ETowerLobbyStateEnum::Outdoor, ETowerLobbySubstateEnum::NormalView);
	}
	else
	{
		TowerLobbyGameMode->ChangeState(InLobbyState, InSubstate);
	}
}

void UTowerLobbyState::OnGameModeInitializationFinished()
{
	// Hook up to TowerLobbyGameMode

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(RGAMEINSTANCE(this), 0));

	TowerLobbyGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!TowerLobbyGameMode.IsValid())
	{
		UE_LOG(LogLobby, Log, TEXT("ULobbyTowerState::OnGameModeInitializationFinished: failed to get game mode"));
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	// Start
//	RGameInstance->LoadingSceneManager->ShowLoadingScene(EGamePlayModeTypeEnum::VE_Campaign);
	RGameInstance->EventManager->TowerStatusResponse.RemoveDynamic(this, &UTowerLobbyState::TowerStatusResponse);
	RGameInstance->EventManager->TowerStatusResponse.AddDynamic(this, &UTowerLobbyState::TowerStatusResponse);
	TowerLobbyGameMode->OnEnterInEntryCompleteDelegate.RemoveAll(this);
	TowerLobbyGameMode->OnEnterInEntryCompleteDelegate.AddUObject(this, &UTowerLobbyState::OnFirstEnterSublevelFinished);
	// Tower Lobby Entry
	if (IsEntry)
	{
		UPacketFunctionLibrary::TOWER_STATUS_RQ();
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_LobbyIndoor);
	}
	else
	{
		if (bIndoorStart)
		{
			UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_LobbyIndoor);
			bIndoorStart = false;
		}
		else 
			UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_LobbyOutdoor);
	}
}

void UTowerLobbyState::OnFirstEnterSublevelFinished()
{
	TowerLobbyGameMode->OnEnterInEntryCompleteDelegate.RemoveAll(this); // RemoveAll dubious

	if (!IsEntry)
	{
		//if have another ui load event keep showing loading scene
		if(GameInstance->RWidgetManager->UIEventQueue.Num() == 0)
			GameInstance->LoadingSceneManager->HideLoadingScene();
	}
}

ATowerLobbyGameMode* UTowerLobbyState::GetTowerLobbyGameMode()
{
	return TowerLobbyGameMode.Get();
}

void UTowerLobbyState::OnLobbyStateChanged()
{
	if (IsEntry)
	{
		// hijack
		// As soon as entry finishes, do entry transition to indoor

		// hide loading scene here instead of at OnGameModeInitializationFinished because there isn't actually Entry sequence
		GameInstance->LoadingSceneManager->HideLoadingScene();

		IsEntry = false;
		TowerLobbyGameMode->IsEntryTransition = true;
		TowerLobbyGameMode->ChangeState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::NormalView);
	}
	else
	{
		if (TowerLobbyGameMode.IsValid())
		{
			TowerLobbyGameMode->OnStateChanged.RemoveAll(this);
		}

		OnLobbyStateUpdated.ExecuteIfBound();
	}
}


void UTowerLobbyState::CampaignInTransition()
{
	check(TowerLobbyGameMode.IsValid());

	if (TowerLobbyGameMode->CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
	{
		TowerLobbyGameMode->OnStateChanged.AddUObject(this, &UTowerLobbyState::OnLobbyStateChanged);
		TowerLobbyGameMode->ChangeState(ETowerLobbyStateEnum::Campaign, ETowerLobbySubstateEnum::NormalView);
	}
	else
	{
		// shortcut?
		OnLobbyStateUpdated.ExecuteIfBound();
	}
}

void UTowerLobbyState::TowerStatusResponse()
{
	if (TowerLobbyGameMode->CurrentLobbyState == ETowerLobbyStateEnum::Campaign)
	{
		RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Campaign);
		RGAMEINSTANCE(this)->RWidgetManager->SetUIVisibility(true);
	}
}


void UTowerLobbyState::SetVisibilityAll(bool IsVisible)
{
	TowerLobbyGameMode->SetVisibilityAll(IsVisible);
}

void UTowerLobbyState::SetIsEntry(bool entry)
{
	IsEntry = entry;
}

void UTowerLobbyState::Refresh()
{
	SetIsEntry(true);
	SetIndoorStart(false);
}