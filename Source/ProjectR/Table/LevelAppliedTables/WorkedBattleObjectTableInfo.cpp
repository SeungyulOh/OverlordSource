// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "WorkedBattleObjectTableInfo.h"

#include "Table/GrowthTables/GrowthHeroMultiplier.h"


void FWorkedBattleObjectTableInfo::SetDataFromBattleObjectInfo(FBattleObjectTableInfo* BattleObjectInfo, int32 Level)
{
	if (BattleObjectInfo == nullptr)
		return;

	Data = *BattleObjectInfo;

	FName PatternKey = BattleObjectInfo->GrowthTableID;
	FName MultiplierPatternKey = BattleObjectInfo->GrowthMultiplierID;
	// Level growth
	ApplyLevel(PatternKey, MultiplierPatternKey, Level);
}


void FWorkedBattleObjectTableInfo::ApplyLevel(FName PatternKey, FName MultiplierPatternKey, int32 Level)
{
	if (!PatternKey.IsNone() && PatternKey != FName(""))
	{		
		const UDataTable* HeroGrowthTable = UTableManager::GetInstancePtr()->GetHeroGrowthTable(PatternKey);
		FGrowthHeroInfo_V2* HeroGrowthInfo = UTableManager::GetHeroGrowthInfo(HeroGrowthTable, Level);
		FGrowthHeroMultiplier* HeroGrowthMultiplierInfo = nullptr;
		if (MultiplierPatternKey != EXCEPTION_NONE)
		{
			HeroGrowthMultiplierInfo = UTableManager::GetInstancePtr()->GetHeroGrowthMultiplierRow(MultiplierPatternKey);
		}
		if (HeroGrowthInfo)
		{
			Data.MaxHP = Data.MaxHP + HeroGrowthInfo->MaxHP * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->MaxHP);
			//Data.MaxMP = Data.MaxMP + GetLevelValue(HeroGrowthInfo->MaxMP, Level);
			Data.PhysicalAttackPower = Data.PhysicalAttackPower + HeroGrowthInfo->PhysicalAttackPower * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->PhysicalAttackPower);
			Data.MagicAttackPower = Data.MagicAttackPower + HeroGrowthInfo->MagicAttackPower * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->MagicAttackPower);
			Data.Armor = Data.Armor + HeroGrowthInfo->Armor * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->Armor);
			Data.MagicResistance = Data.MagicResistance + HeroGrowthInfo->MagicResistance * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->MagicResistance);
			//Data.ParryValue = (Data.ParryValue + GetLevelValue(HeroGrowthInfo->ParryValue, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->ParryValue);
			//Data.DodgeValue = (Data.DodgeValue + GetLevelValue(HeroGrowthInfo->DodgeValue, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->DodgeValue);
			//Data.MPRegen = (Data.MPRegen + GetLevelValue(HeroGrowthInfo->MPRegen, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->MPRegen);
			//Data.CriticalValue = (Data.CriticalValue + GetLevelValue(HeroGrowthInfo->CriticalValue, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->CriticalValue);
			//Data.PierceValue = (Data.PierceValue + GetLevelValue(HeroGrowthInfo->PierceValue, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->PierceValue);
			//Data.AccuracyValue = (Data.AccuracyValue + GetLevelValue(HeroGrowthInfo->AccuracyValue, Level)) * ((HeroGrowthMultiplierInfo == nullptr) ? 1.0f : HeroGrowthMultiplierInfo->AccuracyValue);
		}
	}
}

// float FWorkedBattleObjectTableInfo::GetLevelValue(TArray< float >& GrowthTable, int32 Level)
// {		
// 	if (GrowthTable.Num() <= Level)
// 		Level = GrowthTable.Num() - 1;
// 
// 	if (Level >= 1)
// 	{
// 		return GrowthTable[Level - 1];
// 	}
// 
// 	return 0.0f;
// }