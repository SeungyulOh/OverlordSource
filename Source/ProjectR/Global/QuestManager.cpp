// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "QuestManager.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "UI/RWidgetManager.h"
#include "UI/Quest/UC_QuestList_Data.h"

#include "UI/UIDefs.h"
#include "SharedConstants/GlobalConstants.h"
#include "NotifyList/NotifyEventList.h"




void UQuestManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
	SetImportantQuestList();
}

void UQuestManager::SetDailyQuestStatus(FDAILY_QUEST_STATUS& InData)
{
// 	InData.clearcount;
// 	InData.rewardable;
// 	InData.score;
// 	InData.timeleft;

	ClearCountList = InData.clearcount;
	RewardableList = InData.rewardable;
	CurrentScore = InData.score;
	TimeLeft = InData.timeleft;

	OnReceiveQuestInfo.Broadcast();

	// 
// 	RGAMEINSTANCE(this)->EventManager->UpdateDailyQuestStatus_Delegate.Broadcast();
}

void UQuestManager::SetQuestReward(const TArray<FREWARD>& InQuestReward)
{
//	RGAMEINSTANCE(this)->EventManager->OnQuestRewardRp_Delegate.Broadcast(InQuestReward);
}

bool UQuestManager::FindNewNotify()
{
	return false;
}

//	임시 방편	2018.01.03	엄재영
// void UQuestManager::SetQuestData(FROA_ADVENTURE& Indata)
// {
// 	if (RGAMEINSTANCE(this) == nullptr)
// 		return;
// 
// 	CurQuestData = Indata;
// 	CurExeQuest = nullptr;
// 	int32 count = 0;
// 
// 	for (int32 i = 0; i < CurQuestData.day; ++i)
// 	{
// 		FROA_CHAPTER_QUESTS& chaperquest = CurQuestData.days[i];
// 		for (FROA_QUEST& quest : chaperquest.quests)
// 		{
// 			if (!quest.rewarded && quest.complete)
// 				++count;
// 		}
// 
// 		if (chaperquest.day != CurQuestData.day)
// 			continue;
// 
// 		for (FROA_QUEST& quest : chaperquest.quests)
// 		{
// 			if (CurExeQuest != nullptr)
// 				continue;
// 
// 			if (!quest.rewarded && quest.complete)
// 			{
// 				CurExeQuest = &quest;
// 				break;
// 			}
// 		}
// 
// 		if (CurExeQuest != nullptr)
// 			continue;
// 
// 		for (FROA_QUEST& quest : chaperquest.quests)
// 		{
// 			if (!quest.rewarded && !quest.complete)
// 			{
// 				CurExeQuest = &quest;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if (RGAMEINSTANCE(this)->NotifyManager == nullptr)
// 		return;
// 
// 	if (count > 0)
// 		RGAMEINSTANCE(this)->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Quest_Guide, true);
// 	else
// 		RGAMEINSTANCE(this)->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Quest_Guide, false);
// 
// 	//UUP_QuestGuide* QuestGuide = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidgetT<UUP_QuestGuide>(EWidgetBluePrintEnum::WBP_Quest);
// 	//if (IsValid(QuestGuide))
// 	//{
// 	//	QuestGuide->OnReceiveQuestGuideStatus();
// 	//}
// }

// float UQuestManager::GetQuestPercent(void)
// {
// 	if (CurQuestData.complete)
// 	{
// 		return 1.0f;
// 	}
// 	else
// 	{
// 		int32 maxCount = 0;
// 		int32 curCount = 0;
// 
// 		for (FROA_CHAPTER_QUESTS& quests : CurQuestData.days)
// 		{
// 			for (FROA_QUEST& quest : quests.quests)
// 			{
// 				++maxCount;
// 
// 				if (quest.complete && quest.rewarded)
// 				{
// 					++curCount;
// 				}
// 			}
// 		}
// 
// 		if (maxCount > 0)
// 		{
// 			return (float)(curCount) / (float)(maxCount);
// 		}
// 	}
// 
// 	return 0.0f;
// }

void UQuestManager::SetImportantQuestList()
{
	TArray<FName> RowNames = UTableManager::GetInstancePtr()->GetFloorQuestRowsNames();
	ImportantQuestList.Reset(RowNames.Num());
	for (const FName& TableKey : RowNames)
	{
		FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(TableKey);
		if(!pTableData)
			continue;

		// data
		FFloorQuestData data;

		data.StageID			= pTableData->FloorStageId;
		data.RewardKey			= pTableData->RewardKey;
		data.TitleKey			= pTableData->MissionTitleKey;
		data.DocumentKey		= pTableData->MissionDocumentKey;
		data.TargetStringKey	= pTableData->TargetStringKey;
		data.ConditionGoal		= pTableData->ConditionGoal;

		// add
		int32 nFloor = FCString::Atoi(*pTableData->FloorInfoId.ToString()) - 1;
		if (ImportantQuestList.IsValidIndex(nFloor))
		{
			ImportantQuestList[nFloor].Emplace(data);
		}
		else
		{
			TArray<FFloorQuestData> list;
			list.Emplace(data);
			ImportantQuestList.Emplace(list);
		}
	}
}
