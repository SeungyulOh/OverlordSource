// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpdateService/Auth/AuthState.h"
#include "RequestPermissionsState.generated.h"

/**
 * Do not enter unless Android
 *
 * Check if Android Permissions are already granted by the user.
 * If so, procede to PublisherLoginState
 * Otherwise, show Permissions Notice to the user, then request permissions
 */
UCLASS()
class PROJECTR_API URequestPermissionsState : public UAuthState
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;
	virtual void OnLeave() override;

	UFUNCTION()
	void OnCheckPermissionsComplete(bool bSuccess, bool bGranted);

	UFUNCTION()
	void OnRequestPermissionsComplete(bool bSuccess, bool bGranted);

private:
	FDelegateHandle OnCheckPermissionsCompleteHandle;
	FDelegateHandle OnRequestPermissionsCompleteHandle;
};
