// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "BaseMonsterScrollItem.generated.h"

class UBaseMonsterIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseMonsterScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void		SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = UBaseMonsterScrollItem)
	UBaseMonsterIcon* UC_Icon_Monster_0 = nullptr;
};
