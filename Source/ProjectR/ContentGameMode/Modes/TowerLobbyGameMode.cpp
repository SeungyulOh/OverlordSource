// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TowerLobbyGameMode.h"
#include "LevelSequenceActor.h"
#include "Lobby/BaseLobbyStateActor.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
// START For legacy support
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
// END
// For checking type for transition
#include "Lobby/OutdoorLobbyStateActor.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "GameState/TowerLobbyState.h"
#include "Network/Chat/ChatManager.h"

ATowerLobbyGameMode::ATowerLobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	
}

void ATowerLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableClickEvents = true;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableTouchEvents = true;

	RGAMEINSTANCE(this)->LoadingSceneManager->ShowLoadingScene(EGamePlayModeTypeEnum::VE_Campaign);

	if (LobbyLightingOverride != ETowerLobbyLighting::Max)
	{
		CurrentLighting = LobbyLightingOverride;
	}
	else
	{
		// temp: show random lighting for now
		CurrentLighting = GetRandomLighting();
	}
}

void ATowerLobbyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(GetLobbyStateActor(CurrentLobbyState)))
	{
		GetLobbyStateActor(CurrentLobbyState)->EndSublevel();
		GetLobbyStateActor(CurrentLobbyState)->LeaveSublevel();
	}

	Super::EndPlay(EndPlayReason);
}

void ATowerLobbyGameMode::SetupIndoorLobbyStateActor()
{
	IndoorOriginCoordinate = UUtilFunctionLibrary::FindActor<AActor>(GetWorld(), AActor::StaticClass(), FName(TEXT("IndoorOriginCoordinate")));
	IndoorLobbyStateAct = Cast<AIndoorLobbyStateActor>(GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (IndoorLobbyStateAct)
	{
		IndoorLobbyStateAct->IndoorOriginLocation = IndoorOriginCoordinate->GetRootComponent()->K2_GetComponentLocation();
		IndoorLobbyStateAct->IndoorNormalViewCameraActor = Cast<ACameraActor>(UUtilFunctionLibrary::FindActor<ACameraActor>(GetWorld(), ACameraActor::StaticClass(), FName("IndoorNormalViewTarget")));
		IndoorLobbyStateAct->IndoorHeroManagementViewCameraActor = Cast<ACameraActor>(UUtilFunctionLibrary::FindActor<ACameraActor>(GetWorld(), ACameraActor::StaticClass(), FName("IndoorHeroInventoryViewTarget")));
	}
}

void ATowerLobbyGameMode::SetupOutdoorLobbyStateActor()
{
	m_TowerPivotCoordinate = UUtilFunctionLibrary::FindActor<AActor>(GetWorld(), AActor::StaticClass(), FName(TEXT("TowerPivotCoordinate")));
	OutdoorLobbyStateAct = Cast<AOutdoorLobbyStateActor>(GetLobbyStateActor(ETowerLobbyStateEnum::Outdoor));
	if (OutdoorLobbyStateAct)
	{
		OutdoorLobbyStateAct->OutdoorCameraActor = Cast<ACameraActor>(UUtilFunctionLibrary::FindActor<ACameraActor>(GetWorld(), ACameraActor::StaticClass(), FName("OutdoorCamera")));
		OutdoorLobbyStateAct->TowerPivotLocation = m_TowerPivotCoordinate->GetActorLocation();
	}
}

void ATowerLobbyGameMode::Initialize_PMap()
{
	UWorld* InWorld = GetWorld();//GEngine->GetWorldFromContextObject(GetWorld(), EGetWorldErrorMode::Assert);
	ALevelSequenceActor* Actor = nullptr;
	for (TActorIterator<ALevelSequenceActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		if (!InWorld)
			break;

		Actor = *ActorItr;
		if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Transition_11")))
			m_LobbyOut_Transition = Actor;
		else if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("First_Log_In_Transition_17")))
			m_FirstLogin_Transition = Actor;
		else if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_In_Transition_8")))
			m_LobbyIn_Transition = Actor;
		else if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Campaign_In_Transition_14")))
			m_CampaignIn_Transition = Actor;
	}
	// Let the RGameInstance state know
	auto LobbyTowerState = Cast<UTowerLobbyState>(RGAMEINSTANCE(this)->GetCurrentState());
	if (IsValid(LobbyTowerState))
	{
		LobbyTowerState->OnGameModeInitializationFinished();
	}
	RGAMEINSTANCE(this)->EventManager->UpdateLastMessage.AddDynamic(this, &ATowerLobbyGameMode::SetLastMessage);
}

void ATowerLobbyGameMode::ChangeState(ETowerLobbyStateEnum InLobbyStateEnum, ETowerLobbySubstateEnum InLobbySubstate)
{
	check(InLobbyStateEnum != ETowerLobbyStateEnum::None);
	check(InLobbySubstate != ETowerLobbySubstateEnum::None);

	ABaseLobbyStateActor* Looby = GetCurrentLobbyStateActor();

	ETowerLobbySubstateEnum CurrentSubstate = (IsValid(Looby) && Looby != nullptr) ? Looby->GetCurrentSubstate() : ETowerLobbySubstateEnum::None;

	if (CurrentLobbyState == InLobbyStateEnum && CurrentSubstate == InLobbySubstate)
	{
		// no transition
		OnStateChanged.Broadcast();
		return;
	}

	if (InLobbyStateEnum == ETowerLobbyStateEnum::Campaign)
	{
		CurrentLobbyState = InLobbyStateEnum;
		//CampaignInTransition();
		OnCampaignInTransitionFinished();
	}
	else if (CurrentLobbyState == InLobbyStateEnum)
	{
		// Transition between substate
		Looby->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::OnSubstateChanged);
		Looby->ChangeState(InLobbySubstate);
	}
	else if (CurrentLobbyState == ETowerLobbyStateEnum::None)
	{
		// Entry into state
		if (InLobbySubstate != ETowerLobbySubstateEnum::NormalView)
		{
			UE_LOG(LogLobby, Log, TEXT("ATowerLobbyGameMode::ChangeState: TODO: support entry into substate"));
		}

		CurrentLobbyState = InLobbyStateEnum;

		GetLobbyStateActor(CurrentLobbyState)->OnEnterSublevelComplete.AddDynamic(this, &ATowerLobbyGameMode::OnEnterInEntryComplete);

		/*
			when Only First init
		*/
		if (InLobbyStateEnum == ETowerLobbyStateEnum::Outdoor)
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_Out_NightTime_Ls");
			else
				LightName = FName("Lobby_Out_DayTime_Ls");

			UMapFunctionLibrary::LoadSubLevel(LightName, FName("FirstLightMapLoadedCompleted"), this);
		}
		else if (InLobbyStateEnum == ETowerLobbyStateEnum::Indoor)
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_In_NightTime_Ls");
			else
				LightName = FName("Lobby_In_DayTime_Ls");

			UMapFunctionLibrary::LoadSubLevel(LightName, FName("FirstLightMapLoadedCompleted"), this);
		}
		
	}
	else
	{
		// Transition between states
		if (CurrentLobbyState == ETowerLobbyStateEnum::Campaign)
		{
			PreviousLobbyState = CurrentLobbyState;
			CurrentLobbyState = InLobbyStateEnum;
			LobbySubstate = InLobbySubstate;
			if (CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
			{
				GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::OnSubstateChanged);
				GetLobbyStateActor(CurrentLobbyState)->ChangeState(ETowerLobbySubstateEnum::NormalView);
			}
			else if (CurrentLobbyState == ETowerLobbyStateEnum::Indoor)
			{
				GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::OnSubstateChanged);
				if (LobbySubstate == ETowerLobbySubstateEnum::RaidTowerBoss)
				{
					GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::ChangeUITowerRaid);
					GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject((AOutdoorLobbyStateActor*)GetLobbyStateActor(ETowerLobbyStateEnum::Outdoor), &AOutdoorLobbyStateActor::LeaveSublevel);
				}
				UMapFunctionLibrary::LoadSubLevel(FName("Lobby_In"), FName("LobbyInLoadFinished"), this, FTransform());
			}
			return;
		}

		PreviousLobbyState = CurrentLobbyState;
		CurrentLobbyState = InLobbyStateEnum;

		GetLobbyStateActor(PreviousLobbyState)->EndSublevel();

		if (InLobbySubstate == ETowerLobbySubstateEnum::HeroManagementView
			|| InLobbySubstate == ETowerLobbySubstateEnum::HeroShop)
		{
			//enable to load Lobby_in sublevel without transition sequence and character things.
			LobbySubstate = InLobbySubstate;
			UE_LOG(LogLobby, Log, TEXT("ATowerLobbyGameMode::ChangeState: TODO: support transitioning into substate"));
			GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::OnSubstateChanged);
			GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject((AOutdoorLobbyStateActor*)GetLobbyStateActor(PreviousLobbyState), &AOutdoorLobbyStateActor::LeaveSublevel);
			UMapFunctionLibrary::LoadSubLevel(FName("Lobby_In"), FName("LobbyInLoadFinished"), this, FTransform());			
			return;
		}
		else if (InLobbySubstate == ETowerLobbySubstateEnum::RaidTowerBoss)
		{
			LobbySubstate = InLobbySubstate;
			GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject(this, &ATowerLobbyGameMode::OnSubstateChanged);
			if(PreviousLobbyState == ETowerLobbyStateEnum::Outdoor)
				GetLobbyStateActor(CurrentLobbyState)->OnStateChanged.AddUObject((AOutdoorLobbyStateActor*)GetLobbyStateActor(PreviousLobbyState), &AOutdoorLobbyStateActor::LeaveSublevel);
			UMapFunctionLibrary::LoadSubLevel(FName("Lobby_In"), FName("LobbyInLoadFinished"), this, FTransform());
			return;
		}

		// Load the next level now, but invisibly
		UUIFunctionLibrary::ShowWaitingPopup(true);

		GetLobbyStateActor(CurrentLobbyState)->OnEnterSublevelComplete.AddDynamic(this, &ATowerLobbyGameMode::OnEnterInTransitionFinished);
		GetLobbyStateActor(CurrentLobbyState)->EnterSublevel();
	}
}

void ATowerLobbyGameMode::OnSubstateChanged()
{
	GetCurrentLobbyStateActor()->OnStateChanged.RemoveAll(this);

	OnStateChanged.Broadcast();
}

void ATowerLobbyGameMode::SetVisibilityAll(bool IsVisible)
{
	//GetLobbyStateActor(CurrentLobbyState)->SetVisibilityAll(IsVisible);
}

void ATowerLobbyGameMode::LobbyInLoadFinished()
{
	auto TowerGameMode = RGAMEINSTANCE(this)->GetTowerLobbyGameMode();
	auto IndoorActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	UMapFunctionLibrary::LoadSubLevel(IndoorActor->GetCurrentLightingInFName(), FName("LobbyInDayTimeLoadFinished"), this, FTransform());
}

void ATowerLobbyGameMode::LobbyInDayTimeLoadFinished()
{
	if (LobbySubstate == ETowerLobbySubstateEnum::HeroManagementView)
	{
		GetLobbyStateActor(CurrentLobbyState)->ChangeState(ETowerLobbySubstateEnum::HeroManagementView);
	}
	else if (LobbySubstate == ETowerLobbySubstateEnum::HeroShop)
	{
		GetLobbyStateActor(CurrentLobbyState)->ChangeState(ETowerLobbySubstateEnum::HeroShop);
	}
	else if (LobbySubstate == ETowerLobbySubstateEnum::RaidTowerBoss)
	{
		GetLobbyStateActor(CurrentLobbyState)->ChangeState(ETowerLobbySubstateEnum::RaidTowerBoss);
	}
}

void ATowerLobbyGameMode::ChangeUITowerRaid()
{
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_TowerBossLobby);
}

void ATowerLobbyGameMode::StartTransition()
{
	if (CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
	{
		//why? if current view target is not outdoorcamera , camera blending appears
		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(OutdoorLobbyStateAct->OutdoorCameraActor));

		m_LobbyOut_Transition->SequencePlayer->SetPlaybackPosition(0.0);
		m_LobbyOut_Transition->SequencePlayer->Play();
	}
	else if (CurrentLobbyState == ETowerLobbyStateEnum::Indoor)
	{
		if (IsEntryTransition)
		{
			m_FirstLogin_Transition->SequencePlayer->SetPlaybackPosition(0.0);
			m_FirstLogin_Transition->SequencePlayer->Play();
		}
		else
		{
			m_LobbyIn_Transition->SequencePlayer->SetPlaybackPosition(0.0);
			m_LobbyIn_Transition->SequencePlayer->Play();
		}
	}
}

void ATowerLobbyGameMode::OnTransitionFinished()
{
	GetLobbyStateActor(CurrentLobbyState)->BeginSublevel();

	OnStateChanged.Broadcast();

	IsEntryTransition = false;
}

void ATowerLobbyGameMode::CampaignInTransition()
{
	FScriptDelegate funcDelegate;
 	funcDelegate.BindUFunction(this, "OnFinishiedDelegate");
 	m_CampaignIn_Transition->SequencePlayer->OnFinished.AddUnique(funcDelegate);
 	m_CampaignIn_Transition->SequencePlayer->SetPlaybackPosition(0.0f);
 	m_CampaignIn_Transition->SequencePlayer->Play();
}

void ATowerLobbyGameMode::OnCampaignInTransitionFinished()
{
	OnStateChanged.Broadcast();
}

void ATowerLobbyGameMode::StartEntry()
{
	if (CurrentLobbyState == ETowerLobbyStateEnum::Indoor)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<ACameraActor>(IndoorLobbyStateAct->IndoorNormalViewCameraActor));
	else if (CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<ACameraActor>(OutdoorLobbyStateAct->OutdoorCameraActor));
	
	OnEntryFinished();
}

void ATowerLobbyGameMode::OnEntryFinished()
{
	GetLobbyStateActor(CurrentLobbyState)->BeginSublevel();

	OnStateChanged.Broadcast();
}

ABaseLobbyStateActor* ATowerLobbyGameMode::GetCurrentLobbyStateActor()
{
	return GetLobbyStateActor(CurrentLobbyState);
}

bool ATowerLobbyGameMode::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
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

void ATowerLobbyGameMode::OnEnterInEntryComplete()
{
	GetLobbyStateActor(CurrentLobbyState)->OnEnterSublevelComplete.RemoveAll(this);

	GetLobbyStateActor(CurrentLobbyState)->SetVisibilityAll(true);

	OnEnterInEntryCompleteDelegate.Broadcast();

	StartEntry();
}

void ATowerLobbyGameMode::FirstLightMapLoadedCompleted()
{
	GetLobbyStateActor(CurrentLobbyState)->EnterSublevel();
}

ETowerLobbyLighting ATowerLobbyGameMode::GetRandomLighting()
{
	// random
	int32 ChosenIndex = FMath::RandHelper((int32)ETowerLobbyLighting::Max);
	return ETowerLobbyLighting::Night;//(ETowerLobbyLighting)ChosenIndex;
}

void ATowerLobbyGameMode::OnEnterInTransitionFinished()
{
	GetLobbyStateActor(CurrentLobbyState)->OnEnterSublevelComplete.RemoveAll(this);

	UUIFunctionLibrary::ShowWaitingPopup(false);

	StartTransition();
}

void ATowerLobbyGameMode::OnFinishiedDelegate()
{
	m_CampaignIn_Transition->SequencePlayer->OnFinished.RemoveAll(this);
	OnCampaignInTransitionFinished();
}

void ATowerLobbyGameMode::SetLastMessage(FString nick, FString message)
{
	auto manager = RGAMEINSTANCE(this)->ChatManager;
	manager->m_nick = nick;
	manager->m_message = message;
}

ABaseLobbyStateActor* ATowerLobbyGameMode::GetLobbyStateActor(ETowerLobbyStateEnum InLobbyStateEnum)
{
	int index = (int32)InLobbyStateEnum - 1;
	if (LobbyStateActors.IsValidIndex(index))
	{
		if (InLobbyStateEnum == ETowerLobbyStateEnum::Campaign)
			return LobbyStateActors[index];

		if (InLobbyStateEnum != ETowerLobbyStateEnum::Outdoor && InLobbyStateEnum != ETowerLobbyStateEnum::Indoor)
			return nullptr;

		// TODO: this messed up
		return LobbyStateActors[index];
	}
	return nullptr;
}
