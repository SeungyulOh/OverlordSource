// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AchievementManager.h"

#include "GlobalIntegrated.h"


#include "NotifyList/NotifyEventList.h"
#include "GameInfo/UserInfo.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

void UAchievementManager::SetAchievementStatus(TArray<FACHIEVE_STATUS>& InAchievementStatus)
{
	AchievementStatus = InAchievementStatus;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (FindNewNotify() || RGameInstance->NotifyManager->IsNotify(ENotifyStateFlagEnum::BM_Achievement) == true)
	{
		if ( IsValid(RGameInstance->NotifyEventList) && IsValid(RGameInstance->RWidgetManager))
		{
			if (!RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_Daily_Quest))
			{
				RGameInstance->NotifyEventList->AddNotifyEvent(ENotifyEventTypeEnum::VE_CompleteMission);
			}		

			if (IsValid(RGameInstance->NotifyManager))
				RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Achievement, true);
		}
	}
	else
	{
		if (IsValid(RGameInstance->NotifyManager))
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Achievement, false);
	}
}

void UAchievementManager::GiveAchieveReward(const TArray<FREWARD>& InRewards)
{
	OnAchievementRewardRp.Broadcast(InRewards);
}

void UAchievementManager::SetAchievementUpdate(TArray<FACHIEVE_UPDATE>& InAchievementUpdate)
{
	if (InAchievementUpdate.Num() <= 0)
		return;


	for (FACHIEVE_UPDATE& rUpdate : InAchievementUpdate)
	{
		for (FACHIEVE_STATUS& rStatus : AchievementStatus)
		{
			if (rStatus.type == rUpdate.type && rStatus.passed_level == rUpdate.passed_level)
			{
				rStatus.amt				= rUpdate.amt;
				rStatus.srl				= rUpdate.srl;
				rStatus.unlocked		= rUpdate.unlocked;
			}
		}
	}
}

bool UAchievementManager::FindNewNotify()
{
	auto AchievementRowNames = UTableManager::GetInstancePtr()->GetAchievementRowsNames();
	for (auto& AchievementStatusElem : AchievementStatus)
	{
		for (auto& AchievementRowElem : AchievementRowNames)
		{
			auto AchievementTableInfo = UTableManager::GetInstancePtr()->GetAchievementRow(AchievementRowElem);

			checkf(AchievementTableInfo != nullptr, TEXT("AchievementTableInfo is Empty"));

			//find target achievement
			if (AchievementTableInfo->type == AchievementStatusElem.type && AchievementTableInfo->level == AchievementStatusElem.passed_level + 1)
			{
				if (AchievementStatusElem.amt >= AchievementTableInfo->passValue)
				{
					return true;
				}
			}
		}
	}

	return false;
}

FName UAchievementManager::GetEncodingKey(EACHIEVE_TYPE InAchieveType, int32 InPassValue)
{
	auto AchieveType_IntVal_Index = static_cast<int32>(InAchieveType);

	auto AchieveTypeName = UUtilFunctionLibrary::GetEnumToName(TEXT("EACHIEVE_TYPE"), AchieveType_IntVal_Index - 1);

	//Enconding function  = AchiveEnumTypeName + "_" + PassLevel
	auto EncodingString = AchieveTypeName.ToString() + "_" + FString::FromInt(InPassValue);

	return FName(*EncodingString);
}
