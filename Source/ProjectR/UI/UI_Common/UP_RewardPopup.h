// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UP_RewardPopup.generated.h"

class UUC_Item_Icons;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnRewardPopupClick);

/**
 * BP: UP_Popup_Reward
 * WBP_RewardPopup
 */
UCLASS()
class PROJECTR_API UUP_RewardPopup : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void HideAniPlay();
public:
	void InitRewardPopup();
	void SetRewardItem(FITEM& ItemData, bool bFirst = false);
	void SetRewardItems(TArray<FITEM>& ItemDataList, bool bFirst = false);
	void SetRewardItemItemInfo(TArray<FITEM_ICON_INFO>& InItemIconInfoList, const TArray<FREWARD>* InRewards = nullptr, bool bFirst = false);
	void SetQuestRewardItem(TArray<FITEM_ICON_INFO>& InItemIconInfoList, const TArray<FREWARD>* InRewards = nullptr, FText InQuestText = FText::GetEmpty());

	//total count - Befor Grind Item count
	void SetGrindRewardItems(TArray<FITEM>& ItemDataList, TArray<int32>& BeforeItemCounts);

	UFUNCTION(BlueprintCallable, Category = RewardPopup)
	void RefreshUI();

	UFUNCTION(BlueprintCallable, Category = RewardPopup)
	void Click_Ok();

	UFUNCTION()
	void Click_VerticalBox();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	bool						bCheckHeroRuneSplash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	TArray<UPanelWidget*>		ItemPanelList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	TArray<UUC_Item_Icons*>		ItemIconList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	TArray<UTextBlock*>			ItemTextBlockList;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RewardPopup)
	FDelegate_OnRewardPopupClick		OnClickOK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	ULocalizingTextBlock*		LocalizingTextBlock_FirstRewardLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	UButton*					VerticalBox_QuestGuide;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RewardPopup)
	FDelegate_OnRewardPopupClick		OnClickFirstRewardOK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RewardPopup)
	ULocalizingTextBlock*		QuestRewardsText;

private:
	UPROPERTY()
	TArray<FITEM_ICON_INFO> ItemIconInfoList;
	UPROPERTY()
	TArray<FREWARD> Rewards;

	FText QuestText;

	bool bPlayHideAni = false;
	bool bFirstReward = false;
	bool bQuestReward = false;
};

