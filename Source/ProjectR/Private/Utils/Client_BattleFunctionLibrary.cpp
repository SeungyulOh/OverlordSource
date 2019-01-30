// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Client_BattleFunctionLibrary.h"
#include "SharedUtils/Renewal_BattleFunctionLibrary.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"




int32 UClient_BattleFunctionLibrary::GetHeroCombatStat(FString InHeroKey)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]GameInstance.RInventory is Empty"), __FUNCTION__);

	int32 TotalCombatStat = 0;
	UCharacterBattleInfo* BattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(InHeroKey);
	if (IsValid(BattleInfo) == true)
	{
		FHERO						HeroInfo;
		FWorkedCharacterTableInfo	WorkedCharacterTableInfo;
		GameInstance->RInventory->GetHeroAndTableData(InHeroKey, HeroInfo, WorkedCharacterTableInfo);

		TArray<FITEM>				EquipItem;
		GetEquipItem(HeroInfo, EquipItem);
		//////////////////////////////////////////////////////////////////////////

		TotalCombatStat = CalcHeroCombatStat(BattleInfo, WorkedCharacterTableInfo, EquipItem);
	}
	BattleInfo = nullptr;
	return TotalCombatStat;
}

int32 UClient_BattleFunctionLibrary::GetTargetHeroCombatStat(FBATTLE_USER& MatchList, FString& InHeroKey)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]GameInstance.RInventory is Empty"), __FUNCTION__);

	int32 TotalCombatStat = 0;
	UCharacterBattleInfo* BattleInfo = URenewal_BattleFunctionLibrary::MakeTargetCharacteBattleInfoFromInven(MatchList, InHeroKey);
	if (IsValid(BattleInfo) == true)
	{
		FEQUIPED_HERO				HeroInvenData;
		FWorkedCharacterTableInfo	WorkedCharacterTableInfo;

		for (FEQUIPED_HERO Hero : MatchList.heros)
		{
			if (Hero.heroUD == InHeroKey)
			{
				HeroInvenData = Hero;
				break;
			}
		}

		bool FindRow = URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(WorkedCharacterTableInfo, ECharacterCategoryTypeEnum::VE_Hero, HeroInvenData.heroId, HeroInvenData.level);
		if (FindRow == false)
			return TotalCombatStat;

		//////////////////////////////////////////////////////////////////////////

		TotalCombatStat = CalcHeroCombatStat(BattleInfo, WorkedCharacterTableInfo, HeroInvenData.equipedItems);
		BattleInfo = nullptr;
	}

	return TotalCombatStat;
}

void UClient_BattleFunctionLibrary::GetEquipItem(FHERO& HeroInfo, TArray<FITEM>& EquipItem)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]GameInstance.RInventory is Empty"), __FUNCTION__);

	//TArray<FITEM>				EquipItem;
	for (FString& itemName : HeroInfo.equipedItemUDs)
	{
		if (itemName.IsEmpty())
			continue;

		FITEM* item = GameInstance->RInventory->GetItemDataWithUD(itemName);
		if (item != nullptr)
		{
			EquipItem.Emplace(*item);
		}
	}
}

int32 UClient_BattleFunctionLibrary::CalcHeroCombatStat(UCharacterBattleInfo* BattleInfo, FWorkedCharacterTableInfo& InTableInfo, TArray<FITEM> EquipItem)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UTableManager*			TableManager = UTableManager::GetInstancePtr();
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]GameInstance.RInventory is Empty"), __FUNCTION__);
	checkf(TableManager != nullptr, TEXT("[%s]TableManager is Empty"), __FUNCTION__);

	/**
		* 캐릭터의 전투력: BattlePower
		* 캐릭터 스탯 = (캐릭터의 물리공격력 * 전투력상수) + (캐릭터 마법공격력 * 전투력상수) + (캐릭터 물리방어력 * 전투력상수) +
		(캐릭터 마법방어력 * 전투력 상수) + (캐릭터 최대체력*전투력상수) +
		(캐릭터 명중 * LV에 맞는 전투력 상수) +  (캐릭터 관통 * LV에 맞는 전투력 상수) +
		(캐릭터 공격속도* LV에 맞는 전투력 상수) +
		(캐릭터 크리티컬 확률 *  LV에 맞는 전투력 상수) + (캐릭터 회피 *  LV에 맞는 전투력 상수)"
	*/
	float Calc_TotalCombatStat = 0.0f;
	float Calc_CharacterCombatStat = 0.0f;
	float Calc_ItemCombatStat = 0.0f;
	FCharacterTableInfo& rTableData = InTableInfo.Data;
	FBattleLogicConstants& battleConstants = GameInstance->BattleLogicConstants;

	if (IsValid(BattleInfo) )
	{
		FBattlePowerTable* pBattlePowerTableRow = TableManager->GetInstancePtr()->GetBattlePowerRow(BattleInfo->Level);

		if(pBattlePowerTableRow != nullptr)
		{
			// character combat calculate
			float fPhysicalAttackValue = (float)rTableData.PhysicalAttackPower * battleConstants.PhysicalAttackPower;
			float fMasicalAttackValue = (float)rTableData.MagicAttackPower * battleConstants.MagicAttackPower;
			float fArmorValue = (float)rTableData.Armor * battleConstants.Armor;
			float fMagicResistanceValue = (float)rTableData.MagicResistance * battleConstants.MagicResistance;
			float fMaxHPValue = (float)rTableData.MaxHP * battleConstants.MaxHP;

			float fAccuracyValue = (float)rTableData.AccuracyValue * pBattlePowerTableRow->AccuracyValue;
			float fPierceValue = (float)rTableData.PierceValue * pBattlePowerTableRow->PierceValue;
			float fAttackSpeedValue = (float)rTableData.AttackSpeed * pBattlePowerTableRow->AttackSpeed;
			float fCriticalValue = (float)rTableData.CriticalValue * pBattlePowerTableRow->CriticalValue;
			float fDodgeValue = (float)rTableData.DodgeValue * pBattlePowerTableRow->DodgeValue;

			Calc_CharacterCombatStat =	fPhysicalAttackValue + fMasicalAttackValue + fArmorValue + fMagicResistanceValue + fMaxHPValue + 
										fAccuracyValue + fPierceValue + fAttackSpeedValue + fCriticalValue + fDodgeValue;
		}

		// item combat calculate
		for (FITEM& item : EquipItem)
		{
			if(item.itemId.IsNone() || !item.itemId.IsValid())
				continue;

			int32 CombatPower = FItemTableInfo::GetCombatPower(&item);
			Calc_ItemCombatStat += CombatPower;

			continue;
		}
		
	}

	Calc_TotalCombatStat = Calc_CharacterCombatStat + Calc_ItemCombatStat;

	return static_cast<int32>(Calc_TotalCombatStat);
}


int32 UClient_BattleFunctionLibrary::GetEquipedHeroCombatStat(FEQUIPED_HERO hero)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]GameInstance.RInventory is Empty"), __FUNCTION__);
	int32 TotalCombatStat = 0;
	UCharacterBattleInfo* BattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(hero);
	if (IsValid(BattleInfo)&& !hero.heroId.IsNone() && 0 < hero.level)
	{
		FWorkedCharacterTableInfo	WorkedCharacterTableInfo;
		URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(WorkedCharacterTableInfo, ECharacterCategoryTypeEnum::VE_Hero, hero.heroId, hero.level);

		TotalCombatStat = CalcHeroCombatStat(BattleInfo, WorkedCharacterTableInfo, hero.equipedItems);
		BattleInfo = nullptr;
	}
	return TotalCombatStat;
}