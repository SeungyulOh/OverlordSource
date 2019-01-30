 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_LevelUpDesc.h"
#include "GlobalIntegrated.h"

#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollView.h"
#include "UI/Guild/GuildLevel_ScrollItemData.h"

void UUC_Guild_LevelUpDesc::NativeConstruct()
{
	Super::NativeConstruct();

	MakeLevelUpInfo();

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_Guild_LevelUpDesc::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Guild_LevelUpDesc::OpenLevelUpDesc()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(BP_ScrollView_GuildLevel))
	{
		BP_ScrollView_GuildLevel->InvalidateData();

		int32 GuildLevelIndex = RGameInstance->GuildInfo->MyGuildInfo.level - 1;
		if (0 > GuildLevelIndex)
			GuildLevelIndex = 0;

		if (BP_ScrollView_GuildLevel->ItemDataList.IsValidIndex(GuildLevelIndex))
		{
			BP_ScrollView_GuildLevel->SetScrollPos(BP_ScrollView_GuildLevel->ItemDataList[GuildLevelIndex]);
		}
	}

	this->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Guild_LevelUpDesc::CloseLevelUpDesc()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_Guild_LevelUpDesc::MakeLevelUpInfo()
{
	if (!IsValid(BP_ScrollView_GuildLevel))
		return;

	BP_ScrollView_GuildLevel->Init();
	BP_ScrollView_GuildLevel->ClearScrollItemData();

	for (int32 GuildLevel = 1; GuildLevel <= 10; ++GuildLevel)
	{
		UGuildLevel_ScrollItemData* ScrollItemData = Cast<UGuildLevel_ScrollItemData>(BP_ScrollView_GuildLevel->AddScrollItemData(UGuildLevel_ScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->ShowLevelUpValue = GuildLevel;
		}
	}
	BP_ScrollView_GuildLevel->InvalidateData();
}
