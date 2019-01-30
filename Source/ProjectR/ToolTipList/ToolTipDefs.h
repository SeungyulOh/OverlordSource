// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ToolTipDefs.generated.h"



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EToolTipEventTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Hero					UMETA(DisplayName = "Hero"),
	VE_Item					UMETA(DisplayName = "Item"),
	VE_ItemDetail			UMETA(DisplayName = "ItemDetail"),
	VE_PreviewItem			UMETA(DisplayName = "PreviewItem"),
	VE_Skill				UMETA(DisplayName = "Skill"),
	VE_Goods				UMETA(DisplayName = "Goods"),
	VE_Npc					UMETA(DisplayName = "Npc"),
	VE_DailyQuest			UMETA(DisplayName = "DailyQuest"),
	VE_Achievement			UMETA(DisplayName = "Achievement"),
	VE_ShopItem				UMETA(DisplayName = "ShopItem"),
	VE_Etc					UMETA(DisplayName = "Etc"),

	VE_Max,
};

