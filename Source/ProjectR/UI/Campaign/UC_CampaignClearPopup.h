// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_CampaignClearPopup.generated.h"

class URewards_Scrollview;
class ULocalizingTextBlock;

/**
* BP: UC_Campaign_ClearPopup
*/
UCLASS()
class PROJECTR_API UUC_CampaignClearPopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void	NativeConstruct() override;
	virtual void	NativeDestruct() override;

	UFUNCTION()
	void	OnCampaignEpisodeClearPopup(FName EpisodeID, int32 EpisodeIdx);

	UFUNCTION(BlueprintCallable, Category = UUC_CampaignClearPopup)
	void	OnClick_Close();

	UFUNCTION(BlueprintCallable, Category = UUC_CampaignClearPopup)
	void	OnClick_Enter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_CampaignClearPopup)
	URewards_Scrollview*	UC_RewardsScrollView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_CampaignClearPopup)
	ULocalizingTextBlock*	Text_Caption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_CampaignClearPopup)
	UButton*				Button_Close;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_CampaignClearPopup)
	UButton*				Button_Close2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_CampaignClearPopup)
	UButton*				Button_Enter;

private:
	UPROPERTY()
	FName CurEpisodeID;
	UPROPERTY()
	int32 CurEpisodeIdx;
};
