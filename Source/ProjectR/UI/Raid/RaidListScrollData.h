// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "RaidListScrollData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URaidListScrollData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	bool IsRaidReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_Month;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_Day;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_Hour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_Minute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_End_Month;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_End_Day;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_End_Hour ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_End_Minute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_Raid_SubName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_PlayLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_CurPlayCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_MaxPlayCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FName			ImageResource_Raid_Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	int32			PlayLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FString RaidStageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FName PreviewItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RadListScrollData)
	FText Text_PreviewItemString;
};