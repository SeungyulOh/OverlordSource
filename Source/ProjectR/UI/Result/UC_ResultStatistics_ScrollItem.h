// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UC_ResultStatistics_ScrollItem.generated.h"



class UBattleRecordScrollItemData;
class UTextBlock;
class UImage;
class UProgressBar;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ResultStatistics_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UWidgetSwitcher*	AllySwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UImage*				MyHeroRecord = nullptr;

	// Ally
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UHorizontalBox*		AllyRecordRankBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			AllyRecordRank = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UImage*				AllyPortraitImg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			AllyNickName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			AllyRecordText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			AllyRecord1Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UProgressBar*		AllyRecord1Progress = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			AllyRecord2Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UProgressBar*		AllyRecord2Progress = nullptr;

	// Enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UHorizontalBox*		EnemyRecordRankBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			EnemyRecordRank = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UImage*				EnemyPortraitImg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			EnemyNickName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			EnemyRecordText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			EnemyRecord1Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UProgressBar*		EnemyRecord1Progress = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UTextBlock*			EnemyRecord2Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ResultStatistics_ScrollItem)
	UProgressBar*		EnemyRecord2Progress = nullptr;

private:
	UBattleRecordScrollItemData*	BattleRecordItemData = nullptr;

private:
	void SetViewAllyRecord();
	void SetViewEnemyRecord();

	void SetRecordRankText(UHorizontalBox* RankBox, UTextBlock* RankText);
	void SetNickNamePortrait(UTextBlock* NickNameText, UImage* PortraitImg);
	void SetRecordValueText(UTextBlock* RecordText);
	void SetAttackRecordValueText(UTextBlock* RecordText1, UTextBlock* RecordText2);
	void SetRecordValueProgress(UProgressBar* RecordProgress1, UProgressBar* RecordProgress2);
	void SetAttackRecordValueProgress(UProgressBar* RecordProgress1, UProgressBar* RecordProgress2);
};
