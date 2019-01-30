// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/DamageResultRate.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Table/SkillUnlockLevelTableInfo.h"
#include "Table/BattleObjectTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "CustomStructures/BattleObjectInfo.h"
#include "Network/PacketDataStructures.h"
#include "SharedConstants/BattleLogicConstants.h"

#include "Renewal_BattleFunctionLibrary.generated.h"

struct FSkillTableInfo;
struct FSkillTableInfo_V2;
struct FBuffTableInfo;
class UISkill;
class UCharacterBattleInfo;
class IEntityBaseProperty;
class URGameInstance;

//UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBattleProbabiltyType : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Critical					UMETA(DisplayName = "Critical"),
	VE_Pierce					UMETA(DisplayName = "Pierce"),
	VE_Parry					UMETA(DisplayName = "Parry"),
	VE_PhysicalDefenceRate		UMETA(DisplayName = "PhysicalDefenceRate"),
	VE_MagicDefenceRate			UMETA(DisplayName = "MagicDefenceRate"),
};


/**
*
*/
UCLASS()
class PROJECTR_API URenewal_BattleFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	URenewal_BattleFunctionLibrary() {};

	static void InitBattleLogicConstants(FBattleLogicConstants& BattleLogicConstants);

	/************************************************************************/
	/* Renewal Damage Function												*/
	/************************************************************************/
	static int32 GetFinalDamageValue(AActor* Offender, AActor* Deffender, EBattleDamageResultEnum& DamageResultEnum, bool bPhysicalAttack, bool bSkillAttack);
	static int32 GetFinalDamageByAttackPower(AActor* Offender, AActor* Deffender, EBattleDamageResultEnum& DamageResultEnum, bool bPhysicalAttack);
	static bool DistinguishBetweenHitAndAvoid(AActor* Offender, AActor* Deffender);
	static bool IsCriticalHit(AActor* Offender, AActor* Deffender, bool bPhysicalAttack);
// 	static bool IsPierceHit(AActor* Offender, AActor* Deffender);
	static bool IsParryHit(AActor* Offender, AActor* Deffender);
	static float GetDefenceRate(AActor* Offender, AActor* Deffender, bool bPhysicalAttack);
	static bool GetRateDecision(const float TargetRate);
	static bool IsPhysicalAttack(AActor* TargetActor);
	static bool IsRangeAttack(AActor* TargetActor);
// 	static bool						IsBackAttack(AActor* Offender, AActor* Defender);	


	template<typename T>
	static void						ModifyNegativeNumberToZero(T& Number);

	static UParticleSystemComponent* ParametersUsingParticle(UParticleSystem* TargetEffect, AActor* TargetCharacter);


	//=================================================================================================================================
	// Friend and foe
	//=================================================================================================================================
	static bool						IsEnemy(int32 playerUK, int32 playerGroupKey, int32 otherUK, int32 otherGroupKey);
	static bool						IsAlly(int32 playerUK, int32 playerGroupKey, int32 otherUK, int32 otherGroupKey);

	static bool						IsAllyActor(AActor* firstActor, AActor* secondActor);
	static bool						IsPartyActor(AActor* firstActor, AActor* secondActor);
	static bool						IsEnemyActor(AActor* firstTeam, AActor* secondTeam);

	static bool						IsEnemyAgainstCurrentPlayer(AActor* TargetActor);

	static bool						IsControlCharacter(AActor* TargetActor);

	static bool						IsBossActor(AActor* TargetActor);

	static bool						IsProtectTarget(AActor* InTargetActor);

	static bool						IsRaidAkhan(AActor* TargetActor);

	//=================================================================================================================================
	// Table getter
	//=================================================================================================================================

	static FCharacterTableInfo*		GetCharacterTableInfo(ECharacterCategoryTypeEnum	CharacterCategory, FName RowName);
	static bool						GetCharacterTableWorkedInfo(FWorkedCharacterTableInfo& OutWorkedInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level);

	static void						SetCharacterBattleInfoFromTable(UCharacterBattleInfo* CharacterBattleInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level);
	static void						SetCharacterBattleInfo(UCharacterBattleInfo* CharacterBattleInfo, const FWorkedCharacterTableInfo& WorkedTableInfo);



	// Skill table info	
	static FSkillTableInfo_V2*			GetSkillTableInfo_V2(ECharacterCategoryTypeEnum CharacterCategory, FName RowName);
//	static FSkillUnlockLevelTableInfo*	GetSkillUnlocLevelTableInfo(FName RowName);
	static bool							IsMaxSkillLevel(int32 InSkillLevel);
	static bool							GetSkillTableWorkedInfo(FWorkedSkillTableInfo& OutSkillInfo, ECharacterCategoryTypeEnum CharacterCategory, FName RowName, int32 Level);	

	static int32						GetNpcSkillLevel(int32 InNpcLevel);

	static float						GetTargetDegree(AActor* Offender, AActor* Defender);
	static float						GetTargetDegree(FVector InSource, FVector InDest, FVector InFoward);
	static ECharacterDirectionEnum		GetTargetDirection(AActor* Offender, AActor* Defender);

	//=================================================================================================================================
	//Item Table Info
	//=================================================================================================================================
	static FBattleObjectTableInfo*	GetBattleObjectTableInfo(FName RowName);

	static void						SetBattleObjectInfoFromTable(UCharacterBattleInfo* CharacterBattleInfo, FBattleObjectInfo& BattleObjectInfo, FName RowName, int32 level = 1);
	static float					GetHeroEnchantPoint(EClassTypeEnum ClassTypeEnum, EHERO_ENCHANT_STAT_SLOT eEnchantStateSlot, int32 point);
	//static bool						SetCharacterItemBattleInfoFromTable(const FWorkedCharacterTableInfo& EquippedCharacterWorkedTableInfo, FCharacterBattleProperty& CharacterBattleProperty, FName RowName, TArray<int32>& Options, int32 EnchantValue = 100, int32 EnchantLevel = 0);
	static bool						SetCharacterItemBattleInfoFromTable(TArray<int32>& OutItemStatOptions, const FITEM& ItemData);
	static void						SetCharacterItemBattleInfo(FCharacterBattleProperty& CharacterBattleProperty, FWorkedItemTableInfo& WorkedTableInfo);

//	static void DebugPrintItemStatOptions(FString LabelText, const TArray<int32>& ItemStatOptions);

	static FVector					GetFloorPosition(UWorld* World, FVector InLocation, float fDist = 200.0f, AActor* IgnoreActor = nullptr);

	static float					GetCustomizedSightRange(UWorld* InWorld, UCharacterBattleInfo* CharacterBattleInfo, bool InIsConstantValue = false);


	//=================================================================================================================================
	// Make battle info
	//=================================================================================================================================
	static UCharacterBattleInfo*	MakeCharacteBattleInfoFromInven(FString HeroKey, bool InNextUpgrade = false, int32 InNextLevel = 0);
	static UCharacterBattleInfo*	MakeTargetCharacteBattleInfoFromInven(FBATTLE_USER& MatchList, FString& HeroKey);
	static UCharacterBattleInfo*	MakeCharacteBattleInfoFromEquipedHero(FEQUIPED_HERO& hero);
	static UCharacterBattleInfo*	MakeBObjectBattleInfoFromTable(FName key, int32 Level);

	static ECharacterPropertyTypeEnum GetCharacterPropertyType(FString& InValue);
	static void SetAvatarPointByJobClass(EClassTypeEnum JobClass, TArray<int32>& PointByJobClass, TArray<int32>& AllPoints);
	static void SetAvatarAbilityProperty(UCharacterBattleInfo* CharBattleInfo, TArray<int32>& AbilityPoints);

	static EPLAY_CLEAR_GRADE GetCampaignStageResultType(UObject* WorldContextObject);

	static void GetDeadHeroAndAliveHeroCount(UObject* WorldContextObject, int32& DeadHeroCount, int32& AliveHEroCount);

	//=================================================================================================================================
	// Search functions	
	//=================================================================================================================================
	static bool	IsPreferType(AActor* Target, EPriorityTypeEnum PriorityType);
	static bool IsAISearchTargetType(AActor* Target, EPriorityTypeEnum PriorityType);
	/*
	Search actors
	*/
	// find targets that is satified with some conditions
	/*
	TArray<TWeakObjectPtr< AActor >>& outArray, 
	AActor* Source, 
	FVector targetPos, 
	EFriendshipTypeEnum friendshipType, 
	ESearchShapeTypeEnum applyType  : ex) VE_Circle, VE_Secotr, VE_Box
	FVector value0					: ex) if VE_Cirelc, fill the x component.		
	float rotate = 0.0f, 
	bool bDrawDebug = false
	*/
	static void FindTarget(TArray< TWeakObjectPtr<AActor> >& outArray, AActor* Source, FVector targetPos, EFriendshipTypeEnum friendshipType, ESearchShapeTypeEnum applyType, FVector value0, float rotate = 0.0f, bool bDrawDebug = false);
	// find enemies that is satified with some conditions
	static void	FindFriendshipType(TArray< TWeakObjectPtr<AActor> >& outArray, AActor* Source, EFriendshipTypeEnum friendshipType);



	// find enemies that is satified with some conditions
	static AActor*	FindEnemy(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float range, EEnemySearchTargetTypeEnum FindFlag);
	//only healer
	static AActor*	SearchFriend(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float Range, float RangeThreshold, EPriorityTypeEnum PriorityType, EPriorityTypeEnum InAISearchPreferType);
	// search one enemy
	static AActor*	SearchEnemy(AActor* Target, FVector Origin, ESearchShapeTypeEnum ShapeType, float Range, float RangeThreshold, EPriorityTypeEnum PriorityType, EPriorityTypeEnum InAISearchPreferType);

	static bool KeepSafeRule(AActor* InOwner, AActor* InTarget);

	static bool ExceptionEnemyCondition(AActor* Caster, AActor* Target);

	// is in same floor height
	static bool	IsSameFloor(AActor* InTarget01, AActor* InTarget02, float InThreshold);

	static float	GetSquaredDistanceWithBoundingRadius(AActor* Target01, AActor* Target02);

	static bool		CheckTargetDecalNoti(AActor* InTarget);
	static void		ComputeTargetDecal(AActor* InTarget, bool InVisible);
	static void		VisibleCharacterIndicatorMesh(AActor* InTarget, bool InVisible);
	//static void		VisibleAttackAreaIndicator(AActor* InActor, bool InVisible, float InScale = 100);

	static FVector	FindHitPoint(AActor* From, AActor* Target);

	//=================================================================================================================================
	// State related
	//=================================================================================================================================
	//static EAbonormalStateFlagEnum GetStateFlagBySlotType(EAbnormalSlotTypeEnum InSlotType);

};