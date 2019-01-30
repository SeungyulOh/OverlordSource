// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "EngineMinimal.h"
#include "SoundManager.generated.h"


class URGameInstance;
class ABGMActor;
/**
 * 
 */
UCLASS()
class PROJECTR_API USoundManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	static USoundManager* GetInstancePtr();

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	static void		ReleaseSingleton();

	void		Initialize(void);
	//void		SetGameInstance(URGameInstance* RGameInstance){ GameInstance = RGameInstance; }

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		PlayBattleBGMusic(FName MapKey);
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		PlayBGM(int32 BGMIndex);
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		PauseBGM();
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		StopBGM();
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		ResumeBGM();

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		FadeInBGM();

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		FadeOutBGM();

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		SetBGMVolume(float InVolume);

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		SetEffectVolume(float InVolume);

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		SetVolume(float InVolume, bool bSaveVolume = true, bool bExe = true);

	float		GetComputeVolume();
	float		GetComputeEffectVolume();

	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		RestoreVolume();
	UFUNCTION(BlueprintCallable, Category = SoundManager)
	void		ClearSoundList();

private:
	USoundManager();
	void							CheckBGMActor();
	//float							BGSoundVolume = 1.0f;
	
	float							SoundVolume = 1.0f;
	float							EffectSoundVolume = 1.0f;
	TWeakObjectPtr<ABGMActor>		BGMActorPtr;
	UPROPERTY()
	TMap<FName, USoundCue* >		SoundList;
	static USoundManager*			InstancePtr;
	bool							IsMute = false;

public:
	UFUNCTION()
	void PlaySound2D(FName SoundKey, float VolumeMultiplier, float PitchMultiplier);

	void PlayComputeSound2D(UWorld* InWorld, USoundBase* InSoundBase);
	void PlayComputeSoundAtLocation(UWorld* InWorld, USoundBase* InSoundBase, FVector Location);

	UFUNCTION()
	void SpawnSoundAttached(FName SoundKey, USceneComponent* Comp, FName SocketName, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier);
	UFUNCTION()
	void PlaySoundAtLocation(FName SoundKey, FVector Location, float VolumeMultiplier, float PitchMultiplier);
};