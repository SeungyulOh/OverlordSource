// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "UC_CurrencyScrollItem.generated.h"


class UUC_Currency_Icon;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_CurrencyScrollItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

	UFUNCTION()
	void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	
public:
	UPROPERTY()
	UUC_Currency_Icon* CurrencyIcon = nullptr;

	UPROPERTY()
	UTextBlock*		Text_Name = nullptr;
	
};
