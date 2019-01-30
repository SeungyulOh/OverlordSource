#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "Table/ColosseumTableInfo.h"

#include "Table/RewardPreviewTableInfo.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "UC_Colosseum_RewardInfo.h"

void UUC_Colosseum_RewardInfo::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_Colosseum_RewardInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Colosseum_RewardInfo::Init()
{
	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, ItemIcon, TEXT("ItemIcon_"));
}

void UUC_Colosseum_RewardInfo::SetGradeReward(int32 InGrade)
{
	if (InGrade == 0)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (IsValid(WidgetSwitcher_Reward))
		WidgetSwitcher_Reward->SetActiveWidgetIndex(0);

	if(IsValid(ColosseumGrade))
		ColosseumGrade->SetColosseumClassIndex(InGrade);

	FColosseumGrade* ColosseumGradeTable = UTableManager::GetInstancePtr()->GetColosseumGrade(FName(*(FString::FromInt(InGrade))));

	if (ColosseumGradeTable == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	TArray<FITEM_ICON_INFO> RewardItemIconInfo;
	UUIFunctionLibrary::GetRewardsIconInfoByPreview(ColosseumGradeTable->SeasonReward, RewardItemIconInfo);

	for (int32 i = 0; i < ItemIcon.Num(); i++)
	{
		if (ItemIcon.IsValidIndex(i) && RewardItemIconInfo.IsValidIndex(i))
		{
			ItemIcon[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemIcon[i]->SetItemIconStructure(RewardItemIconInfo[i]);
		}
		else
		{
			ItemIcon[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUC_Colosseum_RewardInfo::SetRankReward(int32 InRank)
{
	if (IsValid(WidgetSwitcher_Reward))
		WidgetSwitcher_Reward->SetActiveWidgetIndex(1);

	if (IsValid(ColosseumRank))
	{
		UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(ColosseumRank->GetWidgetFromName(TEXT("WidgetSwitcher_0")));

		if (IsValid(Switcher))
		{
			if (0 < InRank)
				Switcher->SetActiveWidgetIndex(InRank - 1);
			else
				Switcher->SetActiveWidgetIndex(20);
		}
	}
	FColosseumRank* ColosseumGradeTable = UTableManager::GetInstancePtr()->GetColosseumRank(FName(*(FString::FromInt(InRank))));

	if (ColosseumGradeTable == nullptr)
		return;

	TArray<FITEM_ICON_INFO> RewardItemIconInfo;
	UUIFunctionLibrary::GetRewardsIconInfoByPreview(ColosseumGradeTable->RankReward, RewardItemIconInfo);

	for (int32 i = 0; i < ItemIcon.Num(); i++)
	{
		if (ItemIcon.IsValidIndex(i) && RewardItemIconInfo.IsValidIndex(i))
		{
			ItemIcon[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemIcon[i]->SetItemIconStructure(RewardItemIconInfo[i]);
		}
		else
		{
			ItemIcon[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URColosseum_Reward_ItemData::SetRank(int32 InIndex)
{
	Index = InIndex;
	IsRank = true;
}

void URColosseum_Reward_ItemData::SetGrade(int32 InIndex)
{
	Index = InIndex;
	IsRank = false;
}

void URColosseum_Reward_Item::SetItemData(int32 InIndex, URScrollItemData* item, URScrollView* ScrollView)
{
	URColosseum_Reward_ItemData* RewardItemData = Cast<URColosseum_Reward_ItemData>(item);
	if (RewardItemData->IsRank)
	{
		RankingReward01->SetRankReward(RewardItemData->Index);
		RankingReward02->SetRankReward(RewardItemData->Index + RANKINDEX_MAX);
	}
	else
	{
		RankingReward01->SetGradeReward(RewardItemData->Index + GRADEINDEX_MAX);
		RankingReward02->SetGradeReward(RewardItemData->Index - 1 );
	}
}
