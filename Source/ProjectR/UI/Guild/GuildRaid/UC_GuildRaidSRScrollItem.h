// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UC_GuildRaidSRScrollItem.generated.h"

class UUC_Item_Icons;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaidSRScrollItem : public URScrollItem
{
	GENERATED_BODY()
	

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaidSRScrollItem)
	UTextBlock*				Text_Level = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaidSRScrollItem)
	UUC_Item_Icons*			ItemIcons = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaidSRScrollItem)
	UTextBlock*			    Text_Desc = nullptr;


};
