// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "RItemInfo_ScrollData.generated.h"

class URGameInstance;
/**
 * Used by UC_ItemInfo_ScrollItem
 */
UCLASS()
class PROJECTR_API URItemInfo_ScrollData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemInfo_ScrollData)
	//FString			CurItemUD;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemInfo_ScrollData)
	FName					ItemId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemInfo_ScrollData)
	int32					CurValue_Enchant = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemInfo_ScrollData)
	int32					CurLevel_Enchant = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemInfo_ScrollData)
	int32					CurLevel_Item = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>			FixedOptions;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>			FixedOptionValues;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>			Options;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>			OptionValues;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32					RandOption = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32					RandFixedOption = 0;
};
