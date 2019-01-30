// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

#include "SharedConstants/GlobalConstants.h"
#include "LoadingSceneManager.generated.h"


class UObjectPool;
//class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API ULoadingSceneManager : public UObject
{
	GENERATED_BODY()

public:
	void		Initialize(void);
	void		ShowLoadingScene(EGamePlayModeTypeEnum InContentType = EGamePlayModeTypeEnum::VE_Campaign);
	void		HideLoadingScene(float fTime = 0.0f);

	void		OnHideLoadingScene();
//	void		CacheForAsync();
	void		SetProgressBarPercent(float InValue);

	FORCEINLINE void SetLoadingImageID(const FName InImageID)	{ LoadingImageID = InImageID; }
	FORCEINLINE void SetLoadingTextID(const FName InTextID)		{ LoadingTextID = InTextID; }

	FORCEINLINE bool IsNoneScene() { return (CurrentContentType == EGamePlayModeTypeEnum::VE_None); }

public:
	/*
	Asset references
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LoadingSceneManager)
	FName LoadingScreenWidgetKey = TEXT("WBP_LoadingScene");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LoadingSceneManager )
	UUserWidget* Widget = nullptr;

private:
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen();

private:
	UPROPERTY()
	bool		FadeOutMode = false;

	UPROPERTY()
	float		FadeTime = 1.0f;

	UPROPERTY()
	FTimerHandle	OnHideLoadingSceneHandler;

	UPROPERTY()
	EGamePlayModeTypeEnum	CurrentContentType;

	UPROPERTY()
	FName			LoadingImageID;

	UPROPERTY()
	FName			LoadingTextID;
};