// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UI/HeroManagement/UP_HeroInventory.h"
#include "UC_Boss_ScrollItem.h"

void UUC_Boss_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (UC_Icon_TowerBoss)
		UC_Icon_TowerBoss->SetIsSelectEnable(true);
}

void UUC_Boss_ScrollItem::InvalidateData()
{
	Super::InvalidateData();
	UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(ItemData);
	if (ScrollItemData == nullptr)
		return;

	if (UC_Icon_TowerBoss)
	{
		UC_Icon_TowerBoss->SetBossIconWithUD(ScrollItemData->HeroUD);
	}
}

void UUC_Boss_ScrollItem::SetSelected(bool bSelected)
{
	UHeroScrollItemData* HSIData = Cast<UHeroScrollItemData>(ItemData);
	if (HSIData)
	{
		if (bSelected)
		{
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
			UC_Icon_TowerBoss->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
		}
		else
		{
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
			UC_Icon_TowerBoss->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
		}
	}
	UC_Icon_TowerBoss->SelectIconHero();
}

void UUC_Boss_ScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	auto HeroScrollItemData = Cast<UHeroScrollItemData>(item);
	if (HeroScrollItemData == nullptr)
		return;

	UC_Icon_TowerBoss->SetHeroIconInfoCopy(HeroScrollItemData->HeroIconInfo);
// 	if (HeroScrollItemData->HeroIconInfo.bLock)
// 		UC_Icon_TowerBoss->HideLockPanel(false);
// 	else
// 		UC_Icon_TowerBoss->HideLockPanel(true);
}
