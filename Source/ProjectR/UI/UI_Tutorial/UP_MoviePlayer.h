// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_MoviePlayer.generated.h"

class UMediaPlayer;
class UMediaPlaylist;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_MoviePlayer : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	/*void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;*/
	
	//void MoviePlayerState(bool InIsHide);
	//void Prepare();
	void Start();
	//void Process();
	UFUNCTION()
	void End();
	UFUNCTION()
	void AutoPlaySkipMovie();

	//bool GetFinishMovie();
	//bool GetCacheMovie();

	//void SetMediaPlayerList();

	UFUNCTION()
	void CheckOpen(FString InUrl);

	UFUNCTION()
	void CheckFail(FString InUrl);

	//void UpdatePlayList();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	UImage* Image_BlockScreen = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	UImage* Image_MovieScreen = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	UMediaPlayer* MediaPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	UMediaPlaylist* MoviePlayList = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	USoundBase*	MovieSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MoviePlayer_Ref)
	UButton* Btn_Skip = nullptr;

private:
	void Play();
	
	void UpdateMediaPlayerInfo();

	void CheckAutoPlay();

private:
	int32 MoviePlayListIndex = 0;
	int32 MaxMoviePlayList = 0;

	bool IsPlayState = false;
	bool IsLoadingMovieState = false;
	bool IsCheckLoading = false;
	bool IsCache = false;
	bool IsCheckRewind = false;
	bool IsCheckFail = false;

	int32 RestartCount = 0;

	FTimerHandle			AutoSkipTimeHandler;
};
