// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UP_Setting.generated.h"

class UCheckBox_Radio;
class UCanvasPanel_RadioGroup;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Setting : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void OnClick_ButtonOk();

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void OnClick_ButtonCancel();

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetLanguage(FString InLanguageValue);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetLanguageEnum(ELanguageType InLaguageType);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	FName GetLanguageStringID(ELanguageType InLaguageType);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetUltimateMatinee(bool IsVisible);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetStoryMatinee(bool IsAllVisible);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetModulatedShadow(bool IsOn);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void ShowParticles(bool show);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void MuteSound(bool IsOn);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetSoundVolume(float InVolume);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void SetSelectImmediately(bool IsOn);

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void	SetQuality(EVideoQualityEnum InQualityType);

	void InvalidateData() override;

	UFUNCTION()
		void goToLoginPage();
		

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCanvasPanel_RadioGroup*	LanguageRadioGruop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_UltimateMatinee_On = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_UltimateMatinee_Off = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_StoryMatinee_On = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_StoryMatinee_Off = nullptr;
	
	/*
	Quality
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_QualityVeryHigh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_QualityHigh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_QualityMid = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_QualityLow = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_ModulatedShadow_On = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_ModulatedShadow_Off = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_ShowParticles = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_HideParticles = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_Sound_On = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*			CheckBox_Sound_Off = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*		CheckBox_SpawnRule_RecoverRef = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UCheckBox_Radio*		CheckBox_SpawnRule_ImmediatelyRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	UButton*				TutorialButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	bool						bMuteSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Setting)
	float						SoundVolume = 1.0f;


	// Auth
	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void OnClickLogout();

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void OnClickRegisterCoupon();

	UFUNCTION(BlueprintCallable, Category = UP_Setting)
	void OnClickUnregister();

private:
	UFUNCTION()
	void OnTutorial();

private:
	TMap<ELanguageType, FName>	LanguageStringIDs;
	
	ELanguageType				SelectedLanguageType = ELanguageType::VE_MAX;
	
	//EVideoQualityEnum			QualityPreset = EVideoQualityEnum::VE_VERYHIGH;
	EVideoQualityEnum			QualityPreset;
	bool						IsUltimateMatinee = false;
	bool						IsStoryMatinee = false;
	bool						IsModulatedShadow = false;
	bool						DoParticlesShow = false;	
	bool						IsSelectImmediately = false;
};
