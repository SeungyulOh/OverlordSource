// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UC_QuestList.generated.h"

class UImage;
class ULocalizingTextBlock;
class UTextBlock;
class UProgressBar;
class UUC_Item_Icons;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_QuestList : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = UUC_QuestList)
	void SetQuestList();

	UFUNCTION(BlueprintCallable, Category = UUC_QuestList)
	void OnClick_AcceptReward();

	UFUNCTION(BlueprintCallable, Category = UUC_QuestList)
	void OnClick_GotoContent();

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	bool IsUnlockContents(EUIMenuIDEnum InContents);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UImage* Image_QuestType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	ULocalizingTextBlock*	LocalizingText_QuestType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	ULocalizingTextBlock*	LocalizingText_QuestDescription = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UTextBlock*	Text_CurrentSuccessValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UTextBlock* Text_SuccessValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UProgressBar* ProgressBar_Success = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	TArray<UUC_Item_Icons*>	RewardItemIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UButton*	Btn_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UButton*	Btn_Success = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList)
	UWidgetSwitcher*	WidgetSwitcher_SuccessAndMove = nullptr;

private:
	bool IsValidData();
	void AcceptAchievementReward();
	
	
};
