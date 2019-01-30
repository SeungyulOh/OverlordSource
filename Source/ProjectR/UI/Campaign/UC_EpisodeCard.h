// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Global/EventManager.h"
#include "UC_EpisodeCard.generated.h"


#define CAMPAIGN_BACKGROUND	"Campaign_Card_Background"

class UTextBlock;
class ULocalizingTextBlock;
class UProgressBar;
class UImage;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_EpisodeCard : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
		
	virtual void SetEpisodeInfo(FName InEpisodeID);


	/*
	Template method for children
	*/
	virtual void SetEpisodeInfo_Impl();
	virtual void SetCardSelectedState_Impl();


	UFUNCTION(BlueprintCallable, Category = UUC_CampaignCard)
	void OnClick_EpisodeCard();
	
	virtual void InvalidateData();
	void Refresh();
	void RefreshSelection();

	bool IsEnable();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	EDungeonTypeEnum		EpisodeType = EDungeonTypeEnum::VE_Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	FName					EpisodeID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UTextBlock*				EpisodeNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UTextBlock*				EpisodeNameText1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UTextBlock*				EpisodeIdxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UTextBlock*				EpisodeUnLockText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UTextBlock*				EpisodeUnLockLevelText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UImage*					Image_Episode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UImage*					Image_SelectedEpisode = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UImage*					Image_ContentsLock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	UWidgetSwitcher*		EnableSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EpisodeCard)
	FDelegate_OnOneParamFNameValue		OnClickEpisodeCard;

protected:
	void SetCardImage_Texture();

	bool IsOpenedEpisode();
	virtual void SetLockImage();
	bool IsContentsUnlock();

protected:
	// table data
	UPROPERTY()
	FName						EpisodeName;
	UPROPERTY()
	FName						EpisodeImage_Key;
	

	// runtime data
	bool						bOpened = true;

	
};
