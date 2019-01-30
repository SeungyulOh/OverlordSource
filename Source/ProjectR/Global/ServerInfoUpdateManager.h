// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ServerInfoUpdateManager.generated.h"

//class URGameInstance;

const float GetAvatarInfoInterval = 300.0f;

/**
 * 
 */
UCLASS()
class PROJECTR_API UServerInfoUpdateManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(/*URGameInstance* InRGameInstance*/);

	UFUNCTION()
	void SetTimer();

	virtual void BeginDestroy();

private:
	UFUNCTION()
	void RefreshAvatarInfo();

private:
// 	UPROPERTY()
// 	URGameInstance*	RGameInstance = nullptr;
	

	/*
	TimeHandle Event for Server Notify( Avatarinfo, Server Notify, etc..)
	*/
	FTimerHandle		AvatarInfoEventTimeHandle;
};
  