// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "GameOptionManager.generated.h"


class URGameUserSetting;
//class URGameInstance;

/**
 * 
 */
UCLASS(Config=Game)
class PROJECTR_API UGameOptionManager : public UObject
{
	GENERATED_BODY()

public:
	void		Initialize(void);

	//void		SetGameInstance(URGameInstance* GameInstance){ RGameInstance = GameInstance; }

	void		SetUltimateMatinee(bool IsVisible);
	bool		IsUltimateMatineeView();
	void		SetStoryMatinee(bool IsAllVisible);
	bool		IsStoryMatineeView();
	void		SetModulatedShadow(bool IsOn);
	bool		IsModulatedShadowView();
	void		ShowParticle(bool show, UWorld* InWorld);
	void		MuteSound(bool InMuteSound);
	void		SetSoundVolume(float InVolume);
	void		SetEffectSoundVolume(float InVolume);
	bool		DoParticlesShow(UWorld* InWorld);
	bool		IsSelectImmediately();
	void		SetSelectImmediately(bool InSpawnRecover);
	void		SetQualityPreset(EVideoQualityEnum InQualityType);
	float		GetTooltipDelayTime();

	void		SetPlayIntroMovie(bool InbPlay);
	bool		IsPlayIntroMovie();

	void		ApplyGameOption();
	void		ApplySetting();

	// On complete load game play level
	void		ApplyOptionsAfterLoadLevel(UWorld* InWorld);

	bool		IsMuteSound();
	bool		IsShowParticle();
	EVideoQualityEnum GetQualityType();
	float		GetSoundVolume();
	float		GetEffectSoundVolume();

	void		SetBloomIntensity(UWorld* InWorld, float InIntensity = 1.0f );
	void		EnableDynamicShadow(const bool enable);
	void		EnablePostprocess(UWorld* InWorld, bool InEnable);

	void		SetUserNick(FString InNick);
	FString		GetUserNick();

	//////////////////////////////////////////////////////////////////////////
	//UI String Manager
	//////////////////////////////////////////////////////////////////////////
	void			SetLanguageType(ELanguageType InLanguageType);

	UFUNCTION(BlueprintCallable, Category = GameOptionManager)
	static ELanguageType	GetLanguageType();

	
	/*
	for debugging
	*/

	void		DumpMemreport();
	void		SetMemoryCustomWarningHandler();

private:
	void RefreshOption();

private:
	//UPROPERTY()
	//URGameInstance*		RGameInstance = nullptr;						// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..


	//////////////////////////////////////////////////////////////////////////
	//UI String Manager
	//////////////////////////////////////////////////////////////////////////

	
};