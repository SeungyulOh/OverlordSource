// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AuthState.h"
#include "PreloginAuthState.generated.h"

/**
 * get the server endpoints
 *
 * GET_TARGET_SERVER
 */
UCLASS()
class PROJECTR_API UPreloginAuthState : public UAuthState
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	
	UFUNCTION()
	void OnPreloginComplete(EPreloginResult Result);

private:

};
