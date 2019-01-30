// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectR.h"
#include "UI/RBaseWidget.h"
#include "UpdateService/Auth/RAuth.h"
#include "UI/ContentDownload/ContentDownloadUI.h"
#include "UI/ContentDownload/UP_Popup.h"
#include "OnlineIdentityLoginUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginUIOnClickIdpLogin, FString, IdpCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginUIOnClickDevLogin, FString, DevID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoginUIOnClickCreateAccount, bool, bCancelled, FString, NickName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoginUIOnClickGoToLobby);


class UAuthStateMachine;

/**
 * 
 */
UCLASS()
class PROJECTR_API UOnlineIdentityLoginUI : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();

	/*
	==================================================================================================================
	UI functions
	==================================================================================================================
	*/

	void Show_Panel_Login();
//	void Show_Panel_NickName();
	void Show_Panel_Touch();

	void Show_Panel_Waiting();
	void Hide_Panel_Waiting();

	void HideAllPanels();

	void SetLoginError();

	UContentDownloadUI* ShowContentDownloadUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "LoginUI")
	void OnShowExternalEvent();

public:
	/*
	==================================================================================================================
	Delegate hooks for AuthStateMachine
	==================================================================================================================
	*/

// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "LoginUI")
// 	FLoginUIOnClickIdpLogin OnClickIdpLogin;

	//UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "LoginUI")
	//FLoginUIOnClickDevLogin OnClickDevLogin;

// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "LoginUI")
// 	FLoginUIOnClickCreateAccount OnClickCreateAccount;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "LoginUI")
	FLoginUIOnClickGoToLobby OnPanelTouchClicked;


	/*
	==================================================================================================================
	Widgets
	==================================================================================================================
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginUI")
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginUI")
	UCanvasPanel*			Panel_Waiting = nullptr;


	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "LoginUI")
	UTextBlock*				Text_CopyrightNotice = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnlineIdentityLoginUI|Ref")
	UWidgetSwitcher*		Switcher_SubPage = nullptr;

	// For dev build
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginUI")
	UTextBlock*				Text_DevVersionString = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentDownloadUI)
	TAssetSubclassOf<UUP_Popup> PopupUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentDownloadUI)
	TAssetSubclassOf<UContentDownloadUI> ContentDownloadUIClass;

protected:
	UFUNCTION()
	void RequestFromSubPage();

	UFUNCTION()
	void OnClickButton();

private:
	void RefreshSubPage();

private:
	int32 SubPageIndex = 0;

};
