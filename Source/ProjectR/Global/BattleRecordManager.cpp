// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BattleRecordManager.h"

#include "GlobalIntegrated.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"



#include "UtilFunctionIntegrated.h"
#include "Table/CharacterTableInfo.h"



/////////////////////////////////////////////////////////////////////
FBattleRecord* FUserBattleRecord::GetTotalRecord()
{
	int32 Rec_Count = HeroBattleRecordList.Num();
	if (0 < Rec_Count)
	{
		FBattleRecord* LastRecord = nullptr;
		for (int32 n = Rec_Count - 1; n >= 0; --n)
		{
			LastRecord = HeroBattleRecordList[n];
			if (LastRecord)
			{
				break;
			}
		}

		TotalRecord.PlayerUK = LastRecord->PlayerUK;
		TotalRecord.CharacterID = LastRecord->CharacterID;
		TotalRecord.UserKID = LastRecord->UserKID;
		TotalRecord.GroupKey = LastRecord->GroupKey;

		TotalRecord.NickName = UserNickName;

		TotalRecord.nAmountOfNormalAttack = GetSumOfNormalAttack();
		TotalRecord.nAmountOfSkillAttack = GetSumOfSkillAttack();
		TotalRecord.nAmountOfDamage = GetSumOfDamage();
		TotalRecord.nAmountOfHeal = GetSumOfHeal();
		TotalRecord.nKillCount = GetSumOfKillCount();
		TotalRecord.nSkillCount = GetSumOfSkillCount();
	}
	return (&TotalRecord);
}

int32 FUserBattleRecord::GetSumOfNormalAttack()
{
	int32 Sum = 0;
	if(0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nAmountOfNormalAttack);
			}
		}
	}
	return Sum;
}

int32 FUserBattleRecord::GetSumOfSkillAttack()
{
	int32 Sum = 0;
	if (0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nAmountOfSkillAttack);
			}
		}
	}
	return Sum;
}

int32 FUserBattleRecord::GetSumOfAllAttack()
{
	return (GetSumOfNormalAttack() + GetSumOfSkillAttack());
}

int32 FUserBattleRecord::GetSumOfDamage()
{
	int32 Sum = 0;
	if (0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nAmountOfDamage);
			}
		}
	}
	return Sum;
}

int32 FUserBattleRecord::GetSumOfHeal()
{
	int32 Sum = 0;
	if (0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nAmountOfHeal);
			}
		}
	}
	return Sum;
}

int32 FUserBattleRecord::GetSumOfKillCount()
{
	int32 Sum = 0;
	if (0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nKillCount);
			}
		}
	}
	return Sum;
}

int32 FUserBattleRecord::GetSumOfSkillCount()
{
	int32 Sum = 0;
	if (0 < HeroBattleRecordList.Num())
	{
		for (FBattleRecord* Record : HeroBattleRecordList)
		{
			if (Record)
			{
				Sum += (Record->nSkillCount);
			}
		}
	}
	return Sum;
}

/////////////////////////////////////////////////////////////////////
void UBattleRecordManager::Initialize()
{
	BattleRecordMap.Empty();
	BattleRecordUserKIDMap.Empty();
	BattleRecordGroupMap.Empty();

	ResetDamageMeterRank();
}

void UBattleRecordManager::ResetBattleRecordData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("GameInstance is Empty"));
	checkf(RGameInstance->EventManager != nullptr, TEXT("RGameInstance->EventManager is Empty"));

	RemoveBattleInfoDelegate();

	OnBattleRecord_UpdateHP.Clear();
	OnBattleRecord_UpdateKillCount.Clear();

	if (false == RGameInstance->EventManager->OnDeadActor.IsAlreadyBound(this, &UBattleRecordManager::CheckKillCount))
		RGameInstance->EventManager->OnDeadActor.AddDynamic(this, &UBattleRecordManager::CheckKillCount);
	
	ResetDamageMeterRank();
}

void UBattleRecordManager::SetHeroBattleInfoDelegate(AActor* BattleActor)
{
	if (nullptr == BattleActor)
		return;

	auto Actor_BaseProperty = Cast<IEntityBaseProperty>(BattleActor);
	if (nullptr == Actor_BaseProperty)
		return;

	ECharacterCategoryTypeEnum ActorCategory = Actor_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != ActorCategory && 
		ECharacterCategoryTypeEnum::VE_Npc != ActorCategory && 
		ECharacterCategoryTypeEnum::VE_BattleObject != ActorCategory)
		return;

	if (ECharacterCategoryTypeEnum::VE_Npc == ActorCategory || ECharacterCategoryTypeEnum::VE_BattleObject == ActorCategory)
	{
		FText NPCName = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Npc, Actor_BaseProperty->GetCharacterID());
		if (NPCName.IsEmpty())
			return;
	}

	FBattleRecord* Record = GetBattleRecord(Actor_BaseProperty->GetPlayerUK());
	if (Record)
	{
		if (Record->ptrActor.IsValid())
		{
			AActor* CurrActor = Record->ptrActor.Get();
			if (BattleActor == CurrActor)
			{
				return;
			}
			else
			{
				DetachBattleInfoDelegate(CurrActor);
			}
		}

		AttachBattleInfoDelegate(BattleActor);
		AddBattleRecord(BattleActor, false);
	}
	else
	{
		AttachBattleInfoDelegate(BattleActor);
		AddBattleRecord(BattleActor, true);
	}
}

void UBattleRecordManager::SetUserNickName(int32 UserKID, FString& NickName, FString& SubNick)
{
	if (0 != UserKID && BattleRecordUserKIDMap.Contains(UserKID))
	{
		BattleRecordUserKIDMap[UserKID].UserNickName = NickName;
		if(!SubNick.IsEmpty())
			BattleRecordUserKIDMap[UserKID].UserSubNick = SubNick;
	}
}

void UBattleRecordManager::InitRealTimeBattleRecord()
{
	BattleRecordUserKIDMap.Reset();
	BattleRecordGroupMap.Reset();

	DamageMeterRankerList.Reset();
	FString	SubNick = "";
	for (auto& Elem : BattleRecordMap)
	{
		FBattleRecord& Record = Elem.Value;

		AddUserBattleRecord(Record.UserKID, Record.GroupKey, Record.PlayerUK);
		SetUserNickName(Record.UserKID, Record.NickName, SubNick);
	}

	FUserBattleRecord* UserRecord = nullptr;
	for (auto& UserElem : BattleRecordUserKIDMap)
	{
		if (0 != UserElem.Key)
		{
			UserRecord = GetUserBattleRecord(UserElem.Key);
			if (UserRecord)
			{
				UserRecord->AccumulatedAttack = UserRecord->GetSumOfAllAttack();
			}
		}
	}
	ResetDamageMeterRank();
}

void UBattleRecordManager::RemoveBattleInfoDelegate()
{
	for (auto& RecordElem : BattleRecordMap)
	{
		if ((RecordElem.Value).ptrActor.IsValid())
		{
			DetachBattleInfoDelegate((RecordElem.Value).ptrActor.Get());
		}
	}

	BattleRecordMap.Reset();
	BattleRecordUserKIDMap.Reset();
	BattleRecordGroupMap.Reset();
	DamageMeterRankerList.Reset();
}

//////////////////////////////////////////////////////////////////////////
void UBattleRecordManager::CheckKillCount(AActor* DeadActor, AActor* FromActor)
{
	if (false == IsValid(DeadActor))
		return;

	auto Dead_BaseProperty = Cast<IEntityBaseProperty>(DeadActor);
	if (nullptr == Dead_BaseProperty)
		return;

	ECharacterCategoryTypeEnum DeadActorCategory = Dead_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != DeadActorCategory &&
		ECharacterCategoryTypeEnum::VE_Npc != DeadActorCategory && 
		ECharacterCategoryTypeEnum::VE_BattleObject != DeadActorCategory)
		return;

	if (false == IsValid(FromActor))
		return;

	auto Killer_BaseProperty = Cast<IEntityBaseProperty>(FromActor);
	if (nullptr == Killer_BaseProperty)
		return;

	ECharacterCategoryTypeEnum KillerCategory = Killer_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Npc == KillerCategory &&
		ECharacterCategoryTypeEnum::VE_Npc == DeadActorCategory ||
		ECharacterCategoryTypeEnum::VE_BattleObject == KillerCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject == DeadActorCategory)
		return;

	if (ECharacterCategoryTypeEnum::VE_Minion == KillerCategory)
	{
		auto Killer_BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);
		if (nullptr == Killer_BattleEntComp)
			return;

		AActor* Summoner = Killer_BattleEntComp->GetSummonOwner();
		if (nullptr == Summoner)
			return;

		Killer_BaseProperty = Cast<IEntityBaseProperty>(Summoner);
		if (nullptr == Killer_BaseProperty)
			return;

		FromActor = Summoner;
	}

	FBattleRecord* KillerHeroBattleRecord = GetBattleRecord(Killer_BaseProperty->GetPlayerUK());
	if (KillerHeroBattleRecord)
	{
		KillerHeroBattleRecord->nKillCount++;
	}

	FBattleRecord* DeadHeroBattleRecord = GetBattleRecord(Dead_BaseProperty->GetPlayerUK());
	if (DeadHeroBattleRecord)
	{
		DeadHeroBattleRecord->ptrActor.Reset();
		OnBattleRecord_UpdateKillCount.Broadcast(DeadActor, FromActor);
	}

	OnBattleRecord_UpdateHP.Broadcast();
}

void UBattleRecordManager::CheckAmountDamage(AActor* Attacker, AActor* Victim, int32 damage, bool SkillAttack /*= false*/)
{
	if (damage <= 0)
		return;

	if (nullptr == Victim)
		return;

	auto Victim_BaseProperty = Cast<IEntityBaseProperty>(Victim);
	if (nullptr == Victim_BaseProperty)
		return;

	ECharacterCategoryTypeEnum VictimCategory = Victim_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != VictimCategory &&
		ECharacterCategoryTypeEnum::VE_Npc != VictimCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject != VictimCategory)
		return;

	if (nullptr == Attacker)
		return;

	auto Attacker_BaseProperty = Cast<IEntityBaseProperty>(Attacker);
	if (nullptr == Attacker_BaseProperty)
		return;

	ECharacterCategoryTypeEnum AttackerCategory = Attacker_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Npc == AttackerCategory &&
		ECharacterCategoryTypeEnum::VE_Npc == VictimCategory ||
		ECharacterCategoryTypeEnum::VE_BattleObject == AttackerCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject == VictimCategory)
		return;

	if (ECharacterCategoryTypeEnum::VE_Minion == AttackerCategory)
	{
		auto Attacker_BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Attacker);
		if (nullptr == Attacker_BattleEntComp)
			return;

		AActor* Summoner = Attacker_BattleEntComp->GetSummonOwner();
		if (nullptr == Summoner)
			return;

		Attacker_BaseProperty = Cast<IEntityBaseProperty>(Summoner);
		if (nullptr == Attacker_BaseProperty)
			return;
	}

	FBattleRecord* AttackerBattleRecord = GetBattleRecord(Attacker_BaseProperty->GetPlayerUK());
	if (AttackerBattleRecord)
	{
		if (SkillAttack)
		{
			AttackerBattleRecord->nAmountOfSkillAttack += damage;
		}
		else
		{
			AttackerBattleRecord->nAmountOfNormalAttack += damage;
		}

		if (0 != AttackerBattleRecord->UserKID)
		{
			FUserBattleRecord* UserRecord = GetUserBattleRecord(AttackerBattleRecord->UserKID);
			if (UserRecord)
			{
				UpdateDamageMeterRank(UserRecord);
			}
		}
	}

	FBattleRecord* VictimBattleRecord = GetBattleRecord(Victim_BaseProperty->GetPlayerUK());
	if (VictimBattleRecord)
	{
		VictimBattleRecord->nAmountOfDamage += damage;
	}
	
	if (AttackerBattleRecord && VictimBattleRecord)
	{
		OnBattleRecord_UpdateHP.Broadcast();
	}
}

void UBattleRecordManager::CheckAmountHeal(float Heal, AActor* FromActor)
{
	if (nullptr == FromActor)
		return;

	auto FromActor_BaseProperty = Cast<IEntityBaseProperty>(FromActor);
	if (nullptr == FromActor_BaseProperty)
		return;

	if (ECharacterCategoryTypeEnum::VE_Minion == FromActor_BaseProperty->GetCharacterCategory())
	{
		auto FromActor_BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);
		if (nullptr == FromActor_BattleEntComp)
			return;

		AActor* Summoner = FromActor_BattleEntComp->GetSummonOwner();
		if (nullptr == Summoner)
			return;

		FromActor_BaseProperty = Cast<IEntityBaseProperty>(Summoner);
		if (nullptr == FromActor_BaseProperty)
			return;
	}

	ECharacterCategoryTypeEnum FromActorCategory = FromActor_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != FromActorCategory &&
		ECharacterCategoryTypeEnum::VE_Npc != FromActorCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject != FromActorCategory)
		return;

	FBattleRecord* ActorBattleRecord = GetBattleRecord(FromActor_BaseProperty->GetPlayerUK());
	if (ActorBattleRecord)
	{
		ActorBattleRecord->nAmountOfHeal += Heal;
		OnBattleRecord_UpdateHP.Broadcast();
	}
}

void UBattleRecordManager::CheckSkillCount(AActor* Caster, UISkill_V2* UseSkill)
{
	if (nullptr == Caster)
		return;

	auto Caster_BaseProperty = Cast<IEntityBaseProperty>(Caster);
	if (nullptr == Caster_BaseProperty)
		return;

	ECharacterCategoryTypeEnum CasterCategory = Caster_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != CasterCategory &&
		ECharacterCategoryTypeEnum::VE_Npc != CasterCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject != CasterCategory)
		return;

	FBattleRecord* CasterBattleRecord = GetBattleRecord(Caster_BaseProperty->GetPlayerUK());
	if (CasterBattleRecord)
	{
		CasterBattleRecord->nSkillCount++;
	}
}


//////////////////////////////////////////////////////////////////////////
//bool UBattleRecordManager::GetKillCount(int32 GroupKey, TArray<int32>& count_list)
//{
//	count_list.Empty();
//
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return false;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return false;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return false;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		count_list.Add(Record->nKillCount);
//	}
//	return true;
//}
//
//bool UBattleRecordManager::GetAmountOfAttack(int32 GroupKey, TArray<int32>& amount_list)
//{
//	amount_list.Empty();
//
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return false;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return false;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return false;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		amount_list.Add(Record->nAmountOfNormalAttack + Record->nAmountOfSkillAttack);
//	}
//	return true;
//}
//
//bool UBattleRecordManager::GetAmountOfDamage(int32 GroupKey, TArray<int32>& amount_list)
//{
//	amount_list.Empty();
//
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return false;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return false;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return false;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		amount_list.Add(Record->nAmountOfDamage);
//	}
//	return true;
//}
//
//bool UBattleRecordManager::GetAmountOfHeal(int32 GroupKey, TArray<int32>& amount_list)
//{
//	amount_list.Empty();
//
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return false;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return false;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return false;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		amount_list.Add(Record->nAmountOfHeal);
//	}
//	return true;
//}
//
//bool UBattleRecordManager::GetSkillCount(int32 GroupKey, TArray<int32>& count_list)
//{
//	count_list.Empty();
//
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return false;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return false;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return false;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		count_list.Add(Record->nSkillCount);
//	}
//	return true;
//}

void UBattleRecordManager::UpdateforRTSActorHP(int32 GroupKey)
{
	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
	if (GroupRecord)
	{
		if (0 < GroupRecord->UserBattleRecordList.Num())
		{
			FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
			if (UserRecord)
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
				{
					if (Record->ptrActor.IsValid())
					{
						AActor* pActor = Record->ptrActor.Get();
						for (TPair<int16, FRTS_ACTOR>& Actor : RGameInstance->RealTimeModManager->ActorList)
						{
							FRTS_ACTOR& rActorInfo = Actor.Value;
							IEntityBaseProperty* pEntity = Cast<IEntityBaseProperty>(pActor);
							if(!pEntity)
								continue;
							int32 nKID = pEntity->GetUserKID();
							FName ID = pEntity->GetCharacterID();
							if (nKID == rActorInfo.kId && ID == rActorInfo.actorId)
							{
								UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(pActor);
								rActorInfo.curHp = (uint16)BattleEntityComp->GetHP();
								break;
							}
						}
					}
				}
			}
		}
	}
}

int32 UBattleRecordManager::GetTeamTotalHP(int32 GroupKey, bool bMaxHP)
{
	int32 TotalHP = 0;

	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
	if (GroupRecord)
	{
		if (0 < GroupRecord->UserBattleRecordList.Num())
		{
			UBattleEntityComponent* BattleEntityComp = nullptr;

			FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
			if (UserRecord)
			{
				for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
				{
					if (Record->ptrActor.IsValid())
					{
						BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Record->ptrActor.Get());
						if (BattleEntityComp)
						{
							if(bMaxHP)
							{
								UCharacterBattleInfo* pBattleInfo = BattleEntityComp->GetBattleInfo();
								TotalHP += pBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
							}
							else
								TotalHP += BattleEntityComp->GetHP();
						}
					}
				}
			}
		}
	}
	return TotalHP;
}

//FBattleRecord* UBattleRecordManager::GetBattleRecord(int32 GroupKey, FString FindHeroUD)
//{
//	FGroupBattleRecord* GroupRecord = GetGroupBattleRecord(GroupKey);
//	if (nullptr == GroupRecord)
//		return nullptr;
//
//	if (0 >= GroupRecord->UserBattleRecordList.Num())
//		return nullptr;
//
//	FUserBattleRecord* UserRecord = GetUserBattleRecord(GroupRecord->UserKIDs[0]);
//	if (nullptr == UserRecord)
//		return nullptr;
//
//	for (FBattleRecord* Record : UserRecord->HeroBattleRecordList)
//	{
//		if (FindHeroUD.Equals(Record->HeroUD))
//		{
//			return Record;
//		}
//	}
//	return nullptr;
//}

//////////////////////////////////////////////////////////////////////////

FBattleRecord* UBattleRecordManager::GetBattleRecord(AActor* hero_actor)
{
	auto Actor_BaseProperty = Cast<IEntityBaseProperty>(hero_actor);
	if (Actor_BaseProperty == nullptr)
		return nullptr;

	ECharacterCategoryTypeEnum ActorCategory = Actor_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero != ActorCategory &&
		ECharacterCategoryTypeEnum::VE_Npc != ActorCategory &&
		ECharacterCategoryTypeEnum::VE_BattleObject != ActorCategory)
		return nullptr;

	return GetBattleRecord(Actor_BaseProperty->GetPlayerUK());
}

FBattleRecord* UBattleRecordManager::GetBattleRecord(int32 PlayerUK)
{
	if (BattleRecordMap.Contains(PlayerUK))
	{
		return &(BattleRecordMap[PlayerUK]);
	}
	return nullptr;
}

FUserBattleRecord* UBattleRecordManager::GetUserBattleRecord(int32 UserKID)
{
	if (BattleRecordUserKIDMap.Contains(UserKID))
	{
		FUserBattleRecord& UserRecord = BattleRecordUserKIDMap[UserKID];

		int32 RecordCount = UserRecord.HeroPlayerUKs.Num();
		UserRecord.HeroBattleRecordList.Init(nullptr, RecordCount);

		int32 ListCount = 0;
		int32 AccumAttack = 0;
		FBattleRecord* Record = nullptr;
		for (int32 UK : UserRecord.HeroPlayerUKs)
		{
			Record = GetBattleRecord(UK);
			if (Record)
			{
				UserRecord.HeroBattleRecordList[ListCount] = Record;
				ListCount++;

				AccumAttack += Record->GetAmountOfAttack();
			}
		}
		UserRecord.AccumulatedAttack = AccumAttack;

		return &(BattleRecordUserKIDMap[UserKID]);
	}
	return nullptr;
}

FGroupBattleRecord* UBattleRecordManager::GetGroupBattleRecord(int32 GroupKey)
{
	if (BattleRecordGroupMap.Contains(GroupKey))
	{
		FGroupBattleRecord& GroupRecord = BattleRecordGroupMap[GroupKey];

		int32 UserCount = GroupRecord.UserKIDs.Num();
		GroupRecord.UserBattleRecordList.Init(nullptr, UserCount);

		int32 ListCount = 0;
		FUserBattleRecord* UserRecord = nullptr;
		for (int32 KID : GroupRecord.UserKIDs)
		{
			UserRecord = GetUserBattleRecord(KID);
			if (UserRecord)
			{
				GroupRecord.UserBattleRecordList[ListCount] = UserRecord;
				ListCount++;
			}
		}

		return &(BattleRecordGroupMap[GroupKey]);
	}
	return nullptr;
}

void UBattleRecordManager::GetDamageMeterRank(TArray<FBattleRecord*>& DamageRankList)
{
	DamageRankList.Empty();
	DamageRankList.Reserve(DAMAGE_METER_RANK_MAX);

	FUserBattleRecord* UserRecord = nullptr;
	FBattleRecord* Record = nullptr;
	for (auto& RankData : DamageMeterRankerList)
	{
		if (0 != RankData.RankerKID && BattleRecordUserKIDMap.Contains(RankData.RankerKID))
		{
			UserRecord = GetUserBattleRecord(RankData.RankerKID);
			if (UserRecord)
			{
				DamageRankList.Emplace(UserRecord->GetTotalRecord());
			}
		}
	}
}

void UBattleRecordManager::AddBattleRecord(AActor* BattleActor, bool AddNew)
{
	auto Actor_BaseProperty = Cast<IEntityBaseProperty>(BattleActor);
	if (Actor_BaseProperty == nullptr)
		return;

	int32 ActorPlayerUK = Actor_BaseProperty->GetPlayerUK();
	if (AddNew)
	{
		BattleRecordMap.Emplace(ActorPlayerUK, FBattleRecord());
		FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(Actor_BaseProperty->GetCharacterCategory(), Actor_BaseProperty->GetCharacterID());
		if (CharacterInfo)
		{
			BattleRecordMap[ActorPlayerUK].NickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterInfo->DisplayName).ToString();
		}
	}

	FBattleRecord& BattleRecord = BattleRecordMap[ActorPlayerUK];
	BattleRecord.ptrActor = BattleActor;
	BattleRecord.PlayerUK = ActorPlayerUK;
	BattleRecord.CharacterID = Actor_BaseProperty->GetCharacterID();
	int32 PartyID = Actor_BaseProperty->GetPartyID();
	ECharacterCategoryTypeEnum ActorCategory = Actor_BaseProperty->GetCharacterCategory();
	if (ECharacterCategoryTypeEnum::VE_Hero == ActorCategory)
	{
		BattleRecord.UserKID = Actor_BaseProperty->GetUserKID();
		BattleRecord.GroupKey = Actor_BaseProperty->GetGroupKey();
	}
	else if (ECharacterCategoryTypeEnum::VE_Npc == ActorCategory ||
		ECharacterCategoryTypeEnum::VE_BattleObject == ActorCategory)
	{
		if (PartyID_PC_MY == PartyID)
		{
			BattleRecord.UserKID = KID_NPC_MY;
			BattleRecord.GroupKey = GroupKey_SINGLE_MY;
		}
		else
		{
			BattleRecord.UserKID = KID_NPC_ENEMY;
			BattleRecord.GroupKey = GroupKey_SINGLE_ENEMY;
		}
	}
	
	AddUserBattleRecord(BattleRecord.UserKID, BattleRecord.GroupKey, BattleRecord.PlayerUK);

	// For Debug
	//FBattleRecord* btl_rec = nullptr;
	//for (auto& Elem : BattleRecordMap)
	//{
	//	btl_rec = GetBattleRecord(Elem.Key);

	//	UE_LOG(LogTemp, Warning, TEXT(">>>>>>> AddBattleRecord RecMap(GK(%d) UK(%d) KID(%d) : %x)"),
	//		btl_rec->GroupKey, btl_rec->PlayerUK, btl_rec->UserKID, btl_rec);
	//}
	//
	//FUserBattleRecord* UsrRec = nullptr;
	//for (auto& UsrElem : BattleRecordUserKIDMap)
	//{
	//	UsrRec = GetUserBattleRecord(UsrElem.Key);
	//	for (int n = 0; n < UsrRec->HeroBattleRecordList.Num(); ++n)
	//	{
	//		btl_rec = UsrRec->HeroBattleRecordList[n];

	//		UE_LOG(LogTemp, Warning, TEXT(">>>>>>> AddBattleRecord UsrMap(GK(%d) UK(%d) KID(%d) : %x)"),
	//			btl_rec->GroupKey, btl_rec->PlayerUK, btl_rec->UserKID, btl_rec);
	//	}
	//}

	//FGroupBattleRecord* GrpRec = GetGroupBattleRecord(BattleRecord.GroupKey);
	//for (auto& GrpElem : BattleRecordGroupMap)
	//{
	//	GrpRec = GetGroupBattleRecord(GrpElem.Key);
	//	for (int g = 0; g < GrpRec->UserBattleRecordList.Num(); ++g)
	//	{
	//		UsrRec = GrpRec->UserBattleRecordList[g];
	//		for (int n = 0; n < UsrRec->HeroBattleRecordList.Num(); ++n)
	//		{
	//			btl_rec = UsrRec->HeroBattleRecordList[n];

	//			UE_LOG(LogTemp, Warning, TEXT(">>>>>>> AddBattleRecord GrpMap(GK(%d) UK(%d) KID(%d) : %x)"),
	//				btl_rec->GroupKey, btl_rec->PlayerUK, btl_rec->UserKID, btl_rec);
	//		}
	//	}
	//}
}

void UBattleRecordManager::AddUserBattleRecord(int32 UserKID, int32 GroupKey, int32 PlayerUK)
{
	if (BattleRecordUserKIDMap.Contains(UserKID))
	{
		FUserBattleRecord& UserRecord = BattleRecordUserKIDMap[UserKID];
		for (int32 UK : UserRecord.HeroPlayerUKs)
		{
			if (PlayerUK == UK)
			{
				return;
			}
		}
		UserRecord.HeroPlayerUKs.Emplace(PlayerUK);
	}
	else
	{
		BattleRecordUserKIDMap.Emplace(UserKID, FUserBattleRecord());

		FUserBattleRecord& UserBattleRecord = BattleRecordUserKIDMap[UserKID];
		UserBattleRecord.UserKID = UserKID;
		UserBattleRecord.HeroPlayerUKs.Emplace(PlayerUK);

		AddGroupBattleRecord(GroupKey, UserKID);
	}
}

void UBattleRecordManager::AddGroupBattleRecord(int32 GroupKey, int32 UserKID)
{
	if (BattleRecordGroupMap.Contains(GroupKey))
	{
		FGroupBattleRecord& GroupBattleRecord = BattleRecordGroupMap[GroupKey];
		for (int32 KID : GroupBattleRecord.UserKIDs)
		{
			if (UserKID == KID)
			{
				return;
			}
		}
		GroupBattleRecord.UserKIDs.Emplace(UserKID);
	}
	else
	{
		BattleRecordGroupMap.Emplace(GroupKey, FGroupBattleRecord());

		FGroupBattleRecord& GroupBattleRecord = BattleRecordGroupMap[GroupKey];
		GroupBattleRecord.UserKIDs.Emplace(UserKID);
	}
}

//////////////////////////////////////////////////////////////////////////


void UBattleRecordManager::AttachBattleInfoDelegate(AActor* TargetActor)
{
	auto battle_ent_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (battle_ent_comp)
	{
		if (battle_ent_comp->OnNotifyDamageDelegate.IsAlreadyBound(this, &UBattleRecordManager::CheckAmountDamage) == false)
			battle_ent_comp->OnNotifyDamageDelegate.AddDynamic(this, &UBattleRecordManager::CheckAmountDamage);
		if (battle_ent_comp->OnReceiveHealDelegate.IsAlreadyBound(this, &UBattleRecordManager::CheckAmountHeal) == false)
			battle_ent_comp->OnReceiveHealDelegate.AddDynamic(this, &UBattleRecordManager::CheckAmountHeal);
	}

	auto skill_ent_comp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetActor);
	if (skill_ent_comp)
	{
		if (skill_ent_comp->OnNotifySkillCasting.IsAlreadyBound(this, &UBattleRecordManager::CheckSkillCount) == false)
			skill_ent_comp->OnNotifySkillCasting.AddDynamic(this, &UBattleRecordManager::CheckSkillCount);
	}
}

void UBattleRecordManager::DetachBattleInfoDelegate(AActor* TargetActor)
{
	auto battle_ent_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (battle_ent_comp)
	{
		battle_ent_comp->OnNotifyDamageDelegate.RemoveDynamic(this, &UBattleRecordManager::CheckAmountDamage);
		battle_ent_comp->OnReceiveHealDelegate.RemoveDynamic(this, &UBattleRecordManager::CheckAmountHeal);
	}

	auto skill_ent_comp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetActor);
	if (skill_ent_comp)
	{
		skill_ent_comp->OnNotifySkillCasting.RemoveDynamic(this, &UBattleRecordManager::CheckSkillCount);
	}
}

void UBattleRecordManager::ResetDamageMeterRank()
{
	DamageMeterRankerList.Empty();
	DamageMeterRankerList.Init(FDamageMeterRank(), DAMAGE_METER_RANK_MAX);

	FUserBattleRecord* UserRecord = nullptr;
	for (auto& UserElem : BattleRecordUserKIDMap)
	{
		if(0 != UserElem.Key)
		{
			UserRecord = GetUserBattleRecord(UserElem.Key);
			if (UserRecord)
			{
				UpdateDamageMeterRank(UserRecord);
			}
		}
	}
}

void UBattleRecordManager::UpdateDamageMeterRank(FUserBattleRecord* UserRecord)
{
	if (UserRecord)
	{
		if (0 == UserRecord->UserKID || 0 >= UserRecord->AccumulatedAttack)
			return;

		FDamageMeterRank CheckData;
		FDamageMeterRank TempData;

		CheckData.RankerKID = UserRecord->UserKID;
		CheckData.AccumulatedAttack = UserRecord->AccumulatedAttack;

		bool AlreadySet = false;
		for (int32 n = 0; n < DAMAGE_METER_RANK_MAX; ++n)
		{
			if (0 != DamageMeterRankerList[n].RankerKID)
			{
				if (CheckData.AccumulatedAttack > DamageMeterRankerList[n].AccumulatedAttack)
				{
					if (DamageMeterRankerList[n].RankerKID == UserRecord->UserKID)
					{
						DamageMeterRankerList[n] = CheckData;
						break;
					}
					else
					{
						TempData = DamageMeterRankerList[n];
						DamageMeterRankerList[n] = CheckData;
						CheckData = TempData;
					}
				}
			}
			else
			{
				DamageMeterRankerList[n] = CheckData;
				break;
			}
		}
	}
}