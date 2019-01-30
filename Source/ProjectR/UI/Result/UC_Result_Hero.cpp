// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Result_Hero.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UtilFunctionIntegrated.h"




void UUC_Result_Hero::UpdateInfo(const FHERO_ICON_INFO& HeroIcon, FBattleRecord BattleRecord)
{
	
	if (TextBlock_Kill)
	{
		FString Cnt = FString::FromInt( BattleRecord.nKillCount );
		TextBlock_Kill->SetText( FText::FromString(Cnt) );
	}

	if (TextBlock_Attack)
	{
		int32 AmountOfAttack = BattleRecord.nAmountOfNormalAttack + BattleRecord.nAmountOfSkillAttack;
		FString Amt = FString::FromInt(AmountOfAttack);
		TextBlock_Attack->SetText(FText::FromString(Amt));
	}

	if (TextBlock_Damage)
	{
		FString Amt = FString::FromInt(BattleRecord.nAmountOfDamage);
		TextBlock_Damage->SetText(FText::FromString(Amt));
	}

	if (TextBlock_Heal)
	{
		FString Amt = FString::FromInt(BattleRecord.nAmountOfHeal);
		TextBlock_Heal->SetText(FText::FromString(Amt));
	}


	if (TextBlock_Skill)
	{
		FString Cnt = FString::FromInt(BattleRecord.nSkillCount);
		TextBlock_Skill->SetText(FText::FromString(Cnt));
	}
	
	UUIFunctionLibrary::ClearHeroIconIfo(ResultHeroIconInfo);
	ResultHeroIconInfo = HeroIcon;

	OnUpdateHeroIcon();
}

void UUC_Result_Hero::OnUpdateHeroIcon()
{
	UC_Icon_Hero->SetHeroIconInfoCopy(ResultHeroIconInfo);
}

