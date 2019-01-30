// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildRaidSeasonReward.generated.h"

class URScrollView;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaidSeasonReward : public URBaseWidget
{
	GENERATED_BODY()
	

public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init(int32 RewardIndex, bool NextRewardIndex);
	
public:
	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaidSeasonReward)
	void OnClick_Close();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaidSeasonReward)
	UTextBlock*				Text_Title;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaidSeasonReward)
	URScrollView*			ScrollView_Reward;

private:
	int32					RewardIndex;

};
