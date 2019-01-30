// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "QuestManager.generated.h"

//class URGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnFloorSelect, int32, InFloor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnReceiveQuestInfo);

USTRUCT(BlueprintType)
struct PROJECTR_API FFloorQuestData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	FName		StageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	int32		ConditionGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	FName		RewardKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	FName		TitleKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	FName		DocumentKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorQuestData)
	FName		TargetStringKey;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UQuestManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(/*URGameInstance* InRGameInstance*/);

	void SetDailyQuestStatus(FDAILY_QUEST_STATUS& InData);
	//void SetDailyQuestStatus(const TArray<int32>& InClearCount, const TArray<int32>& InRewardable, const TArray<FString>& InRewardIds, const int32& InTimeleft, const int32& InScore);
	void SetQuestReward(const TArray<FREWARD>& InQuestReward);
	bool FindNewNotify();

	//	임시방편 2018.01.03	엄재영
// 	void				SetQuestData(FROA_ADVENTURE& Indata);
// 	FROA_ADVENTURE*		GetQuestData(void) { return &CurQuestData; }
// 	float				GetQuestPercent(void);
// 	FROA_ADVENTURE		CurQuestData;
// 	FROA_QUEST*			CurExeQuest = nullptr;


	void SetImportantQuestList();
	
	FORCEINLINE bool IsNavigationUsing() { return bUseNavigation; }
	FORCEINLINE void UseNavigation() { bUseNavigation = true; }
	FORCEINLINE void EndNavigation() { bUseNavigation = false; }
public:
	UPROPERTY()
	TArray<int32>		RewardableList;
	UPROPERTY()
	TArray<int32>		ClearCountList;
	int32				CurrentScore;
	int32				TimeLeft;

	TArray< TArray<FFloorQuestData> >	ImportantQuestList;
	
	UPROPERTY()
	FDelegate_OnFloorSelect				OnFloorSelect;

	UPROPERTY()
	FDelegate_OnReceiveQuestInfo		OnReceiveQuestInfo;
private:
	bool				bUseNavigation = false;
};
