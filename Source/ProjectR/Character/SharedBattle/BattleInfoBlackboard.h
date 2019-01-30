// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SharedConstants/GlobalConstants.h"
#include "Skill_V2/SkillApplierBase.h"
#include "CustomStructures/SharedStructures/ProtectInfo.h"
#include "BattleInfoBlackboard.generated.h"


UENUM(BlueprintType)
enum class EWeakType :uint8
{
	STR, AGI, FOC, CLE, FIRE, ICE, END
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMatrix4x6
{
	GENERATED_USTRUCT_BODY()

public:
	float M[4][6];
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMatrix6x4
{
	GENERATED_USTRUCT_BODY()

public:
	float M[6][4];
};


UCLASS()
class PROJECTR_API UBattleInfoBlackboard : public UObject
{
	GENERATED_BODY()
	
public:
    /*
     Getter
     */
    template <typename T>
	T GetBlackBoardData(EBattleInfoPropertyType PropertyType);
    
    
    
    /*
     Setter
     */
    template <typename T>
	void SetBlackBoardData(EBattleInfoPropertyType PropertyType, T Value);

	template <typename T>
	void AddBlackBoardData(EBattleInfoPropertyType PropertyType, T Value);



	void	AddDamageCalcState(EBattleDamageResultEnum InDamageCalcState);
	TArray<EBattleDamageResultEnum> GetDamageCalcStateInfos();





	/*
	Instant skill attack applier
	*/
	FSkillApplierBase*	GetInstantAttackSkillApplier();
	void				SetInstantAttackSkillApplier(FSkillApplierBase& InSkillApplier);
	void				ClearInstantAttackSkillApplier();

    
	void			SetBlackBoardAsAttackType(EAttackTypeEnum AttackType);
	EAttackTypeEnum GetBlackBoardAsAttackType();

	void			SetBlackBoardSkillAttack(bool InbSkillAttack);
	bool			GetBlackBoardSkillAttack();


	/*
	attack pile up
	*/
	void			SetAttachedPileupSlot(bool bAttached, int32 InMaxCount = 0, float InPercent = 0.0f);
	bool			IsAttachedPileupSlot() { return bAttachedPileupSlot; }
	//	공격횟수 증가 	return 값 : (CurrentAttackCount ==  MaxAttackCount)
	bool			IncreaseAttackCount(AActor* InFromActor);
	void			ResetAttackCountStack(AActor* InFromActor);

	int32			GetCurrentAttackCount() { return CurrentAttackCount; }
	int32			GetMaxAttackCount() { return MaxAttackCount; }

	// Clear
	void ClearBlackBoardData();
	void ClearLastDamageInformations();
	void ClearLastAppliedSkillInformations();
	void ClearDamageCalcStateInfos();

	/*Counter*/
	void CounterInit();
	void SetBlessApplier(FSkillApplierBase InApplier);
	void SetProtectApplier(FSkillApplierBase InApplier);
	FORCEINLINE FMatrix GetBlessProtectMat() { return BlessProtectMat; }
	FORCEINLINE FMatrix6x4 GetWeakProtectMat() { return WeakProtectMat; }
	FORCEINLINE FMatrix4x6 GetBlessWeakMat() { return BlessWeakMat; }
	FORCEINLINE FMatrix GetSpiritPlusMinusMat() { return SpiritPlusMinusMat; }
	FORCEINLINE FSkillApplierBase GetBlessApplier() { return BlessApplier; }
	FORCEINLINE FSkillApplierBase GetProtectApplier() { return ProtectApplier; }
	FORCEINLINE TArray<FSkillApplierBase> GetWeakAppliers() { return WeakApplierArray; }
	FORCEINLINE TArray<FSkillApplierBase> GetSpiritPlusAppliers() { return SpiritPlusApplierArray; }
	FORCEINLINE TArray<FSkillApplierBase> GetSpiritMinusAppliers() { return SpiritMinusApplierArray; }
	FORCEINLINE void SetOwner(AActor* owner) { OwnerCharacter = owner; }
	void SetWeakApplier(FSkillApplierBase InApplier, bool InIsAttach);
	void SetSpiritPlusApplier(FSkillApplierBase InApplier, bool InIsAttach);
	void SetSpiritMinusApplier(FSkillApplierBase InApplier, bool InIsAttach);

//private:

	/* specializations of the member templates outside of the class body.
	//http://stackoverflow.com/questions/5777236/gcc-error-explicit-specialization-in-non-namespace-scope-desperate-for-help
	*/

	template< typename T1>
	T1 GetInternalValue(EBattleInfoPropertyType PropertyType);

    template< typename T1>
	void SetInternalValue(EBattleInfoPropertyType PropertyType, T1 Value);

	template< typename T1>
	void AddInternalValue(EBattleInfoPropertyType PropertyType, T1 Value);
   
    

	
	// SkillInformations
	// Some skills ask some information about last casted skill	
	FName							LastAppliedSkill;
	bool							SkillAppliedCritically = false;

	float							AdditionalDamage		= 0.0f;
	float							AdditionalDamageReduce	= 0.0f;
	float							AdditionalHeal = 0.0f;
	float							AdditionalLifeSteal = 0.0f;
	float							FinalDamage = 0.0f;
	float							ReturnDamage = 0.0f;
	EBattleDamageResultEnum			LastDamageResultType = EBattleDamageResultEnum::VE_None;
	EBattleDamageResultEnum			ForceDamageResultType = EBattleDamageResultEnum::VE_None;
	EBattleDamageResultEnum			LastAttackResultType = EBattleDamageResultEnum::VE_None;
	EAttackTypeEnum					LastAttackType = EAttackTypeEnum::VE_None;
	ECharacterPropertyTypeEnum		LastAttackPropertyType = ECharacterPropertyTypeEnum::VE_None;			// TODO : deprecated..
	TWeakObjectPtr< AActor >		LastAttackCharacter;
	TWeakObjectPtr< AActor >		LastDeadTeamCharacter;

	bool							bSkillAttack = false;
	TArray<EBattleDamageResultEnum> DamageCalcStateInfos;

	/*
	attack pile up
	*/
	bool							bAttachedPileupSlot = false;
	int32							CurrentAttackCount = 0;
	int32							MaxAttackCount = 0;
	float							PileupAddDamagePercent = 0.0f;
	
	/*
	Instant skill attack applier
	*/
	FSkillApplierBase							InstantAttackSkillApplier;


// 	/*
// 	Heal bonus applier
// 	*/
// 	FSkillApplierBase							HealBonusApplier;

	/*
	Vamp attack applier
	*/
	FSkillApplierBase							VampAttackApplier;

	/*
	Reflection damage applier
	*/
	FSkillApplierBase							ReflectionDamageApplier;

private:
	/*Counter Implementation*/
	UPROPERTY()
	FSkillApplierBase BlessApplier;
	UPROPERTY()
	FSkillApplierBase ProtectApplier;
	UPROPERTY()
	TArray<FSkillApplierBase> WeakApplierArray;
	UPROPERTY()
	TArray<FSkillApplierBase> SpiritPlusApplierArray;
	UPROPERTY()
	TArray<FSkillApplierBase> SpiritMinusApplierArray;

	UPROPERTY()
	TWeakObjectPtr< AActor >						OwnerCharacter;

	FMatrix6x4 WeakProtectMat;
	FMatrix4x6 BlessWeakMat;
	FMatrix BlessProtectMat;
	FMatrix SpiritPlusMinusMat;
	

	UPROPERTY()
	UParticleSystemComponent* BlessParticleComponent;
	UPROPERTY()
	UParticleSystemComponent* ProtectParticleComponent;
	
	/**/

};

/*
Getter
*/
template <typename T>
T UBattleInfoBlackboard::GetBlackBoardData(EBattleInfoPropertyType PropertyType)
{
	return GetInternalValue<T>(PropertyType);
}

template< typename T1>
T1 UBattleInfoBlackboard::GetInternalValue(EBattleInfoPropertyType PropertyType)
{
	return T1();
}



/*
Setter
*/
template <typename T>
void UBattleInfoBlackboard::SetBlackBoardData(EBattleInfoPropertyType PropertyType, T Value)
{
	SetInternalValue<T>(PropertyType, Value);
}

template< typename T1>
void UBattleInfoBlackboard::SetInternalValue(EBattleInfoPropertyType PropertyType, T1 Value)
{

}

template <typename T>
void UBattleInfoBlackboard::AddBlackBoardData(EBattleInfoPropertyType PropertyType, T Value)
{
	AddInternalValue<T>(PropertyType, Value);
}

template< typename T1>
void UBattleInfoBlackboard::AddInternalValue(EBattleInfoPropertyType PropertyType, T1 Value)
{

}




