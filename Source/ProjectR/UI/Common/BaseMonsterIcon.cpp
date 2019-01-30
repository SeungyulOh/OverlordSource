// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseMonsterIcon.h"

#include "UI/Common/MonsterScrollItemData.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollView.h"

#include "UtilFunctionIntegrated.h"



void UBaseMonsterIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBaseMonsterIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

/*
void UBaseMonsterIcon::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
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
	UUIFunctionLibrary::GetHeroIconInfoForNpc(this->GetWorld(), NPCIconInfo, MonsterScrollItemData->NPCID);
	NPCIconInfo.level = MonsterScrollItemData->MonsterLevel;

	SetMonsterIconByIconInfo(NPCIconInfo);
}
*/

void UBaseMonsterIcon::SetMonsterIconByIconInfo(const FHERO_ICON_INFO& InHeroIconInfo, bool bIsBossMonster)
{
	if (BP_Monster_Image)
	{		
		UUIFunctionLibrary::SetImageBrush(InHeroIconInfo.PortraitTextureRef, BP_Monster_Image);		
	}

// 	if (IsValid(Level_panel))
// 	{
// 		if (InHeroIconInfo.level == 0)
// 		{
// 			Level_panel->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 		else
// 		{
// 			Level_panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 	}

	if (BP_Text_MonsterLevel)
	{
		BP_Text_MonsterLevel->SetText(FText::AsNumber(InHeroIconInfo.level));
	}

	ToolTipEvent = EToolTipEventTypeEnum::VE_Npc;
	ToolTipID = InHeroIconInfo.heroId;

	if (IsValid(Boss_Name_Tag))
	{
		if (bIsBossMonster)
			BP_Monster_Image->SetVisibility(ESlateVisibility::Visible);
		else
			Boss_Name_Tag->SetVisibility(ESlateVisibility::Hidden);
	}

	SetDeath(false);
}

void UBaseMonsterIcon::SetDeath(bool InbDeath)
{
	bDeath = InbDeath;
	if (IsValid(Death))
	{
		Death->SetVisibility(bDeath ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}