// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AuthState.h"
#include "RAuth.h"
#include "GameServerLoginState.generated.h"



/**
 * Handles Game Server login, caching of clientkey maybe
 */
UCLASS()
class PROJECTR_API UGameServerLoginState : public UAuthState
{
	GENERATED_BODY()

public:
	void OnEnter() override;
	void OnLeave() override;

	UFUNCTION()
	void RAuthOnLoginComplete(ELoginResult InLoginResult, FString InClientKey);

	UFUNCTION()
	void OnLoginFail();

private:
	void DefaultPopupFail(FText& InCaption, FText& InContents);

private:
	int32 ReTry = 0;
};
