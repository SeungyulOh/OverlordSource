// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HeroManagementScrollViewState.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Common/HeroScrollItemData.h"


bool UHeroManagementScrollViewState::SetSelected(URScrollItem* InSelectedItem, UUC_BaseHeroScrollView* InBaseHeroScrollView)
{
	if (InSelectedItem == nullptr || InSelectedItem->ItemData == nullptr)
	{
		return false;
	}


	if (InSelectedItem->IsSelected == true)
	{
		return false;
	}

	auto HeroScrollItemData = Cast<UHeroScrollItemData>(InSelectedItem->ItemData);
	if (HeroScrollItemData == nullptr)
	{
		return false;
	}

	/*
	if (HeroScrollItemData->HeroIconInfo.bIsOwned == false)
	{
		return false;
	}
	*/

	return true;
}