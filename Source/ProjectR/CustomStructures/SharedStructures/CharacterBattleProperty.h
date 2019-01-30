// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CharacterBattleData.h"
#include "SharedConstants/GlobalConstants.h"
#include "ProjectR.h"
#include "CharacterBattleProperty.generated.h"


#define RETURN_CHARACTER_PROPERTY_INT( X, Y )\
case ECharacterPropertyTypeEnum::VE_##Y:\
return (int32)(X->Y);

#define SET_CHARACTER_PROPERTY( X, Y )\
case ECharacterPropertyTypeEnum::VE_##X:\
X = Y; break;

#define RETURN_CHARACTER_PROPERTY_FLOAT( X, Y )\
case ECharacterPropertyTypeEnum::VE_##Y:\
return (float)(X->Y);


namespace RSTemplate
{
	template <typename T >
	struct Impl_GetCharacterProperty;
	template <>
	struct Impl_GetCharacterProperty<int32>;
	template <>
	struct Impl_GetCharacterProperty<float>;
	template <>
	struct Impl_GetCharacterProperty<EAttackTypeEnum>;
	template <>
	struct Impl_GetCharacterProperty<EPriorityTypeEnum>;
	template <>
	struct Impl_GetCharacterProperty<ECharacterPropertyTypeEnum>;
}

/**
 * 
 */
USTRUCT()
struct FCharacterBattleProperty : public FCharacterBattleData
{
	GENERATED_USTRUCT_BODY()
	
	 
public:


	void	Clear();


	/*
	Getter
	*/
	template <typename T >
	T GetCharacterProperty(ECharacterPropertyTypeEnum PropertyType)
	{
		return RSTemplate::Impl_GetCharacterProperty<T>::Get(PropertyType, this);
	}
	

	/*
	Setter
	*/
	// set additional property by buff..
	template <typename T>
	void SetCharacterProperty(ECharacterPropertyTypeEnum PropertyType, T Value)
	{
		switch (PropertyType)
		{
			// attack value
			SET_CHARACTER_PROPERTY(PhysicalAttackPower, Value);
			SET_CHARACTER_PROPERTY(MagicAttackPower, Value);
			SET_CHARACTER_PROPERTY(AccuracyValue, Value);
			SET_CHARACTER_PROPERTY(PierceValue, Value);
			SET_CHARACTER_PROPERTY(AttackSpeed, Value);
			SET_CHARACTER_PROPERTY(CriticalValue, Value);
			SET_CHARACTER_PROPERTY(CriticalDamageRate_Scale, Value);
			SET_CHARACTER_PROPERTY(AttackRange, Value);

			//	defence value
			SET_CHARACTER_PROPERTY(Armor, Value);
			SET_CHARACTER_PROPERTY(MagicResistance, Value);
			SET_CHARACTER_PROPERTY(MaxHP, Value);
			SET_CHARACTER_PROPERTY(DodgeValue, Value);

			//	etc value
			SET_CHARACTER_PROPERTY(MoveSpeed, Value);
			SET_CHARACTER_PROPERTY(SightRange, Value);
			SET_CHARACTER_PROPERTY(UnitRadius, Value);

			/************************************************************************/
			/* Renewal Property                                                     */
			/************************************************************************/
// 			SET_CHARACTER_PROPERTY(PhysicalAttackPowerCorrection, Value);			
// 			SET_CHARACTER_PROPERTY(MagicAttackPowerCorrection, Value);
// 			SET_CHARACTER_PROPERTY(AccuracyValueCorrection, Value);
// 			SET_CHARACTER_PROPERTY(PierceValueCorrection, Value);
// 			SET_CHARACTER_PROPERTY(CriticalValueCorrection, Value);
// 			SET_CHARACTER_PROPERTY(ArmorCorrection, Value);
// 			SET_CHARACTER_PROPERTY(MagicResistanceCorrection, Value);
// 			SET_CHARACTER_PROPERTY(DodgeValueCorrection, Value);
// 			SET_CHARACTER_PROPERTY(ParryValueCorrection, Value);

		/************************************************************************/
		/* Old Property                                                     */
		/************************************************************************/
		
// 			SET_CHARACTER_PROPERTY(MaxHP, Value);
// 			//SET_CHARACTER_PROPERTY(MaxMP, Value);
// 			SET_CHARACTER_PROPERTY(PhysicalAttackPower, Value);
// 			SET_CHARACTER_PROPERTY(MagicAttackPower, Value);
// 			SET_CHARACTER_PROPERTY(MagicCriticalRate, Value);
// 			SET_CHARACTER_PROPERTY(MoveSpeed, Value);
// 			SET_CHARACTER_PROPERTY(AttackSpeed, Value);
// 			SET_CHARACTER_PROPERTY(SightRange, Value);
// 			//SET_CHARACTER_PROPERTY(SightRange_PD, Value);
// 			SET_CHARACTER_PROPERTY(AttackRange, Value);
// 			//SET_CHARACTER_PROPERTY(FlyAttackRange, Value);
// 			SET_CHARACTER_PROPERTY(UnitRadius, Value);
// 			SET_CHARACTER_PROPERTY(RangeThreshold, Value);
// 			SET_CHARACTER_PROPERTY(Armor, Value);
// 			SET_CHARACTER_PROPERTY(PierceDefenceBonus, Value);
// 			SET_CHARACTER_PROPERTY(MagicResistance, Value);
// 			SET_CHARACTER_PROPERTY(ParryValue, Value);
// 			SET_CHARACTER_PROPERTY(ParryDamageReduceRate_Scale, Value);
// 			SET_CHARACTER_PROPERTY(DodgeValue, Value);
// 			//SET_CHARACTER_PROPERTY(MPRegen, Value);
// 			SET_CHARACTER_PROPERTY(CriticalValue, Value);
// 			SET_CHARACTER_PROPERTY(CriticalDamageRate_Scale, Value);
// 			SET_CHARACTER_PROPERTY(CriticalResistValue, Value);
// 			SET_CHARACTER_PROPERTY(CriticalResistDamage, Value);
// 			SET_CHARACTER_PROPERTY(PierceValue, Value);
// 			SET_CHARACTER_PROPERTY(AccuracyValue, Value);
// 			//SET_CHARACTER_PROPERTY(LifeStealValue, Value);
// 			SET_CHARACTER_PROPERTY(SpawnCount, Value);



		case ECharacterPropertyTypeEnum::VE_AttackType:
			AttackType = (EAttackTypeEnum)(int32)Value;
			break;

		case ECharacterPropertyTypeEnum::VE_AIPreferType:			
			AIPriorityType = (EPriorityTypeEnum)(int32)Value;
			break;

		case ECharacterPropertyTypeEnum::VE_AttackPropertyType:			
			AttackPropertyType = (ECharacterPropertyTypeEnum)(int32)Value;
			break;

		default:
			UE_LOG(LogBattle, Warning, TEXT("Invalid property request"));
			break;

		}
	}

};



namespace RSTemplate
{
	template <typename T >
	struct Impl_GetCharacterProperty
	{
		static T Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty*){ return T(); };
	};
	

	template <>
	struct Impl_GetCharacterProperty<int32>
	{
		static int32 Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty* BattleProperty)
		{
			switch (PropertyType)
			{
				// attack value
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PhysicalAttackPower);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicAttackPower);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AccuracyValue);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PierceValue);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AttackSpeed);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalValue);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalDamageRate_Scale);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AttackRange);

				//	defence value
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, Armor);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicResistance);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MaxHP);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, DodgeValue);

				//	etc value
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MoveSpeed);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, SightRange);
				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, UnitRadius);

				/************************************************************************/
				/* Renewal Property                                                     */
				/************************************************************************/
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PhysicalAttackPowerCorrection);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicAttackPowerCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AccuracyValueCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PierceValueCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalValueCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, ArmorCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicResistanceCorrection);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, DodgeValueCorrection);				
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, ParryValueCorrection);

				/************************************************************************/
				/* Old Property                                                         */
				/************************************************************************/

// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MaxHP);
// 				//RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MaxMP);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PhysicalAttackPower);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicAttackPower);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicCriticalRate);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MoveSpeed);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AttackSpeed);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, SightRange);
// 				//RETURN_CHARACTER_PROPERTY_INT(BattleProperty, SightRange_PD);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AttackRange);
// 				//RETURN_CHARACTER_PROPERTY_INT(BattleProperty, FlyAttackRange);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, UnitRadius);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, RangeThreshold);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, Armor);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MagicResistance);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PierceDefenceBonus);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, ParryValue);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, ParryDamageReduceRate_Scale);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, DodgeValue);			
// 				//RETURN_CHARACTER_PROPERTY_INT(BattleProperty, MPRegen);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalValue);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalDamageRate_Scale);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalResistValue);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, CriticalResistDamage);
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, PierceValue);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, AccuracyValue);			
// 				//RETURN_CHARACTER_PROPERTY_INT(BattleProperty, LifeStealValue);			
// 				RETURN_CHARACTER_PROPERTY_INT(BattleProperty, SpawnCount);
			

			default:
				UE_LOG(LogBattle, Log, TEXT("Invalid property type request"), (int32)PropertyType);
				break;
			}

			return 1;
		}
	};

	template <>
	struct Impl_GetCharacterProperty<float>
	{
		static float Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty* BattleProperty)
		{
			switch (PropertyType)
			{			
				// attack value
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PhysicalAttackPower);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicAttackPower);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AccuracyValue);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PierceValue);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AttackSpeed);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalValue);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalDamageRate_Scale);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AttackRange);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalResistValue);

				//	defence value
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, Armor);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicResistance);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MaxHP);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, DodgeValue);

				//	etc value
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MoveSpeed);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, SightRange);
				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, UnitRadius);

				/************************************************************************/
				/* Renewal Property                                                     */
				/************************************************************************/
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PhysicalAttackPowerCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicAttackPowerCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AccuracyValueCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PierceValueCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalValueCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, ArmorCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicResistanceCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, DodgeValueCorrection);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, ParryValueCorrection);

				/************************************************************************/
				/* Old Property                                                         */
				/************************************************************************/

// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MaxHP);
// 				//RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MaxMP);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PhysicalAttackPower);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicAttackPower);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicCriticalRate);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MoveSpeed);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AttackSpeed);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, SightRange);
// 				//RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, SightRange_PD);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AttackRange);
// 				//RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, FlyAttackRange);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, UnitRadius);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, RangeThreshold);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, Armor);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MagicResistance);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PierceDefenceBonus);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, ParryValue);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, ParryDamageReduceRate_Scale);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, DodgeValue);
// 				//RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, MPRegen);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalValue);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalDamageRate_Scale);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalResistValue);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, CriticalResistDamage);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, PierceValue);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, AccuracyValue);
// 				//RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, LifeStealValue);
// 				RETURN_CHARACTER_PROPERTY_FLOAT(BattleProperty, SpawnCount);

			default:
				UE_LOG(LogBattle, Log, TEXT("Invalid property type request: %d"), (int32)PropertyType);
				break;
			}

			return 1.0f;
		}
	};

	template <>
	struct Impl_GetCharacterProperty<EAttackTypeEnum>
	{
		static EAttackTypeEnum Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty* BattleProperty)
		{
			switch (PropertyType)
			{
			case ECharacterPropertyTypeEnum::VE_AttackType:
				return BattleProperty->AttackType;

			}

			return EAttackTypeEnum::VE_None;
		}
	};

	template <>
	struct Impl_GetCharacterProperty<EPriorityTypeEnum>
	{
		static EPriorityTypeEnum Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty* BattleProperty)
		{
			switch (PropertyType)
			{
			case ECharacterPropertyTypeEnum::VE_AIPreferType:
				return BattleProperty->AIPriorityType;

			}

			return EPriorityTypeEnum::VE_None;
		}
	};

	template <>
	struct Impl_GetCharacterProperty<ECharacterPropertyTypeEnum>
	{
		static ECharacterPropertyTypeEnum Get(ECharacterPropertyTypeEnum PropertyType, FCharacterBattleProperty* BattleProperty)
		{
			switch (PropertyType)
			{
			case ECharacterPropertyTypeEnum::VE_AttackPropertyType:
				return BattleProperty->AttackPropertyType;
				break;
			}

			return ECharacterPropertyTypeEnum::VE_None;
		}
	};
}