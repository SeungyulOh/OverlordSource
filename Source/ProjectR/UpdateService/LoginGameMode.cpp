// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LoginGameMode.h"
#include "Slate/SceneViewport.h"
#include "Auth/AuthStateMachine.h"
#include "GlobalIntegrated.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/RWidgetManager.h"
#include "MediaPlayer.h"

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALoginGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(AuthStateMachine))
	{
		AuthStateMachine->Shutdown();
		AuthStateMachine = nullptr;
	}
	
	// Below are dubious
	//GameInstance->SequenceManager->ClearSequence();

	//GameInstance->UnLoadSubLevel(TEXT("Login"));

	USoundManager::GetInstancePtr()->StopBGM();
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (GameInstance->RWidgetManager)
	{
		GameInstance->RWidgetManager->RemoveAllUserWidget();
	}

	Super::EndPlay(EndPlayReason);
}

void ALoginGameMode::OnSetupComplete()
{
	AuthStateMachine = NewObject<UAuthStateMachine>(this);
	URGameInstance* RGameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AuthStateMachine->Init(RGameInstance);
}

UUP_Popup*	ALoginGameMode::ShowExceptionLoginPopup(FText InCaption, FText InContents,
	ECommonPopupType InPopupType /*= ECommonPopupType::VE_OK*/)
{
	if (AuthStateMachine)
	{
		return AuthStateMachine->ShowPopup(InCaption, InContents, InPopupType);
	}
	return nullptr;
}

void ALoginGameMode::LoginMediaOpen(FString InUrl)
{
	if (LoginBackground_Movie)
	{
		LoginBackground_Movie->Play();
	}
	OnSetupComplete();
}
