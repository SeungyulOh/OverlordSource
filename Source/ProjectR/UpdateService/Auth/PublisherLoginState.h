// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AuthState.h"
#include "PublisherAPIInterface.h"
#include "PublisherLoginState.generated.h"

/**
 * Handles IDP/DevID Login process
 */
UCLASS()
class PROJECTR_API UPublisherLoginState : public UAuthState
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	
	UFUNCTION()
	void OnPanelTouchClicked();
	
	UFUNCTION()
	void OnCheckPlatformAccount(ELoginResult InResut, FString InClientKey);

	UFUNCTION()
	void LoginUIOnClickIdpLogin(FString IdpCode);

	void TryLogin(FString InPublisherType, FString InUserID, FString InClientId, FString InAccessToken);

	UFUNCTION()
	void LoginUIOnClickDevLogin(FString DevID);

	UFUNCTION()
	void RAuthOnIdpLoginComplete(int32 IdpLoginResult, FString IdpCode, FString ClientId, FString AccessToken, bool IsInReview, FString GameServerAddr, FString CdnAddr);
	
	//void ShowPanelLogin();

private:
	FDelegateHandle OnLoginSuccessDelegateHandle;
};
