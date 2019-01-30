// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ReadyToGoLobbyState.h"

#include "RAuth.h"
//#include "UI/UI_Login/OnlineIdentityLoginUI.h"
#include "AuthStateMachine.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "Network/RTSManager.h"
#include "Utils/UIFunctionLibrary.h"

void UReadyToGoLobbyState::OnEnter()
{

	// DEBUG: disabled for debugging jetsam issue
#if 0
	// Show web in app Publisher Notice View
	URAuthIdProviderInterface* IdProvider = RAuth->GetIdProvider();
	if (IsValid(IdProvider))
	{
		IdProvider->ShowNoticeView();
	}
#endif

	StateMachine->UIWaitingState();

	URGameInstance* RGameInstance = StateMachine->RGameInstance;

	UE_LOG(LogBattle, Log, TEXT("InitBeforeLaunch"));
	RGameInstance->InitBeforeLaunch();

	UE_LOG(LogBattle, Log, TEXT("InitAfterLaunch"));
	RGameInstance->InitAfterLaunch();
	/// BEGIN LOGIN sequence delegates
	RGameInstance->HttpClient->OnAvatarGameDataRpDelegate.Clear();
	RGameInstance->HttpClient->OnAvatarGameDataRpDelegate.AddDynamic(this, &UReadyToGoLobbyState::AvatarGameDataRp);

	//RGameInstance->ConnectRealTimeMod(URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);
	//RGameInstance->RealTimeModManager->Connect(RGameInstance, 
	//URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);

	if(RGameInstance->OptionManager->IsPlayIntroMovie())
	{
		RGameInstance->HttpClient->OnEndIntroMovieDelegate.Clear();
		RGameInstance->HttpClient->OnEndIntroMovieDelegate.AddDynamic(this, &UReadyToGoLobbyState::OnEndIntroMovie);

		RGameInstance->OptionManager->SetPlayIntroMovie(false);
		UUIFunctionLibrary::ShowUIMediaFrameWork(FName(TEXT("IntroMovie")));
	}
	else
	{
		OnEndIntroMovie();
	}
}

void UReadyToGoLobbyState::OnLeave()
{
	URGameInstance* RGameInstance = StateMachine->RGameInstance;

	RGameInstance->HttpClient->OnAvatarGameDataRpDelegate.Clear();
	RGameInstance->HttpClient->OnEndIntroMovieDelegate.Clear();

	RGameInstance->RealTimeModManager->Connect(RGameInstance, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] SetTimerRemainGame >> TcpServerHost %s TcpServerPort %s"), *URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, *URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);
}


void UReadyToGoLobbyState::ReadyToGoToLobby()
{
	GoToLobby();
}

void UReadyToGoLobbyState::GoToLobby()
{
	URGameInstance* RGameInstance = StateMachine->RGameInstance;
	RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
}

/// Login procedure. Will be cleaned up.
/*
==================================================================================================================
Delegate
==================================================================================================================
*/

void UReadyToGoLobbyState::AvatarGameDataRp()
{
	ReadyToGoToLobby();
}

void UReadyToGoLobbyState::OnEndIntroMovie()
{
	/*URGameInstance* RGameInstance = StateMachine->RGameInstance;
	if(RGameInstance->HttpClient->OnEndIntroMovieDelegate.IsBound())
	RGameInstance->HttpClient->OnEndIntroMovieDelegate.RemoveDynamic(this, &UReadyToGoLobbyState::OnEndIntroMovie);*/
	
	//
	UPacketFunctionLibrary::AVATAR_GAME_DATA_RQ();
	//ReadyToGoToLobby();
}