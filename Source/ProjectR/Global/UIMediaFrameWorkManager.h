// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "UIMediaFrameWorkManager.generated.h"

//class URGameInstance;
class UUP_MediaFrameWorkPlayer;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUIMediaFrameWorkManager : public UObject
{
	GENERATED_BODY()
	

public:
	UUIMediaFrameWorkManager();

	void Initialize(/*URGameInstance* InRGameInstance*/);

	bool IsPlaying();

public:
	UFUNCTION(BlueprintCallable, Category = UIMediaFrameWorkManager)
	void ShowMedia(FName MediaName, bool IsVisibleSkipButton = true);
	
	UFUNCTION(BlueprintCallable, Category = UIMediaFrameWorkManager)
	void CloseMedia();

	UFUNCTION()
	void CallbackTimer();

	UFUNCTION()
	void TestEnd(FString InUrl);

public:
	void ClearTimerHandle();
	void CheckAmbientSound();
	void SetSubtitles(FName MediaName);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UIMediaFrameWorkManager)
	UUP_MediaFrameWorkPlayer*		MediaPlayerWidget = nullptr;
	
private:
	bool CreateUIWidget();
	

private:
// 	UPROPERTY()
// 	URGameInstance*		RGameInstance = nullptr;

	UPROPERTY()
	FTimerHandle		TimerHandle_MoviePlayingCheck;

	UPROPERTY()
	FName				MediaAmbientSoundName = FName(TEXT("MediaAmbientSound"));

	const float			CALLBACK_WAIT_TIME = 1.0f;

	UPROPERTY()
	FString				CurrentUrl;
};
