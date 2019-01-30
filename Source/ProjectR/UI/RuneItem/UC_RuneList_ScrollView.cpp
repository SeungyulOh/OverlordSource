// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_RuneList_ScrollView.h"

#include "GlobalIntegrated.h"

#include "UI/RuneItem/UC_RuneScrollItem.h"

void UUC_RuneList_ScrollView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_RuneList_ScrollView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_RuneList_ScrollView::InventoryDataReComposition(const TArray<int32>& InRuneCountList)
{
	CurScrollOffset = 0.0f;
	StartIdx = 0;

	TArray<int32>	ScrollValue;
	ScrollValue.Init(0, 20);

	int Count = 0;

	for (int i = 0; i < 20; i++)
	{
		if (InRuneCountList[i] > 0)
		{
			Count++;
			ScrollValue[i] = i + 1;
		}
	}

	if (IsValid(BGPanel))
	{
		BGPanel->SetRenderTransformPivot(FVector2D::ZeroVector);
		BGPanel->SetRenderScale(InitScale);
	}

	ScrollItemList_RemoveAllElement(Count);

	ScrollItemList_AddElement(Count);

	ClearScrollItemData(Count);
	ReComposition(ScrollValue, InRuneCountList);
	InvalidateData();
}

void UUC_RuneList_ScrollView::SetSelected(URScrollItem* SelecteItem)
{
	Super::SetSelected(SelecteItem);
}

void UUC_RuneList_ScrollView::ReComposition(const TArray<int32>& InRuneIDList, const TArray<int32>& InRuneCountList)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		int RuneIndex = -1;
		for (auto& HeroUDElem : InRuneIDList)
		{
			RuneIndex++;
			if(HeroUDElem == 0)
				continue;
			URuneScrollItemData* RuneInvenScrollItemData = Cast<URuneScrollItemData>(AddScrollItemData(URuneScrollItemData::StaticClass()));
			if (!IsValid(RuneInvenScrollItemData))
				continue;

			RuneInvenScrollItemData->RuneID			=	HeroUDElem;
			RuneInvenScrollItemData->RuneCount		=	InRuneCountList[RuneIndex];
		}
	}
}
