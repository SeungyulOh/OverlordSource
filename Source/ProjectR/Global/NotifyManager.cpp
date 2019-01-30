 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"




#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"

#include "Network/PacketDataStructures.h"
#include "Network/PacketDataEnum.h"
#include "NotifyManager.h"

#include "Table/SkillLevelUpCostTableInfo.h"



#include "UtilFunctionIntegrated.h"

UNotifyManager::UNotifyManager()
{	
}

void UNotifyManager::BeginDestroy()
{
	/*if (RGameInstance && RGameInstance->QuestManager)
	{
		RGameInstance->QuestManager->UpdateDailyQuestStatus_Delegate.RemoveDynamic(this, &UNotifyManager::CheckDailyMission);
	}*/
	
	Super::BeginDestroy();
}

void UNotifyManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;

	Init();
}

bool UNotifyManager::IsNotify(ENotifyStateFlagEnum notifyState)
{
	if (notifyState == ENotifyStateFlagEnum::BM_None)
		return false;

	int32 idx = (int32)notifyState;
	if (NotifyList.IsValidIndex(idx))
	{
		return NotifyList[idx];
	}

	return false;
}

/*void UNotifyManager::CheckLobbyBattle()
{
	bool bExe = IsNotify(ENotifyStateFlagEnum::BM_Raid) ||
}

void UNotifyManager::CheckLobbyChallenge()
{

}*/

void UNotifyManager::SetNotify(ENotifyStateFlagEnum notifyState, bool bState)
{
	SetNotifyExe(notifyState, bState);

	//move logic....
	switch (notifyState)
	{
	/*case ENotifyStateFlagEnum::BM_Raid:
	case ENotifyStateFlagEnum::BM_Crusader:
	case ENotifyStateFlagEnum::BM_WeekDungeon:
	{
		if (!IsNotify(ENotifyStateFlagEnum::BM_Raid) 
			&& !IsNotify(ENotifyStateFlagEnum::BM_Crusader)
			&& !IsNotify(ENotifyStateFlagEnum::BM_WeekDungeon))
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Battle, false);
		}
		else
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Battle, true);
		}
	}
	break;
	case ENotifyStateFlagEnum::BM_Colosseum:
	case ENotifyStateFlagEnum::BM_Arena:
	case ENotifyStateFlagEnum::BM_BattleGround:
	//case ENotifyStateFlagEnum::VE_UltimateBattle:
	//case ENotifyStateFlagEnum::VE_RankBattle:
	{
		if (!IsNotify(ENotifyStateFlagEnum::BM_Colosseum)
			&& !IsNotify(ENotifyStateFlagEnum::BM_Arena)
			&& !IsNotify(ENotifyStateFlagEnum::BM_BattleGround))
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Challenge, false);
		}
		else
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Challenge, true);
		}
	}
	break;*/
	case ENotifyStateFlagEnum::BM_GuildAttendance:
	case ENotifyStateFlagEnum::BM_GuildDonate:
	{
		if (!IsNotify(ENotifyStateFlagEnum::BM_GuildAttendance) && !IsNotify(ENotifyStateFlagEnum::BM_GuildDonate))
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Guild, false);
		}
		else
		{
			SetNotifyExe(ENotifyStateFlagEnum::BM_Guild, true);
		}
	}
	break;
	}
}

void UNotifyManager::SetNotifyExe(ENotifyStateFlagEnum notifyState, bool bState)
{
	int32 idx = (int32)notifyState;
	if (NotifyList.IsValidIndex(idx))
	{
		//if (NotifyList[idx] == bState)
		//	return;

		NotifyList[idx] = bState;
	}

	OnNotifyDelegate.Broadcast(notifyState, bState);
}

void UNotifyManager::Init()
{
	int32 NotifyMax = (int32)ENotifyStateFlagEnum::BM_Max;
	NotifyList.Empty();
	NotifyList.Reserve(NotifyMax);
	for (int32 i = 0; i < NotifyMax; ++i)
	{
		bool bRt = false;
		switch ((ENotifyStateFlagEnum)i)
		{
		//case ENotifyStateFlagEnum::BM_Raid:
		case ENotifyStateFlagEnum::BM_WeekDungeon:
		case ENotifyStateFlagEnum::BM_Arena:
		//case ENotifyStateFlagEnum::BM_Colosseum:
			bRt = true;
			break;
		default:
			bRt = false;
			break;
		}
		NotifyList.Emplace(bRt);
	}

	/*if (RGameInstance->QuestManager)
	{
		RGameInstance->QuestManager->UpdateDailyQuestStatus_Delegate.RemoveDynamic(this, &UNotifyManager::CheckDailyMission);
		RGameInstance->QuestManager->UpdateDailyQuestStatus_Delegate.AddDynamic(this, &UNotifyManager::CheckDailyMission);
	}*/
}

void UNotifyManager::CheckLobby()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (bFirstCheck)
	{
		bFirstCheck = false;

		if ( IsValid(RGameInstance->ContentsUnLockManager))
		{
// 			if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EOVERLORD::RAID))
// 				SetNotify(ENotifyStateFlagEnum::BM_Raid, true);
// 			else
				SetNotify(ENotifyStateFlagEnum::BM_Raid, false);

			if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Challenge/*EOVERLORD::CHALLENGE*/))
				SetNotify(ENotifyStateFlagEnum::BM_WeekDungeon, true);
			else
				SetNotify(ENotifyStateFlagEnum::BM_WeekDungeon, false);

			if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Arena/*EOVERLORD::ARENA*/))
				SetNotify(ENotifyStateFlagEnum::BM_Arena, true);
			else
				SetNotify(ENotifyStateFlagEnum::BM_Arena, false);

			//if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Colosseum/*EOVERLORD::COLOSSEUM*/))
			//	SetNotify(ENotifyStateFlagEnum::BM_Colosseum, true);
			//else
			//	SetNotify(ENotifyStateFlagEnum::BM_Colosseum, false);
		}
	}
	
	/*

	SetNotify(ENotifyStateFlagEnum::BM_Hero, false);

	CheckHeroNew();

	if(!IsNotify(ENotifyStateFlagEnum::BM_Hero))
		CheckHeroSkillUpgradeState();

	if (!IsNotify(ENotifyStateFlagEnum::BM_Hero))
		CheckHeroUpgradeItem();

	if (!IsNotify(ENotifyStateFlagEnum::BM_Hero))
		CheckHeroLevelUp();
	*/

	bool bIsLobbyBattle = (!IsNotify(ENotifyStateFlagEnum::BM_Raid) && !IsNotify(ENotifyStateFlagEnum::BM_Crusader) && !IsNotify(ENotifyStateFlagEnum::BM_WeekDungeon));
	SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Battle, !bIsLobbyBattle);
	

	bool bIsLobbyChallenge = (!IsNotify(ENotifyStateFlagEnum::BM_Colosseum) && !IsNotify(ENotifyStateFlagEnum::BM_Arena) && !IsNotify(ENotifyStateFlagEnum::BM_BattleGround));
	SetNotifyExe(ENotifyStateFlagEnum::BM_Lobby_Challenge, !bIsLobbyChallenge);

	bool IsHeroNoti = (CheckHeroNew() || CheckHeroSkillUpgradeState() || CheckHeroUpgradeItem() || CheckHeroLevelUp());
	SetNotify(ENotifyStateFlagEnum::BM_Hero, IsHeroNoti);

	bool IsItemManufactureNoti = CheckItemManufacture();
	SetNotify(ENotifyStateFlagEnum::BM_ItemCraft, IsItemManufactureNoti);

	bool IsAchievementNoti = CheckAchievement();
	SetNotify(ENotifyStateFlagEnum::BM_Achievement, IsAchievementNoti);

	bool IsDailyNoti = CheckDailyMission();
	//SetNotify(ENotifyStateFlagEnum::BM_DailyMission, IsDailyNoti);

	bool IsAdventureNoti = CheckAdventure();
	SetNotify(ENotifyStateFlagEnum::BM_Quest_Guide, IsAdventureNoti);

	bool IsAllTask = (IsAchievementNoti || IsDailyNoti || IsAdventureNoti);
	SetNotify(ENotifyStateFlagEnum::BM_AllTask, IsAllTask);
}

bool UNotifyManager::CheckHeroNotify(FString InHeroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory))
// 	{
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(InHeroUD);
// 		if (outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				if (RGameInstance->HeroInventory->NewHeroMap.Contains(InHeroUD))
// 				{
// 					return true;
// 				}
// 
// 				if (IsCheckHeroSkillUpgrade(InHeroUD))
// 					return true;
// 
// 				if (IsCheckHeroUpgradeItem(InHeroUD))
// 					return true;
// 
// 				if (IsCheckHeroLevelUp(outHero, false))
// 					return true;
// 			}
// 		}
// 	}

	return false;
}

bool UNotifyManager::CheckItemManufacture()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FName> stringKeyList = UTableManager::GetInstancePtr()->GetItemManufactureRowNames();
	for (FName& stringKey : stringKeyList)
	{
		int32 EnableCount = UUtilFunctionLibrary::GetItemManufactureEnableCount(RGameInstance->GetWorld(), stringKey);
		if (EnableCount > 0)
			return true;
	}
	return false;
}

bool UNotifyManager::CheckAchievement()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	return RGameInstance->AchievementManager->FindNewNotify();
}

bool UNotifyManager::CheckDailyMission()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bResult = false;
	auto& RewardableList = RGameInstance->QuestManager->RewardableList;
	for (auto& Element : RewardableList)
	{
		if (Element == 1)
		{
			bResult = true;
			break;
		}
	}

	SetNotify(ENotifyStateFlagEnum::BM_DailyMission, bResult);

	return bResult;
}

bool UNotifyManager::CheckAdventure()
{
	return IsNotify(ENotifyStateFlagEnum::BM_Quest_Guide);
}

bool UNotifyManager::CheckHeroNew()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bool bResult = false;
	if ( IsValid(RGameInstance->RInventory))
	{
		if (RGameInstance->RInventory->NewHeroMap.Num() > 0)
		{
			bool bPrevState = IsNotify(ENotifyStateFlagEnum::BM_Hero);
			if (!bPrevState)
			{
				bResult = true;
				SetNotify(ENotifyStateFlagEnum::BM_Hero, true);
			}
		}
	}
	return bResult;
}

bool UNotifyManager::CheckHeroSkillUpgradeState()
{
	bool bResult = false;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		TArray<FString> HeroKeys;
		RGameInstance->RInventory->GetHeroUDs(HeroKeys);

		for (FString& key : HeroKeys)
		{
			bResult = IsCheckHeroSkillUpgrade(key);

			if (bResult)
			{
				break;
			}
		}

		if (bResult)
		{
			bool bPrevState = IsNotify(ENotifyStateFlagEnum::BM_Hero);
			if (!bPrevState)
			{
				SetNotify(ENotifyStateFlagEnum::BM_Hero, bResult);
			}
		}
	}
	return bResult;
}

bool UNotifyManager::IsCheckHeroSkillUpgrade(FString heroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(heroUD);
// 		if (outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				for (int32 i = 0; i < outHero->skills.Num(); ++i)
// 				{
// 					if (IsCheckSkillUpgrade(outHero, i))
// 					{
// 						return true;
// 					}
// 				}
// 			}
// 		}		
// 	}
// 
	return false;
}

bool UNotifyManager::IsCheckSkillUpgrade(FHERO* InHero, int32 idx)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (InHero->skills.IsValidIndex(idx))
// 	{
// 		FName SkillLevelUpKey = UUIFunctionLibrary::GetSkillLevelUpKey(RGameInstance->GetWorld(), InHero->heroUD, InHero->skills[idx] + 1);
// 		FSkillLevelUpCostTableInfo* SkillLevelUpRow = UTableManager::GetInstancePtr()->GetSkillLevelUpCostRow(SkillLevelUpKey);
// 		if (SkillLevelUpRow)
// 		{			
// 			if (InHero->skills[idx] > 0)
// 			{
// 				int32 enableCount = 0;
// 				if (!URenewal_BattleFunctionLibrary::IsMaxSkillLevel(InHero->skills[idx]))
// 				{
// 					if (SkillLevelUpRow->Gold.IsValidIndex(idx))
// 					{
// 						if (RGameInstance->RInventory->GetGold() >= SkillLevelUpRow->Gold[idx])
// 						{
// 							++enableCount;
// 						}
// 					}
// 
// 					if (SkillLevelUpRow->RequireItemQty.IsValidIndex(idx))
// 					{
// 						int32 itemCount = 0;//RGameInstance->RInventory->GetItemDataCount(SkillLevelUpRow->RequireItem);
// 						if (itemCount >= SkillLevelUpRow->RequireItemQty[idx])
// 						{
// 							++enableCount;
// 						}
// 					}
// 				}
// 
// 				if (enableCount == 2)
// 				{
// 					return true;
// 				}
// 			}
// 		}
// 	}

	return false;
}

bool UNotifyManager::IsCheckHeroLevelUp(FHERO* Inhero, bool bAllLevelUp)
{
// 	auto LevelGrowthTableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*(FString::FromInt(Inhero->level))));
// 	if (LevelGrowthTableInfo == nullptr)
// 		return false;
// 
// 	return bAllLevelUp ? ((LevelGrowthTableInfo->HeroMaxRune * 2) <= Inhero->rune) : (LevelGrowthTableInfo->HeroMaxRune <= Inhero->rune);
	return false;
}

bool UNotifyManager::CheckHeroUpgradeItem()
{
	bool bResult = false;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		TArray<FString> HeroKeys;
		RGameInstance->RInventory->GetHeroUDs(HeroKeys);

		for (FString& key : HeroKeys)
		{
			bResult = IsCheckHeroUpgradeItem(key);

			if (bResult)
			{
				break;
			}
		}

		if (bResult)
		{
			bool bPrevState = IsNotify(ENotifyStateFlagEnum::BM_Hero);
			if (!bPrevState)
			{
				SetNotify(ENotifyStateFlagEnum::BM_Hero, bResult);
			}
		}
	}
	return bResult;
}

bool UNotifyManager::CheckHeroLevelUp()
{
	bool bResult = false;
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		TArray<FString> HeroKeys;
// 		RGameInstance->HeroInventory->GetInventoryKeys(HeroKeys);
// 
// 		for (FString& key : HeroKeys)
// 		{
// 			FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(key);
// 			if (outHero != nullptr)
// 			{
// 				if (outHero->activated)
// 				{
// 					bResult = IsCheckHeroLevelUp(outHero, false);
// 
// 					if (bResult)
// 					{
// 						break;
// 					}
// 				}
// 			}
// 		}
// 
// 		if (bResult)
// 		{
// 			bool bPrevState = IsNotify(ENotifyStateFlagEnum::BM_Hero);
// 			if (!bPrevState)
// 			{
// 				SetNotify(ENotifyStateFlagEnum::BM_Hero, bResult);
// 			}
// 		}
// 	}
	return bResult;
}

void UNotifyManager::CheckHeroSkillUpgradeEach(FString heroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill1, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill2, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill3, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill4, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill5, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill6, false);
// 
// 		SetNotify(ENotifyStateFlagEnum::BM_TotalUpgradeSkill, false);
// 		
// 
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(heroUD);
// 		if (outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				for (int32 i = 0; i < outHero->skills.Num(); ++i)
// 				{
// 					if (outHero->skills[i] > 0)
// 					{						
// 						if (!URenewal_BattleFunctionLibrary::IsMaxSkillLevel(outHero->skills[i]))
// 						{
// 							int32 skilllevelup = outHero->skills[i] + 1;
// 							FName SkillLevelUpKey = UUIFunctionLibrary::GetSkillLevelUpKey(RGameInstance->GetWorld(), outHero->heroUD, outHero->skills[i] + 1);
// 							FSkillLevelUpCostTableInfo* SkillLevelUpRow = UTableManager::GetInstancePtr()->GetSkillLevelUpCostRow(SkillLevelUpKey);
// 							if (SkillLevelUpRow)
// 							{
// 								int32 enableCount = 0;
// 								if (SkillLevelUpRow->Gold.IsValidIndex(i))
// 								{
// 									if (RGameInstance->UserInfo->Avatar.gold >= SkillLevelUpRow->Gold[i])
// 									{
// 										++enableCount;
// 									}
// 								}
// 
// 								if (SkillLevelUpRow->RequireItemQty.IsValidIndex(i))
// 								{
// 									int32 itemCount = RGameInstance->ItemInventory->GetInventoryDataCount(SkillLevelUpRow->RequireItem);
// 									if (itemCount >= SkillLevelUpRow->RequireItemQty[i])
// 									{
// 										++enableCount;
// 									}
// 								}
// 
// 								if (enableCount == 2)
// 								{
// 									switch (i)
// 									{
// 									case 0:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill1, true);
// 										break;
// 									case 1:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill2, true);
// 										break;
// 									case 2:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill3, true);
// 										break;
// 									case 3:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill4, true);
// 										break;
// 									case 4:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill5, true);
// 										break;
// 									case 5:
// 										SetNotify(ENotifyStateFlagEnum::BM_UpgradeSkill6, true);
// 										break;
// 									}
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		for (int32 i = (int32)ENotifyStateFlagEnum::BM_UpgradeSkill1; i <= (int32)ENotifyStateFlagEnum::BM_UpgradeSkill6; ++i)
// 		{
// 			if (IsNotify((ENotifyStateFlagEnum)i))
// 			{
// 				SetNotify(ENotifyStateFlagEnum::BM_TotalUpgradeSkill, true);
// 				break;
// 			}
// 		}
// 	}
}

void UNotifyManager::CheckHeroUpgradeItemEach(FString heroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, false);
// 
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemWeapon, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemArmor, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemPants, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemRing, false);
// 		SetNotify(ENotifyStateFlagEnum::BM_EquipItemAwaken, false);		
// 
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(heroUD);
// 		if (outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				for (int32 i = 0; i < outHero->equipedItemUDs.Num(); ++i)
// 				{
// 					FString equipkey = outHero->equipedItemUDs[i];
// 
// 					FITEM*  haveItem = nullptr;
// 					haveItem = RGameInstance->ItemInventory->GetInventoryDataWithUD(equipkey);
// 					if (haveItem)
// 					{
// 						FITEM outItem;
// 						if (IsCheckHeroItemEach(equipkey, outItem))
// 						{
// 							FItemTableInfo* sourceTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outItem.itemId);
// 							if (sourceTableInfo)
// 							{
// 								switch (sourceTableInfo->EquipPosition)
// 								{
// 								case EItemEquipPositionEnum::VE_Weapon:
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemWeapon, true);
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 									break;
// 								case EItemEquipPositionEnum::VE_Armor:
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemArmor, true);
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 									break;
// 								case EItemEquipPositionEnum::VE_Pants:
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemPants, true);
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 									break;
// 								case EItemEquipPositionEnum::VE_Ring:
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemRing, true);
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 									break;
// 								case EItemEquipPositionEnum::VE_Awaken:
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemAwaken, true);
// 									SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 									break;
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						if (IsCheckHeroEquipHaveItem(outHero->heroId, i))
// 						{
// 							switch ((EItemEquipPositionEnum)i)
// 							{
// 							case EItemEquipPositionEnum::VE_Weapon:
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemWeapon, true);
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 								break;
// 							case EItemEquipPositionEnum::VE_Armor:
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemArmor, true);
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 								break;
// 							case EItemEquipPositionEnum::VE_Pants:
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemPants, true);
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 								break;
// 							case EItemEquipPositionEnum::VE_Ring:
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemRing, true);
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 								break;
// 							case EItemEquipPositionEnum::VE_Awaken:
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemAwaken, true);
// 								SetNotify(ENotifyStateFlagEnum::BM_EquipItemTotal, true);
// 								break;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
}

void UNotifyManager::CheckHeroLevelEach(FString heroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		SetNotify(ENotifyStateFlagEnum::BM_HeroLevelUp, false);
// 
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(heroUD);
// 		if ( outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				if (IsCheckHeroLevelUp(outHero, false))
// 				{
// 					SetNotify(ENotifyStateFlagEnum::BM_HeroLevelUp, true);
// 				}
// 			}
// 		}
// 	}
}

bool UNotifyManager::IsCheckHeroUpgradeItem(FString heroUD)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( IsValid(RGameInstance->HeroInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->ItemInventory))
// 	{
// 		FHERO* outHero = RGameInstance->HeroInventory->GetInventoryDataWithUD(heroUD);
// 		if (outHero != nullptr)
// 		{
// 			if (outHero->activated)
// 			{
// 				for(int32 i=0; i<outHero->equipedItemUDs.Num(); ++i)				
// 				{
// 					if (outHero->equipedItemUDs[i].Len() > 0)
// 					{
// 						FITEM outItem;
// 						if (IsCheckHeroItemEach(outHero->equipedItemUDs[i], outItem))
// 						{
// 							return true;
// 						}
// 					}
// 					else
// 					{
// 						if (IsCheckHeroEquipHaveItem(outHero->heroId, i))
// 						{
// 							return true;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

	return false;
}

bool UNotifyManager::IsCheckHeroItemEach(FString sourceUD, FITEM& outItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		float sourceValue = 0.0f;
		FITEM* equipItem = nullptr;
		FItemTableInfo* sourceTableInfo = nullptr;
		FName heroId;
		equipItem = RGameInstance->RInventory->GetItemDataWithUD(sourceUD);
		if (equipItem)
		{
			if (equipItem->equipedHeroUD.Len() > 0)
			{
				FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(equipItem->equipedHeroUD);
				if (outHero != nullptr)
				{
					heroId = outHero->heroId;
				}
			}

			sourceTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(equipItem->itemId);
			if (sourceTableInfo)
			{
				sourceValue = GetItemValue(*(equipItem));
			}
		}

		if (sourceTableInfo)
		{
			TArray<FString> itemKeys;
			RGameInstance->RInventory->GetItemKeys(itemKeys);

			for (FString& key : itemKeys)
			{
				if (equipItem->itemUD != key)
				{
					FITEM* outItemTemp = nullptr;
					outItemTemp = RGameInstance->RInventory->GetItemDataWithUD(key);
					if (outItemTemp)
					{
						if (outItemTemp->equipedHeroUD.Len() > 0)
							continue;

						FItemTableInfo* destTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outItemTemp->itemId);
						if (destTableInfo)
						{
							if (sourceTableInfo->EquipItemPropertyType == destTableInfo->EquipItemPropertyType && sourceTableInfo->EquipType == destTableInfo->EquipType)
							{
								if (UUtilFunctionLibrary::CanCharacterEquipItem(heroId, outItemTemp->itemId))
								{
									float destValue = GetItemValue(*(outItemTemp));
									if (sourceValue < destValue)
									{
										outItem = *(outItemTemp);
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool UNotifyManager::GetUpAbilityItem(FString InItemUD, FITEM& outItem)
{
	bool bRt = false;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		float sourceValue = 0.0f;
		FITEM* equipItem = nullptr;
		FItemTableInfo* sourceTableInfo = nullptr;
		FName heroId;
		equipItem = RGameInstance->RInventory->GetItemDataWithUD(InItemUD);
		if (equipItem)
		{
			if (equipItem->equipedHeroUD.Len() > 0)
			{
				FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(equipItem->equipedHeroUD);
				if (outHero != nullptr)
				{
					heroId = outHero->heroId;
				}
			}

			sourceTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(equipItem->itemId);
			if (sourceTableInfo)
			{
				sourceValue = GetItemValue(*(equipItem));
			}
		}

		if (sourceTableInfo)
		{
			TArray<FString> itemKeys;
			RGameInstance->RInventory->GetItemKeys(itemKeys);

			for (FString& key : itemKeys)
			{
				if (equipItem->itemUD != key)
				{
					FITEM* outItemTemp = nullptr;
					outItemTemp = RGameInstance->RInventory->GetItemDataWithUD(key);
					if (outItemTemp)
					{
						if (outItemTemp->equipedHeroUD.Len() > 0)
							continue;

						FItemTableInfo* destTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outItemTemp->itemId);
						if (destTableInfo)
						{
							if (sourceTableInfo->EquipItemPropertyType == destTableInfo->EquipItemPropertyType && sourceTableInfo->EquipType == destTableInfo->EquipType)
							{
								if (UUtilFunctionLibrary::CanCharacterEquipItem(heroId, outItemTemp->itemId))
								{
									float destValue = GetItemValue(*(outItemTemp));
									if (sourceValue < destValue)
									{
										outItem = *(outItemTemp);
										sourceValue = destValue;
										bRt = true;
										//break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return bRt;
}

bool UNotifyManager::IsCheckHeroEquipHaveItem(FName InHeroID, int32 InEquipPosition)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		TArray<FString> itemKeys;
		RGameInstance->RInventory->GetItemKeys(itemKeys);

		for (FString& key : itemKeys)
		{
			FITEM* outItemTemp = nullptr;
			outItemTemp = RGameInstance->RInventory->GetItemDataWithUD(key);
			if (outItemTemp)
			{
				if(outItemTemp->equipedHeroUD.Len() > 0)
					continue;

				FItemTableInfo* destTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outItemTemp->itemId);
				if (destTableInfo)
				{
					if (destTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
					{
						if (destTableInfo->EquipPosition == (EItemEquipPositionEnum)InEquipPosition)
						{
							if (UUtilFunctionLibrary::CanCharacterEquipItem(InHeroID, outItemTemp->itemId))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool UNotifyManager::GetUpAbilityItemHave(FName InHeroID, int32 InEquipPosition, FITEM& outItem)
{
	bool bRt = false;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory) && IsValid(RGameInstance->UserInfo) && IsValid(RGameInstance->RInventory))
	{
		float fSourceValue = 0.0f;
		TArray<FString> itemKeys;
		RGameInstance->RInventory->GetItemKeys(itemKeys);

		for (FString& key : itemKeys)
		{
			FITEM* outItemTemp = nullptr;
			outItemTemp = RGameInstance->RInventory->GetItemDataWithUD(key);
			if (outItemTemp)
			{
				if (outItemTemp->equipedHeroUD.Len() > 0)
					continue;

				FItemTableInfo* destTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(outItemTemp->itemId);
				if (destTableInfo)
				{
					if (destTableInfo->EquipPosition == (EItemEquipPositionEnum)InEquipPosition)
					{
						if (UUtilFunctionLibrary::CanCharacterEquipItem(InHeroID, outItemTemp->itemId))
						{
							float fDestValue = GetItemValue(*(outItemTemp));
							if (fSourceValue < fDestValue)
							{
								outItem = *(outItemTemp);
								bRt = true;
								fSourceValue = fDestValue;
								//break;
							}
						}
					}
				}
			}
		}
	}

	return bRt;
}

float UNotifyManager::GetItemValue(FITEM& sourceItem)
{
	// FWorkedItemTableInfo no more
	//FWorkedItemTableInfo WorkedItemTableInfo;
	//if (URenewal_BattleFunctionLibrary::GetCharacterItemTableWorkedInfo(WorkedItemTableInfo, sourceItem.itemId, sourceItem.enchant, sourceItem.level))
	//{
	//	FItemTableInfo* sourceTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(sourceItem.itemId);
	//	if (sourceTableInfo)
	//	{
	//		switch (sourceTableInfo->EquipItemPropertyType)
	//		{
	//		case EEquipItemPropertyType::VE_PhysicalWeapon:
	//		case EEquipItemPropertyType::VE_MagicWeapon:
	//		{
	//			float rtValue = 0.0f;
	//			rtValue = WorkedItemTableInfo.GetEnchantPhysicalAttackPowerNotAddOption();
	//			rtValue += WorkedItemTableInfo.GetEnchantMagicAttackPowerNotAddOption();
	//			return rtValue;
	//		}

	//		case EEquipItemPropertyType::VE_PhysicalGuard:
	//		case EEquipItemPropertyType::VE_MagicGuard:
	//		{
	//			float rtValue = 0.0f;
	//			rtValue = WorkedItemTableInfo.GetEnchantArmorNotAddOption();
	//			rtValue += WorkedItemTableInfo.GetEnchantMagicResistanceNotAddOption();
	//			return rtValue;
	//		}

	//		case EEquipItemPropertyType::VE_HPGuard:
	//			return WorkedItemTableInfo.GetEnchantMaxHPNotAddOption();
	//		}
	//	}
	//}

	return 0.0f;
}
