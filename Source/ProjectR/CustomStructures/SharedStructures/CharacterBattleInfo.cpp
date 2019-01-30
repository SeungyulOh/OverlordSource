// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "TutorialLevelScriptActor.h"
#include "UtilFunctionIntegrated.h"
#include "CharacterBattleInfo.h"
#include "UtilFunctionLibrary.h"



void UCharacterBattleInfo::Init()
{

	CharacterProperty.Clear();
	AdditionalCharacterProperty.Clear();
	TotalItemCharacterProperty.Clear();
	FinalCharacterProperty.Clear();

	// Item stats
	ItemStatsByEquipPosition.Empty();
	ItemStatsByEquipPosition.AddDefaulted(FItemTableInfo::NumItemEquipPositions());
	for (int32 i = 0; i < FItemTableInfo::NumItemEquipPositions(); i++)
	{
		ItemStatsByEquipPosition[i].AddDefaulted((int32)EItemStatOption::Max);
	}

	// passive skills
	ItemSkillIDList.Init(FName(), (int32)EItemEquipPositionEnum::VE_Max);
}

float UCharacterBattleInfo::GetAvatarAbilityProperty(EAvatarAbilityType AbilityType)
{
	if (AvatarAbilityPropertyMap.Contains(AbilityType))
	{
		return AvatarAbilityPropertyMap[AbilityType];
	}
	return 0.0f;
}

int32 UCharacterBattleInfo::SetHP(int32 hp)
{
	if (HP < hp)
	{
		UE_LOG(LogClass, Log, TEXT("check"));
	}

	if (UUtilFunctionLibrary::isTutorialGameMode())
	{
		float percent = hp / GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP);
		UUtilFunctionLibrary::GetTutorialLevelScriptActor()->OnHPChanged.Broadcast(OwnerActor, percent);
	}


	HP = hp;

	if (HP > GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP))
	{
		HP = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP);
	}
	else if (HP <= 0)
	{
		HP = 0;
	}

	return HP;
}

void UCharacterBattleInfo::AddHP(int32 hp)
{
	SetHP( HP + hp );
}

void UCharacterBattleInfo::ValidateFinalCharacterProperty()
{
	FinalCharacterProperty.MaxHP = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP);
	FinalCharacterProperty.PhysicalAttackPower = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
	FinalCharacterProperty.MagicAttackPower = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MagicAttackPower);
	FinalCharacterProperty.SightRange = GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_SightRange);
	FinalCharacterProperty.AttackRange = GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
	FinalCharacterProperty.MoveSpeed = GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
	FinalCharacterProperty.AttackSpeed = GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackSpeed);
	FinalCharacterProperty.UnitRadius = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_UnitRadius);
	FinalCharacterProperty.Armor = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_Armor);
 	FinalCharacterProperty.MagicResistance = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MagicResistance);
	FinalCharacterProperty.DodgeValue = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_DodgeValue);
	FinalCharacterProperty.CriticalValue = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_CriticalValue);
	FinalCharacterProperty.CriticalDamageRate_Scale = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale);
	FinalCharacterProperty.PierceValue = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_PierceValue);
	FinalCharacterProperty.AccuracyValue = GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_AccuracyValue);
	
	ValidateTotalItemProperty();
}

void UCharacterBattleInfo::ValidateTotalItemProperty()
{
	TotalItemCharacterProperty.MaxHP = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP);
	
	TotalItemCharacterProperty.PhysicalAttackPower = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
	TotalItemCharacterProperty.MagicAttackPower = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_MagicAttackPower);
	TotalItemCharacterProperty.AttackRange = GetCharacterItemPropertyAllWithCharacter< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
	TotalItemCharacterProperty.MoveSpeed = GetCharacterItemPropertyAllWithCharacter< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
	TotalItemCharacterProperty.AttackSpeed = GetCharacterItemPropertyAllWithCharacter< float >(ECharacterPropertyTypeEnum::VE_AttackSpeed);
	TotalItemCharacterProperty.Armor = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_Armor);
	TotalItemCharacterProperty.MagicResistance = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_MagicResistance);
	TotalItemCharacterProperty.DodgeValue = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_DodgeValue);
	TotalItemCharacterProperty.CriticalValue = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_CriticalValue);
	TotalItemCharacterProperty.CriticalDamageRate_Scale = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale);
	TotalItemCharacterProperty.PierceValue = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_PierceValue);
	TotalItemCharacterProperty.AccuracyValue = GetCharacterItemPropertyAllWithCharacter< int32 >(ECharacterPropertyTypeEnum::VE_AccuracyValue);
}


/*
Summon minion related..
*/


TArray< TWeakObjectPtr<AActor> > UCharacterBattleInfo::GetSummonedMinions()
{
	return SummonedMinionList;
}


// Get summoned minion count
int32 UCharacterBattleInfo::GetSummonedMinionCount()
{
	return SummonedMinionList.Num();
	
}

//Check Delete Minion
void UCharacterBattleInfo::CheckDeleteSummonedMinion(FName CheckMinionID)
{
	for (auto& SummonedCharacter : SummonedMinionList)
	{
		if (SummonedCharacter.IsValid())
		{
			auto SummonedCharacter_BaseProperty = Cast<IEntityBaseProperty>(SummonedCharacter.Get());
			if (SummonedCharacter_BaseProperty != nullptr)
			{
				if (SummonedCharacter_BaseProperty->GetCharacterID() == CheckMinionID)
				{
					SummonedCharacter->Destroy();
					SummonedCharacter = nullptr;
					break;
				}
			}
		}		
	}

	SummonedMinionList.RemoveAll([](const TWeakObjectPtr< AActor > Ptr){
		return Ptr == nullptr;
	});
}

// Add summoned minion
void UCharacterBattleInfo::AddSummonedMinion(AActor* SummonedMinion)
{
	SummonedMinionList.Insert(SummonedMinion, 0);
}

// Remove summoned minion
void UCharacterBattleInfo::RemoveSummonedMinion(AActor* SummonedMinion)
{
	const bool Minion = SummonedMinionList.Contains(SummonedMinion);
	if (Minion)
	{
		SummonedMinionList.Remove(SummonedMinion);
	}
}

// Remove some summoned minion as much as count.
void UCharacterBattleInfo::RemoveSummonedMinionAsMuchAsCount(int32 count)
{
	int32 TargetCount = 0;
	for (auto Element : SummonedMinionList)
	{
		if (TargetCount >= count)
			break;

		if (Element.IsValid() )
		{
			Element->Destroy();
		}
		TargetCount++;
		Element = nullptr;
	}

	SummonedMinionList.RemoveAll([](const TWeakObjectPtr< AActor > Ptr){
		return Ptr == nullptr;
	});
}

void UCharacterBattleInfo::RemoveAllSummonedMinion()
{
	for (auto Element : SummonedMinionList)
	{
		if (Element.IsValid() )
		{
			Element->Destroy();
		}
	}

	SummonedMinionList.Empty();
}

void UCharacterBattleInfo::SetHPZeroAllSummonedMinion()
{
	if (SummonedMinionList.Num() <= 0)
		return;

	for (int32 i = 0 ; i < SummonedMinionList.Num(); i++)
	{
		if(SummonedMinionList.IsValidIndex(i) == false)
			continue;

		auto Summoned_EntityBattleComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SummonedMinionList[i].Get());
		if (Summoned_EntityBattleComponent && Summoned_EntityBattleComponent->GetBattleInfo())
		{
			Summoned_EntityBattleComponent->GetBattleInfo()->SetHP(0);
			Summoned_EntityBattleComponent->Dead();
		}
	}

	SummonedMinionList.Empty();
}

void UCharacterBattleInfo::SetBeforeModificationInfo(AActor* InActor, FName InCharacterID, int32 InArmor, int32 InMagicResistance)
{
	OwnerActor = InActor;
	BeforeModificationCharacterID = InCharacterID;
	BeforeModificationArmor = InArmor;
	BeforeModificationMagicResistance = InMagicResistance;
}

void UCharacterBattleInfo::GetBeforeModificationInfo(FName& InCharacterID, int32& InArmor, int32& InMagicResistance)
{
	InCharacterID = BeforeModificationCharacterID;
	InArmor = BeforeModificationArmor;
	InMagicResistance = BeforeModificationMagicResistance;
}

void UCharacterBattleInfo::ModifyTimerActive()
{
	if (IsValid(OwnerActor))
	{
		OwnerActor->GetWorldTimerManager().SetTimer(TimerHandle_ModifyEnd, this, &UCharacterBattleInfo::ModifyOriginalActor, 3.0f);
	}
}

void UCharacterBattleInfo::ModifyOriginalActor()
{
	UObjectFunctionLibrary::ModifyActor(OwnerActor, false);
}
