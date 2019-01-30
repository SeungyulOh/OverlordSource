// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseSequenceActor.h"


// Sets default values
ABaseSequenceActor::ABaseSequenceActor(const FObjectInitializer& Init)
	: Super(Init)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnPlay.AddDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerPlay);
		SequencePlayer->OnStop.AddDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerStop);
		SequencePlayer->OnPause.AddDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerPause);
	}
}

void ABaseSequenceActor::BeginDestroy()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnPlay.RemoveDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerPlay);
		SequencePlayer->OnStop.RemoveDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerStop);
		SequencePlayer->OnPause.RemoveDynamic(this, &ABaseSequenceActor::OnLevelSequencePlayerPause);
	}

	LevelSequence.Reset();
	ResetBindings();

	Super::BeginDestroy();
}

void ABaseSequenceActor::OnLevelSequencePlayerPlay()
{
#if LOG_SEQUENCE
	UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerPlay %s"), *(SequenceTableKey.ToString()));
#endif
	if (OnSequenceEvent.IsBound())
	{
#if LOG_SEQUENCE
		UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerPlay Broadcast %s"), *(SequenceTableKey.ToString()));
#endif
		OnSequenceEvent.Broadcast(SequenceTableKey);
	}
}

void ABaseSequenceActor::OnLevelSequencePlayerStop()
{
#if LOG_SEQUENCE
	UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerStop %s"), *(SequenceTableKey.ToString()));
#endif
	if (OnSequenceEndEvent.IsBound())
	{
#if LOG_SEQUENCE
		UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerStop Broadcast %s"), *(SequenceTableKey.ToString()));
#endif
		OnSequenceEndEvent.Broadcast(SequenceTableKey);
	}
}

void ABaseSequenceActor::OnLevelSequencePlayerPause()
{
	UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerPause %s"), *(SequenceTableKey.ToString()));
	if (OnSequencePauseEvent.IsBound())
	{
		UE_LOG(LogBattle, Log, TEXT("ABaseSequenceActor::OnLevelSequencePlayerPause Broadcast %s"), *(SequenceTableKey.ToString()));
		OnSequencePauseEvent.Broadcast(SequenceTableKey);
	}
}

// Called every frame
void ABaseSequenceActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABaseSequenceActor::Play(bool bLoop, bool bFirstFrame)
{
	if (IsValid(SequencePlayer))
	{
		if (!SequencePlayer->IsPlaying())
		{
			if (bFirstFrame)
			{
				SequencePlayer->SetPlaybackPosition(0.0f);
			}

			if (bLoop)
			{
				SequencePlayer->PlayLooping();
			}
			else
			{
				SequencePlayer->Play();
			}
		}
		else
		{
			SequencePlayer->SetPlaybackPosition(0.f);
			SequencePlayer->Stop();
			SequencePlayer->Play();
		}
			

	}
}

void ABaseSequenceActor::Stop()
{
	if (IsValid(SequencePlayer))
	{
		//SequencePlayer->SetPlaybackPosition(SequencePlayer->GetLength());
		SequencePlayer->SetPlaybackPosition(SequencePlayer->GetPlaybackStart());
		SequencePlayer->Stop();

		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (PCM)
		{
			PCM->FadeAmount = 0;
			PCM->bEnableFading = false;
		}
	}
}

void ABaseSequenceActor::Pause()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->Pause();
	}
}

void ABaseSequenceActor::Resume()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->Play();
	}
}

bool ABaseSequenceActor::IsPlaying()
{
	if (IsValid(SequencePlayer))
	{
		return SequencePlayer->IsPlaying();
	}
	return false;
}

void ABaseSequenceActor::GotoEndStop()
{
	if (IsValid(SequencePlayer))
	{
		SequencePlayer->GoToEndAndStop();
	}
}
