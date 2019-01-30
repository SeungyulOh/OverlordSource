// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CharacterBattleProperty.h"
#include "Character/Bullets/BulletBase.h"
#include "CharacterBattleProperty.h"
#include "Table/ItemTableInfo.h"
#include "CharacterBattleInfo.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UCharacterBattleInfo: public UObject
 {
	GENERATED_BODY()
	
	 
public:

	// temp
	/*
	modification용 정보 저장
	*/
	FName										BeforeModificationCharacterID;
	int32										BeforeModificationArmor = 0;
	int32										BeforeModificationMagicResistance = 0;
	
	UPROPERTY()
	FTimerHandle								TimerHandle_ModifyEnd;

	AActor*		OwnerActor = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	int32		Grade = 1;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	int32		Level = 1;
	
	UPROPERTY( EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	int32		HP = 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	TArray<int32>		skills;



	// projectile info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UClass*		BulletType = nullptr;

	// RWeapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UClass*		RWeapon = nullptr;

	// LWeapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UClass*		LWeapon = nullptr;

	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		ActiveSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		ActiveSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		ActiveSkill03;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		ActiveSkill04;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		PassiveSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		PassiveSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		SpiritSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		SpiritSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		SpiritSkill03;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Battle)
	FName		SpiritSkill04;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	TArray<FName>	ItemSkillIDList;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	FCharacterBattleProperty		CharacterProperty;

	// additional property by buff..(exception Passive,Leader)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	FCharacterBattleProperty		AdditionalCharacterProperty;

	// Item stats.
	// Item stats are all int32
	// ItemStatsByEquipPosition[EItemEquipPositionEnum][EItemStatOption]
	TArray<TArray<int32>> ItemStatsByEquipPosition;

	// For UI
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	FCharacterBattleProperty		TotalItemCharacterProperty;
	// For UI
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterBattleInfo)
	FCharacterBattleProperty		FinalCharacterProperty;
	
	TMap<EAvatarAbilityType, float>	AvatarAbilityPropertyMap;
	float GetAvatarAbilityProperty(EAvatarAbilityType AbilityType);

	void		Init();

	FORCEINLINE void		SetLevel(int32 level)	{ Level = level; }
	FORCEINLINE int32		GetLevel()				{ return Level; }
	FORCEINLINE void		SetGrade(int32 grade)	{ Grade = grade; }
	FORCEINLINE int32		GetGrade()				{ return Grade; }
	int32		SetHP( int32 hp);
	FORCEINLINE int32		GetHP()					{ return HP; }

	FORCEINLINE void			SetSkills(TArray<int32> Inskills) { skills = Inskills; }
	FORCEINLINE TArray<int32>	GetSkills() { return skills; }

	void		AddHP(int32 hp);

	/*
	Refresh helper functions
	*/
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void		ValidateFinalCharacterProperty();
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void		ValidateTotalItemProperty();

	/*
	modification 용 characterID 저장
	*/
	void			SetBeforeModificationInfo(AActor* InActor, FName InCharacterID, int32 InArmor, int32 InMagicResistance);
	void			GetBeforeModificationInfo(FName& InCharacterID, int32& InArmor, int32& InMagicResistance);

	void		ModifyTimerActive();
	UFUNCTION()
	void		ModifyOriginalActor();
	/*
	Summon minion related..
	*/		

	TArray< TWeakObjectPtr<AActor> > GetSummonedMinions();

	// Get summoned minion count
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	int32	GetSummonedMinionCount();

	// Add summoned minion
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	CheckDeleteSummonedMinion(FName CheckMinionID);

	// Add summoned minion
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	AddSummonedMinion(AActor* SummonedMinion);

	// Remove summoned minion
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	RemoveSummonedMinion(AActor* SummonedMinion);

	// Remove some summoned minion as much as count.
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	RemoveSummonedMinionAsMuchAsCount(int32 count);

	// Remove all summoned minion
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	RemoveAllSummonedMinion();

	//Remove all summoned minion hp set zero.
	UFUNCTION(BlueprintCallable, Category = CharacterBattleInfo)
	void	SetHPZeroAllSummonedMinion();



	/*
	Getter
	*/

	template <typename T>
	T GetCharacterProperty(ECharacterPropertyTypeEnum PropertyType)
	{
		return CharacterProperty.GetCharacterProperty< T >(PropertyType);
	}

	template <typename T>
	T GetCharacterPropertyMaxValue(ECharacterPropertyTypeEnum PropertyType)
	{
		switch (PropertyType)
		{
		case ECharacterPropertyTypeEnum::VE_HP:
			return GetCharacterFinalProperty<T>(ECharacterPropertyTypeEnum::VE_MaxHP);
		}
		return T();
	}

	// additional property by buff..(exception Passive,Leader)
	template <typename T>
	T GetCharacterAdditionalProperty(ECharacterPropertyTypeEnum PropertyType)
	{
		return AdditionalCharacterProperty.GetCharacterProperty< T >(PropertyType);
	}

	// character item property all
	template <typename T>
	T GetCharacterItemPropertyAllWithCharacter(ECharacterPropertyTypeEnum PropertyType)
	{
		T TotalProperty = GetCharacterProperty<T>(PropertyType);
		// Truncate
		return GetCharacterItemPropertyAll<T>(TotalProperty, PropertyType);
	}

	// character item property all
	template <typename T>
	T GetCharacterItemPropertyAll(T BaseValue, ECharacterPropertyTypeEnum PropertyType)
	{
		int32 ItemStatAdditive = 0;
		for (int32 i = 0; i < (int32)FItemTableInfo::NumItemEquipPositions(); i++)
		{
			if (!ItemStatsByEquipPosition.IsValidIndex(i))
				continue;
			const TArray<int32>& ItemStats = ItemStatsByEquipPosition[i];

			switch (PropertyType)
			{
				case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::PhysicalAttackPower];
					break;
				case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::MagicAttackPower];
					break;
				case ECharacterPropertyTypeEnum::VE_Armor:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::Armor];
					break;
				case ECharacterPropertyTypeEnum::VE_MagicResistance:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::MagicResistance];
					break;
				case ECharacterPropertyTypeEnum::VE_MaxHP:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::MaxHP];
					break;
				case ECharacterPropertyTypeEnum::VE_DodgeValue:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::DodgeValue];
					break;
				case ECharacterPropertyTypeEnum::VE_CriticalValue:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::CriticalValue];
					break;
				case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::CriticalDamageRate_Scale];
					break;
				case ECharacterPropertyTypeEnum::VE_CriticalResistValue:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::CriticalResistValue];
					break;
				case ECharacterPropertyTypeEnum::VE_PierceValue:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::PierceValue];
					break;
				case ECharacterPropertyTypeEnum::VE_AccuracyValue:
					ItemStatAdditive += ItemStats[(int32)EItemStatOption::AccuracyValue];
					break;

				default:
					// No item stat corresponding to the character property; continue.
					break;
			}
		}

		float Value = (float)ItemStatAdditive;
		// (float)ItemStatAdditive + ((float)BaseValue + (float)ItemStatAdditive) * ((float)ItemStatMultiplicative / 1000.); // Item multiplicative stats are per mil

		return (T)Value; // truncate
	}

	// final property( base + additional + Item + Passive)
	template <typename T>
	T GetCharacterNativeTotalProperty(ECharacterPropertyTypeEnum PropertyType)
	{
		switch (PropertyType)
		{
		case ECharacterPropertyTypeEnum::VE_HP:
			return HP;
		}
		
		T TotalProperty = GetCharacterProperty<T>(PropertyType);
		// Truncate
		TotalProperty += GetCharacterItemPropertyAll<T>(TotalProperty, PropertyType);

		//// Add AvatarAbility
		//switch (PropertyType)
		//{
		//case ECharacterPropertyTypeEnum::VE_Armor:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_PhysicalDefence));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_MaxHP:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_MaxHP));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_MagicResistance:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_MagicalDefence));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_CriticalValue:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_CriticalRate));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_CriticalDamageRate_Scale:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_CriticalDamage));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_DodgeValue:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_Dodge));
		//	break;


		//case ECharacterPropertyTypeEnum::VE_PierceValue:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_Pierce));
		//	break;

		//case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_MagicalAttack));
		//	break;

 	//	case ECharacterPropertyTypeEnum::VE_CriticalResistValue:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::cri));
 	//		break;

		//case ECharacterPropertyTypeEnum::VE_MoveSpeed:
		//	TotalProperty = static_cast<T>((float)TotalProperty + GetAvatarAbilityProperty(EAvatarAbilityType::VE_MoveSpeed));
		//	//std::printf("GetCharacterNativeTotalProperty : VE_MoveSpeed(%f) + EAvatarAbilityType::VE_MoveSpeed(%f) \n",
		//	//(float)TotalProperty, GetAvatarAbilityProperty(EAvatarAbilityType::VE_MoveSpeed));
		//	break;
		//}
		return TotalProperty;
	}

	// final property( base + additional + Item + Passive)
	template <typename T>
	T GetCharacterFinalProperty(ECharacterPropertyTypeEnum PropertyType)
	{
		switch (PropertyType)
		{
			case ECharacterPropertyTypeEnum::VE_HP:
				return HP;	

			case ECharacterPropertyTypeEnum::VE_AttackSpeed:
				return CharacterProperty.AttackHitperSecond;
		}

		//Stat Correction
		T TotalProperty = GetCharacterNativeTotalProperty<T>(PropertyType) + GetCharacterAdditionalProperty<T>(PropertyType);
		return TotalProperty;
		
	}

	/*
	Setter
	*/
	// set additional property by buff..
	template <typename T>
	void SetCharacterAdditionalProperty(ECharacterPropertyTypeEnum PropertyType, T Value)
	{
		AdditionalCharacterProperty.SetCharacterProperty< T >(PropertyType, Value);
	}

	// add additional property by buff..
	template <typename T>
	void AddCharacterAdditionalProperty(ECharacterPropertyTypeEnum PropertyType, T Value)
	{
		T NewValue = GetCharacterAdditionalProperty<T>(PropertyType) +Value;
		SetCharacterAdditionalProperty(PropertyType, NewValue);
	}


private:
	TArray< TWeakObjectPtr<AActor> >	SummonedMinionList;
};
