// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseHeroScrollViewState.h"

#include "UI/Control/RScrollItem.h"

bool UBaseHeroScrollViewState::SetSelected(URScrollItem* InSelectedItem, UUC_BaseHeroScrollView* InBaseHeroScrollView)
{
	/*if (InSelectedItem == nullptr || InSelectedItem->ItemData == nullptr)
		return false;

	if (InSelectedItem->IsSelected == true || InBaseHeroScrollView->LimitSelectCount < 0 || InBaseHeroScrollView->SelectedCount < InBaseHeroScrollView->LimitSelectCount)
	{
		if (InSelectedItem->IsSelected == true)
		{
			++(InBaseHeroScrollView->SelectedCount);
		}
		else
		{
			--(InBaseHeroScrollView->SelectedCount);
		}

		return true;
	}*/

	return false;
}