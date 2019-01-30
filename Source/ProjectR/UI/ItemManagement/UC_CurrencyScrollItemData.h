// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItemData.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "UC_CurrencyScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_CurrencyScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUC_CurrencyScrollItemData)
	FName Iconkey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUC_CurrencyScrollItemData)
	FName CommonStringKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUC_CurrencyScrollItemData)
	int32 amount;
	
};
