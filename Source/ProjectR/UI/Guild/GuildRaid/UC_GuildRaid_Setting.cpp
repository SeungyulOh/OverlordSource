// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_Setting.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"

#include "UtilFunctionIntegrated.h"
#include "Table/BattleGuildRaidTableInfo.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/UC_CurrencyDisplay.h"


void UUC_GuildRaid_Setting::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_RaidSpawn))
	{	
		Button_RaidSpawn->SetIsEnabled(false);	
	}

	if (IsValid(Button_Difficulty))
	{
		Button_Difficulty->SetIsEnabled(false);
	}
}

void UUC_GuildRaid_Setting::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRaid_Setting::Init(FGUILD_RAID&	InRaidData)
{
	RaidData = InRaidData;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*RaidData.modId));

		if(IsValid(Text_GuildPoint))
			Text_GuildPoint->SetText(FText::AsNumber(RGameInstance->GuildInfo->MyGuildInfo.point));


		if (IsValid(Text_RequestPointOfRaidSpawn))
			Text_RequestPointOfRaidSpawn->SetText(FText::AsNumber(RaidTableInfo->OpenCost));

		if (IsValid(Text_RequestPointOfDifficulty))
			Text_RequestPointOfDifficulty->SetText(FText::AsNumber(RaidTableInfo->GradeCost));


		if (IsValid(Button_RaidSpawn))
		{
			if (RGameInstance->GuildInfo->MyGuildInfo.point < RaidTableInfo->OpenCost)
				Button_RaidSpawn->SetIsEnabled(false);
			else
				Button_RaidSpawn->SetIsEnabled(true);			
		}

		if (IsValid(Button_Difficulty))
		{
			if (RGameInstance->GuildInfo->MyGuildInfo.point < RaidTableInfo->GradeCost || RaidData.grade  >= GetMaxGrade())
				Button_Difficulty->SetIsEnabled(false);
			else
				Button_Difficulty->SetIsEnabled(true);

		}	

		if (IsValid(Text_NumOfDifficulty))
		{
			Text_NumOfDifficulty->SetText(FText::AsNumber(RaidData.grade));
		}
		
	}
	
}

void UUC_GuildRaid_Setting::Init(FGUILD_RAID&	InRaidData, int32 InQuantity, UCanvasPanel* InBackPanel)
{
	RaidData = InRaidData;
// 	if (CurrencyDisplay)
// 		CurrencyDisplay->SetCurrency(EREWARD_TYPE::NONE, InQuantity, false, true);
	BackPanel = InBackPanel;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*RaidData.modId));
	if (RaidTableInfo == nullptr)
		return;

	if (IsValid(Button_RaidSpawn))
	{
		if (RGameInstance->GuildInfo->MyGuildInfo.gold < RaidTableInfo->OpenCost)
			Button_RaidSpawn->SetIsEnabled(false);
		else
			Button_RaidSpawn->SetIsEnabled(true);
	}
}

void UUC_GuildRaid_Setting::OnClick_LevelOfDifficulty()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*RaidData.modId));

		if (RGameInstance->GuildInfo->MyGuildInfo.point < RaidTableInfo->GradeCost)
			return;
	}

	if( RaidData.grade >= 6)
		return;

//	UPacketFunctionLibrary::GUILD_RAID_UPGRADE_RQ(this, RaidData.modId);

}

void UUC_GuildRaid_Setting::OnClick_RaidSpawn()
{	
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance)
// 	{
// 		FBattleGuildRaidTableInfo* RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*RaidData.modId));
// 		
// 		if (RGameInstance->GuildInfo->MyGuildInfo.gold < RaidTableInfo->OpenCost)
// 			return;
// 	}
// 
// 	UPacketFunctionLibrary::GUILD_RAID_SPAWN_RQ(RaidData.modId);
// 	OnClick_Close();
}

void UUC_GuildRaid_Setting::OnClick_Close()
{
// 	SetVisibility(ESlateVisibility::Collapsed);
	if(IsValid(BackPanel))
		BackPanel->SetVisibility(ESlateVisibility::Collapsed);
}

int32 UUC_GuildRaid_Setting::GetMaxGrade()
{
	FBattleGuildRaidTableInfo* curRaidTable = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow( FName(*RaidData.modId));

	if( curRaidTable == nullptr)
		return RaidData.grade;

	TArray<FName> RaidTableIndexes = UTableManager::GetInstancePtr()->GetBattleGuildRaidRowsNames();

	TArray<int32> GradeList;

	FBattleGuildRaidTableInfo* searchTable = nullptr;

	for (FName tableIndex : RaidTableIndexes)
	{
		searchTable = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(tableIndex);

		if( curRaidTable->modGroup == searchTable->modGroup)
			GradeList.Emplace(searchTable->Grade);
	}


	GradeList.Sort();

	return GradeList[ GradeList.Num() - 1];
	
}
