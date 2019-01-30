// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "Network/ClientProtocols.h"
#include "AchievementManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnUpdateAchievement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnAchievementRewardRp, const TArray<FREWARD>&, rewards);
//class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UAchievementManager : public UObject
{
	GENERATED_BODY()
	
	
public:
	FORCEINLINE void Initialize() { }

	//Receive Data From Server
	void SetAchievementStatus(TArray<FACHIEVE_STATUS>& InAchievementStatus);
	void GiveAchieveReward(const TArray<FREWARD>& InRewards);
	void SetAchievementUpdate(TArray<FACHIEVE_UPDATE>& InAchievementUpdate);
	FORCEINLINE TArray<FACHIEVE_STATUS>& GetAchievementStatus() { return AchievementStatus; }

	//temp lobbyui event
	void SetIsStartAchievement(bool InValue) { IsStartAchievement = InValue; }
	bool GetIsStartAchievement() { return IsStartAchievement; }

	bool FindNewNotify();

	FName GetEncodingKey(EACHIEVE_TYPE InAchieveType, int32 InPassValue);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = AchievementManager)
	FDelegate_OnUpdateAchievement OnUpdateAchievement;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = AchievementManager)
	FDelegate_OnAchievementRewardRp OnAchievementRewardRp;

private:
// 	UPROPERTY()
// 	URGameInstance*	RGameInstance = nullptr;

	UPROPERTY()
	TArray<FACHIEVE_STATUS> AchievementStatus;

	bool IsStartAchievement = false;
};
