// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TutorialState.h"
#include "MapFunctionLibrary.h"
#include "Global/TableManager.h"
#include "Table/CampaignTableInfo.h"
#include "../IGameState.h"
#include "Global/RGameInstance.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "Global/LoadingSceneManager.h"
#include "UtilFunctionLibrary.h"




UTutorialState::UTutorialState()
{
	StateType = EGameStateEnum::VE_Tutorial;
}

void UTutorialState::Enter()
{
	RGAMEINSTANCE(GEngine)->RWidgetManager->RemoveAllUserWidget();

	/*Need Resource Cache here before loading Map*/
	UResourceCacheManager* ResourceCacheManager = RGAMEINSTANCE(GEngine)->ResourceCacheManager;
	if (IsValid(ResourceCacheManager))
	{
		ResourceCacheManager->Clear();

		FStreamableDelegate Callback;
		Callback.BindLambda([ResourceCacheManager]() {
			ResourceCacheManager->OnAsyncLoadedCompleted.ExecuteIfBound();

			UGameplayStatics::OpenLevel(RGAMEINSTANCE(GEngine)->GetWorld(), TEXT("IngameTutorial_P"));
		});

		RGAMEINSTANCE(GEngine)->LoadingSceneManager->ShowLoadingScene(EGamePlayModeTypeEnum::VE_Campaign);
		ResourceCacheManager->cache_TutorialMap(Callback);
	}
	/**/
}


void UTutorialState::Leave()
{
	
}

void UTutorialState::BeginState()
{
	Super::BeginState();
}

