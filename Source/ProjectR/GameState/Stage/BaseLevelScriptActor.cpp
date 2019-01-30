// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseLevelScriptActor.h"

#include "ProjectRGameMode.h"
#include "SharedConstants/GlobalConstants.h"

#include "UtilFunctionIntegrated.h"
//#include "GameState/Stage/LevelActor.h"
#include "UI/RWidgetManager.h"

#include "UI/UIDefs.h"
#include "GlobalIntegrated.h"

#include "DirectionTrackManager.h"



#include "Character/AnimInstance/AnimInstanceDefs.h"


ABaseLevelScriptActor::ABaseLevelScriptActor(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;	
}

void ABaseLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();	
	
	GameInstance = RGAMEINSTANCE(this);
	if (IsInPersistentLevel())
	{
		GameInstance->CurrentStateBeginState();
		GameInstance->EditorRefresh();
	}
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	GameInstance->OptionManager->SetQualityPreset(RGameUserSetting->QualityPreset);		
	for (auto& postProcessVolume : GetWorld()->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			InitialVignetteIntensity = PPVolume->Settings.VignetteIntensity;
			break;
		}
	}
}

void ABaseLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	GameInstance->PartyManager->ClearPartyList();
	GameInstance = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ABaseLevelScriptActor::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
}

/*
LevelSequence Event
*/

// Use : Stage_Clear_Sequence
void ABaseLevelScriptActor::StageClearSequenceEnd()
{
	UUtilFunctionLibrary::PauseSequence(GameInstance);
	UUtilFunctionLibrary::StageClearSequenceEnd(GameInstance);
}

// Use : CamVictory_Sequence / Arena_End_Sequence / Colosseum_End_Sequence
void ABaseLevelScriptActor::ShowVictoryAnim()
{
	UUtilFunctionLibrary::ShowVictoryAnim(GameInstance);
}

// Use : CamVictory_Sequence
void ABaseLevelScriptActor::EndVictorySequence()
{
	UUtilFunctionLibrary::PauseSequence(GameInstance);

	if (IsValid(GameInstance) && IsValid(GameInstance->EventManager))
	{
		GameInstance->SetIsVictoryState(false);
		GameInstance->EventManager->OnGameMode_ReadyToResult.Broadcast();
	}
}

void ABaseLevelScriptActor::BossName()
{
	UUtilFunctionLibrary::BossName(nullptr);
}

void ABaseLevelScriptActor::RaidBossName()
{
	UUtilFunctionLibrary::RaidBossName(GameInstance, true);
}

void ABaseLevelScriptActor::RaidBossNameHide()
{
	UUtilFunctionLibrary::RaidBossName(GameInstance, false);
}

void ABaseLevelScriptActor::SequenceSubTitle(FName InTableKey)
{
	GameInstance->SequenceManager->SetSubTitle(InTableKey);
}

// Use : Colosseum_Start_Sequence
void ABaseLevelScriptActor::ColosseumStart_LeftAni()
{
	UUtilFunctionLibrary::ShowColosseumTeamAnimMontageEvent(GameInstance, ANIM_MONTAGE_EVENT_BATTLEREADY, false);
}

// Use : Colosseum_Start_Sequence
void ABaseLevelScriptActor::ColosseumStart_RightAni()
{
	UUtilFunctionLibrary::ShowColosseumTeamAnimMontageEvent(GameInstance, ANIM_MONTAGE_EVENT_BATTLEREADY, true);
}
// Use : GetNewHero_Sequence
void ABaseLevelScriptActor::GetNewHero_StartAni()
{
	//if (IsValid(GameInstance))
	{
		GameInstance->EventManager->OnNewHeroSequence_AnimationStart.Broadcast();
	}
}
// Use : GetNewHero_Sequence
void ABaseLevelScriptActor::GetNewHero_PauseSequence()
{
	UUtilFunctionLibrary::PauseSequence(GameInstance);

	//if (IsValid(GameInstance))
	{
		GameInstance->EventManager->OnNewHeroSequence_AnimationEnd.Broadcast();
	}
}

void ABaseLevelScriptActor::PauseSequence()
{
	UUtilFunctionLibrary::PauseSequence(GameInstance);
}

void ABaseLevelScriptActor::PauseCSSequence()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;
	ABaseSequenceActor* CurSequenceActor = RGameInstance->SequenceManager->GetCurrentSequenceActor();

	if (CurSequenceActor && CurSequenceActor->IsPlaying())
	{
		CurSequenceActor->Pause();
		uint8 CinematicSequence = RGameInstance->RealTimeModManager->GetCinematicSequence();
		URTSManager::GetInstancePtr()->ACK_CINEMATIC_DONE(CinematicSequence);
	}
		
}

void ABaseLevelScriptActor::ShowVS()
{
	UUIFunctionLibrary::ReadyToStartUI_PVP();
	RGAMEINSTANCE(GEngine)->EventManager->OnUI_ReadyToStart.Clear();
}

void ABaseLevelScriptActor::PositionSetting()
{
	FVector LeftSide = FVector(-1950.f, 0.f, 10.f);
	FVector RightSide = FVector(1950.f, 0.f, 10.f);

	ARaidRGameMode* RaidGM = Cast<ARaidRGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (RaidGM->IsValidLowLevel())
	{
		UAnimStateComponent* AnimStateComponent = nullptr;
		if (RaidGM->MyLeader)
		{
			RaidGM->MyLeader->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			/*Fix later*/
			if (RaidGM->MyLeader->GetActorLocation().X < 0) // left side
				RaidGM->MyLeader->SetActorLocation(LeftSide);
			else
				RaidGM->MyLeader->SetActorLocation(RightSide);
			/**/

			AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(RaidGM->MyLeader);
			if (AnimStateComponent)
				AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
		}

		if (RaidGM->EnemyLeader)
		{
			RaidGM->EnemyLeader->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			/*Fix later*/
			if (RaidGM->EnemyLeader->GetActorLocation().X < 0) // left side
				RaidGM->EnemyLeader->SetActorLocation(LeftSide);
			else
				RaidGM->EnemyLeader->SetActorLocation(RightSide);
			/**/

			AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(RaidGM->EnemyLeader);
			if (AnimStateComponent)
				AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
		}
	}
}

void ABaseLevelScriptActor::CreateWidget()
{
	//if (IsValid(GameInstance))
	{
		GameInstance->EventManager->OnSequence_CreateWidget.Broadcast();
	}
}


