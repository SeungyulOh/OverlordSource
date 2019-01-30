// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "SharedConstants/GlobalConstants.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/BossComponents/GigantComponent.h"
#include "Character/Components/BossComponents/AkhanComponent.h"

#include "Character/Components/SkillAreaIndicatorMeshComponent.h"




#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "GameInfo/RInventory.h"

#include "GameInfo/PlayStateInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"


#include "GlobalIntegrated.h"
//#include "Global/BattleFieldManager.h"

#include "Table/SkillTableInfo_V2.h"
#include "Table/BulletTableInfo.h"
#include "Table/BattleLogicConfigTable.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedBattleObjectTableInfo.h"


#include "UtilFunctionIntegrated.h"
#include "Kismet/KismetMathLibrary.h"

#include "Skill_V2/SharedSkill/Skill_Passive_V2.h"

#include "Network/RTSManager.h"



const float MIN_HIT_RATE = 0.05f;
const float MAX_HIT_RATE = 0.95f;
const float MIN_CRIT_RATE = 0.05f;
const float MAX_CRIT_RATE = 0.41f;
const float CRITICAL_BACK_ATTACK_CONSTANT = 0.5f;
const float PIERCE_BACK_ATTACK_CONSTANT = 0.5f;

float CommonFunctionConstant = 1.03f;
float CriticalConstant = 0.9f;
float CriticalDamageConstant = 0.0f;
float PierceConstant = 0.9f;
float ParryConstant = 0.8f;
float DefenceRateConstant = 0.9f;
float HitConstant = 0.2f;
float BackHitRateBonus = 0.0f;
float BackCriticalRateBonus = 0.0f;
float MagicCriticalDamageConstant = 1.0f;
float CriticalDamageValue = 1000.0f;
//#define DRAW_DEBUG



void URenewal_BattleFunctionLibrary::InitBattleLogicConstants(FBattleLogicConstants& BattleLogicConstants)
{	
	UTableManager* pTableManager = UTableManager::GetInstancePtr();
	if (pTableManager == nullptr)
		return;

	BattleLogicConstants.PhysicalAttackPower = pTableManager->GetBattleLogicConfigValue("PhysicalAttackBP");
	BattleLogicConstants.MagicAttackPower = pTableManager->GetBattleLogicConfigValue("MagicAttackBP");
	BattleLogicConstants.Armor = pTableManager->GetBattleLogicConfigValue("ArmorBP");
	BattleLogicConstants.MagicResistance = pTableManager->GetBattleLogicConfigValue("MagicResistanceBP");
	BattleLogicConstants.MaxHP = pTableManager->GetBattleLogicConfigValue("MaxHPBP");

	CommonFunctionConstant = BattleLogicConstants.CommonFunctionConstant = pTableManager->GetBattleLogicConfigValue("CommonFunctionConstant");
	CriticalConstant = BattleLogicConstants.CriticalConstant = pTableManager->GetBattleLogicConfigValue("CriticalConstant");
	CriticalDamageConstant = BattleLogicConstants.CriticalDamageConstant = pTableManager->GetBattleLogicConfigValue("CriticalDamageConstant");
	PierceConstant = BattleLogicConstants.PierceConstant = pTableManager->GetBattleLogicConfigValue("PierceConstant");
	ParryConstant = BattleLogicConstants.ParryConstant = pTableManager->GetBattleLogicConfigValue("ParryConstant");
	DefenceRateConstant = BattleLogicConstants.DefenceRateConstant = pTableManager->GetBattleLogicConfigValue("DefenceRateConstant");
	HitConstant = BattleLogicConstants.HitConstant = pTableManager->GetBattleLogicConfigValue("HitConstant");
	BackHitRateBonus = BattleLogicConstants.BackHitRateBonus = pTableManager->GetBattleLogicConfigValue("BackHitRateBonus");
	BackCriticalRateBonus = BattleLogicConstants.BackCriticalRateBonus = pTableManager->GetBattleLogicConfigValue("BackCriticalRateBonus");
	MagicCriticalDamageConstant = BattleLogicConstants.MagicCriticalDamageC = pTableManager->GetBattleLogicConfigValue("MagicCriticalDamageConstant");

	CriticalDamageValue = BattleLogicConstants.CriticalDamageValue = pTableManager->GetBattleLogicConfigValue("CriticalDamageValue");
}


/************************************************************************/
/* Renewal Damage Function												*/
/************************************************************************/
int32 URenewal_BattleFunctionLibrary::GetFinalDamageValue(AActor* Offender, AActor* Deffender, EBattleDamageResultEnum& DamageResultEnum, bool bPhysicalAttack, bool bSkillAttack)
{
	if (! IsValid(Offender) || !IsValid(Deffender))
	{
		return 0;
	}

	bool bHit = false;
	int32 FinalDamage = 0;

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return 0;
	}
	
	DamageResultEnum = EBattleDamageResultEnum::VE_Hit;

	//Skill attack must hit target
	if (bSkillAttack)
	{
		bHit = true;
	}
	else
	{
		bHit = DistinguishBetweenHitAndAvoid(Offender, Deffender);
	}

	if (!bHit)
	{
		DamageResultEnum = EBattleDamageResultEnum::VE_Dodge;
		return FinalDamage;
	}

	FinalDamage = GetFinalDamageByAttackPower(Offender, Deffender, DamageResultEnum, bPhysicalAttack);

	// +-10% Random Damage
	/*float RandomDamageRate = (float)(FMath::RandRange(-100, 100)) * 0.001f;
	float RandomDamage = (float)FinalDamage * RandomDamageRate;
	FinalDamage = FinalDamage + (int32)RandomDamage;*/

	return FinalDamage;
}

int32 URenewal_BattleFunctionLibrary::GetFinalDamageByAttackPower(AActor* Offender, AActor* Deffender, EBattleDamageResultEnum& DamageResultEnum, bool bPhysicalAttack)
{
	if (!IsValid(Offender) || !IsValid(Deffender))
	{
		return 0;
	}

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return 0;
	}

	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
	if (!IsValid(Deffender_BattleEntityComponent))
	{
		return 0;
	}

	float	ValueScale = static_cast<float>(ScaleNumber);
	bool	bCriticalHit = false;

	//Physical Attacker is able to Critical Attack
	bCriticalHit = IsCriticalHit(Offender, Deffender, bPhysicalAttack);
	if (bCriticalHit)
	{
		Offender_BattleEntityComponent->GetBattleInfoBlackboard()->AddDamageCalcState(EBattleDamageResultEnum::VE_Critical);
	}

	float OffenderPierceRate = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PierceValue);
	OffenderPierceRate /= 1000.0f;

	//Range Attacker is able to PierceAttack
// 	Offender_BattleEntityComponent->GetBattleInfoBlackboard()->AddDamageCalcState(EBattleDamageResultEnum::VE_Pierce);

	float DefenceRate = GetDefenceRate(Offender, Deffender, bPhysicalAttack);
	float CriticalDamageBonus = 0.0f;
	ECharacterPropertyTypeEnum OffenderAttackType = Offender_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackPropertyType;
	if (bCriticalHit)
	{
// 		if (bPhysicalAttack)
		{
			float OffenderCriticalDamageRate = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale);
			CriticalDamageBonus = CriticalDamageConstant + (OffenderCriticalDamageRate / CriticalDamageValue);
			DamageResultEnum = EBattleDamageResultEnum::VE_Critical;
		}
	}
	
	float OffenderAttackPower = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(OffenderAttackType);
	float FinalDefenceRate = FMath::Clamp<float>(((1 - DefenceRate) - OffenderPierceRate), 0.0f, 1.0f);
	float BaseDamage = OffenderAttackPower * (1 - FinalDefenceRate);

	// TO DO..
	float AbnormalBonusDamage = 1;

	float FinalDamage = (AbnormalBonusDamage) * (1 + CriticalDamageBonus) * BaseDamage;
	int32 FinalDamage_Int = static_cast<int32>(FinalDamage);

	return FinalDamage_Int;
}

bool URenewal_BattleFunctionLibrary::DistinguishBetweenHitAndAvoid(AActor* Offender, AActor* Deffender)
{
	if (!IsValid(Offender) || !IsValid(Deffender))
	{
		return false;
	}

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return false;
	}

	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
	if (!IsValid(Deffender_BattleEntityComponent))
	{
		return false;
	}

	bool bHit = false;
	float OffenderAccuracyValue = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_AccuracyValue);
	float DefenderAvoiadValue = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_DodgeValue);
	float HitRate = (OffenderAccuracyValue - (HitConstant * DefenderAvoiadValue)) / OffenderAccuracyValue;

	HitRate = FMath::Clamp<float>(HitRate, MIN_HIT_RATE, MAX_HIT_RATE);

	bHit = GetRateDecision(HitRate);

	return bHit;
}

bool URenewal_BattleFunctionLibrary::IsCriticalHit(AActor* Offender, AActor* Deffender, bool bPhysicalAttack)
{
	if (!IsValid(Offender) || !IsValid(Deffender))
	{
		return false;
	}

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return false;
	}

	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
	if (!IsValid(Deffender_BattleEntityComponent))
	{
		return false;
	}

	float CriticalRate = 0.0f;

	float OffenderCriticalvalue = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalValue);
	float DefenderCritialResistValue = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_CriticalResistValue);
	if (0.0f < OffenderCriticalvalue)
	{
		CriticalRate = (OffenderCriticalvalue - CriticalConstant * DefenderCritialResistValue) / OffenderCriticalvalue;
	}

	CriticalRate = FMath::Clamp<float>(CriticalRate, MIN_CRIT_RATE, MAX_CRIT_RATE);
	
	bool bIsCritical = GetRateDecision(CriticalRate);

	return bIsCritical;
}

// bool URenewal_BattleFunctionLibrary::IsPierceHit(AActor* Offender, AActor* Deffender)
// {
// 	if (!IsValid(Offender) || !IsValid(Deffender))
// 	{
// 		return false;
// 	}
// 
// 	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
// 	if (!IsValid(Offender_BattleEntityComponent))
// 	{
// 		return false;
// 	}
// 
// 	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
// 	if (!IsValid(Deffender_BattleEntityComponent))
// 	{
// 		return false;
// 	}
// 
// 	float OffenderPierceRate = Offender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PierceValue);
// 	float DeffencerPierceDeffenceRate = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PierceValue);
// // 	float PierceRate = ((OffenderPierceRate / PierceParryValue) - PierceConstant) / PierceConstant;
// 	
// 	float DefenceRate = GetDefenceRate(Offender, Deffender, bPhysicalAttack);
// 	float PierceRate = DefenceRate - OffenderPierceRate;
// 
// 	PierceRate = FMath::Clamp<float>(PierceRate, 0.0f, 1.0f);
// 	bool bPierceHit = GetRateDecision(PierceRate);
// 
// 	return bPierceHit;
// }

bool URenewal_BattleFunctionLibrary::IsParryHit(AActor* Offender, AActor* Deffender)
{
	return false;
	/*
	if (!IsValid(Offender) || !IsValid(Deffender))
	{
		return false;
	}

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return false;
	}

	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
	if (!IsValid(Deffender_BattleEntityComponent))
	{
		return false;
	}

	bool bParryHit = false;
	if (IsRangeAttack(Deffender) == true)
	{
		bParryHit = false;
	}
	else
	{
// 		if (IsBackAttack(Offender, Deffender))
// 		{
// 			bParryHit = false;
// 		}
// 		else
		{
			float DeffenderParryRate = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_ParryValue);
			float ParryRate = ((DeffenderParryRate / PierceParryValue) - ParryConstant) / ParryConstant;

			ParryRate = FMath::Clamp(ParryRate, 0.0f, 1.0f);
			bParryHit = GetRateDecision(ParryRate);
		}
	}

	return bParryHit;
	*/
}

float URenewal_BattleFunctionLibrary::GetDefenceRate(AActor* Offender, AActor* Deffender, bool bPhysicalAttack)
{
	if (!IsValid(Offender) || !IsValid(Deffender))
	{
		return 0.0f;
	}

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (!IsValid(Offender_BattleEntityComponent))
	{
		return 0.0f;
	}

	auto Deffender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Deffender);
	if (!IsValid(Deffender_BattleEntityComponent))
	{
		return 0.0f;
	}

	float DeffenderDefence = 0.000001f;
	if (bPhysicalAttack)
	{
		DeffenderDefence = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_Armor);
	}
	else
	{
		DeffenderDefence = Deffender_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicResistance);
	}

	float DefenceRate = (DeffenderDefence + DefenceRateConstant) / ((DeffenderDefence * 10) + DefenceRateConstant);

	return DefenceRate;
}

bool URenewal_BattleFunctionLibrary::GetRateDecision(const float TargetRate)
{
// 	auto RandomValue = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
// 
// 	if (UKismetMathLibrary::LessEqual_FloatFloat(RandomValue, TargetRate))
// 	{
// 		return true;
// 	}

	auto RandomValue = FMath::FRandRange(0.0f, 1.0f);		
	if ( RandomValue <= TargetRate )
	{
		return true;
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsPhysicalAttack(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	ECharacterPropertyTypeEnum AttackPropertyType = ECharacterPropertyTypeEnum::VE_None;
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (Target_BattleEntityComponent)
		AttackPropertyType = Target_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackPropertyType;

	if (AttackPropertyType == ECharacterPropertyTypeEnum::VE_PhysicalAttackPower)
	{
		return true;
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsRangeAttack(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	EAttackTypeEnum AttackType = EAttackTypeEnum::VE_None;
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if( Target_BattleEntityComponent )
		AttackType = Target_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;

	if (AttackType == EAttackTypeEnum::VE_Range)
	{
		return true;
	}

	return false;
}

// bool URenewal_BattleFunctionLibrary::IsBackAttack(AActor* Offender, AActor* Defender)
// {
// 	auto Offender_Location = Offender->GetActorLocation();
// 	auto Defender_Location = Defender->GetActorLocation();
// 
// 	FVector Offender_To_Defender_Vector = Defender_Location - Offender_Location;
// 	FVector Defender_Forward_Vector = Defender->GetActorForwardVector();
// 
// 	auto ResultDir = UUtilFunctionLibrary::GetBetweenDirFrontBack(Offender_To_Defender_Vector, Defender_Forward_Vector);
// 
// 	if (ResultDir == ECharacterDirection4REnum::VE_Front)
// 	{
// 		return true;
// 	}
// 
// 	return false;
// }

ECharacterDirectionEnum URenewal_BattleFunctionLibrary::GetTargetDirection(AActor* Offender, AActor* Defender)
{
	const FVector& OffenderActorLocation = Offender->GetActorLocation();
	const FVector& DefenderActorLocation = Defender->GetActorLocation();

	FVector DirectionVector = DefenderActorLocation - OffenderActorLocation;
	DirectionVector = DirectionVector.GetSafeNormal();

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (IsValid(Offender_BattleEntityComponent))
	{
		const FVector& TargetFowardVector = Offender_BattleEntityComponent->GetForwardVector();

		ECharacterDirection4REnum dir = UUtilFunctionLibrary::GetBetweenDirFrontBack(TargetFowardVector, DirectionVector);
		switch (dir)
		{
		case ECharacterDirection4REnum::VE_Front:
			return ECharacterDirectionEnum::VE_Front;
		case ECharacterDirection4REnum::VE_Back:
			return ECharacterDirectionEnum::VE_Back;
		}
	}
	else
	{
		const FVector& TargetFowardVector = Offender->GetActorForwardVector();

		ECharacterDirection4REnum dir = UUtilFunctionLibrary::GetBetweenDirFrontBack(TargetFowardVector, DirectionVector);
		switch (dir)
		{
		case ECharacterDirection4REnum::VE_Front:
			return ECharacterDirectionEnum::VE_Front;
		case ECharacterDirection4REnum::VE_Back:
			return ECharacterDirectionEnum::VE_Back;
		}
	}
	return ECharacterDirectionEnum::VE_None;
}

float URenewal_BattleFunctionLibrary::GetTargetDegree(AActor* Offender, AActor* Defender)
{
	const FVector& OffenderActorLocation = Offender->GetActorLocation();
	const FVector& DefenderActorLocation = Defender->GetActorLocation();

	FVector DirectionVector = DefenderActorLocation - OffenderActorLocation;
	DirectionVector = DirectionVector.GetSafeNormal();

	auto Offender_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Offender);
	if (IsValid(Offender_BattleEntityComponent))
	{
		const FVector& TargetFowardVector = Offender_BattleEntityComponent->GetForwardVector();
		float Value = FVector::DotProduct(TargetFowardVector, DirectionVector);
		float Degree = FMath::Acos(Value) * (180.f / PI);

		return Degree;
	}
	else
	{
		const FVector& TargetFowardVector = Offender->GetActorForwardVector();
		float Value = FVector::DotProduct(TargetFowardVector, DirectionVector);
		float Degree = FMath::Acos(Value) * (180.f / PI);

		return Degree;
	}
	return -1;
}

float URenewal_BattleFunctionLibrary::GetTargetDegree(FVector InSource, FVector InDest, FVector InFoward)
{
	const FVector& OffenderActorLocation = InSource;
	const FVector& DefenderActorLocation = InDest;

	FVector DirectionVector = DefenderActorLocation - OffenderActorLocation;
	DirectionVector = DirectionVector.GetSafeNormal();

	//InFoward = FVector(1, 0, 0);
	float Value = FVector::DotProduct(InFoward, DirectionVector);
	float Degree = FMath::Acos(Value) * (180.f / PI);
	float fix = 0;
	if (DirectionVector.X < 0)
	{
		fix = (180 - Degree) + 180;;
		//Degree = (180 - Degree) + 180;
	}
	if (fix != 0)
		Degree = fix;

	return Degree;
}

//=================================================================================================================================
// Friend and foe
//=================================================================================================================================

bool URenewal_BattleFunctionLibrary::IsEnemy(int32 playerUK, int32 playerGroupKey, int32 otherUK, int32 otherGroupKey)
{
	return !IsAlly(playerUK, playerGroupKey, otherUK, otherGroupKey);
}

bool URenewal_BattleFunctionLibrary::IsAlly(int32 playerUK, int32 playerGroupKey, int32 otherUK, int32 otherGroupKey)
{
	bool bIsAlly = false;

	//if other character type is SUMMON
	/*if (playerUK / PLAYER_UK_UNIT == otherUK / PLAYER_UK_UNIT)
		return true;*/

	//if self character type is NPC
// 	if (playerGroupKey == Proud::BattleFeildGroupType::BF_GroupNPC && otherGroupKey == Proud::BattleFeildGroupType::BF_GroupNPC)
// 		return true;

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		auto Actor = pGameMode->ObjectHelper.GetObjectByPlayerUK(otherUK);
		if (!Actor.IsValid())
			return false;
		auto Actor_BaseProperty = Cast<IEntityBaseProperty>(Actor);
		if (Actor_BaseProperty && Actor_BaseProperty->GetGroupKey() == playerGroupKey)
			bIsAlly = true;
		if (Actor_BaseProperty && Actor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_NeutralNpc)
			bIsAlly = true;
	}

	return bIsAlly;
}

bool URenewal_BattleFunctionLibrary::IsAllyActor(AActor* firstActor, AActor* secondActor)
{
	if (!IsValid(firstActor) || !IsValid(secondActor))
		return false;

	auto firstActor_BaseProperty = Cast<IEntityBaseProperty>(firstActor);
	if (firstActor_BaseProperty == nullptr)
		return false;

	auto secondActor_BaseProperty = Cast<IEntityBaseProperty>(secondActor);
	if (secondActor_BaseProperty == nullptr)
		return false;

	return (firstActor_BaseProperty->GetGroupKey() == secondActor_BaseProperty->GetGroupKey());
}

bool URenewal_BattleFunctionLibrary::IsPartyActor(AActor* firstActor, AActor* secondActor)
{
	if (!IsValid(firstActor) || !IsValid(secondActor))
		return false;

	auto firstActor_BaseProperty = Cast<IEntityBaseProperty>(firstActor);
	if (firstActor_BaseProperty == nullptr)
		return false;

	auto secondActor_BaseProperty = Cast<IEntityBaseProperty>(secondActor);
	if (secondActor_BaseProperty == nullptr)
		return false;

	return (firstActor_BaseProperty->GetPartyID() == secondActor_BaseProperty->GetPartyID());
}

bool URenewal_BattleFunctionLibrary::IsEnemyActor(AActor* firstActor, AActor* secondActor)
{
	if (!IsValid(firstActor) || !IsValid(secondActor))
		return false;

	auto firstActor_BaseProperty = Cast<IEntityBaseProperty>(firstActor);
	if (firstActor_BaseProperty == nullptr)
		return false;

	auto secondActor_BaseProperty = Cast<IEntityBaseProperty>(secondActor);
	if (secondActor_BaseProperty == nullptr)
		return false;

	return IsEnemy(firstActor_BaseProperty->GetPlayerUK(), firstActor_BaseProperty->GetGroupKey(), secondActor_BaseProperty->GetPlayerUK(), secondActor_BaseProperty->GetGroupKey());
}

bool URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(AActor* TargetActor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;

// 	if(URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 	{
// 		if (IsValid(RGameInstance->RealTimeModManager))
// 		{
// 			int32 myActorUK = RGameInstance->RealTimeModManager->GetMyPlayActorUK();
// 			TWeakObjectPtr<AActor> CurrentPlayer = nullptr;// RGameInstance->GameObjectMgr->GetObjectByPlayerUK(myActorUK);
// // 			UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// // 			if (pGamePlayMode)
// // 			{
// // 				CurrentPlayer = pGamePlayMode->GetObjectByPlayerUK(myActorUK);
// // 			}
// 			AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
// 			if (pGameMode)
// 			{
// 				CurrentPlayer = pGameMode->ObjectHelper.GetObjectByPlayerUK(myActorUK);
// 			}
// 			if (CurrentPlayer.IsValid()) {
// 				return IsEnemyActor(TargetActor, CurrentPlayer.Get());
// 			}
// 		}
// // 		if (IsValid(RGameInstance->BattleFieldManager))
// // 		{
// // 			AActor* CurrentPlayer = RGameInstance->BattleFieldManager->GetMyBattleFieldHero();
// // 			if (CurrentPlayer)
// // 			{
// // 				return IsEnemyActor(TargetActor, CurrentPlayer);
// // 			}
// // 		}
// 	}
	AActor* LeaderActor = nullptr;// RGameInstance->PlayStateInfo->GetOwnerCharacter(ROLE_LEADER_INDEX);
	TArray<FHeroSlotInfo>& rSelectCharacterArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (int32 i = 0; i < rSelectCharacterArray.Num(); ++i)
	{
		if(rSelectCharacterArray.IsValidIndex(i) && rSelectCharacterArray[i].CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero)
		{
			LeaderActor = rSelectCharacterArray[i].SpawnedTargetActor;
		}

		if (LeaderActor)
			break;
	}

	auto LeaderActor_BaseProperty = Cast<IEntityBaseProperty>(LeaderActor);
	auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(TargetActor);
	if (TargetActor_BaseProperty /*&& IsValid(RGameInstance->GameObjectMgr)*/)
	{
		if (false == TargetActor_BaseProperty->GetAccountCharacter())
		{
			if(IsValid(LeaderActor))
				return IsEnemy(TargetActor_BaseProperty->GetPlayerUK(), TargetActor_BaseProperty->GetGroupKey(),LeaderActor_BaseProperty->GetPlayerUK(), UObjectFunctionLibrary::GetMyTeamKey());
		}
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsControlCharacter(AActor* TargetActor)
{
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (IsValid(Target_BattleEntityComponent))
	{
		if (Target_BattleEntityComponent->GetCommandSlotIndex() != INVALID_INDEXNUM)
		{
			return true;
		}
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsBossActor(AActor* TargetActor)
{
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (IsValid(Target_BattleEntityComponent))
	{
		if (Target_BattleEntityComponent->GetBossComponent())
		{
			return true;
		}
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsProtectTarget(AActor* InTargetActor)
{
	if (InTargetActor == nullptr)
		return false;

	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;
	
// 	auto GamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (GamePlayMode == nullptr)
// 		return false;
	
	//auto StageBase = GamePlayMode->GetStageSpawner();
	//if (StageBase == nullptr)
	//	return false;

	//return (StageBase->IsProtectTargetActor(InTargetActor));
	return false;
}

bool URenewal_BattleFunctionLibrary::IsRaidAkhan(AActor* TargetActor)
{
	auto AkhanComponent = UCharacterHelpFunctionLibrary::GetAkhanComponent(TargetActor);
	if(AkhanComponent)
		return true;

	return false;
}

template<typename T>
void URenewal_BattleFunctionLibrary::ModifyNegativeNumberToZero(T& Number)
{
	if (Number < 0)
		Number = 0;
}


int32 URenewal_BattleFunctionLibrary::GetNpcSkillLevel(int32 InNpcLevel)
{
	if (6 < InNpcLevel)
		return 6;
	else
		return InNpcLevel;

// #ifdef RULE_TESTDELETE
// 	int32 SkillLevel = InNpcLevel * 0.1f + 1;
// #else
// 	int32 SkillLevel = InNpcLevel / 10 + 1;
// #endif
// 
// 	TArray<int32> levelList;
// 	levelList.Reserve(9);
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn2")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn3")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn4")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn5")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn6")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn7")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn8")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn9")));
// 	levelList.Emplace((int32)UTableManager::GetInstancePtr()->GetBattleLogicConfigValue(TEXT("NPCSkillLearn10")));
// 
// 	SkillLevel = 10;
// 
// 	for (int32 i = 0; i < levelList.Num(); ++i)
// 	{
// 		if (InNpcLevel < levelList[i])
// 		{
// 			SkillLevel = i + 1;
// 			break;
// 		}
// 	}
// 
// 	// 	for (int i = 1; i <= MAX_Skill_LEVEL; i++)
// 	// 	{
// 	// 		FSimpleOneNumberLookup* LookUpTable = UTableManager::GetInstancePtr()->GetNpcSkillLevelLookupRow(FName((*FText::AsNumber(i).ToString())));
// 	// 		if (LookUpTable)
// 	// 		{
// 	// 			SkillLevel = i;
// 	// 			if (LookUpTable->Value > InNpcLevel)
// 	// 				break;
// 	// 		}
// 	// 	}
// 
// 	return SkillLevel;
}


void URenewal_BattleFunctionLibrary::SetBattleObjectInfoFromTable(UCharacterBattleInfo* BattleInfo, FBattleObjectInfo& BattleObjectInfo, FName RowName, int32 level)
{
	FBattleObjectTableInfo* TableInfo = GetBattleObjectTableInfo(RowName);
	if (!TableInfo)
		return;
	FWorkedBattleObjectTableInfo WorkedBattleObjectInfo;
	WorkedBattleObjectInfo.SetDataFromBattleObjectInfo(TableInfo, level);

	BattleInfo->Level = 1;
//	BattleInfo->EXP = 0;
	BattleInfo->HP = WorkedBattleObjectInfo.Data.MaxHP;

	FCharacterBattleData* BattleDataSrc = (FCharacterBattleData*)(&(WorkedBattleObjectInfo.Data));
	FCharacterBattleData* BattleDataTarget = (FCharacterBattleData*)(&(BattleInfo->CharacterProperty));

	*BattleDataTarget = *BattleDataSrc;

	if (WorkedBattleObjectInfo.Data.BulletID.IsNone() == false)
	{
		FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(WorkedBattleObjectInfo.Data.BulletID);
		if (BulletTableInfo)
		{
			BattleInfo->BulletType = BulletTableInfo->GetBulletBlueprintClass();
			BattleInfo->CharacterProperty.BulletSpeed = BulletTableInfo->BulletSpeed;
		}
	}

	BattleInfo->PassiveSkill01 = WorkedBattleObjectInfo.Data.SkillSlot1;
	BattleInfo->PassiveSkill02 = WorkedBattleObjectInfo.Data.SkillSlot2;

	BattleObjectInfo.BattleObjectType = WorkedBattleObjectInfo.Data.ObjectType;
	BattleObjectInfo.Effect_1 = WorkedBattleObjectInfo.Data.Effect_1;
	BattleObjectInfo.Effect_2 = WorkedBattleObjectInfo.Data.Effect_2;
	BattleObjectInfo.DestroyEffect = WorkedBattleObjectInfo.Data.DestroyEffect;
	BattleObjectInfo.ShapeTableRow = WorkedBattleObjectInfo.Data.ShapeTableRow;
	BattleObjectInfo.FireDelay = WorkedBattleObjectInfo.Data.FireDelay;
	BattleObjectInfo.ShowHP = WorkedBattleObjectInfo.Data.ShowHP;
}

bool URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(TArray<int32>& OutItemStatOptions, const FITEM& ItemData)
{
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData.itemId);
	if (ItemInfo == nullptr) {
		UE_LOG(LogUI, Log, TEXT("URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable returned ItemData with unknown idtemId"));
		return false;
	}

// 	UE_LOG(LogGamePlay, Log, TEXT("URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable: printage"));
// 	FString HeaderLine = TEXT("Stat, ");
// 	for (int32 i = 0; i < (int32)EItemStatOption::Max; i++)
// 	{
// 		HeaderLine += UUtilFunctionLibrary::GetEnumToName(TEXT("EItemStatOption"), i).ToString() + TEXT(", ");
// 	}
	//UE_LOG(LogGamePlay, Log, TEXT("%s"), *HeaderLine);

	// basic options
	ItemInfo->GetBasicOptions(OutItemStatOptions);
	//DebugPrintItemStatOptions(TEXT("BasicOptions"), OutItemStatOptions);

	// level up
	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, &ItemData, OutItemStatOptions);
	//DebugPrintItemStatOptions(TEXT("AfterItemLevel"), OutItemStatOptions);

	// enchant
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, &ItemData, OutItemStatOptions);
	//DebugPrintItemStatOptions(TEXT("AfterEnchant"), OutItemStatOptions);

	// random options
	FItemRandomOptionTableInfo::ApplyItemRandomOptions(ItemInfo, &ItemData, OutItemStatOptions);
	//DebugPrintItemStatOptions(TEXT("AfterRandomOptions"), OutItemStatOptions);

	// rune
	FRuneTable::ApplyRunes(&ItemData, OutItemStatOptions);
	//DebugPrintItemStatOptions(TEXT("AfterRuneOptions"), OutItemStatOptions);

 	// Attach passive skill
 	//USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InOwnerActor);
 	//if (SkillEntityComponent)
 	//{
 	//	if (!WorkedTableInfo.Data.PassiveSkillOnEquip.IsNone())
 	//	{
 	//		SkillEntityComponent->AttachSkill_V2(WorkedTableInfo.Data.PassiveSkillOnEquip, 1, ESkillCategoryTypeEnum::VE_ItemSkill);
 	//	}
 	//}
	return true;
}

void URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfo(FCharacterBattleProperty& CharacterBattleProperty, FWorkedItemTableInfo& WorkedTableInfo)
{
	FCharacterBattleData* BattleDataSrc = (FCharacterBattleData*)(&(WorkedTableInfo.Data));
	FCharacterBattleData* BattleDataTarget = (FCharacterBattleData*)(&(CharacterBattleProperty));

	*BattleDataTarget = *BattleDataSrc;
}

// void URenewal_BattleFunctionLibrary::DebugPrintItemStatOptions(FString LabelText, const TArray<int32>& ItemStatOptions)
// {
//
// 	FString DataLine = LabelText + TEXT(", ");
// 	for (int32 i = 0; i < (int32)EItemStatOption::Max; i++)
// 	{
// 		DataLine += FString::FromInt(ItemStatOptions[(int32)i]) + TEXT(", ");
// 	}
//	UE_LOG(LogGamePlay, Log, TEXT("%s"), *DataLine);
//}

void URenewal_BattleFunctionLibrary::SetCharacterBattleInfoFromTable(UCharacterBattleInfo* BattleInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level)
{
	FWorkedCharacterTableInfo WorkedTableInfo;
	bool FindRow = GetCharacterTableWorkedInfo(WorkedTableInfo, CharacterCategory, RowName, Level);
	if (!FindRow)
		return;

	SetCharacterBattleInfo(BattleInfo, WorkedTableInfo);

	if (IsValid(BattleInfo))
	{
		if (WorkedTableInfo.Data.BulletID.IsNone() == false)
		{
			FBulletTableInfo* BulletInfo = UTableManager::GetInstancePtr()->GetBulletRow(WorkedTableInfo.Data.BulletID);
			if (BulletInfo != nullptr)
			{
				BattleInfo->BulletType = BulletInfo->GetBulletBlueprintClass();
			}
		}

		BattleInfo->RWeapon = WorkedTableInfo.Data.GetDefaultWeaponBlueprintClass(true);
		BattleInfo->LWeapon = WorkedTableInfo.Data.GetDefaultWeaponBlueprintClass(false);
	}
}

void URenewal_BattleFunctionLibrary::SetCharacterBattleInfo(UCharacterBattleInfo* BattleInfo, const FWorkedCharacterTableInfo& WorkedTableInfo)
{
	BattleInfo->HP = WorkedTableInfo.Data.MaxHP;

	FCharacterBattleData* BattleDataSrc = (FCharacterBattleData*)(&(WorkedTableInfo.Data));
	FCharacterBattleData* BattleDataTarget = (FCharacterBattleData*)(&(BattleInfo->CharacterProperty));

	*BattleDataTarget = *BattleDataSrc;


	BattleInfo->CharacterProperty.AttackHitperSecond = 1.f / WorkedTableInfo.Data.NormalAttackTime;


	if (WorkedTableInfo.Data.BulletID.IsNone() == false)
	{
		FBulletTableInfo* BulletInfo = UTableManager::GetInstancePtr()->GetBulletRow(WorkedTableInfo.Data.BulletID);
		if (BulletInfo != nullptr)
		{
			//BattleInfo->BulletType = BulletInfo->GetBulletBlueprintClass();
			BattleInfo->BulletType = nullptr;
			BattleInfo->CharacterProperty.BulletSpeed = BulletInfo->BulletSpeed;
		}
	}

	BattleInfo->RWeapon = nullptr;
	BattleInfo->LWeapon = nullptr;

	//BattleInfo->RWeapon = WorkedTableInfo.Data.GetDefaultWeaponBlueprintClass(true);
	//BattleInfo->LWeapon = WorkedTableInfo.Data.GetDefaultWeaponBlueprintClass(false);

	// skill

	BattleInfo->ActiveSkill01 = WorkedTableInfo.Data.ActiveSkill01;
	BattleInfo->ActiveSkill02 = WorkedTableInfo.Data.ActiveSkill02;
	BattleInfo->ActiveSkill03 = WorkedTableInfo.Data.ActiveSkill03;
	BattleInfo->ActiveSkill04 = WorkedTableInfo.Data.ActiveSkill04;

	BattleInfo->PassiveSkill01 = WorkedTableInfo.Data.PassiveSkill01;
	BattleInfo->PassiveSkill02 = WorkedTableInfo.Data.PassiveSkill02;

	BattleInfo->SpiritSkill01 = WorkedTableInfo.Data.SpiritSkill01;
	BattleInfo->SpiritSkill02 = WorkedTableInfo.Data.SpiritSkill02;
	BattleInfo->SpiritSkill03 = WorkedTableInfo.Data.SpiritSkill03;
	BattleInfo->SpiritSkill04 = WorkedTableInfo.Data.SpiritSkill04;

}

bool URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(FWorkedCharacterTableInfo& OutWorkedInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level)
{
	FCharacterTableInfo* TableInfo = GetCharacterTableInfo(CharacterCategory, RowName);
	if (TableInfo == nullptr)
		return false;

	// Hero만 level비례 능력치 보정.
	if (CharacterCategory != ECharacterCategoryTypeEnum::VE_Hero)
		Level = 0;

	OutWorkedInfo.SetDataFromCharacterInfo(TableInfo, Level);

	return true;
}

FCharacterTableInfo* URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum	CharacterCategory, FName RowName)
{
	FCharacterTableInfo* Result = nullptr;
	switch (CharacterCategory)
	{
	case ECharacterCategoryTypeEnum::VE_Hero:
	case ECharacterCategoryTypeEnum::VE_UICharacter:
		Result = UTableManager::GetInstancePtr()->GetCharacterRow(RowName);
		break;
	case ECharacterCategoryTypeEnum::VE_Minion:
	case ECharacterCategoryTypeEnum::VE_Npc:
	case ECharacterCategoryTypeEnum::VE_NeutralNpc:
	case ECharacterCategoryTypeEnum::VE_BattleObject:
		Result = UTableManager::GetInstancePtr()->GetCharacterNpcRow(RowName);
		break;
	}

	return Result;
}

FSkillTableInfo_V2* URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(ECharacterCategoryTypeEnum CharacterCategory, FName RowName)
{
	if (RowName.IsNone())
		return nullptr;

	FSkillTableInfo_V2* BaseInfo = nullptr;

	switch (CharacterCategory)
	{
	case ECharacterCategoryTypeEnum::VE_Hero:
	case ECharacterCategoryTypeEnum::VE_UICharacter:
	case ECharacterCategoryTypeEnum::VE_Object:
	case ECharacterCategoryTypeEnum::VE_Minion:
	case ECharacterCategoryTypeEnum::VE_Npc:
	case ECharacterCategoryTypeEnum::VE_NeutralNpc:
	case ECharacterCategoryTypeEnum::VE_BattleObject:
		BaseInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(RowName);
		break;
	}

	// Check item skill
	if (nullptr == BaseInfo)
	{
		BaseInfo = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(RowName);
	}

	return BaseInfo;
}

// FSkillUnlockLevelTableInfo*	URenewal_BattleFunctionLibrary::GetSkillUnlocLevelTableInfo(FName RowName)
// {
// 	FSkillUnlockLevelTableInfo* SkillUnlockLevelTableInfo = UTableManager::GetInstancePtr()->GetSkillUnlockLevelRow(RowName);
// 
// 	return SkillUnlockLevelTableInfo;
// }

bool URenewal_BattleFunctionLibrary::IsMaxSkillLevel(int32 InSkillLevel)
{
	if (MAX_Skill_LEVEL <= InSkillLevel)
	{
		return true;
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(FWorkedSkillTableInfo& OutSkillTableInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level)
{
	FName SkillTableID = UUtilFunctionLibrary::GetResultSkillID(CharacterCategory, RowName, Level);

	FSkillTableInfo_V2* BaseInfo = GetSkillTableInfo_V2(CharacterCategory, SkillTableID);
	if (BaseInfo)
		OutSkillTableInfo.SkillTableInfo_V2 = BaseInfo;

	return (BaseInfo != nullptr);
}


FBattleObjectTableInfo*	URenewal_BattleFunctionLibrary::GetBattleObjectTableInfo(FName RowName)
{
	return UTableManager::GetInstancePtr()->GetBattleObjectRow(RowName);
}

FVector URenewal_BattleFunctionLibrary::GetFloorPosition(UWorld* World, FVector InLocation, float fDist, AActor* IgnoreActor)
{
	FVector Location = InLocation;
	Location.Z = Location.Z + fDist;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Trace")), true, nullptr);
	CollisionParams.bTraceComplex = false;

	if (IsValid(IgnoreActor))
	{
		CollisionParams.AddIgnoredActor(IgnoreActor);
	}

#ifdef DRAW_DEBUG
	DrawDebugLine(World, Location, Location - FVector::UpVector * 100000.0f, FColor::Blue, false, 30.0f, 0, 3);
#endif // DRAW_DEBUG

	if (World->LineTraceSingleByChannel(OutHit, Location, Location - FVector::UpVector * 100000.0f, ECC_WorldStatic, CollisionParams) == true)
	{
		return OutHit.Location;
	}

	return FVector::ZeroVector;
}


UParticleSystemComponent* URenewal_BattleFunctionLibrary::ParametersUsingParticle(UParticleSystem* TargetEffect, AActor* TargetActor)
{
	UParticleSystemComponent* SpawnEffect = UUtilFunctionLibrary::SpawnEmitterAttached(TargetEffect, TargetActor);
	if (SpawnEffect != nullptr)
	{
		SpawnEffect->SetWorldScale3D(FVector(1, 1, 1));
		SpawnEffect->SetActorParameter(PARAMETER_ACTOR, TargetActor);
	}

	return SpawnEffect;
}

float URenewal_BattleFunctionLibrary::GetCustomizedSightRange(UWorld* InWorld, UCharacterBattleInfo* CharacterBattleInfo, bool InIsConstantValue /*= false*/)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (InIsConstantValue)
		return NET_SEARCHSIZE;

//	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	
// 	{
		float sight_range = CharacterBattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_SightRange);
		if (EGamePlayModeTypeEnum::VE_Arena == BaseStageInfo->GetGameMode() ||
			EGamePlayModeTypeEnum::VE_MirrorDungeon == BaseStageInfo->GetGameMode())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Arena Sight Range : %.02f => %.02f"), sight_range, sight_range * 2.0f);
			return sight_range * 2.0f;
		}
		return sight_range;
//	}

	return 0.0f;
}



//=================================================================================================================================
// Make battle info
//=================================================================================================================================
UCharacterBattleInfo* URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(FEQUIPED_HERO& hero)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UCharacterBattleInfo*	BattleInfo = GameInstance->BattleInfo;

	BattleInfo->Init();

	FWorkedCharacterTableInfo	WorkedCharacterTableInfo;

	GetCharacterTableWorkedInfo(WorkedCharacterTableInfo, ECharacterCategoryTypeEnum::VE_Hero, hero.heroId, hero.level);

	SetCharacterBattleInfo(BattleInfo, WorkedCharacterTableInfo);

	for (int32 i = 0; i < hero.equipedItems.Num(); i++)
	{
		if (!hero.equipedItems[i].itemId.IsNone())
		{
			URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(BattleInfo->ItemStatsByEquipPosition[i], hero.equipedItems[i]);
		}
	}

	BattleInfo->ValidateFinalCharacterProperty();

	BattleInfo->SetLevel(hero.level);
	//BattleInfo->SetGrade(hero.grade);
	BattleInfo->SetSkills(hero.skills);

	return BattleInfo;
}

UCharacterBattleInfo* URenewal_BattleFunctionLibrary::MakeBObjectBattleInfoFromTable(FName key, int32 Level)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UCharacterBattleInfo*	BattleInfo = GameInstance->BattleInfo;

	BattleInfo->Init();

	FBattleObjectInfo BattleObjectInfo;
	SetBattleObjectInfoFromTable(BattleInfo, BattleObjectInfo, key, Level);

	BattleInfo->ValidateFinalCharacterProperty();
	BattleInfo->SetLevel(Level);
	BattleInfo->SetGrade(1);

	return BattleInfo;
}

UCharacterBattleInfo* URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(FString HeroKey, bool InNextUpgrade, int32 InNextLevel)
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif
	URGameInstance*			GameInstance	=	RGAMEINSTANCE(GEngine);
	UCharacterBattleInfo*	BattleInfo		=	GameInstance->BattleInfo;

	BattleInfo->Init();

	FHERO						HeroInvenData;
	FWorkedCharacterTableInfo	WorkedCharacterTableInfo;

	GameInstance->RInventory->GetHeroAndTableData(HeroKey, HeroInvenData, WorkedCharacterTableInfo, InNextUpgrade, InNextLevel);

	//Set Worked Character Battleinfo(Apply level, Grade, Enchant)
	URenewal_BattleFunctionLibrary::SetCharacterBattleInfo(BattleInfo, WorkedCharacterTableInfo);
	
	int32 Max = HeroInvenData.equipedItemUDs.Num();
	for (int32 i = 0; i < Max; i++)
	{
		if (HeroInvenData.equipedItemUDs[i].IsEmpty())
			continue;

		FITEM* EquipedItemData = GameInstance->RInventory->GetItemDataWithUD(HeroInvenData.equipedItemUDs[i]);
		if (EquipedItemData)
		{
			URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(BattleInfo->ItemStatsByEquipPosition[i], *EquipedItemData);
		}
	}
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData.heroId);
	Max = HeroInvenData.enchantStat.Num();
	for (int32 i = 0; i < Max; i++)
	{
		if (HeroInvenData.enchantStat[i] > 0)
		{
			EHERO_ENCHANT_STAT_SLOT slot = (EHERO_ENCHANT_STAT_SLOT)i;
			float plus = URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(CharacterTableInfo->Class, slot, HeroInvenData.enchantStat[i]);
			switch (slot)
			{
			case EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MagicAttackPower:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::Armor:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_Armor, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MagicResistance:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicResistance, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MaxHP:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP, plus);
				break;
			}
		}
	}

	BattleInfo->ValidateFinalCharacterProperty();

	BattleInfo->SetLevel(HeroInvenData.level + InNextLevel);
	BattleInfo->SetSkills(HeroInvenData.skills);

	TArray<int32> AbilityPoints;
	TArray<int32>& AllPoints = GameInstance->UserInfo->Avatar.ability;
	SetAvatarPointByJobClass(WorkedCharacterTableInfo.Data.Class, AbilityPoints, AllPoints);
	SetAvatarAbilityProperty(BattleInfo, AbilityPoints);

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	//UE_LOG(LogGamePlay, Log, TEXT("%f ____MakeCharacteBattleInfoFromInven()"), PrintTime);
#endif

	return BattleInfo;
}

UCharacterBattleInfo* URenewal_BattleFunctionLibrary::MakeTargetCharacteBattleInfoFromInven(FBATTLE_USER& MatchList, FString& HeroKey)
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	UCharacterBattleInfo*	BattleInfo = GameInstance->BattleInfo;

	BattleInfo->Init();

	FEQUIPED_HERO				HeroInvenData;
	FWorkedCharacterTableInfo	WorkedCharacterTableInfo;

	for (FEQUIPED_HERO& Hero : MatchList.heros)
	{
		if (Hero.heroUD == HeroKey)
		{
			HeroInvenData = Hero;
			break;
		}
	}

	bool FindRow = URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(WorkedCharacterTableInfo, ECharacterCategoryTypeEnum::VE_Hero, HeroInvenData.heroId, HeroInvenData.level);
	if (FindRow == false)
		return nullptr;

	//Set Worked Character Battleinfo(Apply level, Grade, Enchant)
	URenewal_BattleFunctionLibrary::SetCharacterBattleInfo(BattleInfo, WorkedCharacterTableInfo);

	int32 Max = HeroInvenData.equipedItems.Num();
	for (int32 i = 0; i < Max; i++)
	{
		FITEM* EquipedItemData = &HeroInvenData.equipedItems[i];
		if (EquipedItemData)
		{
			URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(BattleInfo->ItemStatsByEquipPosition[i], *EquipedItemData);
		}
	}
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData.heroId);
	Max = HeroInvenData.enchantStat.Num();
	for (int32 i = 0; i < Max; i++)
	{
		if (HeroInvenData.enchantStat[i] > 0)
		{
			EHERO_ENCHANT_STAT_SLOT slot = (EHERO_ENCHANT_STAT_SLOT)i;
			float plus = URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(CharacterTableInfo->Class, slot, HeroInvenData.enchantStat[i]);
			switch (slot)
			{
			case EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MagicAttackPower:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::Armor:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_Armor, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MagicResistance:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicResistance, plus);
				break;
			case EHERO_ENCHANT_STAT_SLOT::MaxHP:
				BattleInfo->AddCharacterAdditionalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP, plus);
				break;
			}
		}
	}

	BattleInfo->ValidateFinalCharacterProperty();

	BattleInfo->SetLevel(HeroInvenData.level);
	BattleInfo->SetSkills(HeroInvenData.skills);

	TArray<int32> AbilityPoints;
	TArray<int32>& AllPoints = GameInstance->UserInfo->Avatar.ability;
	SetAvatarPointByJobClass(WorkedCharacterTableInfo.Data.Class, AbilityPoints, AllPoints);
	SetAvatarAbilityProperty(BattleInfo, AbilityPoints);

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	//UE_LOG(LogGamePlay, Log, TEXT("%f ____MakeCharacteBattleInfoFromInven()"), PrintTime);
#endif

	return BattleInfo;
}

ECharacterPropertyTypeEnum URenewal_BattleFunctionLibrary::GetCharacterPropertyType(FString& InValue)
{
	//UUIFunctionLibrary::GetOptionName - pair
	ECharacterPropertyTypeEnum Result = ECharacterPropertyTypeEnum::VE_None;

	if		(InValue == "AccuracyValue")				Result = ECharacterPropertyTypeEnum::VE_AccuracyValue;
	else if (InValue == "CriticalValue")				Result = ECharacterPropertyTypeEnum::VE_CriticalValue;
	else if (InValue == "CriticalDamageRate_Scale")		Result = ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale;
	//else if (InValue == "None")						Result = ECharacterPropertyTypeEnum::VE_None;
	else if (InValue == "MaxHP")						Result = ECharacterPropertyTypeEnum::VE_MaxHP;
	//else if (InValue == "None")						Result = ECharacterPropertyTypeEnum::VE_None;
	else if (InValue == "DodgeValue")					Result = ECharacterPropertyTypeEnum::VE_DodgeValue;
	else if (InValue == "Armor")						Result = ECharacterPropertyTypeEnum::VE_Armor;
	//else if (InValue == "None")						Result = ECharacterPropertyTypeEnum::VE_None;
	//else if (InValue == "ParryValue")					Result = ECharacterPropertyTypeEnum::VE_ParryValue;
	else if (InValue == "MagicResistance")				Result = ECharacterPropertyTypeEnum::VE_MagicResistance;
	//else if (InValue == "None")						Result = ECharacterPropertyTypeEnum::VE_None;
	else if (InValue == "PierceValue")					Result = ECharacterPropertyTypeEnum::VE_PierceValue;
	else if (InValue == "MagicAttackPower")				Result = ECharacterPropertyTypeEnum::VE_MagicAttackPower;
	else if (InValue == "PhysicalAttackPower")			Result = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower;
	//else if (InValue == "MagicCriticalRate")			Result = ECharacterPropertyTypeEnum::VE_MagicCriticalRate;
	else if (InValue == "MoveSpeed")					Result = ECharacterPropertyTypeEnum::VE_MoveSpeed;
	//else if (InValue == "None")						Result = ECharacterPropertyTypeEnum::VE_None;
	//else if (InValue == "CriticalResistValue")			Result = ECharacterPropertyTypeEnum::VE_CriticalResistValue;
	//else if (InValue == "CriticalResistDamage")			Result = ECharacterPropertyTypeEnum::VE_CriticalResistDamage;

	return Result;
}

void URenewal_BattleFunctionLibrary::SetAvatarPointByJobClass(EClassTypeEnum JobClass, TArray<int32>& PointByJobClass, TArray<int32>& AllPoints)
{
	if (0 >= AllPoints.Num())
		return;

	PointByJobClass.Init(0, 18);
	switch (JobClass)
	{
	case EClassTypeEnum::VE_Tanker:
		if (AllPoints.IsValidIndex(0))	PointByJobClass[0] = AllPoints[0];
		if (AllPoints.IsValidIndex(1))	PointByJobClass[1] = AllPoints[1];
		if (AllPoints.IsValidIndex(2))	PointByJobClass[2] = AllPoints[2];
		break;

	case EClassTypeEnum::VE_RangeDealer:
		if (AllPoints.IsValidIndex(3))	PointByJobClass[3] = AllPoints[3];
		if (AllPoints.IsValidIndex(4))	PointByJobClass[4] = AllPoints[4];
		if (AllPoints.IsValidIndex(5))	PointByJobClass[5] = AllPoints[5];
		break;

	case EClassTypeEnum::VE_MeleeDealer:
		if (AllPoints.IsValidIndex(6))	PointByJobClass[6] = AllPoints[6];
		if (AllPoints.IsValidIndex(7))	PointByJobClass[7] = AllPoints[7];
		if (AllPoints.IsValidIndex(8))	PointByJobClass[8] = AllPoints[8];
		break;

	case EClassTypeEnum::VE_MagicDealer:
		if (AllPoints.IsValidIndex(9))	PointByJobClass[9] = AllPoints[9];
		if (AllPoints.IsValidIndex(10))	PointByJobClass[10] = AllPoints[10];
		if (AllPoints.IsValidIndex(11))	PointByJobClass[11] = AllPoints[11];
		break;

	case EClassTypeEnum::VE_Supporter:
		if (AllPoints.IsValidIndex(12))	PointByJobClass[12] = AllPoints[12];
		if (AllPoints.IsValidIndex(13))	PointByJobClass[13] = AllPoints[13];
		if (AllPoints.IsValidIndex(14))	PointByJobClass[14] = AllPoints[14];
		break;
	}

	// Common
	if (AllPoints.IsValidIndex(15))		PointByJobClass[15] = AllPoints[15];
	if (AllPoints.IsValidIndex(16))		PointByJobClass[16] = AllPoints[16];
	if (AllPoints.IsValidIndex(17))		PointByJobClass[17] = AllPoints[17];
}

void URenewal_BattleFunctionLibrary::SetAvatarAbilityProperty(UCharacterBattleInfo* CharBattleInfo, TArray<int32>& AbilityPoints)
{
	if (0 >= AbilityPoints.Num())
		return;

	if (false == IsValid(CharBattleInfo))
		return;

	FName AbilityID;
	FAvatarAbilityTableInfo* AbilityTableInfo = nullptr;
	int32 AbilityCount = AbilityPoints.Num();
	for (int32 n = 0; n < AbilityCount; ++n)
	{
		if (0 >= AbilityPoints[n])
			continue;

		switch (n)
		{
		case 0:		AbilityID = "Tank_1";	break;
		case 1:		AbilityID = "Tank_2";	break;
		case 2:		AbilityID = "Tank_3";	break;
		case 3:		AbilityID = "Range_1";	break;
		case 4:		AbilityID = "Range_2";	break;
		case 5:		AbilityID = "Range_3";	break;
		case 6:		AbilityID = "Melee_1";	break;
		case 7:		AbilityID = "Melee_2";	break;
		case 8:		AbilityID = "Melee_3";	break;
		case 9:		AbilityID = "Magic_1";	break;
		case 10:	AbilityID = "Magic_2";	break;
		case 11:	AbilityID = "Magic_3";	break;
		case 12:	AbilityID = "Support_1"; break;
		case 13:	AbilityID = "Support_2"; break;
		case 14:	AbilityID = "Support_3"; break;
		case 15:	AbilityID = "Common_1";	break;
		case 16:	AbilityID = "Common_2";	break;
		case 17:	AbilityID = "Common_3";	break;
		}

		AbilityTableInfo = UTableManager::GetInstancePtr()->GetAvatarAbilityTableRow(AbilityID);
		if (AbilityTableInfo)
		{
			CharBattleInfo->AvatarAbilityPropertyMap.Emplace(AbilityTableInfo->Ability_type,
				AbilityTableInfo->GetValue(AbilityPoints[n]));
		}
	}
}

EPLAY_CLEAR_GRADE URenewal_BattleFunctionLibrary::GetCampaignStageResultType(UObject* WorldContextObject)
{
	EPLAY_CLEAR_GRADE CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_FAILED;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return CampaignStageResultType;

	float ClearTime = RGameInstance->PlayStateInfo->GetClearTime();
	float RemainingTime = RGameInstance->PlayStateInfo->GetRemainingTime();
	int32 DeadHeroCount = 0;
	int32 AliveHeroCount = 0;
	GetDeadHeroAndAliveHeroCount(RGameInstance, DeadHeroCount, AliveHeroCount);

	bool bAllDie = RGameInstance->PlayStateInfo->IsAllHeroDie();

	if ((ClearTime <= 0.0f) || (ClearTime >= CAMPAIGN_STAGE_LIMIT_TIME) || (RemainingTime <= 0.0f) || (AliveHeroCount <= 0) || bAllDie)
	{
		CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_FAILED;
	}
	else if ((ClearTime < CAMPAIGN_STAGE_LIMIT_TIME) && (RemainingTime > 0.0f) && (DeadHeroCount < COMMANDSLOT_MAX))
	{
		switch (DeadHeroCount)
		{
		case 0:
			CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_STAR_THR;
			break;
		case 1:
			CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_STAR_TWO;
			break;
		default:
			CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_STAR_ONE;
			break;
		}
	}
	else
	{
		CampaignStageResultType = EPLAY_CLEAR_GRADE::CLEAR_FAILED;
	}

	return CampaignStageResultType;
}

void URenewal_BattleFunctionLibrary::GetDeadHeroAndAliveHeroCount(UObject* WorldContextObject, int32& DeadHeroCount, int32& AliveHeroCount)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return;
	auto SelectedCharacterArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (FHeroSlotInfo& SelectedCharacterElem : SelectedCharacterArray)
	{
		if (SelectedCharacterElem.HeroUD.IsEmpty() == true)
			continue;

		if (SelectedCharacterElem.SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
		{
			++DeadHeroCount;
		}
		else if (SelectedCharacterElem.SlotSpawnState != EHeroSpawnStateEnum::VE_None)
		{
			++AliveHeroCount;
		}
	}
}


bool URenewal_BattleFunctionLibrary::IsSameFloor(AActor* InTarget01, AActor* InTarget02, float InThreshold)
{
	if (!IsValid(InTarget01) || !IsValid(InTarget02))
		return false;

	ACharacter* SrcCharacter = Cast< ACharacter >(InTarget01);
	float SrcBasePosZ = SrcCharacter->GetActorLocation().Z;
	if (SrcCharacter)
		SrcBasePosZ -= SrcCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	ACharacter* TargetCharacter = Cast< ACharacter >(InTarget02);
	float TargetBasePosZ = TargetCharacter->GetActorLocation().Z;

	// gigant pivot is lower tnan base plane.
	UGigantComponent* GigantComponent = nullptr; // Cast< UGigantComponent >(InTarget02->GetComponentByClass(UGigantComponent::StaticClass()));
	if (GigantComponent)
	{
		TargetBasePosZ += GigantComponent->PivotZOffset;
	}

	if (TargetCharacter)
		TargetBasePosZ -= TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float heightDiff = FMath::Abs(SrcBasePosZ - TargetBasePosZ);
	if (heightDiff <= InThreshold)
		return true;


	return false;
}


FVector URenewal_BattleFunctionLibrary::FindHitPoint(AActor* From, AActor* Target)
{
	FVector Result = FVector::ZeroVector;

	if (IsValid(From) && IsValid(Target))
	{
		FVector StartPos = From->GetActorLocation();
		FVector EndPos = Target->GetActorLocation();

		TArray< FHitResult > OutHitResults;
		FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Trace")), true, From);
		CollisionParams.bTraceComplex = false;

		FCollisionObjectQueryParams CollisionObjectParams = FCollisionObjectQueryParams(ECC_Pawn);

		if (From->GetWorld()->LineTraceMultiByObjectType(OutHitResults, StartPos, EndPos, CollisionObjectParams, CollisionParams) == true)
		{
			for (FHitResult& OutHit : OutHitResults)
			{
				if (OutHit.GetActor() == Target)
				{
					Result = OutHit.ImpactPoint;
					break;
				}
			}
		}

	}
	return Result;
}


float	URenewal_BattleFunctionLibrary::GetSquaredDistanceWithBoundingRadius(AActor* Target01, AActor* Target02)
{
	if (!IsValid(Target01) || !IsValid(Target02))
		return 1.0f;

	auto Target01_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target01);
	auto Target02_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target02);

	if (IsValid(Target01_BattleEntityComponent) && IsValid(Target02_BattleEntityComponent))
	{
		float fUnitRadius01 = Target01_BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;
		float fUnitRadius02 = Target02_BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;

		float SquaredDist = FVector::DistSquared(Target01->GetActorLocation(), Target02->GetActorLocation()) + (fUnitRadius01 * fUnitRadius01) + (fUnitRadius02 * fUnitRadius02);
		return SquaredDist;
	}

	return -1;
}

bool URenewal_BattleFunctionLibrary::CheckTargetDecalNoti(AActor* InTarget)
{
	if (!IsValid(InTarget))
		return false;

	auto Character_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTarget);
	if (!IsValid(Character_BattleEntityComponent))
		return false;

	auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(InTarget->GetWorld());
	if (BasePlayerController)
	{
		auto SelectCharacter = BasePlayerController->GetSelectedCharacter();
		if (SelectCharacter != nullptr)
		{
			auto SelectedCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectCharacter);
			if (IsValid(SelectedCharacter_BattleEntityComponent))
			{
				if (SelectedCharacter_BattleEntityComponent->GetCommandSlotIndex() == Character_BattleEntityComponent->GetCommandSlotIndex())
				{
					return true;
				}
			}
		}
	}

	if (Character_BattleEntityComponent->IsBoss())
	{
		return true;
	}
	return false;
}


void URenewal_BattleFunctionLibrary::ComputeTargetDecal(AActor* InTarget, bool InVisible)
{
	if (IsValid(InTarget))
	{
		TArray<USelectionIndicatorMeshComponent*> SelectionMeshs;
		InTarget->GetComponents(SelectionMeshs);
		//GetSelectionIndicatorMeshComponent()
		for (auto Element : SelectionMeshs)
		{
			if (Element != nullptr)
			{
				if (Element->ComponentHasTag(TAG_TARGETINGMESH))
				{
					Element->SetVisibility(InVisible);
				}
			}
		}
	}
}

void URenewal_BattleFunctionLibrary::VisibleCharacterIndicatorMesh(AActor* InTarget, bool InVisible)
{
	if (IsValid(InTarget))
	{
		UActorComponent* SelectionMeshComponent = InTarget->GetComponentByClass(USelectionIndicatorMeshComponent::StaticClass());

		if (IsValid(SelectionMeshComponent))
		{
			USceneComponent* SceneComponent = Cast<USceneComponent>(SelectionMeshComponent);
			if (SceneComponent)
				SceneComponent->SetVisibility(InVisible, InVisible);
		}
	}
}


//void URenewal_BattleFunctionLibrary::VisibleAttackAreaIndicator(AActor* InActor, bool InVisible, float InScale /*= 100*/)
//{
//	if (IsValid(InActor))
//	{
//		auto TargetComponentArray = InActor->GetComponentsByClass(USkillAreaIndicatorMeshComponent::StaticClass());
//		for (auto& Element : TargetComponentArray)
//		{
//			auto AttackAreaMeshComponent = Cast<USkillAreaIndicatorMeshComponent>(Element);
//			bool IsAttackAreaMesh = (IsValid(AttackAreaMeshComponent) && AttackAreaMeshComponent->ComponentHasTag(TAG_ATTACKAREAMESH));
//			if (IsAttackAreaMesh)
//			{
//				AttackAreaMeshComponent->SetVisibility(InVisible);
//				AttackAreaMeshComponent->ShowSkillArea(InScale, InScale);
//			}
//		}
//	}
//}

//=================================================================================================================================
// State related
//=================================================================================================================================
//EAbonormalStateFlagEnum URenewal_BattleFunctionLibrary::GetStateFlagBySlotType(EAbnormalSlotTypeEnum InSlotType)
//{
//	switch (InSlotType)
//	{
//	case EAbnormalSlotTypeEnum::VE_Slot_Stun:
//		return EAbonormalStateFlagEnum::BM_Stun;
//	case EAbnormalSlotTypeEnum::VE_Slot_Down:
//		return EAbonormalStateFlagEnum::BM_Down;
//	case EAbnormalSlotTypeEnum::VE_Slot_Freezing:
//		return EAbonormalStateFlagEnum::BM_Frozen;
//	case EAbnormalSlotTypeEnum::VE_Slot_Hold:
//		return EAbonormalStateFlagEnum::BM_Hold;
//	case EAbnormalSlotTypeEnum::VE_Slot_Fear:
//		return EAbonormalStateFlagEnum::BM_Fear;
//	case EAbnormalSlotTypeEnum::VE_Slot_Silence:
//		return EAbonormalStateFlagEnum::BM_Silence;
//	case EAbnormalSlotTypeEnum::VE_Slot_Hide:
//		return EAbonormalStateFlagEnum::BM_Hide;
//	case EAbnormalSlotTypeEnum::VE_Slot_Airborne:
//		return EAbonormalStateFlagEnum::BM_Airborne;
//	}
//
//	return EAbonormalStateFlagEnum::BM_None;
//}


//=================================================================================================================================
// Search functions
//=================================================================================================================================

bool URenewal_BattleFunctionLibrary::IsPreferType(AActor* Target, EPriorityTypeEnum PriorityType)
{
	if (!IsValid(Target))
		return false;
		
	auto Target_BaseProperty = Cast<IEntityBaseProperty>(Target);
	if ( Target_BaseProperty != nullptr)
	{
		EClassTypeEnum TargetJobClass = Target_BaseProperty->GetJobClass();
		bool CheckPrefer = false;
		switch (PriorityType)
		{
			case EPriorityTypeEnum::VE_Tanker:
				if (TargetJobClass == EClassTypeEnum::VE_Tanker)
					CheckPrefer = true;
				break;
			case EPriorityTypeEnum::VE_MeleeDealer:
				if (TargetJobClass == EClassTypeEnum::VE_MeleeDealer)
					CheckPrefer = true;
				break;
			case EPriorityTypeEnum::VE_RangeDealer:
				if (TargetJobClass == EClassTypeEnum::VE_RangeDealer)
					CheckPrefer = true;
				break;
			case EPriorityTypeEnum::VE_MagicDealer:
				if (TargetJobClass == EClassTypeEnum::VE_MagicDealer)
					CheckPrefer = true;
				break;
			case EPriorityTypeEnum::VE_Supporter:
				if (TargetJobClass == EClassTypeEnum::VE_Supporter)
					CheckPrefer = true;
				break;

			case EPriorityTypeEnum::VE_Object:
			{
				if (Target_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Object)
					CheckPrefer = true;
			}
			break;
		}

		return CheckPrefer;
	}

	return false;
}

bool URenewal_BattleFunctionLibrary::IsAISearchTargetType(AActor* Target, EPriorityTypeEnum PriorityType)
{

	return true;
}


void URenewal_BattleFunctionLibrary::FindTarget(TArray< TWeakObjectPtr<AActor> >& outArray, AActor* Source, FVector targetLocation, EFriendshipTypeEnum friendshipType, ESearchShapeTypeEnum applyType, FVector value0, float rotate, bool bDrawDebug)
{
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->FindTarget(outArray, Source, targetLocation, friendshipType, applyType, value0, rotate, bDrawDebug);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.FindTarget(outArray, Source, targetLocation, friendshipType, applyType, value0, rotate, bDrawDebug);
	}
// 	UGameObjectManager* objMgr = UUtilFunctionLibrary::GetGameObjectManagerFromActor(Source);
// 	if (IsValid(objMgr))
// 	{
// 		objMgr->FindTarget(outArray, Source, targetLocation, friendshipType, applyType, value0, rotate, bDrawDebug);
// 	}
}


void URenewal_BattleFunctionLibrary::FindFriendshipType(TArray< TWeakObjectPtr<AActor> >& outArray, AActor* Source, EFriendshipTypeEnum friendshipType)
{
// 	UGameObjectManager* objMgr = UUtilFunctionLibrary::GetGameObjectManagerFromActor(Source);
// 	if (IsValid(objMgr))
	{
		TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
// 		objMgr->FindTargetFriendType(CheckBattleCharacter, Source, friendshipType);
// 		UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 		if (pGamePlayMode)
// 		{
// 			pGamePlayMode->FindTargetFriendType(CheckBattleCharacter, Source, friendshipType);
// 		}
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.FindTargetFriendType(CheckBattleCharacter, Source, friendshipType);
		}
		for (auto& CheckCharacter : CheckBattleCharacter)
		{
			bool bIsNotException = ExceptionEnemyCondition(Source, CheckCharacter.Get());

			if (!bIsNotException)
				continue;

			outArray.Emplace(CheckCharacter);
		}
	}
}


AActor* URenewal_BattleFunctionLibrary::FindEnemy(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float range, EEnemySearchTargetTypeEnum FindFlag)
{
	AActor* ResultCharacter = nullptr;

	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
	FindTarget(CheckBattleCharacter, Target, Origin, EFriendshipTypeEnum::VE_Enemy, ShapeType, FVector(range, range, range));

	if ( CheckBattleCharacter.Num() == 0)
		return nullptr;

	if (CheckBattleCharacter.Num() == 1)
	{
		AActor* CheckCharacterActor = CheckBattleCharacter[0].Get();
		const bool bIsNotException = ExceptionEnemyCondition(Target, CheckCharacterActor);

		if (bIsNotException)
		{
			return CheckCharacterActor;
		}
		else
		{
			return nullptr;
		}
	}

	switch (FindFlag)
	{
		case EEnemySearchTargetTypeEnum::VE_Close:
		{
			float minDist = 10000000.0f;
			AActor* nearestCharacter = nullptr;

			for (auto& Character : CheckBattleCharacter)
			{				
				bool bIsNotException = ExceptionEnemyCondition(Target, Character.Get() );
				if (!bIsNotException)
					continue;

				float Dist = FVector::DistSquared(Origin, Character->GetActorLocation());
				if (Dist < minDist)
				{
					minDist = Dist;
					nearestCharacter = Character.Get();
				}
			}
			ResultCharacter = nearestCharacter;
		}
		break;
	}

	return ResultCharacter;
}

AActor* URenewal_BattleFunctionLibrary::SearchFriend(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float Range, float RangeThreshold, EPriorityTypeEnum PriorityType, EPriorityTypeEnum InAISearchPreferType)
{
	AActor* ResultCharacter = nullptr;
	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;

	FindTarget(CheckBattleCharacter, Target, Origin, EFriendshipTypeEnum::VE_Friend, ShapeType, FVector(Range, Range, Range));
	
	if ( CheckBattleCharacter.Num() == 0)
		return nullptr;

	if (CheckBattleCharacter.Num() == 1)
	{
		AActor* CheckCharacterActor = CheckBattleCharacter[0].Get();

		auto CheckBattleCharacter_BaseProperty = Cast<IEntityBaseProperty>(CheckCharacterActor);
		if (CheckBattleCharacter_BaseProperty != nullptr)
		{
			bool bIsReady = CheckBattleCharacter_BaseProperty->IsReady();
			bool bIsEqualTargetAndCaster = (Target == CheckCharacterActor );
			if (bIsReady == false || bIsEqualTargetAndCaster)
				return nullptr;

			return CheckCharacterActor;
		}
	}

	// Has ai search prefer type?
	EPriorityTypeEnum CheckPreferType = PriorityType;
	if (InAISearchPreferType != EPriorityTypeEnum::VE_None)
		CheckPreferType = InAISearchPreferType;

	float minDist		= 10000000.0f;	
	float PreferMinDist = 10000000.0f;
	float PreferMaxDist = 0.0f;
	AActor* PreferCheckTarget = nullptr;
	AActor* SafeTargetActor = nullptr;	
	float MinHPPercent = 1.0f;
	float MaxHPPercent = 0.0f;	
	
	for (auto& CheckCharacter : CheckBattleCharacter)
	{
		auto CheckBattleCharacter_BaseProperty = Cast<IEntityBaseProperty>( CheckCharacter.Get() );
		auto CheckBattleCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent( CheckCharacter.Get() );
		if (CheckBattleCharacter_BaseProperty != nullptr && IsValid(CheckBattleCharacter_BattleEntityComponent))
		{
			bool bIsReady = CheckBattleCharacter_BaseProperty->IsReady();
			bool bIsNotFriend = CheckBattleCharacter_BattleEntityComponent->IsNonFriend;
			bool bIsEqualTargetAndCaster = (Target == CheckCharacter.Get());
			if (bIsReady == false || bIsEqualTargetAndCaster || bIsNotFriend)
			{
				//++Index;
				continue;
			}

			float CheckTargetHPPercent = CheckBattleCharacter_BattleEntityComponent->GetCurrentHPPercent();
			if (CheckTargetHPPercent != 1.0f)
			{
				SafeTargetActor = CheckCharacter.Get();
			}

			float Dist = FVector::DistSquared(Origin, CheckCharacter->GetActorLocation());
			// Save default nearest checkTarget
			if (Dist < minDist)
			{
				minDist = Dist;
				ResultCharacter = CheckCharacter.Get();
			}

			/*
			Prefer type check..
			*/
			{
				// job class prefer
				bool CheckPrefer = IsPreferType(CheckCharacter.Get(), CheckPreferType);
				if (CheckPrefer)
				{					
					if (Dist < PreferMinDist)
					{
						PreferMinDist = Dist;
						PreferCheckTarget = CheckCharacter.Get();
					}
				}
				else
				{
					// hp or other condition prefer
					switch (CheckPreferType)
					{
					case EPriorityTypeEnum::VE_LowestHP:
					{
						if (CheckTargetHPPercent < MinHPPercent)
						{
							MinHPPercent = CheckTargetHPPercent;
							PreferCheckTarget = CheckCharacter.Get();
						}
					}
					break;
					case EPriorityTypeEnum::VE_HighestHP:
					{
						if (CheckTargetHPPercent > MaxHPPercent)
						{
							MaxHPPercent = CheckTargetHPPercent;
							PreferCheckTarget = CheckCharacter.Get();
						}
					}
					break;
					case EPriorityTypeEnum::VE_Far:
					{
						if (Dist > PreferMaxDist)
						{
							PreferMaxDist = Dist;
							PreferCheckTarget = CheckCharacter.Get();
						}
					}
					break;
					case EPriorityTypeEnum::VE_Near:
					{
						if (Dist < PreferMinDist)
						{
							PreferMinDist = Dist;
							PreferCheckTarget = CheckCharacter.Get();
						}
					}
					break;
					}
				}
			}
		}
	}

	if ( IsValid(PreferCheckTarget))
		ResultCharacter = PreferCheckTarget;

	auto ResultCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(ResultCharacter);
	if (IsValid(ResultCharacter_BattleEntityComponent))
	{
		if (IsValid(ResultCharacter) && ResultCharacter_BattleEntityComponent->GetCurrentHPPercent() == 1.0f)
		{
			ResultCharacter = SafeTargetActor;
		}
	}

#ifdef USE_SEARCH_THRESHOLD
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (IsValid(Target_BattleEntityComponent))
	{
		auto CurrentAttackTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target_BattleEntityComponent->GetCurrentAttackTarget());
		if (IsValid(CurrentAttackTarget_BattleEntityComponent))
		{
			// If i find prefer target, compare current attack target preference..
			if (CheckPreferType != EPriorityTypeEnum::VE_None && PreferCheckTarget != nullptr)
			{

				if (IsValid(Target) && IsValid(Target_BattleEntityComponent->GetCurrentAttackTarget()) && !(CurrentAttackTarget_BattleEntityComponent->IsDead()) && IsPreferType(Target_BattleEntityComponent->GetCurrentAttackTarget(), PriorityType))
				{
					float oldTargetDist = FVector::DistSquared(Target_BattleEntityComponent->GetCurrentAttackTarget()->GetActorLocation(), Origin);
					float difference = oldTargetDist - minDist;
					if (difference > (RangeThreshold * RangeThreshold))
						return ResultCharacter;
					else
					{
						//UE_LOG(LogBattle, Log, TEXT(" Keep the old prefer target "));
						return Target_BattleEntityComponent->GetCurrentAttackTarget();
					}
				}
				return ResultCharacter;
			}
		}

		//If new nearest target is not far from me than threshold, keep the current target.
		if (IsValid(Target) && IsValid(Target_BattleEntityComponent->GetCurrentAttackTarget()) && !(CurrentAttackTarget_BattleEntityComponent->IsDead()))
		{
			float oldTargetDist = FVector::DistSquared(Target_BattleEntityComponent->GetCurrentAttackTarget()->GetActorLocation(), Origin);
			float difference = oldTargetDist - minDist;
			if (difference > (RangeThreshold * RangeThreshold))
				return ResultCharacter;
			else
			{
				if (CurrentAttackTarget_BattleEntityComponent->GetCurrentHPPercent() == 1)
				{
					return ResultCharacter;
				}
		
				return Target_BattleEntityComponent->GetCurrentAttackTarget();
		
				/*todo Caster heal...
				bool bIsEqualTargetAndCaster = (Target == Target->CurrentAttackTarget);
				if (bIsEqualTargetAndCaster)
				{
				return NearestCharacter;
				}
				else
				{*/
				//UE_LOG(LogBattle, Log, TEXT(" Keep the old target "));
		
				//}
			}
		}
	}
#endif
	
	return ResultCharacter;
}

AActor* URenewal_BattleFunctionLibrary::SearchEnemy(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float Range, float RangeThreshold, EPriorityTypeEnum PriorityType, EPriorityTypeEnum InAISearchPreferType )
{
	AActor* ResultCharacter = nullptr;
	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
	FindTarget(CheckBattleCharacter, Target, Origin, EFriendshipTypeEnum::VE_Enemy, ShapeType, FVector(Range, Range, Range));

	if ( CheckBattleCharacter.Num() == 0)
		return nullptr;

	if (CheckBattleCharacter.Num() == 1)
	{
		AActor* CheckCharacterActor = CheckBattleCharacter[0].Get();

		auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(CheckCharacterActor);
		if (OwnerActor_BaseProperty != nullptr)
		{
			//bool bIsReady = OwnerActor_BaseProperty->IsReady();
			//if (bIsReady == false)
			//	return nullptr;

			bool bIsNotException = ExceptionEnemyCondition(Target, CheckCharacterActor);
			if (bIsNotException)
			{
				return CheckCharacterActor;
			}
			else
			{
				return nullptr;
			}
		}
	}

	EPriorityTypeEnum CheckPreferType = PriorityType;
	if (InAISearchPreferType != EPriorityTypeEnum::VE_None)
		CheckPreferType = InAISearchPreferType;

	float minDist		= 100000000.0f;
	float PreferMinDist = 100000000.0f;
	float PreferMaxDist = 0.0f;
	AActor* PreferCheckTarget = nullptr;
	int32 MinHPPercent = 100000000;
	int32 MaxHPPercent = 0;
	for (auto& CheckCharacter : CheckBattleCharacter)
	{
		auto CheckCharacter_BaseProperty = Cast<IEntityBaseProperty>( CheckCharacter.Get() );
		if (CheckCharacter_BaseProperty)
		{
			bool bIsReady = CheckCharacter_BaseProperty->IsReady();
			if (bIsReady == false)
				continue;

			bool bIsNotException = ExceptionEnemyCondition(Target, CheckCharacter.Get() );
			if (!bIsNotException)
				continue;

			float Dist = FVector::DistSquared(Origin, CheckCharacter->GetActorLocation());			
			// Save default nearest checkTarget
			if (Dist < minDist)
			{
				minDist = Dist;
				ResultCharacter = CheckCharacter.Get();
			}
			
			/*
			Prefer type check..
			*/
			if(EPriorityTypeEnum::VE_None != CheckPreferType)
			{
				// job class prefer
				bool CheckPrefer = IsPreferType(CheckCharacter.Get(), CheckPreferType);
				if (CheckPrefer)
				{
					if (Dist < PreferMinDist)
					{
						PreferMinDist = Dist;
						PreferCheckTarget = CheckCharacter.Get();
					}
				}
				else
				{
					// hp or other condition prefer
					switch (CheckPreferType)
					{
						case EPriorityTypeEnum::VE_LowestHP:
						{
							auto CheckBattleCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CheckCharacter.Get());
							if (IsValid(CheckBattleCharacter_BattleEntityComponent) == false)
								continue;

							float CheckTargetHPPercent = CheckBattleCharacter_BattleEntityComponent->GetCurrentHPPercent();
							if (CheckTargetHPPercent < MinHPPercent)
							{
								MinHPPercent = CheckTargetHPPercent;
								PreferCheckTarget = CheckCharacter.Get();
							}
						}
						break;
						case EPriorityTypeEnum::VE_HighestHP:
						{
							auto CheckBattleCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CheckCharacter.Get());
							if (IsValid(CheckBattleCharacter_BattleEntityComponent) == false)
								continue;

							float CheckTargetHPPercent = CheckBattleCharacter_BattleEntityComponent->GetCurrentHPPercent();
							if (CheckTargetHPPercent > MaxHPPercent)
							{
								MaxHPPercent = CheckTargetHPPercent;
								PreferCheckTarget = CheckCharacter.Get();
							}
						}
						break;
						case EPriorityTypeEnum::VE_ProtectTarget:
						{
							if (IsProtectTarget(CheckCharacter.Get()))
							{
								PreferCheckTarget = CheckCharacter.Get();
							}
						}
						break;
						case EPriorityTypeEnum::VE_Far:
						{
							if (Dist > PreferMaxDist)
							{
								PreferMaxDist = Dist;
								PreferCheckTarget = CheckCharacter.Get();
							}
						}
						break;
						case EPriorityTypeEnum::VE_Near:
						{
							if (Dist < PreferMinDist)
							{
								PreferMinDist = Dist;
								PreferCheckTarget = CheckCharacter.Get();
							}
						}
						break;
					}
				}
			}
		}
	}

	if ( IsValid(PreferCheckTarget))
		ResultCharacter = PreferCheckTarget;

#ifdef USE_SEARCH_THRESHOLD
	// If i find prefer target, compare current attack target preference..
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (IsValid(Target_BattleEntityComponent))
	{
		auto CurrentAttackTarget = Target_BattleEntityComponent->GetCurrentAttackTarget();
		if (IsValid(CurrentAttackTarget))
		{
			auto CurrentAttackTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget);
			if (IsValid(CurrentAttackTarget_BattleEntityComponent))
			{
				auto PreferNearestCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PreferNearestCharacter);
				if (CheckPreferType != EPriorityTypeEnum::VE_None
					&& ResultCharacter != nullptr
					&& PreferNearestCharacter_BattleEntityComponent->IsDead() == false)
				{
					if ((CurrentAttackTarget_BattleEntityComponent->IsDead() == false)
						&& IsPreferType(CurrentAttackTarget, CheckPreferType))
					{
						float oldTargetDist = FVector::DistSquared(CurrentAttackTarget->GetActorLocation(), Origin);
						float difference = oldTargetDist - minDist;
						if (difference > (RangeThreshold * RangeThreshold))
							return ResultCharacter;
						else
						{
							//UE_LOG(LogBattle, Log, TEXT(" Keep the old prefer target "));
							return CurrentAttackTarget;
						}
					}
					return ResultCharacter;
				}
			}
		}
	}
#endif

	return ResultCharacter;
}

bool URenewal_BattleFunctionLibrary::KeepSafeRule(AActor* InOwner, AActor* InTarget)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (InTarget == InOwner)
		return false;
	
	auto World = InOwner->GetWorld();
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (IsValid(RGameInstance) == false)
// 		return false;
	
	bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(InOwner, InTarget));
	if (IsException)
		return false;

	bool bResult = true;
	auto GameMode = BaseStageInfo->GetGameMode();
	switch (GameMode)
	{
		case EGamePlayModeTypeEnum::VE_BattleGround:
		case EGamePlayModeTypeEnum::VE_BattleField:
		case EGamePlayModeTypeEnum::VE_RaidKillBoss:
		{
			float SquaredDist = FVector::DistSquared(InOwner->GetActorLocation(), InTarget->GetActorLocation());
			if (SquaredDist > (NET_SEARCHSIZE * NET_SEARCHSIZE))
			{
				bResult = false;
			}
		}
		break;
	}
	

	return bResult;
}


bool URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(AActor* Caster, AActor* Target)
{
	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	auto Caster_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (!IsValid(Target_BattleEntityComponent) || !IsValid(Caster_BattleEntityComponent))
		return false;

	if (Target_BattleEntityComponent->IsDead() || Target_BattleEntityComponent->IsSearchSkip()) 
		return false;

	if (Target_BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Hide))
	{
		if (URenewal_BattleFunctionLibrary::IsEnemyActor(Caster, Target) == false)
		{
			return true;
		}
		
// 		UGameObjectManager* objMgr = UUtilFunctionLibrary::GetGameObjectManagerFromActor(Caster);
// 		if (IsValid(objMgr))
// 		{
// 			if (objMgr->IsInDectectArea(Target))
// 			{
// 				return true;
// 			}
// 		}
		return false;
	}

	if (Caster_BattleEntityComponent->IsInSituationState(ECharacterSituationState::VE_Ground))
	{
		switch (Target_BattleEntityComponent->GetSituationState())
		{
		case ECharacterSituationState::VE_ProgressAir:
		case ECharacterSituationState::VE_Air:
			if (Caster_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<EAttackTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackType) == EAttackTypeEnum::VE_Melee)
			{
				return false;
			}
			break;
		case ECharacterSituationState::VE_ProgressAirHigh:
		case ECharacterSituationState::VE_AirHigh:
		{
			return false;
		}
		break;
		default:
			break;
		}
	}

	return true;
}

float URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(EClassTypeEnum ClassTypeEnum, EHERO_ENCHANT_STAT_SLOT eEnchantStateSlot, int32 point)
{
	if (point == 0)
		return 0.0f;
	const UDataTable* EnchantTable = UTableManager::GetInstancePtr()->GetHeroEnchantTable(ClassTypeEnum);
	FHeroEnchant_V2* EnchantInfo = UTableManager::GetHeroEnchantInfo(EnchantTable, point);

	if (EnchantInfo == nullptr)
		return 0.0f;

	switch (eEnchantStateSlot)
	{
	case EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower:
		return EnchantInfo->PhysicalAttackPower;
		break;
	case EHERO_ENCHANT_STAT_SLOT::MagicAttackPower:
		return EnchantInfo->MagicAttackPower;
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::Armor:
		return EnchantInfo->Armor;
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MagicResistance:
		return EnchantInfo->MagicResistance;
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MaxHP:
		return EnchantInfo->MaxHP;
		break;
	}
	
	return 0.0f;
}
