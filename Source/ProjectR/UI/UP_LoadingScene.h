// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Table/LoadingTableInfo.h"
#include "UP_LoadingScene.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_LoadingScene : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void MatchLoadingType(EGamePlayModeTypeEnum LoadingType);
	void SetLoadingImage(const FName InTextureID);
	void SetLoadingText(const FName InTextID);
	//void Temp();
	void SetProgressBarPercent(float InValue);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UP_LoadingScene)
	UTextBlock*				Text_Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UP_LoadingScene)
	UImage*					LoadingBackground;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UP_LoadingScene)
	UProgressBar*			LoadingBar = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UP_LoadingScene)
	UOverlay*				Overlay_LoadingBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UP_LoadingScene)
	UTextBlock*				Text_CurrentSuccessValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_LoadingScene|Ref")
	UWidgetAnimation*		LoadingAni_Ref = nullptr;

	UPROPERTY()
	float CurrentPercent = 0.f;
	UPROPERTY()
	float UnitPercent = 0.f;

private:
	void SaveLoadingInfoToLoadingSceneManager();
	void GetLoadingTypeTableInfo(TArray<FLoadingTableInfo*>& InLoadingTableInfo, EGamePlayModeTypeEnum LoadingType);
	//int32 SelectLoadingImage(TArray<FLoadingTableInfo*>& InLoadingTableInfo);
	//void SelectLoadingText(TArray<FLoadingTableInfo*>& InLoadingTableInfo, const int32 ImageSelectedNumber);

private:
	UPROPERTY()
	FName TextureID;
	
	UPROPERTY()
	FName TextID;
};
