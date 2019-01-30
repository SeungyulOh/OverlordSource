// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Setting.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "UI/RWidgetManager.h"

#include "UpdateService/Auth/RAuth.h"
#include "UtilFunctionIntegrated.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "Global/RGameInstance.h"
#include "GameState/TowerLobbyState.h"


void UUP_Setting::NativeConstruct()
{
	//todo : test(CommonStringTable Keys are "KOREAN" and "ENGLISH")
	LanguageStringIDs.Empty(4);
	LanguageStringIDs.Emplace(ELanguageType::VE_KOR, FName(*(FString("KOREAN"))));
	LanguageStringIDs.Emplace(ELanguageType::VE_ENG, FName(*(FString("ENGLISH"))));
	LanguageStringIDs.Emplace(ELanguageType::VE_CHN, FName(*(FString("CHINA"))));
	LanguageStringIDs.Emplace(ELanguageType::VE_TWN, FName(*(FString("TAIWAN"))));
	

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->OptionManager))
	{
		SelectedLanguageType = RGameInstance->OptionManager->GetLanguageType();
		
		IsUltimateMatinee = RGameInstance->OptionManager->IsUltimateMatineeView();
		IsStoryMatinee = RGameInstance->OptionManager->IsStoryMatineeView();
		bMuteSound = RGameInstance->OptionManager->IsMuteSound();
		SoundVolume = RGameInstance->OptionManager->GetSoundVolume();
		DoParticlesShow = RGameInstance->OptionManager->IsShowParticle();
		IsSelectImmediately = RGameInstance->OptionManager->IsSelectImmediately();
		QualityPreset = RGameInstance->OptionManager->GetQualityType();

		if (RGameInstance->CurState == EGameStateEnum::VE_Lobby)
		{
			if (TutorialButton)
			{
				TutorialButton->SetIsEnabled(true);
				TutorialButton->OnClicked.RemoveDynamic(this, &UUP_Setting::OnTutorial);
				TutorialButton->OnClicked.AddDynamic(this, &UUP_Setting::OnTutorial);
			}
		}
		else
		{
			if (TutorialButton)
			{
				TutorialButton->SetIsEnabled(false);
			}
		}
	}

	Super::NativeConstruct();

	InvalidateData();
}


void UUP_Setting::OnClick_ButtonOk()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		//UI_Language
		if (IsValid(RGameInstance->OptionManager))
		{
			RGameInstance->OptionManager->SetLanguageType(SelectedLanguageType);
			RGameInstance->OptionManager->SetUltimateMatinee(IsUltimateMatinee);
			RGameInstance->OptionManager->SetStoryMatinee(IsStoryMatinee);
			RGameInstance->OptionManager->MuteSound(bMuteSound);
			RGameInstance->OptionManager->SetSoundVolume(SoundVolume);
			RGameInstance->OptionManager->SetSelectImmediately(IsSelectImmediately);
			RGameInstance->OptionManager->SetQualityPreset(QualityPreset);
			RGameInstance->OptionManager->ApplyGameOption();
		}

		if (IsValid(RGameInstance->RWidgetManager))
		{
			RGameInstance->RWidgetManager->RefreshLocalizingTextForVisibleUI();
		}

				
		if (bMuteSound)
		{
			USoundManager::GetInstancePtr()->SetVolume(0.0f, false);
		}
		else
		{
			USoundManager::GetInstancePtr()->SetVolume(SoundVolume);
		}
	}

	OnClick_ButtonCancel();
}

void UUP_Setting::OnClick_ButtonCancel()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (IsValid(RGameInstance->RWidgetManager))
		{
			RGameInstance->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_Setting);
		}

		if (IsValid(RGameInstance->OptionManager) )
		{
			bool bMute = RGameInstance->OptionManager->IsMuteSound();
			if (bMute)
			{
				USoundManager::GetInstancePtr()->SetVolume(RGameInstance->OptionManager->GetSoundVolume(), true, false);
				USoundManager::GetInstancePtr()->SetVolume(0.0f, false);
			}
			else
			{
				USoundManager::GetInstancePtr()->SetVolume(RGameInstance->OptionManager->GetSoundVolume());
			}			
		}
	}
}

void UUP_Setting::SetLanguage(FString InLanguageValue) 
{
	FName LaguangeValue		= FName(*InLanguageValue);
	SelectedLanguageType	= *(LanguageStringIDs.FindKey(LaguangeValue));
}

void UUP_Setting::SetLanguageEnum(ELanguageType InLaguageType)
{
	SelectedLanguageType = InLaguageType;
}

FName UUP_Setting::GetLanguageStringID(ELanguageType InLaguageType)
{
	if (LanguageStringIDs.Contains(InLaguageType) == true)
	{
		return LanguageStringIDs[InLaguageType];
	}

	return FName();
}

void UUP_Setting::SetUltimateMatinee(bool IsVisible)
{
	IsUltimateMatinee = IsVisible;

	if (RGAMEINSTANCE(GEngine)->OptionManager)
	{
		RGAMEINSTANCE(GEngine)->OptionManager->SetUltimateMatinee(IsVisible);
	}
}

void UUP_Setting::SetStoryMatinee(bool IsAllVisible)
{
	IsStoryMatinee = IsAllVisible;
}

void UUP_Setting::SetModulatedShadow(bool IsOn)
{
	IsModulatedShadow = IsOn;
}

//by danzang
void UUP_Setting::ShowParticles(bool show)
{
	DoParticlesShow = show;
}

void UUP_Setting::MuteSound(bool IsOn)
{
	bMuteSound = IsOn;

	auto RGameInstance = RGAMEINSTANCE(this);

	if (bMuteSound)
	{
		USoundManager::GetInstancePtr()->SetVolume(0.0f, false);
	}
	else
	{
		USoundManager::GetInstancePtr()->RestoreVolume();
	}
}

void UUP_Setting::SetSoundVolume(float InVolume)
{
	SoundVolume = InVolume;
	USoundManager::GetInstancePtr()->SetVolume(InVolume);
}

void UUP_Setting::SetSelectImmediately(bool IsOn)
{
	IsSelectImmediately = IsOn;
}

void UUP_Setting::SetQuality(EVideoQualityEnum InQualityType)
{
	this->QualityPreset = InQualityType;
	InvalidateData();
}

void UUP_Setting::InvalidateData()
{
	if (IsValid(LanguageRadioGruop))
	{
		LanguageRadioGruop->SetActiveRadioIndex((int32)SelectedLanguageType);
	}

	if (CheckBox_UltimateMatinee_On && CheckBox_UltimateMatinee_Off)
	{
		if (IsUltimateMatinee == true)
			CheckBox_UltimateMatinee_On->RadioButtonOnCheckStateChanged(true);
		else
			CheckBox_UltimateMatinee_Off->RadioButtonOnCheckStateChanged(true);
	}

	if (CheckBox_StoryMatinee_On && CheckBox_StoryMatinee_Off)
	{
		if (IsStoryMatinee == true)
			CheckBox_StoryMatinee_On->RadioButtonOnCheckStateChanged(true);
		else
			CheckBox_StoryMatinee_Off->RadioButtonOnCheckStateChanged(true);
	}

	if (CheckBox_QualityVeryHigh && CheckBox_QualityHigh && CheckBox_QualityMid && CheckBox_QualityLow)
	{
		switch (QualityPreset)
		{
			case EVideoQualityEnum::VE_VERYHIGH:
				CheckBox_QualityVeryHigh->RadioButtonOnCheckStateChanged(true);
				break;
			case EVideoQualityEnum::VE_HIGH:
				CheckBox_QualityHigh->RadioButtonOnCheckStateChanged(true);
				break;
			case EVideoQualityEnum::VE_MID:
				CheckBox_QualityMid->RadioButtonOnCheckStateChanged(true);
				break;
			case EVideoQualityEnum::VE_LOW:
				CheckBox_QualityLow->RadioButtonOnCheckStateChanged(true);
				break;
		}
	}

	if (CheckBox_Sound_Off && CheckBox_Sound_On)
	{
		if (bMuteSound == true)
			CheckBox_Sound_Off->RadioButtonOnCheckStateChanged(true);
		else
			CheckBox_Sound_On->RadioButtonOnCheckStateChanged(true);
	}
	
	if (CheckBox_SpawnRule_RecoverRef && CheckBox_SpawnRule_ImmediatelyRef)
	{
		if (IsSelectImmediately == true)
			CheckBox_SpawnRule_RecoverRef->RadioButtonOnCheckStateChanged(true);
		else
			CheckBox_SpawnRule_ImmediatelyRef->RadioButtonOnCheckStateChanged(true);
	}
}

void UUP_Setting::goToLoginPage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RealTimeModManager->ClearRealTimeModData();
	URTSManager::GetInstancePtr()->Disconnect();
	auto lobbyTowerState = Cast<UTowerLobbyState>(RGameInstance->GetCurrentState());
	if(lobbyTowerState)
		lobbyTowerState->SetIsEntry(true);
	RGameInstance->CurrentState = nullptr;
	RGameInstance->CurState = EGameStateEnum::VE_None;

	UMapFunctionLibrary::LoadLevel(FName("Startup_Map"));

}

void UUP_Setting::OnClickLogout()
{
	URAuth::GetInstancePtr()->Logout();

	UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(FText::FromString(TEXT("Notice")),
		FText::FromString(TEXT("Your account login information has been cleared.")),
		ECommonPopupType::VE_OK);

	if (IsValid(commonPopup))
	{
		commonPopup->OnClickOK.AddDynamic(this, &UUP_Setting::goToLoginPage);
	}
}

void UUP_Setting::OnClickRegisterCoupon()
{
	auto RGameInstance = RGAMEINSTANCE(this);

	// TODO
	/*
	if (IsValid(RGameInstance->KakaoManager))
	{
		RGameInstance->KakaoManager->ShowCouponPopup();
	}
	*/
}

void UUP_Setting::OnClickUnregister()
{
#if API_NZIN
	URAuth::GetInstancePtr()->GetPublisherAPI()->Unregister();
#endif
}

void UUP_Setting::OnTutorial()
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	RGameInstance->ChangeState(EGameStateEnum::VE_NewTutorial);
}