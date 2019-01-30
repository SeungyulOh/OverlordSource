// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI_Setting_New.generated.h"

class UCheckBox_Radio;
class UCanvasPanel_RadioGroup;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUI_Setting_New : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//ui function
	UFUNCTION()
		void OnClickButton_Close();
	UFUNCTION()
		void OnClickButton_Guest();
	UFUNCTION()
		void OnClickButton_Copy();
	UFUNCTION()
		void OnClickButton_Logout();
	UFUNCTION()
		void OnClickRadio_GameOption(bool isChecked);
	UFUNCTION()
		void OnClickRadio_AccountInfo(bool isChecked);
	UFUNCTION()
		void OnValueChanged_GameSound(float changeVal);
	UFUNCTION()
		void OnValueChanged_EffectSound(float changeVal);
	UFUNCTION()
		void OnClickRadio_Graphic_Low(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Graphic_Mid(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Graphic_High(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Graphic_VeryHigh(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Camera_Off(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Camera_On(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Intro_Off(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Intro_On(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Dialog_Off(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Dialog_On(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Platform_Google(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Platform_FaceBook(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Language_Korean(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Language_English(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Language_China(bool isChecked);
	UFUNCTION()
		void OnClickRadio_Language_Taiwan(bool isChecked);

	UFUNCTION()
	void PopupLogoutOK();
	UFUNCTION()
	void PopupLogoutCancel();
	UFUNCTION()
	void goToLoginPage();

	void SetSlider_GameSound();
	void SetSlider_EffectSound();
	void SetRadioGroup_Graphic();
	void SetRadioGroup_Camera();
	void SetRadioGroup_Intro();
	void SetRadioGroup_Dialog();
	void SetRadioGroup_Platform();
	void SetRadioGroup_Language();
	
	///////property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UButton* Button_Close = nullptr;
	//main tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_GameOption = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_AccountInfo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UWidgetSwitcher* WidgetSwitcher_MainLayout = nullptr;

	//slider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
	USlider* Slider_GameSoundVolume = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
	USlider* Slider_EffectSoundVolume = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
	UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_Graphic = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Graphic_Low = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Graphic_Mid = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Graphic_High = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Graphic_VeryHigh = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_Camera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Camera_Off = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Camera_On = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_Intro = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Intro_Off = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Intro_On = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_Dialog = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Dialog_Off = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Dialog_On = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UTextBlock* Text_NickName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UButton* Button_Guest = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UTextBlock* Text_AccountID = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UButton* Button_Copy = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_AccountPlatform = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Google = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_FaceBook = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UButton* Button_Logout = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCanvasPanel_RadioGroup* CanvasPanel_RadioGroup_Language = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Korean = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_English = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_China = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUI_Setting_New)
		UCheckBox_Radio* CheckBox_Radio_Taiwan = nullptr;
};
