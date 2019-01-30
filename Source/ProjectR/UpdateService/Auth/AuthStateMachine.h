// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Network/ClientProtocols.h"
#include "UI/ContentDownload/UP_Popup.h"
#include "AuthStateMachine.generated.h"

class URGameInstance;
class UAuthState;
class UOnlineIdentityLoginUI;
class UContentDownloadUI;
class URAuth;
class UMediaPlayer;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAuthStateEnum: uint8
{
	VE_None,
	VE_Prelogin,
	VE_PublisherLogin,
	VE_GameServerLogin,
	VE_CreateAccount,
	VE_Patch,
	VE_ReadyToGoLobby,
	VE_RequestPermissions,
	VE_Max,
};

/**
 * Transition between AuthStates and store shared variables
 */
UCLASS()
class PROJECTR_API UAuthStateMachine : public UObject
{
	GENERATED_BODY()
	
public:

	// Initialize and start
	UFUNCTION(BlueprintCallable, Category = "RAuth")
	void Init(URGameInstance* InRGameInstance);

	void Shutdown();

	//===================================================================================
	//
	// Interface to states
	//
	//===================================================================================

	void ChangeState(EAuthStateEnum InStateEnum);

	
	UUP_Popup*	ShowPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType);
	FMulticastScriptDelegate& ShowPopup(FText InCaption, FText InContents);
	void HidePopup();

	//===================================================================================
	//
	// Shared variables
	//
	//===================================================================================


	//// Publisher
	FString IdpCode;
	FString AccessToken;
	FString ClientId;

	void InvalidateIdpCredentials();

	
	//// Game Server
	FString ClientKey;

	void InvalidateClientKey();

	void UIPatchState();
	void UIWaitingState();
	void UIClickState();

public:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	UOnlineIdentityLoginUI* LoginUI = nullptr;

private:
	UPROPERTY()
	TMap<EAuthStateEnum, UAuthState*> StateTable;
	
	UPROPERTY()
	UAuthState* CurrentState = nullptr;

	UPROPERTY()
	UUP_Popup* PopupUI = nullptr;

	UPROPERTY()
	URAuth* RAuth = nullptr;

	void AddState(EAuthStateEnum InStateEnum, UAuthState* InState);

};
