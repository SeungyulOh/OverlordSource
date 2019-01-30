// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_MediaFrameWorkPlayer.generated.h"

class UButton;

class UMediaPlayer;
class UFileMediaSource;
// class UMediaSoundWave;


USTRUCT(BlueprintType)
struct PROJECTR_API FSubtitleInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleInfo)
	bool							IsValid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleInfo)
	float							StartTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleInfo)
	float							EndTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubtitleInfo)
	FText							Subtitle;



};



/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_MediaFrameWorkPlayer : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	bool OpenUrl(FString url, bool visibleSkipBtn = true);	

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void OnSkipButtonClick();

	UFUNCTION()
	void StopMedia();

private:
	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void CallbackMediaPlayStart();

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void CallbackMediaPlayEnd();

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void CallbackMediaClosed();

	

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void CallbackMediaPlayerMediaOpend(FString url);

	UFUNCTION(BlueprintCallable, Category = UP_MediaFrameWorkPlaye)
	void CallbackMediaPlayerMediaOpendFailed(FString url);


public:
	void SetSubtitleInfo(float startTime, float endTime, FName stringID);
	void ClearSubtitleInfos();

	void ChangeSequenceSubtitleType();
	void SetSequenceSubtitle(FName InSubtitle);

private:
	void ShowSubtitle();
	void CloseSubtitle();

	void CheckAutoPlay();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	FString				NewUrl;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	bool				IsPlaying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UButton*			SkipButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UTextBlock*			Text_Subtitle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UMediaPlayer*		NativeMediaPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UFileMediaSource*	NativeFileMediaSource = nullptr;
	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
// 	UMediaSoundWave*	NativeMediaSoundWave = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UImage*				Image_BG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MediaFrameWorkPlaye)
	UImage*				Image_MovieViewer;

private:
	UPROPERTY()
	TArray<FSubtitleInfo>		SubtitleInfos;
	float						PlaybackEllapsedTime = 0.0f;	
	bool						IsReadySubtitle = false;

	UPROPERTY()
	FSubtitleInfo				CurrentSubtitleInfo;

	bool						IsSequenceType = false;
	
	FTimerHandle				AutoSkipTimeHandler;
};
