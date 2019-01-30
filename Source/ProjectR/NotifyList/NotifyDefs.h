// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NotifyDefs.generated.h"



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ENotifyEventTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_UnlockContents		UMETA(DisplayName = "Unlock contents"),
	VE_GetNewHero			UMETA(DisplayName = "New Hero"),
	VE_GetNewItem			UMETA(DisplayName = "New Item"),
	VE_AvatarLevelUp		UMETA(DisplayName = "Avatar level up"),
	VE_DailyQuest			UMETA(DisplayName = "Daily quest"),
	VE_CompleteMission		UMETA(DisplayName = "Complete mission"),

	VE_Max,
};

