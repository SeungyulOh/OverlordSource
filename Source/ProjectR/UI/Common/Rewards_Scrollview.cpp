// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/Common/RewardsScrollItemData.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Item_Icons.h"
#include "Rewards_Scrollview.h"

void URewards_Scrollview::NativeConstruct()
{
	Super::NativeConstruct();

	this->Init();
	for (auto& ScrollItemElem : this->ScrollItemList)
	{
		auto UC_ItemIcons = Cast<UUC_Item_Icons>(ScrollItemElem);
		if (UC_ItemIcons == nullptr)
			continue;

		// TODO: fix
		//UC_ItemIcons->SetItemIconType(EItemIconType::VE_Reward);
	}
}

void URewards_Scrollview::NativeDestruct()
{
	Super::NativeDestruct();
}

void URewards_Scrollview::InvalidateByRewardKey(const FName& InRewardKey)
{
	TArray<FITEM_ICON_INFO> RewardsIconInfoArray;
	UUIFunctionLibrary::GetRewardsIconInfoByPreview(InRewardKey, RewardsIconInfoArray);

	InvalidateByRewardsArray(RewardsIconInfoArray);
}

void URewards_Scrollview::InvalidateByRewardsArray(const TArray<FITEM_ICON_INFO>& InRewardsIconInfoArray)
{
	CreateRewardItemData(this->GetScrollItemDataCount() - InRewardsIconInfoArray.Num());
	if (this->GetScrollItemDataCount() != InRewardsIconInfoArray.Num())
		return;

	int32 Index = 0;
	for (auto& RewardIconInfoElem : InRewardsIconInfoArray)
	{
		if (this->ItemDataList.IsValidIndex(Index) == false)
			continue;
		if (IsValid(ItemDataList[Index]) == false)
			continue;

		auto RewardsScrollItemData = Cast<URewardsScrollItemData>(this->ItemDataList[Index]);
		if (IsValid(RewardsScrollItemData) == false)
			continue;

		RewardsScrollItemData->SetData(RewardIconInfoElem);
		++Index;
	}

	this->InvalidateData();
}

void URewards_Scrollview::CreateRewardItemData(int32 Count)
{
	if (Count > 0)
	{
		int32 Index = 0;
		for (auto& ItemDataElem : this->ItemDataList)
		{
			++Index;
			ItemDataElem = nullptr;
			if (Index == Count)
				break;
		}

		this->ItemDataList.RemoveAll(
			[](const URScrollItemData* InItemData)
			{
				if (InItemData == nullptr)
					return true;
				return false;
			}
		);
	}
	else if (Count < 0)
	{
		Count = -Count;
		for (int32 CreateIndex = 0; CreateIndex < Count; ++CreateIndex)
			this->AddScrollItemData(URewardsScrollItemData::StaticClass());
	}
}

