// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"





#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"

#include "UI/Control/RScrollView.h"
#include "UI/Raid/RaidListScrollData.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketDataEnum.h"
#include "Network/HttpRequestClient.h"

#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "UP_Raid_World_Boss.h"

const int32 retryCount = 3;

void UUP_Raid_World_Boss::NativeConstruct()
{
	Super::NativeConstruct();
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 	GameInstance->RaidManager->OnGetRaidListAllDelegate.RemoveDynamic(this, &UUP_Raid_World_Boss::OnGET_RAID_STAGE_ALL_RP);
// 	GameInstance->RaidManager->OnGetRaidListAllDelegate.AddDynamic(this, &UUP_Raid_World_Boss::OnGET_RAID_STAGE_ALL_RP);
// 	GameInstance->RaidManager->OnRaidStagePlayDelegate.RemoveDynamic(this, &UUP_Raid_World_Boss::OnGET_RAID_STAGE_USER_RP);
// 	GameInstance->RaidManager->OnRaidStagePlayDelegate.AddDynamic(this, &UUP_Raid_World_Boss::OnGET_RAID_STAGE_USER_RP);
// 	BP_Text_RemainPlayCount->SetText(FText::AsNumber(GameInstance->RaidManager->GetCurrentTryCount()));
// 	BP_Text_MaxPlayCount->SetText(FText::AsNumber(GameInstance->RaidManager->GetTryMaxCount()));
	BP_Text_RecommendPower->SetText(FText::FromString(TEXT("")));
	BP_Text_PlayTime->SetText(FText::FromString(TEXT("")));
	BP_Btn_Enter_Single_Raid->OnClicked.RemoveDynamic(this, &UUP_Raid_World_Boss::OnClick_Single_Raid);
	BP_Btn_Enter_Single_Raid->OnClicked.AddDynamic(this, &UUP_Raid_World_Boss::OnClick_Single_Raid);
	BP_WorldBossLevelScroll->Init();
	BP_WorldBossLevelScroll->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_Raid_World_Boss::OnSelectLevelScrollItem);
	BP_WorldBossLevelScroll->OnScrollViewSelectedItem.AddDynamic(this, &UUP_Raid_World_Boss::OnSelectLevelScrollItem);
	SetRaidCategory(EUIMenuIDEnum::VE_AkhanRaid);
// 	UPacketFunctionLibrary::GET_RAID_STAGE_ALL_RQ();
}

void UUP_Raid_World_Boss::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_Raid_World_Boss::SetRaidCategory(EUIMenuIDEnum InUIMenuIDEnum)
{
	const FName SINGLE_PLAY_MODE(TEXT("S"));
	const FName MULTI_PLAY_MODE(TEXT("M"));
	SingleStageInfos.Empty();
	MultiStageInfos.Empty();
	TArray<FName> TableIDs = UTableManager::GetInstancePtr()->GetWorldBossStageNames();
	if (IsValid(BP_WorldBossLevelScroll))
		BP_WorldBossLevelScroll->ClearScrollItemData();

	if (InUIMenuIDEnum == EUIMenuIDEnum::VE_AkhanRaid)
	{
		const FName BOSS_NAME(TEXT("AKHAN"));
		for (FName ID : TableIDs)
		{
			FWorldBossStageInfo* WorldBossInfo = UTableManager::GetInstancePtr()->GetWorldBossStage(ID);
			if (WorldBossInfo->BossName == BOSS_NAME)
			{
				if(WorldBossInfo->PlayMode == SINGLE_PLAY_MODE)
					SingleStageInfos.Add(ID, WorldBossInfo);
				else if (WorldBossInfo->PlayMode == MULTI_PLAY_MODE)
					MultiStageInfos.Add(ID, WorldBossInfo);
			}
		}

		for (auto v : SingleStageInfos)
		{
			if (IsValid(BP_WorldBossLevelScroll))
			{
				UWorldRaidLevel_ScrollData* ScrollItemData = Cast<UWorldRaidLevel_ScrollData>(BP_WorldBossLevelScroll->AddScrollItemData(UWorldRaidLevel_ScrollData::StaticClass()));
				if (ScrollItemData)
				{
					ScrollItemData->StageInfo = v.Value;
					ScrollItemData->BossKey = v.Key;
				}
			}
		}
		
	}
	if (IsValid(BP_WorldBossLevelScroll))
		BP_WorldBossLevelScroll->InvalidateData();
}

void UUP_Raid_World_Boss::OnGET_RAID_STAGE_ALL_RP()
{

}

void UUP_Raid_World_Boss::OnGET_RAID_STAGE_USER_RP(FString RaidStageID)
{

}

void UUP_Raid_World_Boss::OnSelectDifficulty(FName InDifficulty)
{
	for (auto InWorldBossStageInfo : SingleStageInfos)
	{
		if (InWorldBossStageInfo.Value->Difficulty != InDifficulty)
			continue;
		FBossStage* BossStage = UTableManager::GetInstancePtr()->GetBossStage(InWorldBossStageInfo.Key);
		if (BossStage) {
			BP_Text_RecommendPower->SetText(FText::AsNumber(BossStage->CommonRecommendPowerValue));
			BP_Text_PlayTime->SetText(FText::AsNumber(BossStage->CommonPlayTime));
		}
	}
//	BP_WorldBossLevel->SelectedLevel(InDifficulty);
	SelectedDifficulty = InDifficulty;
}

void UUP_Raid_World_Boss::OnClick_Single_Raid()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);

	if (IsValid(BP_WorldBossLevelScroll))
	{
// 		if (BP_WorldBossLevelScroll->IsSelected())
// 		{
			TArray<int32> SelectedIndex = BP_WorldBossLevelScroll->GetSelectedScrollItem();
			if (SelectedIndex.Num() > 0)
			{
				UWorldRaidLevel_ScrollItem* SelectedItem = Cast<UWorldRaidLevel_ScrollItem>(BP_WorldBossLevelScroll->GetScrollItem(SelectedIndex[0]));
				GameInstance->RealTimeModManager->EnterWorldRaid(SelectedItem->BossKey);
			}
//		}
	}
}

void UUP_Raid_World_Boss::OnSelectLevelScrollItem(URScrollItem* selectItem)
{
	UWorldRaidLevel_ScrollItem* SelectedItem = Cast<UWorldRaidLevel_ScrollItem>(selectItem);
	if (IsValid(SelectedItem))
	{
		if (SelectedItem->BossStageInfo) {
			BP_Text_RecommendPower->SetText(FText::AsNumber(SelectedItem->BossStageInfo->CommonRecommendPowerValue));
			BP_Text_PlayTime->SetText(FText::AsNumber(SelectedItem->BossStageInfo->CommonPlayTime));
		}
	}
}

void UWorldRaidLevel_ScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UWorldRaidLevel_ScrollData* RaidScrollItemData = Cast<UWorldRaidLevel_ScrollData>(item);
	if (IsValid(RaidScrollItemData))
	{
		BossKey = RaidScrollItemData->BossKey;
		BossStageInfo = UTableManager::GetInstancePtr()->GetBossStage(BossKey);
		BP_Image_Stage_Lock->SetVisibility(ESlateVisibility::Hidden);
		BP_Image_Stage_Selected->SetVisibility(ESlateVisibility::Hidden);
		UPaperSprite* sprite = UUtilFunctionLibrary::GetSpriteFromTable(BossStageInfo->CommonDifficultySpriteDefault);
		UUIFunctionLibrary::SetImageBrush(sprite, BP_Image_Stage_Difficulty);
		BP_Text_Difficulty->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, BossStageInfo->SingleSelectDifficultyString));
	}
	Super::SetScrollItemData(Index, item, ScrollView);
}

void UWorldRaidLevel_ScrollItem::SetSelected(bool bSelected)
{
	Super::SetSelected(bSelected);
//	FBossStage* BossStageInfo = UTableManager::GetInstancePtr()->GetBossStage(BossKey);
	UPaperSprite* sprite = nullptr;
	if(bSelected)
		sprite = UUtilFunctionLibrary::GetSpriteFromTable(BossStageInfo->CommonDifficultySpriteActive);
	else
		sprite = UUtilFunctionLibrary::GetSpriteFromTable(BossStageInfo->CommonDifficultySpriteDefault);

	if(sprite)
		UUIFunctionLibrary::SetImageBrush(sprite, BP_Image_Stage_Difficulty);
}
