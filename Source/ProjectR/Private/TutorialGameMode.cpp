// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TutorialGameMode.h"
#include "ProjectRGameMode.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "DirectionTrackManager.h"
#include "Global/LoadingSceneManager.h"
#include "Global/AsyncLoader.h"




ATutorialGameMode::ATutorialGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AStagePlayerController::StaticClass();
}

void ATutorialGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = RGAMEINSTANCE(this);

	if (IsValid(DirectionTrackManager))
		DirectionTrackManager->Initialize();

	if (IsValid(DamageFloaterMgr))
		DamageFloaterMgr->Init();

	RGAMEINSTANCE(GEngine)->LoadingSceneManager->HideLoadingScene();
}

void ATutorialGameMode::EndPlay(const EEndPlayReason::Type Type)
{
	Super::EndPlay(Type);

	ObjectHelper.RemoveObjectAll();
}

void ATutorialGameMode::Tick(float DeltaTime)
{
	if (IsValid(DirectionTrackManager) && DirectionTrackManager->GetSizeHeap())
		DirectionTrackManager->Update(DeltaTime);

	URAsyncLoader::GetInstancePtr()->LoadFx();
}
