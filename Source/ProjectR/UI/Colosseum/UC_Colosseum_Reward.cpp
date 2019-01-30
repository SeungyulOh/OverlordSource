#include "ProjectR.h"
#include "UC_Colosseum_Reward.h"

void UUC_Colosseum_Reward::NativeConstruct()
{
	Super::NativeConstruct();

	if (CachedScrollItemData.Num() == 0)
	{
		for (int32 i = 0; i < 10; i++)
		{
			URColosseum_Reward_ItemData* ItemData = Cast<URColosseum_Reward_ItemData>(RewardScroll->CreateScrollItemData(URColosseum_Reward_ItemData::StaticClass()));
			ItemData->WidgetSize = RewardScroll->GetItemInitSize();

			CachedScrollItemData.Emplace(ItemData);
		}
	}
	Init();
}

void UUC_Colosseum_Reward::NativeDestruct()
{
	Reset();

	Super::NativeDestruct();
}

void UUC_Colosseum_Reward::Init()
{
	if (IsValid(Radio_RankReward))
		Radio_RankReward->OnCheckRadioCheck.AddDynamic(this, &UUC_Colosseum_Reward::OnRadioCheckRankReward);
	if (IsValid(Radio_GradeReward))
		Radio_GradeReward->OnCheckRadioCheck.AddDynamic(this, &UUC_Colosseum_Reward::OnRadioCheckGradeReward);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.AddDynamic(this, &UUC_Colosseum_Reward::OnClickClose);
	OnRadioCheckRankReward(true);
}

void UUC_Colosseum_Reward::Reset()
{
	if (IsValid(Radio_RankReward))
		Radio_RankReward->OnCheckRadioCheck.RemoveDynamic(this, &UUC_Colosseum_Reward::OnRadioCheckRankReward);
	if (IsValid(Radio_GradeReward))
		Radio_GradeReward->OnCheckRadioCheck.RemoveDynamic(this, &UUC_Colosseum_Reward::OnRadioCheckGradeReward);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Colosseum_Reward::OnClickClose);
}

void UUC_Colosseum_Reward::OnRadioCheckRankReward(bool InIsCheck)
{
	if (IsValid(RewardScroll) == false)
		return;

	if(!RewardScroll->ScrollItemList.Num())
		RewardScroll->Init_NoClear(12);

	RewardScroll->ClearScrollItemData();

	URScrollItemData*					scroll_item_data = nullptr;
	URColosseum_Reward_ItemData*		reward_data = nullptr;

	//int32 RankCount = ColosseumRank.Num();
	for (int32 n = 0; n < 5; ++n)
	{
// 		scroll_item_data = RewardScroll->AddScrollItemData(URColosseum_Reward_ItemData::StaticClass());
// 		reward_data = Cast<URColosseum_Reward_ItemData>(scroll_item_data);
		reward_data = CachedScrollItemData[n];
		reward_data->SetRank(n+1);
		RewardScroll->ItemDataList.Emplace(reward_data);
	}

	RewardScroll->InvalidateData();
}

void UUC_Colosseum_Reward::OnRadioCheckGradeReward(bool InIsCheck)
{
	if (IsValid(RewardScroll) == false)
		return;


	if (!RewardScroll->ScrollItemList.Num())
		RewardScroll->Init_NoClear(12);

	RewardScroll->ClearScrollItemData();

	URScrollItemData*					scroll_item_data = nullptr;
	URColosseum_Reward_ItemData*		reward_data = nullptr;

//	int32 RankCount = ColosseumRank.Num();
	for (int32 n = 8; n >= 0; --n)
	{
// 		scroll_item_data = RewardScroll->AddScrollItemData(URColosseum_Reward_ItemData::StaticClass());
// 		reward_data = Cast<URColosseum_Reward_ItemData>(scroll_item_data);
		reward_data = CachedScrollItemData[n];
		reward_data->SetGrade(n + 1);
		RewardScroll->ItemDataList.Emplace(reward_data);
	}

	RewardScroll->InvalidateData();
}

void UUC_Colosseum_Reward::OnClickClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
