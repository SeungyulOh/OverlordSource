// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TowerLobbyLevelScriptActor.h"

#include "GlobalIntegrated.h"

#include "IndoorLobbyStateActor.h"
#include "OutdoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "CustomActors/BaseSequenceActor.h"
#include "CustomActors/RHeroGacha.h"
#include "UI/RWidgetManager.h"
#include "UI/Shop/UP_HeroShop.h"

#include "Utils/MapFunctionLibrary.h"


void ATowerLobbyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseLobbyStateActor::StaticClass(), OutActors);
	for (AActor* actor : OutActors)
		CurrentMode->LobbyStateActors.Emplace(Cast<ABaseLobbyStateActor>(actor));

	CurrentMode->SetupIndoorLobbyStateActor();
	CurrentMode->SetupOutdoorLobbyStateActor();
	CurrentMode->Initialize_PMap();
}

void ATowerLobbyLevelScriptActor::OnSpawnGacha()
{
	if (bDisableEvent)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	if (IsPendingKill() || World->IsPendingKill())
		return;

	ARHeroGacha::GACHATYPE eType;
	int32 NumGachaHeroList = GachaHeroList.Num();
	switch (NumGachaHeroList)
	{
		case 1:		eType = ARHeroGacha::ONE;		break;
		case 5:		eType = ARHeroGacha::FIVE;		break;
		case 10:	eType = ARHeroGacha::TEN;		break;
		case 11:	eType = ARHeroGacha::ELEVEN;	break;
		default:
			return;
		break;
	}

	FTransform SpawnLocAndRot;
	SpawnLocAndRot.SetLocation(FVector(0.f, 200.f, 300.f));
	HeroGachaActor = World->SpawnActorDeferred<ARHeroGacha>(ARHeroGacha::StaticClass(), SpawnLocAndRot);
	HeroGachaActor->Init();
	HeroGachaActor->GachaHeroList = GachaHeroList;
	HeroGachaActor->SetGachaType(eType);
	HeroGachaActor->FinishSpawning(SpawnLocAndRot);
	
	GachaHeroList.Empty();
}

void ATowerLobbyLevelScriptActor::PauseSequence()
{
	if (bDisableEvent)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;
	ABaseSequenceActor* CurSequenceActor = RGameInstance->SequenceManager->GetCurrentSequenceActor();

	if (CurSequenceActor && CurSequenceActor->IsPlaying())
		CurSequenceActor->Pause();

	if (RGameInstance->RWidgetManager)
	{
		UUP_HeroShop* HeroShop = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
		if (HeroShop)
		{
			HeroShop->SetVisibility_SummonMenus();
		}
	}
}

void ATowerLobbyLevelScriptActor::BGParticleActivate()
{
	if (bDisableEvent)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UUP_HeroShop* HeroShop = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
	if (HeroShop)
	{
		FTransform transform;
		transform.SetTranslation(FVector(-60.f, 300.f, -7.1f));
		transform.SetScale3D(FVector(0.2, 0.1, 0.2));
		if (HeroShop->PS_Component == nullptr)
		{
			HeroShop->PS_Component = UGameplayStatics::SpawnEmitterAtLocation(RGameInstance->GetWorld(), HeroShop->PS, transform);
			if (HeroShop->PS_Component)
				HeroShop->PS_Component->ActivateSystem();
		}
	}
}

void ATowerLobbyLevelScriptActor::EndProcess_ShopHero()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (HeroGachaActor)
	{
		for (size_t i = 0; i < HeroGachaActor->HeroCardArray.Num(); ++i)
			HeroGachaActor->HeroCardArray[i]->Destroy();

		HeroGachaActor->HeroCardArray.Empty();
		HeroGachaActor->Destroy();
		HeroGachaActor = nullptr;
	}
	SetDiableEvent(true);
	RGameInstance->SequenceManager->ClearSequence();
}

void ATowerLobbyLevelScriptActor::TransitionStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;

	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TowerGameMode)
	{
		ULevelSequencePlayer* SequencePlayer = TowerGameMode->m_FirstLogin_Transition->SequencePlayer;
		if (SequencePlayer && SequencePlayer->IsPlaying())
			SequencePlayer->Pause();

		ULevelSequencePlayer* InSequencePlayer = TowerGameMode->m_LobbyIn_Transition->SequencePlayer;
		if (InSequencePlayer && InSequencePlayer->IsPlaying())
			InSequencePlayer->Pause();

		ULevelSequencePlayer* OutSequencePlayer = TowerGameMode->m_LobbyOut_Transition->SequencePlayer;
		if (OutSequencePlayer && OutSequencePlayer->IsPlaying())
			OutSequencePlayer->Pause();



		if (TowerGameMode->PreviousLobbyState == ETowerLobbyStateEnum::Outdoor)
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_Out_NightTime_Ls");
			else
				LightName = FName("Lobby_Out_DayTime_Ls");

			UMapFunctionLibrary::UnLoadSubLevel(LightName, FName("UnLoadLightCompleted"), this);

		}
		else
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_In_NightTime_Ls");
			else
				LightName = FName("Lobby_In_DayTime_Ls");

			UMapFunctionLibrary::UnLoadSubLevel(LightName, FName("UnLoadLightCompleted"), this);
		}
	}
}

void ATowerLobbyLevelScriptActor::TransitionFinished()
{
	ATowerLobbyGameMode* mode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	mode->OnTransitionFinished();
	if (mode->CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(mode->OutdoorLobbyStateAct->OutdoorCameraActor));

}

void ATowerLobbyLevelScriptActor::UnLoadLightCompleted()
{
	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TowerGameMode)
	{
		if (TowerGameMode->PreviousLobbyState == ETowerLobbyStateEnum::Outdoor)
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_In_NightTime_Ls");
			else
				LightName = FName("Lobby_In_DayTime_Ls");

			UMapFunctionLibrary::LoadSubLevel(LightName, FName("LoadLightCompleted"), this);
		}
		else
		{
			FName LightName = TEXT("");
			if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
				LightName = FName("Lobby_Out_NightTime_Ls");
			else
				LightName = FName("Lobby_Out_DayTime_Ls");

			UMapFunctionLibrary::LoadSubLevel(LightName, FName("LoadLightCompleted"), this);
		}
		
	}
	
}

void ATowerLobbyLevelScriptActor::LoadLightCompleted()
{
	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TowerGameMode)
	{
		ULevelSequencePlayer* SequencePlayer = TowerGameMode->m_FirstLogin_Transition->SequencePlayer;
		if (SequencePlayer && SequencePlayer->IsPaused())
			SequencePlayer->Play();

		ULevelSequencePlayer* InSequencePlayer = TowerGameMode->m_LobbyIn_Transition->SequencePlayer;
		if (InSequencePlayer && InSequencePlayer->IsPaused())
			InSequencePlayer->Play();

		ULevelSequencePlayer* OutSequencePlayer = TowerGameMode->m_LobbyOut_Transition->SequencePlayer;
		if (OutSequencePlayer && OutSequencePlayer->IsPaused())
			OutSequencePlayer->Play();

		UWorld* world = TowerGameMode->GetWorld();
		if (world)
		{
			TArray<class ULevelStreaming*> LSArray = world->StreamingLevels;
			for (size_t i = 0; i < LSArray.Num(); ++i)
			{
				if (LSArray[i]->bShouldBeLoaded && LSArray[i]->bShouldBeVisible)
				{
					UE_LOG(LogClass, Log, TEXT("%s"), *LSArray[i]->GetName());
				}
			}
		}

		TowerGameMode->GetLobbyStateActor(TowerGameMode->PreviousLobbyState)->LeaveSublevel();
	}

	
}

void ALobbyOutScriptActor::BeginPlay()
{
	Super::BeginPlay();
}
