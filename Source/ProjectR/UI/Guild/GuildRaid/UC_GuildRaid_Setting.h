// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_GuildRaid_Setting.generated.h"

class UUC_CurrencyDisplay;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_Setting : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init(FGUILD_RAID&	InRaidData);
	void Init(FGUILD_RAID&	InRaidData, int32 InQuantity, UCanvasPanel* InBackPanel);
public:

	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Setting)
	void OnClick_LevelOfDifficulty();

	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Setting)
	void OnClick_RaidSpawn();

	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Setting)
	void OnClick_Close();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UButton*			Button_Difficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UTextBlock*			Text_NumOfDifficulty;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UTextBlock*			Text_RequestPointOfDifficulty;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UButton*			Button_RaidSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UTextBlock*			Text_GuildPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UTextBlock*			Text_RequestPointOfRaidSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UCanvasPanel*		BackPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Setting)
	UUC_CurrencyDisplay*	CurrencyDisplay;

private:
	int32				GetMaxGrade();

private:
	FGUILD_RAID			RaidData;
	
};
