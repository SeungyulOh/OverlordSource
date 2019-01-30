// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UpdateService/Auth/AuthState.h"
#include "PatchState.generated.h"

class UContentDownloadUI;
class UUpdateService;

/**
 * Do not enter if WITH_EDITOR
 */
UCLASS()
class PROJECTR_API UPatchState : public UAuthState
{
	GENERATED_BODY()
	
public:
	void OnEnter() override;
	void OnLeave() override;

	void LaunchContentDownloadUI();

	

	UFUNCTION()
	void OnUpdatePending();

	UFUNCTION()
	void OnUpdateServiceFinish(bool bSuccess, FString Message);

protected:
	UFUNCTION()
	void ContentDownloadUIOnFinish();

private:
	
	UPROPERTY()
	UContentDownloadUI* ContentDownloadUI = nullptr;

	UPROPERTY()
	UUpdateService* UpdateService = nullptr;
};
