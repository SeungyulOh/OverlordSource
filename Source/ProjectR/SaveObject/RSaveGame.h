// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UpdateService/LoginDefs.h"
#include "UpdateService/Struct/OnlineAccountInfo.h"
#include "RSaveGame.generated.h"

/* Bryan
This Class is designed to save our game variables in local device.
*/

USTRUCT(BlueprintType)
struct PROJECTR_API FColosseumSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 WinCount = 0;
	UPROPERTY()
	int32 LoseCount = 0;
	UPROPERTY()
	int32 WinningStreakCount = 0;

	void Clear();
};

/**
 * 
 */
UCLASS()
class PROJECTR_API URSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	FString SaveSlotName;
	uint32 UserIndex;

	UPROPERTY()
	FOnlineAccountInfo AccountInfo;
	
	UPROPERTY()
	FString LastPlayedHeroUD;

	/*
	Colosseum Win/Lose Data Save
	*/
	UPROPERTY()
	FColosseumSaveData ColosseumRecord;



	//RGameUsersetting
	UPROPERTY()
	ELanguageType		LaguageType = ELanguageType::VE_KOR;
	UPROPERTY()
	bool				IsUltimateMatinee = true;
	UPROPERTY()
	bool				IsStoryMatinee = true;
	UPROPERTY()
	bool				MuteSound = false;
	UPROPERTY()
	float				SoundVolume = 1.0f;
	UPROPERTY()
	float				EffectSoundVolume = 1.0f;
	UPROPERTY()
	EVideoQualityEnum	QualityPreset = EVideoQualityEnum::VE_VERYHIGH;
	UPROPERTY()
	bool				AutoMode = false;
	UPROPERTY()
	EAIControlModeEnum	AutoSkillMode = EAIControlModeEnum::VE_ManualMode;
	UPROPERTY()
	bool				SlotAction = false;
	UPROPERTY()
	bool				bPlayIntroMovie = true;

public:
	URSaveGame();

	bool IsAlreadyUserInfo() { return AccountInfo.bIsAcceptTerms; }

	void Save();
	URSaveGame* Load();
};
