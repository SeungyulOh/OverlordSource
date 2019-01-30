// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "SharedConstants/GlobalConstants.h"
#include "RGameUserSetting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URGameUserSetting : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	void Initialize();

	FString& GetUserID()					{ return SavedDevID; }
	void SetUserID(FString InSavedDevID);	//{ SavedDevID = InSavedDevID; }
	bool IsValidUserId()					{ return !(SavedDevID.IsEmpty()); }

	int32 GetPublisher()					{ return CachePublisher; } //del

	static URGameUserSetting* GetRGameUserSettings();

	virtual void SaveSettings() override;
	
	bool IsEditorState = true;
	void SetIsEditorState(bool InValue) { IsEditorState = InValue; }

public:
	/** Saved Dev ID ("n14"). */
	UPROPERTY(config)
	FString SavedDevID = TEXT("");

	//UPROPERTY(config)
	//FString ClientID;

	UPROPERTY(config)
	FString NickName = TEXT("");

	UPROPERTY(config)
	int32 CachePublisher = 0;

	//UPROPERTY(config)
	//FString DeviceID;

	UPROPERTY(config)
	ELanguageType LaguageType = ELanguageType::VE_KOR;

	UPROPERTY(config)
	bool				IsUltimateMatinee = true;
	UPROPERTY(config)
	bool				IsStoryMatinee = true;
	//UPROPERTY(config)
	//bool				IsModulatedShadow = false;
	UPROPERTY(config)
	bool				MuteSound = false;
	UPROPERTY(config)
	float				SoundVolume = 1.0f;

	UPROPERTY(config)
	float				EffectSoundVolume = 1.0f;

	UPROPERTY(config)
	bool				ShowEffect = true;

	UPROPERTY(config)
	EVideoQualityEnum	QualityPreset = EVideoQualityEnum::VE_VERYHIGH;

	//UPROPERTY(config)
	//bool				IsSelectImmediately = true;

	//InsideValue
	UPROPERTY(config)
	bool				AutoMode = false;

	UPROPERTY(config)
	EAIControlModeEnum	AutoSkillMode = EAIControlModeEnum::VE_ManualMode;

	UPROPERTY(config)
	bool				SlotAction = false;

	/*UPROPERTY(config)
	bool				DeckHero1 = true;

	UPROPERTY(config)
	bool				DeckHero2 = true;

	UPROPERTY(config)
	bool				DeckHero3 = true;

	UPROPERTY(config)
	bool				DeckHero4 = true;

	UPROPERTY(config)
	bool				DeckHero5 = true;

	UPROPERTY(config)
	int32				TutorialState = 0;

	UPROPERTY(config)
	float				Tooltip_DelayTime = 0.3f;*/

	UPROPERTY(config)
	bool				FirstExecute = true;

	UPROPERTY(config)
	FString				UserNick;

	// Item Inventory saved Filter states
	// saved format is <category name>+<factor name> for each factor. e.g. Grade+Unique
	UPROPERTY(config)
	TArray<FString> ItemInventoryFilter_Weapons_CheckedFactors;
	UPROPERTY(config)
	TArray<FString> ItemInventoryFilter_Defense_CheckedFactors;
	UPROPERTY(config)
	TArray<FString> ItemInventoryFilter_Accessories_CheckedFactors;
	UPROPERTY(config)
	TArray<FString> ItemInventoryFilter_HeroPieces_CheckedFactors;


	UPROPERTY(config)
	bool			bPlayIntroMovie = true;
};
