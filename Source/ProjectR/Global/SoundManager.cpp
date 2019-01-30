// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SoundManager.h"
#include "RGameInstance.h"
#include "GlobalIntegrated.h"

#include "Table/ResourceSoundTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BGMActor.h"
#include "Runtime/Engine/Public/AudioDevice.h"

USoundManager::USoundManager()
{

}

USoundManager* USoundManager::InstancePtr = nullptr;

void USoundManager::PlaySound2D(FName SoundKey, float VolumeMultiplier, float PitchMultiplier)
{
	USoundManager* SoundManager = GetInstancePtr();
	if (SoundManager->SoundList.Contains(SoundKey))
	{
		VolumeMultiplier = GetComputeEffectVolume();
		UGameplayStatics::PlaySound2D(RGAMEINSTANCE(this)->GetWorld(), SoundManager->SoundList[SoundKey], VolumeMultiplier, PitchMultiplier);
	}
	else
	{
		FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
		if (info)
		{
			TArray<FSoftObjectPath> Assets;
			Assets.AddUnique(info->Sound.ToSoftObjectPath());
			URAsyncLoader::GetInstancePtr()->LoadAsyncResource(Assets, FStreamableDelegate::CreateLambda([=] {
				SoundManager->SoundList.Emplace(SoundKey, info->Sound.Get());
				SoundManager->PlaySound2D(SoundKey, VolumeMultiplier, PitchMultiplier);
				
			}));
		}
		else
		{
			UE_LOG(LogSound, Log, TEXT("Invalid SoundKey"));
		}
	}
}

void USoundManager::PlayComputeSound2D(UWorld* InWorld, USoundBase* InSoundBase)
{
	UGameplayStatics::PlaySound2D(InWorld, InSoundBase, GetComputeEffectVolume());
}

void USoundManager::PlayComputeSoundAtLocation(UWorld* InWorld, USoundBase* InSoundBase, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(InWorld, InSoundBase, Location, GetComputeEffectVolume());
}

void USoundManager::SpawnSoundAttached(FName SoundKey, USceneComponent* Comp, FName SocketName, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier)
{
	if (GetInstancePtr()->SoundList.Contains(SoundKey))
	{
		VolumeMultiplier = GetComputeEffectVolume();
		UGameplayStatics::SpawnSoundAttached(GetInstancePtr()->SoundList[SoundKey], 
			Comp, SocketName, FVector(ForceInit), EAttachLocation::KeepRelativeOffset, false, VolumeMultiplier, PitchMultiplier);
	}
	else
	{
		FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
		if (info)
		{
			TArray<FSoftObjectPath> Assets;
			Assets.AddUnique(info->Sound.ToSoftObjectPath());
			URAsyncLoader::GetInstancePtr()->LoadAsyncResource(Assets, FStreamableDelegate::CreateLambda([=] {
				USoundManager::GetInstancePtr()->SoundList.Emplace(SoundKey, info->Sound.Get());
				USoundManager::GetInstancePtr()->SpawnSoundAttached(SoundKey, 
					Comp, SocketName, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier);
			}));
		}
	}
}

void USoundManager::PlaySoundAtLocation(FName SoundKey, FVector Location, float VolumeMultiplier, float PitchMultiplier)
{
	USoundManager* SoundManager = GetInstancePtr();
	if (SoundManager->SoundList.Contains(SoundKey))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UWorld* World = RGameInstance->IsValidLowLevel() ? RGameInstance->GetWorld() : nullptr;
		VolumeMultiplier = GetComputeEffectVolume();
		UGameplayStatics::PlaySoundAtLocation(World, SoundManager->SoundList[SoundKey], Location, VolumeMultiplier, PitchMultiplier);
	}
	else
	{
		FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
		if (info)
		{
			TArray<FSoftObjectPath> Assets;
			Assets.AddUnique(info->Sound.ToSoftObjectPath());
			URAsyncLoader::GetInstancePtr()->LoadAsyncResource(Assets, FStreamableDelegate::CreateLambda([=] {
				SoundManager->SoundList.Emplace(SoundKey, info->Sound.Get());
				SoundManager->PlaySoundAtLocation(SoundKey, Location, VolumeMultiplier, PitchMultiplier);
			}));
		}
		else
		{
			UE_LOG(LogSound, Log, TEXT("Invalid SoundKey"));
		}
	}
}

USoundManager* USoundManager::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< USoundManager >((UObject*)GetTransientPackage(), USoundManager::StaticClass());
		InstancePtr->Initialize();
		InstancePtr->AddToRoot();
	}

	return InstancePtr;
}

void USoundManager::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
		InstancePtr = nullptr;
	}
}

void USoundManager::Initialize(void)
{
	SoundList.Empty();
	BGMActorPtr = nullptr;
	if (RGAMEINSTANCE(this)->OptionManager != nullptr)
	{
		SoundVolume = RGAMEINSTANCE(this)->OptionManager->GetSoundVolume();
		EffectSoundVolume = RGAMEINSTANCE(this)->OptionManager->GetEffectSoundVolume();
		IsMute = RGAMEINSTANCE(this)->OptionManager->IsMuteSound();

		if (IsMute)
		{
			SetVolume(0.0f, false);
		}
		else
		{
			SetVolume(SoundVolume);
			//RestoreVolume();
		}
	}
	else
	{
		SetVolume(1.0f, false);
	}
}

void USoundManager::PlayBattleBGMusic(FName MapKey)
{
	FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(MapKey);
	if (MapTableInfo)
	{
		PlayBGM(MapTableInfo->BGSound);
	}
}

void USoundManager::PlayBGM(int32 BGMIndex)
{	
	CheckBGMActor();

	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->PlayBGMIndex(BGMIndex);
		BGMActorPtr->SetVolume(GetComputeVolume());
	}
}

void USoundManager::PauseBGM()
{
	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->PauseBGM();
	}
}

void USoundManager::StopBGM()
{
	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->StopBGM();		
	}
}

void USoundManager::ResumeBGM()
{
	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->ResumeBGM();
	}
}

void USoundManager::FadeInBGM()
{
	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->FadeInBGM();
	}
}

void USoundManager::FadeOutBGM()
{
	if (BGMActorPtr.IsValid())
	{
		BGMActorPtr->FadeOutBGM();
	}
}


void USoundManager::CheckBGMActor()
{
	if (BGMActorPtr.IsValid())
		return;

	if (RGAMEINSTANCE(this) == nullptr)
		return;

#if DISABLE_SPAWN_BGM_ACTOR
	return;
#endif

//	BGMActorPtr = RGAMEINSTANCE(this)->GetWorld()->SpawnActor<ABGMActor>(ABGMActor::StaticClass());
}


void USoundManager::SetBGMVolume(float InVolume)
{
	CheckBGMActor();

	if (BGMActorPtr.IsValid())
		BGMActorPtr->SetVolume(InVolume);
}


void USoundManager::SetEffectVolume(float InVolume)
{
	EffectSoundVolume = InVolume;
	//Does not manage the effects sound
}

void USoundManager::SetVolume(float InVolume, bool bSaveVolume, bool bExe)
{
	if (bSaveVolume)
	{
		SoundVolume = InVolume;
	}

	//if (BGMActorPtr.IsValid())
	//	BGMActorPtr->SetVolume(InVolume);

	
	if (bExe)
	{
		if (IsValid(GEngine))
		{
			FAudioDevice* Device = GEngine->GetActiveAudioDevice();
			if (Device)
			{
				Device->SetPlatformAudioHeadroom(InVolume);
			}
		}
	}
}

float USoundManager::GetComputeVolume()
{
	//todo(qsmname) - del
	if (RGAMEINSTANCE(this)->OptionManager != nullptr)
	{
		SoundVolume = RGAMEINSTANCE(this)->OptionManager->GetSoundVolume();
		IsMute = RGAMEINSTANCE(this)->OptionManager->IsMuteSound();
	}

	if (IsMute)
		return 0.0f;

	return SoundVolume;
}

float USoundManager::GetComputeEffectVolume()
{
	//todo(qsmname) - del
	if (RGAMEINSTANCE(this)->OptionManager != nullptr)
	{
		EffectSoundVolume = RGAMEINSTANCE(this)->OptionManager->GetEffectSoundVolume();
		IsMute = RGAMEINSTANCE(this)->OptionManager->IsMuteSound();
	}

	if (IsMute)
		return 0.0f;

	return EffectSoundVolume;
}

void USoundManager::RestoreVolume()
{
	SetVolume(SoundVolume);
}

void USoundManager::ClearSoundList()
{
	for (auto SoundIt = GetInstancePtr()->SoundList.CreateIterator(); SoundIt; ++SoundIt)
	{
		SoundIt->Value = nullptr;
	}
	GetInstancePtr()->SoundList.Empty();
}
