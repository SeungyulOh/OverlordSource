// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseMonsterScrollItem.h"

#include "UI/Common//BaseMonsterIcon.h"
#include "UI/Common/MonsterScrollItemData.h"
#include "UtilFunctionIntegrated.h"


void UBaseMonsterScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBaseMonsterScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBaseMonsterScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (IsValid(item) == false)
	{
		return;
	}

	auto MonsterScrollItemData = Cast<UMonsterScrollItemData>(item);
	if (IsValid(MonsterScrollItemData) == false)
	{
		return;
	}

	FHERO_ICON_INFO NPCIconInfo;
	UUIFunctionLibrary::GetHeroIconInfoForNpc(NPCIconInfo, MonsterScrollItemData->NPCID);
	NPCIconInfo.level = MonsterScrollItemData->MonsterLevel;

	if (UC_Icon_Monster_0)
	{
		UC_Icon_Monster_0->SetMonsterIconByIconInfo(NPCIconInfo, MonsterScrollItemData->IsBossMonster);
	}
}