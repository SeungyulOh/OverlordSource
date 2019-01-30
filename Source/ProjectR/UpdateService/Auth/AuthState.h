// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AuthState.generated.h"

class URAuth;
class UAuthStateMachine;
class UOnlineIdentityLoginUI;

/**
 * 
 */
UCLASS()
class PROJECTR_API UAuthState : public UObject
{
	GENERATED_BODY()
	
public:

	// Initialize
	virtual void Init(UAuthStateMachine* InStateMachine);

	virtual void OnEnter() PURE_VIRTUAL(UAuthState::OnEnter, return;);
	virtual void OnLeave() PURE_VIRTUAL(UAuthState::OnLeave, return;);
	
protected:
	UPROPERTY()
	UAuthStateMachine* StateMachine = nullptr;

	UPROPERTY()
	URAuth* RAuth = nullptr;

	UPROPERTY()
	UOnlineIdentityLoginUI* LoginUI = nullptr;
};
