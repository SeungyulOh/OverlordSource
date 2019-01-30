// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UpdateService/Auth/AuthState.h"
#include "RAuth.h"
#include "CreateAccountState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCreateAccountState : public UAuthState
{
	GENERATED_BODY()

public:
	void OnEnter() override;
	void OnLeave() override;
	
	UFUNCTION()
	void LoginUIOnClickCreateAccount(FString NickName);

	UFUNCTION()
	void OnCreateAccountRp(ECreateAccountResult Result);
};
