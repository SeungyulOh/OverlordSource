// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RGameUserSetting.h"
#include "SaveObject/RSaveGame.h"



void URGameUserSetting::Initialize()
{
	MinResolutionScale = 10.0f;

	auto RSaveGame = RGAMEINSTANCE(this)->GetSaveGame();
	if (RSaveGame)
	{
		LaguageType = RSaveGame->LaguageType;
		IsUltimateMatinee = RSaveGame->IsUltimateMatinee;
		IsStoryMatinee = RSaveGame->IsStoryMatinee;
		MuteSound = RSaveGame->MuteSound;
		SoundVolume = RSaveGame->SoundVolume;
		EffectSoundVolume = RSaveGame->EffectSoundVolume;
		QualityPreset = RSaveGame->QualityPreset;
		AutoMode = RSaveGame->AutoMode;
		AutoSkillMode = RSaveGame->AutoSkillMode;
		SlotAction = RSaveGame->SlotAction;
		bPlayIntroMovie = RSaveGame->bPlayIntroMovie;
	}
}


void URGameUserSetting::SetUserID(FString InSavedDevID)
{
	SavedDevID = InSavedDevID;
}


URGameUserSetting* URGameUserSetting::GetRGameUserSettings()
{
	return Cast< URGameUserSetting>(Super::GetGameUserSettings());
}

void URGameUserSetting::SaveSettings()
{
	UpdateVersion();
	Super::SaveSettings();

	auto RSaveGame = RGAMEINSTANCE(this)->GetSaveGame();
	if (RSaveGame)
	{
		RSaveGame->LaguageType = LaguageType;
		RSaveGame->IsUltimateMatinee = IsUltimateMatinee;
		RSaveGame->IsStoryMatinee = IsStoryMatinee;
		RSaveGame->MuteSound = MuteSound;
		RSaveGame->SoundVolume = SoundVolume;
		RSaveGame->EffectSoundVolume = EffectSoundVolume;
		RSaveGame->QualityPreset = QualityPreset;
		RSaveGame->AutoMode = AutoMode;
		RSaveGame->AutoSkillMode = AutoSkillMode;
		RSaveGame->SlotAction = SlotAction;
		RSaveGame->bPlayIntroMovie = bPlayIntroMovie;
		
		RSaveGame->Save();
	}

	/*FString Path = this->GetPathName();
	FString FileName(TEXT("GameUserSettings.ini"));
	this->UpdateDefaultConfigFile(FileName);
	this->UpdateGlobalUserConfigFile();*/
}
