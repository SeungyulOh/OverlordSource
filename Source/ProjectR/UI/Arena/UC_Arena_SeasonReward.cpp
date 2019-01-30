// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Arena_SeasonReward.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Arena/ArenaSeasonReward_ScrollItemData.h"

void UUC_Arena_SeasonReward::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, IconsArray, TEXT("UCItemIcons"));
}

bool UUC_Arena_SeasonReward::Initialize()
{
	Super::Initialize();
	return true;
}

void UUC_Arena_SeasonReward::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UArenaSeasonReward_ScrollItemData* ScrollItemData = Cast<UArenaSeasonReward_ScrollItemData>(item);
	if (!IsValid(ScrollItemData))
		return;

	ScrollItemData->SetSwitcherSeasonRewardDivision(SeasonRewardSwitcher);
	UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(UC_Arena_Tier->GetWidgetFromName(TEXT("WidgetSwitcher_0")));
	
	if (IsValid(Switcher))
		ScrollItemData->SetTextSeasonRewardKind(RewardText, Switcher);

	SetItem(IconsArray[0], ScrollItemData->RewardItemData1);
	SetItem(IconsArray[1], ScrollItemData->RewardItemData2);
	SetItem(IconsArray[2], ScrollItemData->RewardItemData3);
}

void UUC_Arena_SeasonReward::SetItem(UUC_Item_Icons* _itemicon, FITEM_ICON_INFO& _iteminfo)
{
	if (_iteminfo.ItemCategory == EREWARD_TYPE::NONE)
	{
		_itemicon->SetVisibility(ESlateVisibility::HitTestInvisible);
		_itemicon->SetItemIconStructure(_iteminfo);
	}
	else
		_itemicon->SetVisibility(ESlateVisibility::Collapsed);
}
