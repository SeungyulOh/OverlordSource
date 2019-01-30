// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_CurrencyScrollItem.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "UC_Currency_Icon.h"
#include "UI/ItemManagement/UC_CurrencyScrollItemData.h"
#include "UtilFunctionIntegrated.h"


void UUC_CurrencyScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUC_CurrencyScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UUC_CurrencyScrollItemData* itemData = Cast<UUC_CurrencyScrollItemData>(item);
	if (!IsValid(ItemData))
		return;
	
	// Currency icon
	CurrencyIcon->Refresh(itemData->Iconkey, itemData->amount);
	
	//Text Setting
	FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, itemData->CommonStringKey);
	if(text.ToString().Contains(TEXT("NotFound")))
		text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, itemData->CommonStringKey);

	Text_Name->SetText(text);	
}
