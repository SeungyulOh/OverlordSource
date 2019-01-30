// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_RuneScrollItem.h"

void UUC_RuneScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (RuneIcon)
		RuneIcon->SetIsSelectEnable(true);


}

void UUC_RuneScrollItem::SelectIconRune()
{
	if (IsSelectEnable == false)
		return;

	if (RuneIcon)
	{
		RuneIcon->SetIsSelectEnable(IsSelectEnable);
		RuneIcon->SetIsSelected(IsSelected);
		RuneIcon->SelectIconRune();
	}
}

void UUC_RuneScrollItem::SetItemDataSource(int32 InIndex, URScrollItemData* InItem, URScrollView* InScrollView)
{
	auto RuneScrollItemData = Cast<URuneScrollItemData>(InItem);
	if (RuneScrollItemData == nullptr)
		return;

	if(RuneIcon)
		RuneIcon->SetRune(RuneScrollItemData->RuneID, RuneScrollItemData->RuneCount);
}

void UUC_RuneScrollItem::SetSelected(bool bSelected)
{
	SelectIconRune();
}

void UUC_RuneScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetItemDataSource(0, item, ScrollView);
}
