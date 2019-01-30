// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_StatChange.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"


void UUC_StatChange::Refresh(EItemStatOption ItemStatOption, int32 BeforeValue, int32 AfterValue)
{
	FName ItemStatOptionStringKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	FText ItemStatOptionText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemStatOptionStringKey);

	Text_StatName->SetText(ItemStatOptionText);

	Text_StatValueBefore->SetText(UUtilFunctionLibrary::ItemBasicOptionStatValueToText(ItemStatOption, BeforeValue));
	Text_StatValueAfter->SetText(UUtilFunctionLibrary::ItemBasicOptionStatValueToText(ItemStatOption, AfterValue));
}
