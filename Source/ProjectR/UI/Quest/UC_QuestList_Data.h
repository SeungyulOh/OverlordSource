// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataEnum.h"
#include "UI/UIDefs.h"
#include "UC_QuestList_Data.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_QuestList_Data : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	//Common Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		UTexture2D*			Texture_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Achievement_UI_INFO)
		FName RewardKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		FName				NameString_Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		FName				DescriptionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		int32				CurrentSuccessValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		int32				SuccessValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		float					SuccessRateProgressBarValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		TArray<FITEM_ICON_INFO> RewardItemIconInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementScrollItemData)
		int32 srl;

	////////////////////////////////////////////////////////////////////////////////////
	//Achievement Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
	EACHIEVE_TYPE		AchieveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
	EUIMenuIDEnum		Shortcut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AchievementScrollItemData)
	int32 passed_level;

	bool bIsActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuestList_Data)
		FName		type_pt;



};
