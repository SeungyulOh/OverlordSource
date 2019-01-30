// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI_Setting_New.h"

#include "Global/RGameInstance.h"
#include "Global/SoundManager.h"
#include "Global/GameOptionManager.h"
#include "UpdateService/Auth/RAuth.h"

#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UIFunctionLibrary.h"
//#include "Network/RTSManager.h"
#include "Network/PacketDataStructures.h"
#include "MapFunctionLibrary.h"
//#include "GameState/TowerLobbyState.h"
#include "GameInfo/UserInfo.h"
#include "UtilFunctionLibrary.h"

void UUI_Setting_New::NativeConstruct()
{
	Super::NativeConstruct();

	auto RGameInstance = RGAMEINSTANCE(this);

	if (Text_NickName)
	{
		FText accountNick = FText::FromString(RGameInstance->UserInfo->Avatar.nick);
		Text_NickName->SetText(accountNick);
	}
	if (Text_AccountID)
	{
		FText accountID = FText::AsNumber(RGameInstance->UserInfo->kId);
		Text_AccountID->SetText(accountID);
	}

	if (RGameInstance->OptionManager)
	{
		if (CheckBox_Radio_GameOption)
			CheckBox_Radio_GameOption->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_GameOption);

		if (CheckBox_Radio_AccountInfo)
			CheckBox_Radio_AccountInfo->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_AccountInfo);

		if (Slider_GameSoundVolume)
		{
			Slider_GameSoundVolume->OnValueChanged.AddDynamic(this, &UUI_Setting_New::OnValueChanged_GameSound);
			SetSlider_GameSound();
		}

		if (Slider_EffectSoundVolume)
		{
			Slider_EffectSoundVolume->OnValueChanged.AddDynamic(this, &UUI_Setting_New::OnValueChanged_EffectSound);
			//SetSlider_EffectSound();
		}

		if (CheckBox_Radio_Graphic_Low && CheckBox_Radio_Graphic_Mid && CheckBox_Radio_Graphic_High && CheckBox_Radio_Graphic_VeryHigh)
		{
			CheckBox_Radio_Graphic_Low->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_Low);
			CheckBox_Radio_Graphic_Mid->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_Mid);
			CheckBox_Radio_Graphic_High->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_High);
			CheckBox_Radio_Graphic_VeryHigh->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_VeryHigh);
			SetRadioGroup_Graphic();
		}

		if (CheckBox_Radio_Camera_Off && CheckBox_Radio_Camera_On)
		{
			CheckBox_Radio_Camera_Off->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Camera_Off);
			CheckBox_Radio_Camera_On->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Camera_On);
			SetRadioGroup_Camera();
		}

		if (CheckBox_Radio_Intro_Off && CheckBox_Radio_Intro_On)
		{
			CheckBox_Radio_Intro_Off->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Intro_Off);
			CheckBox_Radio_Intro_On->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Intro_On);
			SetRadioGroup_Intro();
		}

		if (CheckBox_Radio_Dialog_Off && CheckBox_Radio_Dialog_On)
		{
			CheckBox_Radio_Dialog_Off->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Dialog_Off);
			CheckBox_Radio_Dialog_On->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Dialog_On);
			SetRadioGroup_Dialog();
		}

		if (CheckBox_Radio_Google && CheckBox_Radio_FaceBook)
		{
			CheckBox_Radio_Google->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Platform_Google);
			CheckBox_Radio_FaceBook->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Platform_FaceBook);
			SetRadioGroup_Platform();
		}

		if (CheckBox_Radio_Korean && CheckBox_Radio_English && CheckBox_Radio_China && CheckBox_Radio_Taiwan)
		{
			CheckBox_Radio_Korean->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Language_Korean);
			CheckBox_Radio_English->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Language_English);
			CheckBox_Radio_China->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Language_China);
			CheckBox_Radio_Taiwan->OnCheckRadioCheck.AddDynamic(this, &UUI_Setting_New::OnClickRadio_Language_Taiwan);
			SetRadioGroup_Language();
		}
	}



	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUI_Setting_New::OnClickButton_Close);

	if (Button_Guest)
		Button_Guest->OnClicked.AddDynamic(this, &UUI_Setting_New::OnClickButton_Guest);

	if (Button_Copy)
		Button_Copy->OnClicked.AddDynamic(this, &UUI_Setting_New::OnClickButton_Copy);

	if (Button_Logout)
		Button_Logout->OnClicked.AddDynamic(this, &UUI_Setting_New::OnClickButton_Logout);

}

void UUI_Setting_New::NativeDestruct()
{
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUI_Setting_New::OnClickButton_Close);

	if (Button_Guest)
		Button_Guest->OnClicked.RemoveDynamic(this, &UUI_Setting_New::OnClickButton_Guest);

	if (Button_Copy)
		Button_Copy->OnClicked.RemoveDynamic(this, &UUI_Setting_New::OnClickButton_Copy);

	if (Button_Logout)
		Button_Logout->OnClicked.RemoveDynamic(this, &UUI_Setting_New::OnClickButton_Logout);

	if (CheckBox_Radio_GameOption)
		CheckBox_Radio_GameOption->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_GameOption);

	if (CheckBox_Radio_AccountInfo)
		CheckBox_Radio_AccountInfo->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_AccountInfo);

	if (Slider_GameSoundVolume)
		Slider_GameSoundVolume->OnValueChanged.RemoveDynamic(this, &UUI_Setting_New::OnValueChanged_GameSound);

	if (Slider_EffectSoundVolume)
		Slider_EffectSoundVolume->OnValueChanged.RemoveDynamic(this, &UUI_Setting_New::OnValueChanged_EffectSound);


	if (CheckBox_Radio_Graphic_Low && CheckBox_Radio_Graphic_Mid && CheckBox_Radio_Graphic_High && CheckBox_Radio_Graphic_VeryHigh)
	{
		CheckBox_Radio_Graphic_Low->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_Low);
		CheckBox_Radio_Graphic_Mid->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_Mid);
		CheckBox_Radio_Graphic_High->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_High);
		CheckBox_Radio_Graphic_VeryHigh->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Graphic_VeryHigh);
	}

	if (CheckBox_Radio_Camera_Off && CheckBox_Radio_Camera_On)
	{
		CheckBox_Radio_Camera_Off->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Camera_Off);
		CheckBox_Radio_Camera_On->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Camera_On);
	}

	if (CheckBox_Radio_Intro_Off && CheckBox_Radio_Intro_On)
	{
		CheckBox_Radio_Intro_Off->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Intro_Off);
		CheckBox_Radio_Intro_On->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Intro_On);
	}

	if (CheckBox_Radio_Dialog_Off && CheckBox_Radio_Dialog_On)
	{
		CheckBox_Radio_Dialog_Off->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Dialog_Off);
		CheckBox_Radio_Dialog_On->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Dialog_On);
	}

	if (CheckBox_Radio_Google && CheckBox_Radio_FaceBook)
	{
		CheckBox_Radio_Google->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Platform_Google);
		CheckBox_Radio_FaceBook->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Platform_FaceBook);
	}

	if (CheckBox_Radio_Korean && CheckBox_Radio_English && CheckBox_Radio_China && CheckBox_Radio_Taiwan)
	{
		CheckBox_Radio_Korean->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Language_Korean);
		CheckBox_Radio_English->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Language_English);
		CheckBox_Radio_China->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Language_China);
		CheckBox_Radio_Taiwan->OnCheckRadioCheck.RemoveDynamic(this, &UUI_Setting_New::OnClickRadio_Language_Taiwan);
	}

	Super::NativeDestruct();
}

void UUI_Setting_New::OnClickButton_Close()
{
	RGAMEINSTANCE(this)->OptionManager->ApplySetting();
	RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Setting);
}

void UUI_Setting_New::OnClickButton_Guest()
{
	UE_LOG(LogClass, Log, TEXT("guest"));
}

void UUI_Setting_New::OnClickButton_Copy()
{
	UE_LOG(LogClass, Log, TEXT("copy"));
}

void UUI_Setting_New::OnClickButton_Logout()
{
	FName Key = FName(TEXT("UI_Logout_Msg_Check"));
	FText Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Key);

	UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(FText::FromString(TEXT("Warning")),
		//FText::FromString(TEXT("Logout?")),
		Contents,
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(commonPopup))
	{
		commonPopup->SetBlockHidePopup();
		commonPopup->OnClickOK.AddDynamic(this, &UUI_Setting_New::PopupLogoutOK);
		commonPopup->OnClickCancel.AddDynamic(this, &UUI_Setting_New::PopupLogoutCancel);
	}
}

void UUI_Setting_New::OnClickRadio_GameOption(bool isChecked)
{
	WidgetSwitcher_MainLayout->SetActiveWidgetIndex(0);
}

void UUI_Setting_New::OnClickRadio_AccountInfo(bool isChecked)
{
	WidgetSwitcher_MainLayout->SetActiveWidgetIndex(1);
}

void UUI_Setting_New::OnValueChanged_GameSound(float changeVal)
{
	RGAMEINSTANCE(this)->OptionManager->SetSoundVolume(changeVal);
	USoundManager::GetInstancePtr()->SetVolume(changeVal);
}

void UUI_Setting_New::OnValueChanged_EffectSound(float changeVal)
{
	RGAMEINSTANCE(this)->OptionManager->SetEffectSoundVolume(changeVal);
	USoundManager::GetInstancePtr()->SetEffectVolume(changeVal);
}

void UUI_Setting_New::OnClickRadio_Graphic_Low(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetQualityPreset(EVideoQualityEnum::VE_LOW);
	RGAMEINSTANCE(this)->OptionManager->ApplyGameOption();
}

void UUI_Setting_New::OnClickRadio_Graphic_Mid(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetQualityPreset(EVideoQualityEnum::VE_MID);
	RGAMEINSTANCE(this)->OptionManager->ApplyGameOption();
}

void UUI_Setting_New::OnClickRadio_Graphic_High(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetQualityPreset(EVideoQualityEnum::VE_HIGH);
	RGAMEINSTANCE(this)->OptionManager->ApplyGameOption();
}

void UUI_Setting_New::OnClickRadio_Graphic_VeryHigh(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetQualityPreset(EVideoQualityEnum::VE_VERYHIGH);
	RGAMEINSTANCE(this)->OptionManager->ApplyGameOption();
}

void UUI_Setting_New::OnClickRadio_Camera_Off(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetUltimateMatinee(false);
}

void UUI_Setting_New::OnClickRadio_Camera_On(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetUltimateMatinee(true);
}

void UUI_Setting_New::OnClickRadio_Intro_Off(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetPlayIntroMovie(false);
}

void UUI_Setting_New::OnClickRadio_Intro_On(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetPlayIntroMovie(true);
}

void UUI_Setting_New::OnClickRadio_Dialog_Off(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetStoryMatinee(false);
}

void UUI_Setting_New::OnClickRadio_Dialog_On(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetStoryMatinee(true);
}

void UUI_Setting_New::OnClickRadio_Platform_Google(bool isChecked)
{
	if (isChecked)
	{
	//	UPacketFunctionLibrary::ADD_REFER_PUBLISHER_RQ(FString publisher, FString code,
	//		FString access_token, FString redirect_uri, FString id, FString pwd);
	}
}

void UUI_Setting_New::OnClickRadio_Platform_FaceBook(bool isChecked)
{

}

void UUI_Setting_New::OnClickRadio_Language_Korean(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetLanguageType(ELanguageType::VE_KOR);
	RGAMEINSTANCE(this)->RWidgetManager->RefreshLocalizingTextForVisibleUI();
}

void UUI_Setting_New::OnClickRadio_Language_English(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetLanguageType(ELanguageType::VE_ENG);
	RGAMEINSTANCE(this)->RWidgetManager->RefreshLocalizingTextForVisibleUI();
}

void UUI_Setting_New::OnClickRadio_Language_China(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetLanguageType(ELanguageType::VE_CHN);
	RGAMEINSTANCE(this)->RWidgetManager->RefreshLocalizingTextForVisibleUI();
}

void UUI_Setting_New::OnClickRadio_Language_Taiwan(bool isChecked)
{
	RGAMEINSTANCE(this)->OptionManager->SetLanguageType(ELanguageType::VE_TWN);
	RGAMEINSTANCE(this)->RWidgetManager->RefreshLocalizingTextForVisibleUI();
}

void UUI_Setting_New::PopupLogoutOK()
{
	FName Key = FName(TEXT("UI_Logout_Ok_Msg_Check"));
	FText Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Key);
	
	UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(FText::FromString(TEXT("Notice")),
		//FText::FromString(TEXT("Your account login information has been cleared.")),
		Contents,
		ECommonPopupType::VE_OK);

	if (IsValid(commonPopup))
	{
		commonPopup->OnClickOK.AddDynamic(this, &UUI_Setting_New::goToLoginPage);
	}
}

void UUI_Setting_New::PopupLogoutCancel()
{
	UUIFunctionLibrary::HideCommonPopup();
}

void UUI_Setting_New::goToLoginPage()
{
	RGAMEINSTANCE(this)->Logout();
	
	UMapFunctionLibrary::LoadLevel(FName("Startup_Map"));
}

void UUI_Setting_New::SetSlider_GameSound()
{
	Slider_GameSoundVolume->SetValue(RGAMEINSTANCE(this)->OptionManager->GetSoundVolume());
}

void UUI_Setting_New::SetSlider_EffectSound()
{
	Slider_EffectSoundVolume->SetValue(RGAMEINSTANCE(this)->OptionManager->GetEffectSoundVolume());
}

void UUI_Setting_New::SetRadioGroup_Graphic()
{
	EVideoQualityEnum quialityType = RGAMEINSTANCE(this)->OptionManager->GetQualityType();
	int32 RadioIndex = int32(quialityType);

	switch (quialityType)
	{
		case EVideoQualityEnum::VE_LOW:
			CanvasPanel_RadioGroup_Graphic->SetActiveRadioIndex(0);
		break;
		case EVideoQualityEnum::VE_MID:
			CanvasPanel_RadioGroup_Graphic->SetActiveRadioIndex(1);
		break;
		case EVideoQualityEnum::VE_HIGH:
			CanvasPanel_RadioGroup_Graphic->SetActiveRadioIndex(2);
		break;
		case EVideoQualityEnum::VE_VERYHIGH:
			CanvasPanel_RadioGroup_Graphic->SetActiveRadioIndex(3);
		break;
		default:
		break;
	}
}

void UUI_Setting_New::SetRadioGroup_Camera()
{
	if (RGAMEINSTANCE(this)->OptionManager->IsUltimateMatineeView())
		CanvasPanel_RadioGroup_Camera->SetActiveRadioIndex(1);
	else
		CanvasPanel_RadioGroup_Camera->SetActiveRadioIndex(0);
}

void UUI_Setting_New::SetRadioGroup_Intro()
{
	if (RGAMEINSTANCE(this)->OptionManager->IsPlayIntroMovie())
		CanvasPanel_RadioGroup_Intro->SetActiveRadioIndex(1);
	else
		CanvasPanel_RadioGroup_Intro->SetActiveRadioIndex(0);
}

void UUI_Setting_New::SetRadioGroup_Dialog()
{
	if (RGAMEINSTANCE(this)->OptionManager->IsStoryMatineeView())
		CanvasPanel_RadioGroup_Dialog->SetActiveRadioIndex(1);
	else
		CanvasPanel_RadioGroup_Dialog->SetActiveRadioIndex(0);
}

void UUI_Setting_New::SetRadioGroup_Platform()
{

}

void UUI_Setting_New::SetRadioGroup_Language()
{
	ELanguageType langType = RGAMEINSTANCE(this)->OptionManager->GetLanguageType();
	switch (langType)
	{
	case ELanguageType::VE_KOR:
		CanvasPanel_RadioGroup_Language->SetActiveRadioIndex(0);
		break;
	case ELanguageType::VE_ENG:
		CanvasPanel_RadioGroup_Language->SetActiveRadioIndex(1);
		break;
	case ELanguageType::VE_CHN:
		break;
	case ELanguageType::VE_TWN:
		break;
	case ELanguageType::VE_MAX:
		break;
	default:
		break;
	}
}
