// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_ScrollItem.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_ScrollItemData.h"


void UUC_GuildRaid_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_GuildRaid_ScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRaid_ScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UUC_GuildRaid_ScrollItemData* RaidScrollItemData = Cast<UUC_GuildRaid_ScrollItemData>(item);
	if (IsValid(RaidScrollItemData))
	{
		RaidTableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(FName(*RaidScrollItemData->RaidData.modId));

		SetBaseInfo(RaidScrollItemData);
		SetBossName(RaidScrollItemData);
		SetRemainTime(RaidScrollItemData);
		SetClearInfo(RaidScrollItemData);
		SetEnableCard(RaidScrollItemData);
		SetDisableCard(RaidScrollItemData);

		SetRaidOpenInfo(RaidScrollItemData->RaidData.isOpen, RaidScrollItemData->IsLock);

	}

	Super::SetScrollItemData(Index, item, ScrollView);

}

void UUC_GuildRaid_ScrollItem::SetBaseInfo(UUC_GuildRaid_ScrollItemData* RaidItemData)
{	
	if( RaidTableInfo == nullptr)
		return;

	if (IsValid(Text_NumOfDifficulty))
	{
		Text_NumOfDifficulty->SetText(FText::AsNumber(RaidTableInfo->Grade));
	}
}

void UUC_GuildRaid_ScrollItem::SetRemainTime(UUC_GuildRaid_ScrollItemData* RaidItemData)
{
	if (!IsValid(RaidItemData))
		return;

	int32 Hour	= RaidItemData->RaidData.left / 3600;
	int32 Min	= (RaidItemData->RaidData.left % 3600) / 60 ;
	int32 Sec	= RaidItemData->RaidData.left % 60;


	if (IsValid(Text_RemainTime_Hour))
	{
		Text_RemainTime_Hour->SetText(FText::AsNumber(Hour));
	}

	if (IsValid(Text_RemainTime_Min))
	{
		Text_RemainTime_Min->SetText(FText::AsNumber(Min));
	}

	if (IsValid(Text_RemainTime_Sec))
	{
		Text_RemainTime_Sec->SetText(FText::AsNumber(Sec));
	}
}

void UUC_GuildRaid_ScrollItem::SetBossName(UUC_GuildRaid_ScrollItemData* RaidItemData)
{
	if (!IsValid(RaidItemData))
		return;
		
	if(RaidTableInfo == nullptr)
		return;

	if ( IsValid( Text_BossName) )
	{
		Text_BossName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI,RaidTableInfo->MapNameUIStringKey ));
	}

	if (IsValid(Text_BossNameDisabled))
	{
		Text_BossNameDisabled->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, RaidTableInfo->MapNameUIStringKey));
	}

	if (IsValid(Text_BossSubName))
	{
		Text_BossSubName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, RaidTableInfo->MapDescUIStringKey));
	}

}

void UUC_GuildRaid_ScrollItem::SetClearInfo(UUC_GuildRaid_ScrollItemData* RaidItemData)
{
	if (!IsValid(RaidItemData))
		return;
	/*
	int32 clearNum = 0;
	for (int32 clearIndex : RaidItemData->RaidData.boss)
	{
		if (clearIndex == 1)
			clearNum++;
	}

	if (IsValid(Text_CurrentClearNum))
	{
		Text_CurrentClearNum->SetText(FText::AsNumber(clearNum));
	}

	if (IsValid(Text_TotalClearNum))
	{
		Text_TotalClearNum->SetText(FText::AsNumber(RaidItemData->RaidData.boss.Num()));
	}
	*/
}

void UUC_GuildRaid_ScrollItem::SetEnableCard(UUC_GuildRaid_ScrollItemData* RaidItemData)
{
	if (!IsValid(RaidItemData))
		return;

	if(RaidTableInfo == nullptr)
		return;
	
	if (IsValid(Image_BG))
	{
		UObject* obj = UUIFunctionLibrary::GetIconImageFromTable(RaidTableInfo->UnLockSprite);
		UUIFunctionLibrary::SetImageBrush(obj, Image_BG);
	}

	if (IsValid(Switcher_OpenInfo))
	{
		if( RaidItemData->RaidData.isOpen )
			Switcher_OpenInfo->SetActiveWidgetIndex(0);
		else
		{			
			Switcher_OpenInfo->SetActiveWidgetIndex(1);
		}
	}


}

void UUC_GuildRaid_ScrollItem::SetDisableCard(UUC_GuildRaid_ScrollItemData* RaidItemData)
{
	if(!IsValid(RaidItemData) )
		return;

	if (RaidTableInfo == nullptr)
		return;

	if (IsValid(Image_Lock))
	{
		UObject* obj = UUIFunctionLibrary::GetIconImageFromTable(RaidTableInfo->LockSprite);
		UUIFunctionLibrary::SetImageBrush( obj, Image_Lock);
	}	
}

void UUC_GuildRaid_ScrollItem::SetRaidOpenInfo(bool bOpen, bool bLock)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if( RaidTableInfo == nullptr)
		return;

	if (Canvas_Disable)
	{
		if (bOpen)
			Canvas_Disable->SetVisibility(ESlateVisibility::Collapsed);
		else
		{
			if ( bLock || RGameInstance->GuildInfo->MyGuildInfo.point < RaidTableInfo->OpenCost)
			{
				Canvas_Disable->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				Canvas_Disable->SetVisibility(ESlateVisibility::Collapsed);

		}
	}
}
