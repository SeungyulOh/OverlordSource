// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildRaid_SeasonInfo.generated.h"

class UUP_GuildRaidMain;
class UUC_Item_Icons;
struct FGuildRaidRewardTableInfo;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_SeasonInfo : public URBaseWidget
{
	GENERATED_BODY()
	
	

public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init(TAssetPtr<UUP_GuildRaidMain> InRaidMain);

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void UpdateRemainTime();



public:
	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_SeasonInfo)
	void OnClick_Reward();

	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_SeasonInfo)
	void OnClick_ItemIcon(bool bLeft);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UUC_Item_Icons*			CurrentItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UUC_Item_Icons*			NextItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UProgressBar*			Progress_Season;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UTextBlock*				Text_Hour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UTextBlock*				Text_Minute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UTextBlock*				Text_Second;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_SeasonInfo)
	UButton*				Button_Reward;
	
private:
	void					SetProgress();
	void					SetRewardItemIcon();
	void					SetRemainFinishTime();
	void					SetActivateRewardButton();

	int32					GetRewardIndex();
	void					StopTimer();
	void					StartTimer();

private:
	FName							LastReward;
	int32							LastLevel;
	FName							CurrentReward;
	int32							CurrentPoint;
	int32							CurrentMaxPoint;
	int32							CurrentLevel;
	bool							IsAlreadyReward;
	int32							RemainTime;

	FGuildRaidRewardTableInfo*		GuildRaidRewardTableInfo;

	TAssetPtr<UUP_GuildRaidMain>	GuildRaidMain;

	FTimerHandle					TimerHandle_RemainTime;
};
