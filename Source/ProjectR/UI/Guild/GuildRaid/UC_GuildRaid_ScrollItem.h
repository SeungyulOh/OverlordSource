// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "Global/TableManager.h"

#include "UC_GuildRaid_ScrollItem.generated.h"

class UUC_GuildRaid_ScrollItemData;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;	
	void NativeDestruct() override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UImage*					Image_BG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UWidgetSwitcher*		Switcher_OpenInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_RemainTime_Hour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_RemainTime_Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_RemainTime_Sec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_CurrentClearNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_TotalClearNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_BossName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_NumOfDifficulty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UCanvasPanel*			Canvas_Disable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UImage*					Image_Lock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_BossNameDisabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_ScrollItem)
	UTextBlock*				Text_BossSubName;


private:

	void SetBaseInfo(UUC_GuildRaid_ScrollItemData* RaidItemData);
	void SetRemainTime(UUC_GuildRaid_ScrollItemData* RaidItemData);
	void SetBossName(UUC_GuildRaid_ScrollItemData* RaidItemData);
	void SetClearInfo(UUC_GuildRaid_ScrollItemData* RaidItemData);
	void SetEnableCard(UUC_GuildRaid_ScrollItemData* RaidItemData);
	void SetDisableCard(UUC_GuildRaid_ScrollItemData* RaidItemData);

	void SetRaidOpenInfo(bool bOpen, bool bLock);


private:
	FBattleGuildRaidTableInfo* RaidTableInfo;

};
