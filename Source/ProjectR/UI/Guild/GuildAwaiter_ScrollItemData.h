// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "GuildAwaiter_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildAwaiter_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FGUILD_AWAITER		GuildAwaiterData;
	UPROPERTY()
	EGUILD_GRADE MyGuildGrade;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildApplicant_ScrollItemData)
	void SetAwaiterData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UButton* BtnApprove, UButton* BtnReject,
		UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord, UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildApplicant_ScrollItemData)
	void OnClick_AWaiterAccept(UObject* WorldContextObject, bool accept);
};