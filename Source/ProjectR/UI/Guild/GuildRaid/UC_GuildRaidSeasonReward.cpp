// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaidSeasonReward.h"

#include "UI/Control//RScrollView.h"
#include "UI/Guild/GuildRaid/RGuildRaidSRScrollItemData.h"

#include "GlobalIntegrated.h"


#include "GameInfo/GuildInfo.h"

#include "UtilFunctionIntegrated.h"




void UUC_GuildRaidSeasonReward::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_GuildRaidSeasonReward::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRaidSeasonReward::Init(int32 InRewardIndex, bool NextRewardIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if( !IsValid( RGameInstance) )
		return;

	if (IsValid(Text_Title))
	{
		if (!NextRewardIndex)
			Text_Title->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0007"))));
		else
			Text_Title->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0008"))));
	}

	if (ScrollView_Reward)
	{
		ScrollView_Reward->Init();
		ScrollView_Reward->ClearScrollItemData();

		 RewardIndex = InRewardIndex;

		 FGuildRaidRewardTableInfo* RewardTable = UTableManager::GetInstancePtr()->GetGuildRaidRewardTableRow(FName("1"));

		 if (RewardTable)
		 {
			
			 TArray<FITEM_ICON_INFO> RewardItemIcons;

			 UUIFunctionLibrary::GetRewardsIconInfoByPreview(RewardTable->guildRaidReward[RewardIndex], RewardItemIcons );

			 if (RewardItemIcons.Num() > 0)
			 {
				 for (FITEM_ICON_INFO& iconInfo : RewardItemIcons)
				 {
					 URGuildRaidSRScrollItemData* RewardItemData = Cast<URGuildRaidSRScrollItemData>( ScrollView_Reward->AddScrollItemData(URGuildRaidSRScrollItemData::StaticClass()));
					 if (IsValid(RewardItemData))
					 {
						 RewardItemData->RewardIndex = RewardIndex;
						 RewardItemData->ItemIconInfo = iconInfo;						
					 }
				 }
			 }
		 }

		 ScrollView_Reward->InvalidateData();

	}


}

void UUC_GuildRaidSeasonReward::OnClick_Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
