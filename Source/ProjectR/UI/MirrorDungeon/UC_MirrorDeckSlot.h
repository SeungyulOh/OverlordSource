// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_MirrorDeckSlot.generated.h"

class UUC_Item_Icons;
/**
 * 거울던전 덱정보 UI
 */
UCLASS()
class PROJECTR_API UUC_MirrorDeckSlot : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UFUNCTION()
	void OnMatchingInfoButtonClick();

	void SetSlotButtonEnable(bool bEnable);

	void SetDataAndOpenSlot(int32 InIndex, FMIRROR& rMirror);
	void CloseSlot();

	void SetType(EMIRROR_TYPE InType);
	void SetReward(TArray<FREWARD>& InRewards);
	void SetSlotHP(FMIRROR& InMirror);
	int32 GetMaxHP(FEQUIPED_HERO& InHero);
	int32 GetItemHP(FEQUIPED_HERO& InHero);
private:
	void InitializeSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	ULocalizingTextBlock*			MirrorSlotTitleText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	TArray<UUC_Item_Icons*>			RewardIconList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
		TArray<USizeBox*>			RewardSizeBoxList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	UButton*						MatchingInfoButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	UProgressBar*					MirrorHPProgressBar = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	UTextBlock*						MirrorHPText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_MirrorDeckSlot)
	UWidgetSwitcher*				SlotActiveSwitcher = nullptr;



private:
	bool							bSetData = false;
	FMIRROR							MirrorData;
	int32							DeckIndex = 0;
};
