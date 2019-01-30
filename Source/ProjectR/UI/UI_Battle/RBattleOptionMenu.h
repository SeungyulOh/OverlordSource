// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RBattleOptionMenu.generated.h"


class ABasePlayerController;
class URGameInstance;
class UPlayStateInfo;

/**
 * BP: UC_HUD_Option
 */
UCLASS()
class PROJECTR_API URBattleOptionMenu : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	bool Init();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void ExitButton();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void ExitButtonOK();

	void GoContentsLobby();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void ExitButtonCancel();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void OptionButton();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void PauseButton();

	UFUNCTION(BlueprintCallable, Category = RBattleOptionMenu)
	void ResumeButton();

	void IsPrevProcess();

	UFUNCTION()
	void OnClickExit();

	UFUNCTION()
	void OnOptionMenuOpen();
	UFUNCTION()
	void OnOptionMenuClose();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UWidgetSwitcher*		Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Btn_Pause;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Button_Setting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Button_Exit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Btn_Lobby;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Button_Close;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UButton*				Button_Resume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UVerticalBox*			Popup_Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UImage*					BackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleOptionMenu)
	UPlayStateInfo*			PlayStateInfo = nullptr;
};
