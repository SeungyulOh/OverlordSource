// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UI/Common/UC_ItemIcon.h"

#include "UC_Arena_reward_popup.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

void UUC_Arena_reward_popup::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonClose)
	{
		ButtonClose->OnClicked.RemoveAll(this);
		ButtonClose->OnClicked.AddDynamic(this, &UUC_Arena_reward_popup::OnClickPopupClose);
	}
	if (RankListScrollView)
		RankListScrollView->Init();
	//FindChildWidgetsWithCertainName<UUC_Arena_reward_list>(this, UC_Arena_reward_list, TEXT("UC_Arena_reward_list_C_"));
}

void UUC_Arena_reward_popup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Arena_reward_popup::OnClickPopupClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Arena_reward_popup::SetArenaRankInfo(const TArray<FName>& InRankerRewardLIst, const TArray<FName>& InAllUserRewardLIst)
{
	RankerRewardLIst = InRankerRewardLIst;
	AllUserRewardLIst = InAllUserRewardLIst;

	if (RankListScrollView->IsValidLowLevel())
	{
		for (int32 i = 0; i < RankerRewardLIst.Num(); ++i)
		{
			UArenaRankScrollItemData* RankerData = Cast<UArenaRankScrollItemData>(RankListScrollView->AddScrollItemData(UArenaRankScrollItemData::StaticClass()));
			RankerData->leftInfo.IsRanker = true;
			RankerData->leftInfo.Rank = i;
			RankerData->leftInfo.RewardKey = RankerRewardLIst[i];

			RankerData->rightInfo.IsRanker = false;
			RankerData->rightInfo.Rank = i;
			RankerData->rightInfo.RewardKey = AllUserRewardLIst[i];
		}
		RankListScrollView->InvalidateData();
	}
}

void UUC_Arena_reward_list::NativeConstruct()
{
	Super::NativeConstruct();
	FindChildWidgetsWithCertainName<UUC_MaterialIcon>(this, CurrencyData, TEXT("UC_MaterialIcon_"));
}

void UUC_Arena_reward_list::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Arena_reward_list::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	Super::SetScrollItemData(Index, item, ScrollView);
	UUC_Arena_reward_Data* RewardData = Cast<UUC_Arena_reward_Data>(item);
	SetData(RewardData);
}

void UUC_Arena_reward_list::SetDataWithArenaRankInfo(FArenaRankInfo info)
{
	SetDetailData(info.IsRanker, info.Rank, info.RewardKey);
}

void UUC_Arena_reward_list::SetData(UUC_Arena_reward_Data* RewardData)
{
	if (!RewardData)
		return;

	SetDetailData(RewardData->IsRanker, RewardData->Rank, RewardData->RewardKey);
}

void UUC_Arena_reward_list::SetDetailData(bool _IsRanker, int32 _Rank, FName _RewardKey)
{
	if (_IsRanker)
		SetRankImage(_Rank);
	else
		SetPercentImage(_Rank);

	FRewardPreviewTableInfo* reward_table_info = nullptr;
	reward_table_info = UTableManager::GetInstancePtr()->GetRewardPreviewRow(_RewardKey);
	if (!reward_table_info)
		return;

	int32 count_items = 12;

	TArray<FName> item_codes;
	item_codes.Reserve(count_items);
	item_codes.Emplace(reward_table_info->CodeName1);
	item_codes.Emplace(reward_table_info->CodeName2);
	item_codes.Emplace(reward_table_info->CodeName3);
	TArray<int32> item_amounts;
	item_amounts.Reserve(count_items);
	item_amounts.Emplace(reward_table_info->max_amount1);
	item_amounts.Emplace(reward_table_info->max_amount2);
	item_amounts.Emplace(reward_table_info->max_amount3);

	int32		IconCount = 0;
	for (UUC_MaterialIcon* Data : CurrencyData)
	{
		Data->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FCurrencyPropertyTableInfo* property_table = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(item_codes[IconCount]);
		if (property_table)
		{
			Data->Refresh(property_table->ItemIconKey, item_amounts[IconCount], false, 0);
			IconCount++;
		}
		else
		{
			FRewardPreviewItemTableInfo* rwd_prev_item_info = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(item_codes[IconCount]);
			if (rwd_prev_item_info)
			{
				Data->Refresh(rwd_prev_item_info->ItemIconKey, item_amounts[IconCount], false, 0);
				IconCount++;
			}
			else
			{
				Data->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_Arena_reward_list::SetRankImage(int32 InRank)
{
	FName	ImageKey;
	switch (InRank)
	{
		case	0:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_001"));
			break;
		}
		case	1:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_002"));
			break;
		}
		case	2:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_003"));
			break;
		}
		case	3:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_004"));
			break;
		}
		case	4:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_005"));
			break;
		}
		case	5:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_006"));
			break;
		}
		case	6:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_007"));
			break;
		}
		case	7:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_008"));
			break;
		}
		case	8:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_009"));
			break;
		}
		case	9:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_010"));
			break;
		}
	}

	rank_icon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ImageKey));
}

void UUC_Arena_reward_list::SetPercentImage(int32 InRank)
{
	FName	ImageKey;
	switch (InRank)
	{
		case	0:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_10percent"));
			break;
		}
		case	1:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_20percent"));
			break;
		}
		case	2:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_30percent"));
			break;
		}
		case	3:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_40percent"));
			break;
		}
		case	4:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_50percent"));
			break;
		}
		case	5:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_60percent"));
			break;
		}
		case	6:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_70percent"));
			break;
		}
		case	7:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_80percent"));
			break;
		}
		case	8:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_90percent"));
			break;
		}
		case	9:
		{
			ImageKey = FName(TEXT("Icon_ArenaTier_100percent"));
			break;
		}
	}

	rank_icon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ImageKey));
}

void UArenaRankScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UArenaRankScrollItem::NativeDestruct()
{

	Super::NativeDestruct();
}

void UArenaRankScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
//	Super::SetScrollItemData(Index, item, ScrollView);

	UArenaRankScrollItemData* pData = Cast<UArenaRankScrollItemData>(item);
	LeftRank->SetDataWithArenaRankInfo(pData->leftInfo);
	RightRank->SetDataWithArenaRankInfo(pData->rightInfo);

}
