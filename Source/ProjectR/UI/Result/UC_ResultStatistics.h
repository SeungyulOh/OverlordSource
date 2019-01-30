// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Result/BattleRecordScrollItemData.h"
#include "UC_ResultStatistics.generated.h"



const int32 RECORD_TYPE_MAX = (int32)EBattleRecordType::VE_RecordTypeMax;

class UButton;
class UCheckBox_Radio;
class UTextBlock;
class UProgressBar;
class URScrollView;
class UWidgetSwitcher;
class UHorizontalBox;
struct FBattleRecord;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ResultStatistics : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void InitResultStatistics(); // First of all, IDs and NickName set and Call 

	UFUNCTION(BlueprintCallable, Category = UUC_ResultStatistics)
	void OnClickCloseButton();

	UFUNCTION(BlueprintCallable, Category = UUC_ResultStatistics)
	void SelectRadioTabs(EBattleRecordType RecordType);

	void SetMyIDs(int32 UserKID, int32 GroupID) { MyUserKID = UserKID; MyGroupID = GroupID; }
	void SetEnemyID(int32 UserKID, int32 GroupID) { EnemyUserKID = UserKID; EnemyGroupID = GroupID; }

private:
	void DisplayUserBattleRecord();
	void DisplayGroupBattleRecord();
	void DisplayCampaignBattleRecord();

	void SumAllUserRecord(FUserBattleRecord* UserRecord, TArray<int32>& SumList);
	void SumGroupAllRecord(FGroupBattleRecord* GroupRecord, TArray<int32>& SumList);
	void InitUserScrollItemData(TArray<FBattleRecord*>& RecordList, int32 InitSum, URScrollView* ScrollView);
	void InitGroupScrollItemData(FGroupBattleRecord* GroupRecord, int32 InitSum, URScrollView* ScrollView);

	void SortItemDatas(URScrollView* ScrollView, EBattleRecordType RecordType, TArray<int32>& RecordSum);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UCanvasPanel*				ParentCanvasPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	TArray<UCheckBox_Radio*>	RadioTabs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	TArray<UWidgetSwitcher*>	RadioTabSwitchers;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UHorizontalBox*				AvatarInfoBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UTextBlock*					MyNickNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UTextBlock*					MyTotalRecordText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UProgressBar*				MyTotalRecordProgress = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UTextBlock*					EnemyNickNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UTextBlock*					EnemyTotalRecordText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	UProgressBar*				EnemyTotalRecordProgress = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	URScrollView*				AllyRecordScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ResultStatistics)
	URScrollView*				EnemyRecordScrollView = nullptr;

private:
	EGamePlayModeTypeEnum		PlayModeType = EGamePlayModeTypeEnum::VE_None;

	int32						MyUserKID = 0;
	int32						MyGroupID = 0;
	FString						MyNickName = "";

	int32						EnemyUserKID = 0;
	int32						EnemyGroupID = 0;
	FString						EnemyNickName = "";

	EBattleRecordType			CurrentViewRecordType = EBattleRecordType::VE_RecordTypeMax;

	TArray<UBattleRecordScrollItemData*>	AllyRecordScrollItemDatas;
	TArray<UBattleRecordScrollItemData*>	EnemyRecordScrollItemDatas;

	TArray<int32>				AllyRecordSum;
	TArray<int32>				EnemyRecordSum;

	UBattleRecordManager*		BattleRecordMgr = nullptr;
};