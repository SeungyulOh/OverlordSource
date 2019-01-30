//// Fill out your copyright notice in the Description page of Project Settings.
//
#include "ProjectR.h"
//#include "UC_RewardItemList.h"
//

//
//
//
//
//void UUC_RewardItemList::NativeConstruct()
//{
//	Super::NativeConstruct();
//}
//void UUC_RewardItemList::NativeDestruct()
//{
//	Super::NativeDestruct();
//}
//
//void UUC_RewardItemList::SetRewardItemIconList(TArray<FREWARD>& rewardList)
//{
//	TArray<FITEM_ICON_INFO> iconList;
//	for (FREWARD& reward : rewardList)
//	{
//		FITEM_ICON_INFO info;
//		if (UUIFunctionLibrary::GetItemIconInfoWithID(GetWorld(), info, reward.rewardId))
//		{
//			iconList.Emplace(info);
//		}
//	}
//
//	SetItemIconList(iconList);
//}
//
//void UUC_RewardItemList::SetItemIconList(TArray<FITEM_ICON_INFO>& iconList)
//{
//	RewardIconList = iconList;
//
//	OnSetItemIconList();
//}