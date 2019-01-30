// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedGlobalConstants.generated.h"


#define ENUM_TO_BITFIELD(x)	(1<<(x))


//=====================================================================================================
// Character
//=====================================================================================================
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterCategoryTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Hero 		UMETA(DisplayName = "Hero"),
	VE_Minion 		UMETA(DisplayName = "Minion"),
	VE_Npc			UMETA(DisplayName = "Npc"),
	VE_Object		UMETA(DisplayName = "Object"),
	VE_UICharacter	UMETA(DisplayName = "UICharacter"),
	VE_NeutralNpc	UMETA(DisplayName = "NeutralNpc"),
	VE_BattleObject	UMETA(DisplayName = "BattleObject"),
};

UENUM(BlueprintType)
enum class EContinentTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	Luna				UMETA(DisplayName = "Luna"),
	Lux					UMETA(DisplayName = "Lux"),
	Nox					UMETA(DisplayName = "Nox"),
	Solona				UMETA(DisplayName = "Solona"),
	VE_Max				UMETA(DisplayName = "ContinentMax"),
};

UENUM(BlueprintType)
enum class ECounterType :uint8
{
	SOLAR		UMETA(DisplayName = "Solar"),
	LIGHT		UMETA(DisplayName = "Light"),
	DARK		UMETA(DisplayName = "Dark"),
	MOON		UMETA(DisplayName = "Moon"),
	END			UMETA(DisplayName = "End"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAttackTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Magic 			UMETA(DisplayName = "Magic"),
	VE_Melee			UMETA(DisplayName = "Melee"),
	VE_Range 			UMETA(DisplayName = "Range"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EClassTypeEnum : uint8
{	
	VE_Tanker		UMETA(DisplayName = "Tanker"),
	VE_MeleeDealer 	UMETA(DisplayName = "MeleeDealer"),
	VE_RangeDealer 	UMETA(DisplayName = "RangeDealer"),
	VE_Healer		UMETA(DisplayName = "Healer"),
	VE_Buffer		UMETA(DisplayName = "Buffer"),
	VE_Supporter	UMETA(DisplayName = "Supporter"),
	VE_MagicDealer	UMETA(DisplayName = "MagicDealer"),
	VE_ClassTypeMax	UMETA(DisplayName = "ClassTypeMax"),
};

UENUM(BlueprintType)
enum class ECharacterPropertyTypeEnum : uint8
{
	VE_None								UMETA(DisplayName = "None"),
//	attack value
	VE_AttackPower						UMETA(DisplayName = "AttackPower"),
	VE_PhysicalAttackPower				UMETA(DisplayName = "PhysicalAttackPower"),
	VE_MagicAttackPower					UMETA(DisplayName = "MagicAttackPower"),
	VE_AccuracyValue					UMETA(DisplayName = "AccuracyValue"),
	VE_PierceValue						UMETA(DisplayName = "PierceValue"),
	VE_AttackSpeed						UMETA(DisplayName = "AttackSpeed"),
	VE_CriticalValue					UMETA(DisplayName = "CriticalValue"),
	VE_CriticalDamageRate_Scale			UMETA(DisplayName = "CriticalDamageRate_Scale"),
	VE_AttackRange						UMETA(DisplayName = "AttackRange"),

//	defence value
	VE_Armor							UMETA(DisplayName = "Armor"),
	VE_CriticalResistValue				UMETA(DisplayName = "CriticalResistValue"),
	VE_MagicResistance					UMETA(DisplayName = "MagicResistance"),
	VE_DodgeValue						UMETA(DisplayName = "DodgeValue"),
	VE_MaxHP 							UMETA(DisplayName = "MaxHP"),
	VE_HP 								UMETA(DisplayName = "Hp"), 


//	etc value
	VE_MoveSpeed						UMETA(DisplayName = "MoveSpeed"),
	VE_SightRange						UMETA(DisplayName = "SightRange"),

//	temp
	VE_AttackType						UMETA(DisplayName = "AttackType"),
	VE_AIPreferType						UMETA(DisplayName = "AIPreferType"),
	VE_AttackPropertyType				UMETA(DisplayName = "AttackPropertyType"),
	VE_UnitRadius						UMETA(DisplayName = "UnitRadius"),
	VE_MaxLevel							UMETA(DisplayName = "MaxLevel"),
};

UENUM(BlueprintType)
enum class ENormalAttackTypeEnum : uint8
{
	VE_NormalAttack01	UMETA(DisplayName = "NormalAttackAttack01"),
	VE_NormalAttack02	UMETA(DisplayName = "NormalAttackAttack02"),
	VE_NormalAttack03	UMETA(DisplayName = "NormalAttackAttack03"),
	VE_NormalAttack04	UMETA(DisplayName = "NormalAttackAttack04"),
	VE_NormalAttack05	UMETA(DisplayName = "NormalAttackAttack05"),

	VE_NormalAttackNone	UMETA(DisplayName = "NormalAttackAttackNone"),
};

UENUM(BlueprintType)
enum class ESpawnTypeEnum : uint8
{
	VE_SpawnNone		UMETA(DisplayName = "SpawnNone"),
	VE_Spawn01			UMETA(DisplayName = "Spawn01"),
	VE_Spawn02			UMETA(DisplayName = "Spawn02"),
	VE_Spawn03			UMETA(DisplayName = "Spawn03"),
	VE_Spawn04			UMETA(DisplayName = "Spawn04"),
	VE_Spawn_Effect		UMETA(DisplayName = "Spawn Effect"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEntityTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Normal 			UMETA(DisplayName = "Normal"),
	VE_RemoteEntity		UMETA(DisplayName = "RemoteEntity"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ENetNormalAttackStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Attack_Start 	UMETA(DisplayName = "Attack_Start"),
	VE_Attack_Doing 	UMETA(DisplayName = "Attack_Doing"),
	VE_Attack_End 		UMETA(DisplayName = "Attack_End"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETeamNO : uint8
{
	VE_None		UMETA(DisplayName = "None"),
	VE_TEAM_0 	UMETA(DisplayName = "Team_0"),
	VE_TEAM_1 	UMETA(DisplayName = "Team_1"),

	VE_NEUTRALITY				UMETA(DisplayName = "Neutrality"),			// Enemy NPC
	VE_CURRENT_PLAYER			UMETA(DisplayName = "CurrentPlayer"),		// Help NPC
};

UENUM(BlueprintType)
enum class EPriorityTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),	
	VE_Tanker			UMETA(DisplayName = "Tanker"),
	VE_MeleeDealer		UMETA(DisplayName = "MeleeDealer"),
	VE_RangeDealer		UMETA(DisplayName = "RangeDealer"),
	VE_MagicDealer		UMETA(DisplayName = "MagicDealer"),
	VE_Supporter		UMETA(DisplayName = "Supporter"),
	VE_LowestHP			UMETA(DisplayName = "LowestHP"),
	VE_HighestHP		UMETA(DisplayName = "HighestHP"),
	VE_Object			UMETA(DisplayName = "Object"),
	VE_ProtectTarget	UMETA(DisplayName = "ProtectTarget"),
	VE_Far				UMETA(DisplayName = "Far"),
	VE_Near				UMETA(DisplayName = "Near"),
};

UENUM(BlueprintType)
enum class ECharacterSituationState : uint8
{
	VE_Ground				UMETA(DisplayName = "Ground"),
	VE_ProgressGround		UMETA(DisplayName = "ProgressGround"),
	VE_Air 					UMETA(DisplayName = "Air"),
	VE_ProgressAir			UMETA(DisplayName = "ProgressAir"),
	VE_AirHigh				UMETA(DisplayName = "AirHigh"),
	VE_ProgressAirHigh		UMETA(DisplayName = "ProgressAirHigh"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterDirection4Enum : uint8
{
	VE_FrontRight  		UMETA(DisplayName = "FrontRight"),
	VE_BackRight  		UMETA(DisplayName = "BackRight"),
	VE_BackLeft 		UMETA(DisplayName = "BackLeft"),
	VE_FrontLeft 		UMETA(DisplayName = "FrontLeft")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterDirectionEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Front  		UMETA(DisplayName = "Front"),
	VE_Back 		UMETA(DisplayName = "Back"),
	VE_Side 		UMETA(DisplayName = "Side"),
	VE_Left 		UMETA(DisplayName = "Left"),
	VE_Right 		UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterDirection4REnum : uint8
{
	VE_Front  		UMETA(DisplayName = "Front"),
	VE_Right  		UMETA(DisplayName = "Right"),
	VE_Back 		UMETA(DisplayName = "Back"),
	VE_Left 		UMETA(DisplayName = "Left"),
	VE_End			UMETA(DisplayName = "End")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBattleObjectTypeEnum : uint8
{
	VE_Ignit			UMETA(DisplayName = "Ignit"),
	VE_Poison			UMETA(DisplayName = "Poison"),
	VE_Speed			UMETA(DisplayName = "Speed"),
	VE_Heal				UMETA(DisplayName = "Heal"),
	VE_Summon			UMETA(DisplayName = "Summon"),

	//

	VE_None				UMETA(DisplayName = "None"),
	VE_FloorType 		UMETA(DisplayName = "FloorType"),
	VE_CannonType 		UMETA(DisplayName = "CannonType"),
	VE_TotemType 		UMETA(DisplayName = "TotemType"),
	VE_TotemTickType 	UMETA(DisplayName = "TotemTickType"),
	VE_GimmickType		UMETA(DisplayName = "GimmickType"),
	VE_TrapType			UMETA(DisplayName = "TrapType"),
	VE_RaidGigantType	UMETA(DisplayName = "RaidGigantType"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterStateTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Idle				UMETA(DisplayName = "Idle"),
	VE_Moving 			UMETA(DisplayName = "Moving"),
	VE_Attacking 		UMETA(DisplayName = "Attacking"),
	VE_SkillCasting 	UMETA(DisplayName = "SkillCasting"),
	VE_SkillActing 		UMETA(DisplayName = "SkillActing"),

};

UENUM(BlueprintType)
enum class ECharacterRoleEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Leader			UMETA(DisplayName = "Leader"),
	VE_Crew1			UMETA(DisplayName = "Crew1"),
	VE_Crew2			UMETA(DisplayName = "Crew2"),
	VE_Boss				UMETA(DisplayName = "Boss"),
	VE_Neutral			UMETA(DisplayName = "Neutral"),
};

UENUM(BlueprintType)
enum class ECharacterCrewStateEnum : uint8 
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Battle			UMETA(DisplayName = "Battle"),
	VE_Rest				UMETA(DisplayName = "Rest"),
};


//=====================================================================================================
// Skill
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillAniTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),

	VE_SkillAni01 	UMETA(DisplayName = "SkillAni01"),
	VE_SkillAni02 	UMETA(DisplayName = "SkillAni02"),
	VE_SkillAni03 	UMETA(DisplayName = "SkillAni03"),
	VE_SkillAni04 	UMETA(DisplayName = "SkillAni04"),
	VE_SkillAni05 	UMETA(DisplayName = "SkillAni05"),
	VE_SkillAni06 	UMETA(DisplayName = "SkillAni06"),
	VE_SkillAni07 	UMETA(DisplayName = "SkillAni07"),
	VE_SkillAni08 	UMETA(DisplayName = "SkillAni08"),
	VE_SkillAni09 	UMETA(DisplayName = "SkillAni09"),
	VE_SkillAni10 	UMETA(DisplayName = "SkillAni10"),

	VE_SkillAni11 	UMETA(DisplayName = "SkillAni11"),
	VE_SkillAni12 	UMETA(DisplayName = "SkillAni12"),
	VE_SkillAni13 	UMETA(DisplayName = "SkillAni13"),
	VE_SkillAni14 	UMETA(DisplayName = "SkillAni14"),
	VE_SkillAni15 	UMETA(DisplayName = "SkillAni15"),
	VE_SkillAni16 	UMETA(DisplayName = "SkillAni16"),
	VE_SkillAni17 	UMETA(DisplayName = "SkillAni17"),
	VE_SkillAni18 	UMETA(DisplayName = "SkillAni18"),
	VE_SkillAni19 	UMETA(DisplayName = "SkillAni19"),
	VE_SkillAni20 	UMETA(DisplayName = "SkillAni20"),
};

UENUM(BlueprintType)
enum class ESkillStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Casting 			UMETA(DisplayName = "Casting"),
	VE_DoAction 		UMETA(DisplayName = "Acting"),
	VE_Channeling		UMETA(DisplayName = "Channeling"),
	VE_Fire 			UMETA(DisplayName = "Fire"),
	VE_End 				UMETA(DisplayName = "End"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillCastingTypeEnum : uint8
{
	VE_None							UMETA(DisplayName = "None"),
	VE_Immediate 					UMETA(DisplayName = "Immediate"),
	VE_ImmediateAction 				UMETA(DisplayName = "ImmediateAction"),
	VE_ImmediateActionRotate		UMETA(DisplayName = "ImmediateActionRotate"),
	VE_ColdWave						UMETA(DisplayName = "ColdWave"),
	VE_CounterAttack				UMETA(DisplayName = "CounterAttack"),
	VE_Summon						UMETA(DisplayName = "Summon"),
	VE_Blackhole					UMETA(DisplayName = "Blackhole"),
	VE_PenetrateBullet				UMETA(DisplayName = "PenetrateBullet"),
	VE_CastingBattleObject			UMETA(DisplayName = "CastingBattleObject"),
	VE_Teleport						UMETA(DisplayName = "Teleport"),
	VE_OnlyAnimation				UMETA(DisplayName = "OnlyAnimation"),
	VE_OnlyAnimationEnd				UMETA(DisplayName = "OnlyAnimationEnd"),
	VE_NoCollision					UMETA(DisplayName = "NoCollision"),
	VE_CommandingShout				UMETA(DisplayName = "CommandingShout"),
	VE_Guts							UMETA(DisplayName = "Guts"),
	VE_Regeneration					UMETA(DisplayName = "Regeneration"),
	VE_Doppelganger					UMETA(DisplayName = "Doppelganger"),
	VE_LoopAction					UMETA(DisplayName = "LoopAction"),
	VE_Sacrifice					UMETA(DisplayName = "Sacrifice"),
	VE_Invincible					UMETA(DisplayName = "Invincible"),
	VE_IgnoreAbnormal				UMETA(DisplayName = "IgnoreAbnormal"),
	VE_Casting						UMETA(DisplayName = "Casting"),
	VE_Channeling					UMETA(DisplayName = "Channeling"),
};

UENUM(BlueprintType)		// Casting target type
enum class ESkillTargetTypeEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Me 						UMETA(DisplayName = "Me"),
	VE_Target 					UMETA(DisplayName = "Target"),
	VE_TargetLocation 			UMETA(DisplayName = "TargetLocation"),
};

//UENUM(BlueprintType)		// Apply target type
//enum class ESkillApplyTargetTypeEnum : uint8
//{
//	VE_None						UMETA(DisplayName = "None"),	
//	VE_Friend					UMETA(DisplayName = "Friend"),
//	VE_Enemy 					UMETA(DisplayName = "Enemy"),
//	VE_FriendExceptMe			UMETA(DisplayName = "FriendExceptMe"),
//	VE_MySummon					UMETA(DisplayName = "MySummon"),
//	VE_FriendMagic				UMETA(DisplayName = "FriendMagic"),
//	VE_FriendPhysical			UMETA(DisplayName = "FriendPhysical"),
//
//	VE_Alone					UMETA(DisplayName = "Alone"),
//};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillBulletTypeEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Immediate 				UMETA(DisplayName = "Immediate"),
	VE_Bullet 					UMETA(DisplayName = "Bullet"),
	VE_MultiBullet 				UMETA(DisplayName = "MultiBullet"),
	VE_LocationBullet 			UMETA(DisplayName = "LocationBullet"),
	VE_BeamBullet				UMETA(DisplayName = "BeamBullet"),
	VE_CurveLocationBullet		UMETA(DisplayName = "CurveLocationBullet"),
	VE_HomingBullet				UMETA(DisplayName = "HomingBullet"),
	VE_LaserBullet				UMETA(DisplayName = "LaserBullet"),
	VE_ContinueBullet			UMETA(DisplayName = "ContinueBullet"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESearchShapeTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Circle 		UMETA(DisplayName = "Circle"),
	VE_Sphere 		UMETA(DisplayName = "Sphere"),
	VE_Box	 		UMETA(DisplayName = "Box"),
	VE_Sector 		UMETA(DisplayName = "Sector"),
	VE_Cylinder 	UMETA(DisplayName = "Cylinder"),
	VE_Box2D 		UMETA(DisplayName = "Box2D"),
	VE_Donut		UMETA(DisplayName = "Donut"),
	VE_Sector2D 	UMETA(DisplayName = "VE_Sector2D"),
	//
	VE_Line
};

UENUM(BlueprintType)
enum class EBattleInfoPropertyType : uint8
{
	VE_None							UMETA(DisplayName = "None"),
	VE_LastAppliedSkillTargets		UMETA(DisplayName = "LastAppliedSkillTargets"),
	VE_LastAppliedSkill				UMETA(DisplayName = "LastAppliedSkill"),
	VE_SkillAppliedCritically		UMETA(DisplayName = "SkillAppliedCritically"),
	VE_FinalDamage					UMETA(DisplayName = "FinalDamage"),
	VE_LastDamageResultType			UMETA(DisplayName = "LastDamageResultType"),
	VE_LastAttackType				UMETA(DisplayName = "LastAttackType"),

	VE_BattleInfoPropertyTypeMax,
	//==========================================================

	VE_LastDamageCritical			UMETA(DisplayName = "LastDamageCritical"),
	VE_LastDamageMiss				UMETA(DisplayName = "LastDamageMiss"),
	VE_LastDamageDodge				UMETA(DisplayName = "LastDamageDodge"),
	VE_LastDamageParry				UMETA(DisplayName = "LastDamageParry"),
	VE_MeleeAttackType				UMETA(DisplayName = "MeleeAttackType"),
	VE_RangeAttackType				UMETA(DisplayName = "RangeAttackType"),
	VE_LastAttackCritical			UMETA(DisplayName = "LastAttackCritical"),

	//_V2
	VE_AdditionDamagePercent		UMETA(DisplayName = "AdditionDamagePercent"),
	VE_AdditionDamageValue			UMETA(DisplayName = "AdditionDamageValue"),
};

UENUM(BlueprintType)
enum class ESkillCategoryTypeEnum : uint8
{
	VE_ActiveSkill01		UMETA(Displayname = "ActiveSkill01"),
	VE_ActiveSkill02		UMETA(Displayname = "ActiveSkill02"),
	VE_ActiveSkill03		UMETA(Displayname = "ActiveSkill03"),
	VE_ActiveSkill04		UMETA(Displayname = "ActiveSkill04"),
	VE_PassiveSkill01		UMETA(Displayname = "PassiveSkill01"),
	VE_PassiveSkill02		UMETA(Displayname = "PassiveSkill02"),
	VE_Spirit01				UMETA(Displayname = "Spirit01"),
	VE_Spirit02				UMETA(Displayname = "Spirit02"),
	VE_Spirit03				UMETA(Displayname = "Spirit03"),
	VE_Spirit04				UMETA(Displayname = "Spirit04"),
	VE_MaxSkillCount		UMETA(Displayname = "MAxSkillCount"),
	VE_ALL					UMETA(Displayname = "All"),
	VE_None					UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class ESkillControlTypeEnum : uint8
{
	VE_Auto				UMETA(Displayname = "Auto"),
	VE_Active			UMETA(Displayname = "Active"),
	VE_Passive			UMETA(Displayname = "Passive"),
	VE_Aura				UMETA(Displayname = "Aura"),
	VE_None				UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class ESkillAttributeEnum : uint8
{
	VE_Physical			UMETA(Displayname = "Physical"),
	VE_Magical			UMETA(Displayname = "Magical"),
	VE_None				UMETA(Displayname = "None"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ENotiApplySkillType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_NormalSkill			UMETA(DisplayName = "NormalSkill"),
	VE_ChannelSkillAct		UMETA(DisplayName = "ChannelSkillAct"),
	VE_ChannelSkillEnd		UMETA(DisplayName = "ChannelSkillEnd"),
	VE_PassiveSkill			UMETA(DisplayName = "PassiveSkill"),
	VE_ContinuousSkill		UMETA(DisplayName = "ContinuousSkill"),
};

enum class EAbonormalStateFlagEnum
{
	BM_None = 0				UMETA(DisplayName = "0.NONE"),

	//Animation related..
	BM_Stun = (1 << 0)			UMETA(DisplayName = "1.Stun"),
	BM_Down = (1 << 1)			UMETA(DisplayName = "2.Down"),
	BM_Frozen = (1 << 2)		UMETA(DisplayName = "3.Frozen"),

	BM_Silence = (1 << 3)		UMETA(DisplayName = "4.Silence"),
	BM_Hold = (1 << 4)			UMETA(DisplayName = "5.Hold"),
	BM_Hide = (1 << 5)			UMETA(DisplayName = "6.Hide"),

	BM_Fear = (1 << 6)			UMETA(DisplayName = "7.Fear"),
	BM_Airborne = (1 << 7)		UMETA(DisplayName = "8.Airborne"),
	BM_KnockBack = (1 << 8)		UMETA(DisplayName = "9.KnockBack"),
	BM_Push = (1 << 9)			UMETA(DisplayName = "10.Push"),
	BM_Captivate = (1 << 10)	UMETA(DisplayName = "11.Captivate"),
	BM_Pull = (1 << 11)			UMETA(DisplayName = "12.Pull"),
};
ENUM_CLASS_FLAGS(EAbonormalStateFlagEnum)

UENUM(BlueprintType)
enum class EAbnormalApplyTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Buff				UMETA(DisplayName = "Buff"),
	VE_DeBuff			UMETA(DisplayName = "DeBuff"),
};

UENUM(BlueprintType)
enum class EAbnormalGroupTypeEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),

	VE_Group_Invincible			UMETA(DisplayName = "Group_Invincible"),
	VE_Group_SemiInvincible		UMETA(DisplayName = "Group_SemiInvincible"),
	VE_Group_StatUp				UMETA(DisplayName = "Group_StatUp"),
	VE_Group_HOT				UMETA(DisplayName = "Group_HOT"),

	VE_Group_Helplessness		UMETA(DisplayName = "Group_Helplessness"),
	VE_Group_SemiHelplessness	UMETA(DisplayName = "Group_SemiHelplessness"),
	VE_Group_StatDown			UMETA(DisplayName = "Group_StatDown"),
	VE_Group_DOT_Poison			UMETA(DisplayName = "Group_DOT_Position"),
	VE_Group_DOT_Bleeding		UMETA(DisplayName = "Group_DOT_Bleeding"),
	VE_Group_DOT_Fire			UMETA(DisplayName = "Group_DOT_Fire"),
	VE_Group_DOT_Ignite			UMETA(DisplayName = "Group_DOT_Ignite"),
	VE_Group_Mark_Weapon		UMETA(DisplayName = "Group_Mark_Weapon"),

	VE_Group_Aura				UMETA(DisplayName = "Group_Aura"),

	VE_Max,
};

UENUM(BlueprintType)
enum class EAbnormalSlotTypeEnum : uint8
{
	VE_None,

	/*Invincibility Start*/
	VE_Slot_Invincible,
	VE_Slot_Invincible_Akhan,
	/*Invincibility End*/


	/*Immune Start*/
	VE_Slot_PhysicalImmun,
	VE_Slot_MagicalImmun,
	VE_Slot_HelplessnessImmun,
	VE_Slot_DebuffImmun,
	VE_Slot_InstantDamageImmun,
	/*Immune End*/

	/*Hide Start*/
	VE_Slot_Hide,
	/*Hide End*/
	
	/*Berserker Start*/
	VE_Slot_Berserker,
	/*Berserker End*/


	/*Shield Start*/
	VE_Slot_DamageShield,
	VE_Slot_Damage_IceShield,
	/*Shield End*/

	/*Abnormal - Crowd Control Start*/
	VE_Slot_Stun,
	VE_Slot_Down,
	VE_Slot_Fear,
	VE_Slot_Captivate,
	VE_Slot_Freezing,
	VE_Slot_Airborne,
	VE_Slot_Hold,
	VE_Slot_Blindness,
	VE_Slot_Silence,
	VE_Slot_SilenceMagical,
	VE_Slot_SilencePhysical,
	VE_Slot_Provoke,
	/*Abnormal - Crowd Control End*/

	/*Stat Up/Down Start*/
	VE_Slot_StatUpPhysicalAttack,
	VE_Slot_StatUpMagicalAttack,
	VE_Slot_StatUpPhysicalDefence,
	VE_Slot_StatUpMagicalDefence,
	VE_Slot_StatUpAccuracy,
	VE_Slot_StatUpHit,
	VE_Slot_StatUpAvoid,
	VE_Slot_StatUpAttackSpeed,
	VE_Slot_StatUpMoveSpeed,
	VE_Slot_StatUpCriticalRate,
	VE_Slot_StatUpCriticalDamage,
	VE_Slot_StatUpPierceValue,
	VE_Slot_StatUpMaxHP,
// 	VE_Slot_StatUpParryValue,
// 	VE_Slot_StatUpCriticalResistValue,
// 	VE_Slot_StatUpCriticalResistDamage,
// 	VE_Slot_StatUpMagicCriticalRate,

	VE_Slot_StatDownPhysicalAttack,
	VE_Slot_StatDownMagicalAttack,
	VE_Slot_StatDownPhysicalDefence,
	VE_Slot_StatDownMagicalDefence,
	VE_Slot_StatDownAccuracy,
	VE_Slot_StatDownAvoid,
	VE_Slot_StatDownAttackSpeed,
	VE_Slot_StatDownMoveSpeed,
	VE_Slot_StatDownCriticalRate,
	VE_Slot_StatDownCriticalDamage,
	VE_Slot_StatDownPierceValue,
// 	VE_Slot_StatDownParryValue,
// 	VE_Slot_StatDownCriticalResistValue,
// 	VE_Slot_StatDownCriticalResistDamage,
// 	VE_Slot_StatDownMagicCriticalRate,
	/*Stat Up/Down End*/

	/*Hero Speciality Start*/
	VE_Slot_Poison_Yoseph,
	VE_Slot_Poison_Jin,
	VE_Slot_Bleeding_Than,
	VE_Slot_Bleeding_Buddy,
	VE_Slot_Fire_Flame,
	VE_Slot_Fire_Scarlet,
	VE_Slot_Ignite_Flame,
	VE_Slot_Ignite_Scarlet,
	VE_Slot_Fire_Akhan,
	VE_Slot_Poison_Zagan,
	VE_Slot_Bleeding_Bartor,
	VE_Slot_Aura_Bartor,
	VE_Slot_Aura_Pang,
	VE_Slot_Aura_Grant,
	VE_Slot_Aura_Murmur,
	VE_Slot_Aura_Bonnie,
	VE_Slot_Aura_Buddy,
	VE_Slot_Aura_Bard,
	VE_Slot_Aura_Flame,
	VE_Slot_Aura_Scarlet,
	VE_Slot_Aura_Yoseph,
	VE_Slot_Aura_Amon,
	VE_Slot_Aura_Rhea,
	VE_Slot_Aura_Iola,
	VE_Slot_Aura_Moment,
	/*Hero Speciality End*/

	/*Aura Start*/
	VE_Slot_Aura_Buff,
	VE_Slot_Aura_Debuff,
	/*Aura End*/

	/*Regeneration Start*/
	VE_Slot_Regeneration,
	VE_Slot_Regeneration_02,
	VE_Slot_Regeneration_03,
	/*Regeneration End*/

	/*Bless */
	VE_Slot_Bless,
	/*Bless*/

	VE_Slot_Vampire,
	VE_Slot_InstantSlotBonus,
	VE_Slot_ReflectDamage,

	VE_Slot_Mark_Dagger,
	VE_Slot_Ice,


	VE_Slot_BonusHeal,
	VE_Slot_StatDownHeal,

	VE_Slot_StatUpAttackRange,
	VE_Slot_StatDownAttackRange,

	VE_Slot_ForceDamage,
	VE_Slot_BerserkerNPC,

	VE_Slot_DamageShieldRecovery,

	VE_Slot_Active_Max,

	//Passive slot

	VE_Slot_Passive,
	VE_Slot_Passive_01,
	VE_Slot_Passive_02,
	VE_Slot_Passive_03,
	VE_Slot_Passive_04,
	VE_Slot_Passive_05,

	VE_Slot_Max,
};

//AP_NONE -
//AP_IMMEDIATE			즉시성 효력을 가짐																		I_
//AP_CONDITIONAL		특정 트리거에 반응하는 상태를 설정함														T_
//AP_DURATION			일정 시간동안 효력을 발생																	C_
//AP_PERSISTENT			영구, 또는 일정 시간 동안 상태를 변경함													P_
//AP_SLOT_TRANSFORMER	시너지 효력을 위해, 특정 상태에 따라 반응하는 유형을 의미함									F_
//AP_SINGLE				위치 이동기 중, 단일 타겟을 대상으로 적용되어야 하는 효력임
//						해당 어플라이어들은 SHAPE를 가진 범위 타격이라도, 그 중 단일을 선택하는 로직이 겸비되어야 함	I_
//AP_SUMMON				소환 관련 AP																				I_

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillApplierTypeEnum : uint8
{
	VE_None,

	/*1. AP_Immediate Start*/
	VE_i_BEGIN,
		/*1.1 Instant Applier Start*/
			/*1.1.1 Attack Applier Start*/
			VE_i_p_attack,
			VE_i_m_attack,
			VE_i_hp_percent_attack,
			VE_i_p_split_attack,
			/*1.1.1 Attack Applier End*/

			/*1.1.2 Heal Applier Start*/
			VE_i_heal,
			VE_i_hp_percent_heal,
			/*1.1.2 Heal Applier End*/

			/*1.1.3 Utility Applier Start*/
			VE_i_dispel_buff,
			VE_i_dispel_debuff,
			VE_i_cooltime_bonus,
			VE_i_despawn,
			VE_i_recall_friend,
			/*1.1.3 Utility Applier End*/

			/*1.1.4 Summon Applier Start*/
			VE_i_summon_1Max1,
			VE_i_summon_1Max2,
			VE_i_summon_1Max3,
			VE_i_summon_1Max4,
			VE_i_summon_1Max5,
			VE_i_summon_Once2,
			VE_i_summon_Once3,
			VE_i_summon_Once4,
			VE_i_summon_Once5,
			/*1.1.4 Summon Applier End*/
		/*1.1 Instant Applier End*/

		/*1.2 Location Applier Start*/
		VE_i_dash,
		VE_i_dash_forward,
		VE_i_pull,
		VE_i_push,
		VE_i_blink,
		VE_i_blink_front,
		VE_i_blink_back,
		VE_i_switch,
		VE_i_pull_Amon,
		/*1.2 Location Applier End*/
	VE_i_END,
	/*1. AP_Immediate End*/

	/*2. AP_Persistant Start*/
	VE_p_BEGIN,
		/*2.1 Stat_Up/Down Applier Start*/
		VE_p_stat_up_BEGIN,
		VE_p_stat_up_PhysicalAttackPower,
		VE_p_stat_up_MagicAttackPower,
		VE_p_stat_up_AccuracyValue,
		VE_p_stat_up_PierceValue,
		VE_p_stat_up_AttackSpeed,
		VE_p_stat_up_CriticalValue,
		VE_p_stat_up_CriticalDamageValue,
		VE_p_stat_up_PhysicalDefenceValue,
		VE_p_stat_up_MagicDefenceValue,
		VE_p_stat_up_AvoidValue,
		// 	VE_p_stat_up_ParryValue,
		VE_p_stat_up_Maxhp,
		VE_p_stat_up_MoveSpeed,
		VE_p_stat_up_AttackRange,
		VE_p_stat_up_ModelScale,
		// 	VE_p_stat_up_CriticalResistValue,
		// 	VE_p_stat_up_CriticalResistDamage,
		// 	VE_p_stat_up_MagicCriticalRate,
		VE_p_stat_up_END,
		/*2.1 Stat_Up/Down Applier End*/

		/*2.2 Counter Applier Start*/ 
		VE_p_Counter_BEGIN,
		VE_p_bless_solar,
		VE_p_bless_light,
		VE_p_bless_dark,
		VE_p_bless_moon,
		VE_p_Counter_MID1,
		VE_p_weak_strength,
		VE_p_weak_agility,
		VE_p_weak_focus,
		VE_p_weak_clever,
		VE_p_weak_fire,
		VE_p_weak_ice,
		VE_p_Counter_MID2,
		VE_p_protect_solar,
		VE_p_protect_light,
		VE_p_protect_dark,
		VE_p_protect_moon,
		VE_p_Counter_END,

		VE_p_Spirit_BEGIN,
		VE_p_spirit_plus_solar,
		VE_p_spirit_plus_light,
		VE_p_spirit_plus_dark,
		VE_p_spirit_plus_moon,
		VE_p_Spirit_MID,
		VE_p_spirit_minus_solar,
		VE_p_spirit_minus_light,
		VE_p_spirit_minus_dark,
		VE_p_spirit_minus_moon,
		VE_p_Spirit_END,
		/*2.2 Counter Applier End*/

		/*2.3 Abnormal State Applier Start*/
		VE_p_Abnormal_BEGIN,
		VE_p_stun,
		VE_p_down,
		VE_p_frozen,
		VE_p_silence,
		VE_p_hold,
		VE_p_hide,
		VE_p_fear,
		VE_p_airborne,
		VE_p_captivate,
		VE_p_push,
		VE_p_knockback,
		VE_p_Abnormal_END,
		/*2.3 Abnormal State Applier End*/


		/*2.4 Status & Featured Applier Start*/
		VE_p_Status_BEGIN,
		VE_p_modification,
		VE_p_blindness,
		VE_p_damage_ice_shield,
		VE_p_damage_shield,
		VE_p_damage_shield_recovery,
		VE_p_fear_self,
		VE_p_vamp_attack,
		VE_p_self_despawn,
		VE_p_provoke,
		VE_p_reflect_damage,
		VE_p_reflect_damage_All,
		VE_p_force_damage,
		VE_p_timebomb,
		VE_p_timeHeal,
		VE_p_heal_bonus,
		VE_p_multi_attack,
		VE_p_cost_bonus,
		VE_p_Status_END,
		/*2.4 Status & Featured Applier End*/

		/*2.5 Blocking Applier Start*/
		VE_p_block_BEGIN,
			/*2.5.1 Block All Start*/
			VE_p_block_abnormal_debuff,
			VE_p_block_abnormal_buff,
			/*2.5.1 Block All End*/

			/*2.5.2 Block Group Start*/
			VE_p_block_group_helplessness,
			VE_p_block_group_dot_bleeding,
			VE_p_block_group_dot_fire,
			VE_p_block_group_dot_poison,
			VE_p_block_debuff_except_group_poison,
			VE_p_block_debuff_except_group_fire_ignite,
			VE_p_block_debuff_except_group_bleeding,
			VE_p_block_debuff_except_group_mark_weapon,
			/*2.5.2 Block Group End*/

			/*2.5.3 Block Slot Start*/
			VE_p_block_slot_down,
			VE_p_block_slot_stun,
			VE_p_block_slot_fear,
			VE_p_block_slot_ice,
			VE_p_block_slot_freezing,
			VE_p_block_slot_hold,
			VE_p_block_slot_provoke,
			VE_p_block_slot_silence,
			VE_p_block_slot_airborne,
			VE_p_block_slot_captivate,
			VE_p_block_debuff_except_slot_stun,
			VE_p_block_debuff_except_slot_down,
			VE_p_block_debuff_except_slot_fear,
			VE_p_block_debuff_except_slot_hold,
			VE_p_block_debuff_except_slot_ice_Freezing,
			/*2.5.3 Block Slot End*/

			/*2.5.4 Block Applier Start*/
			VE_p_block_stat_up_ModelScale,
			VE_p_block_state_change, /* blocking Abnormal State Appliers for Immun */
			VE_p_block_pull,
			VE_p_block_push,
			VE_p_block_cooltime_bonus,
			/*2.5.4 Block Applier End*/

			/*2.5.5 Block Speciality Start*/
			VE_p_block_instant_damage,
			VE_p_block_physical_damage,
			VE_p_block_magical_damage,
			VE_p_block_melee_damage,
			VE_p_block_range_damage,
			/*2.5.5 Block Speciality End*/
		VE_p_block_END,
		/*2.5 Blocking Applier End*/
	VE_p_END,
	/*2. AP_Persistant End*/

	/*3. AP_Duration Start*/
	VE_c_BEGIN,
	VE_c_hp_hot,
	VE_c_hp_dot,
	VE_c_hp_group_fire,
	VE_c_hp_percent,
	VE_c_END,
	/*3. AP_Duration End*/

	/*4. AP_Conditional Start*/
	VE_t_BEGIN,
	VE_t_guts,
	VE_t_kill,
	VE_t_die,
	VE_t_critical,
	VE_t_parry,
	VE_t_avoid,
	VE_t_pierce,
	VE_t_accuracy,
	VE_t_magic_critical,

	VE_t_caster_group_poison,
	VE_t_caster_group_fire,
	VE_t_caster_group_bleeding,
	VE_t_caster_group_ignite,
	VE_t_caster_group_mark,

	VE_t_caster_slot_freezing,
	VE_t_caster_slot_hold,
	VE_t_caster_slot_stun,
	VE_t_caster_slot_down,
	VE_t_caster_slot_fear,
	VE_t_caster_slot_ice,
	VE_t_attack_pile_up,
	VE_t_kill_hide,
	VE_t_END,
	/*4. AP_Conditional End*/
	
	VE_p_split_shot,

	VE_Applier_Max,

	//table error
	VE_p_block_move,
	VE_p_block_action,
	VE_p_range_defence_bonus,
	VE_p_melee_defence_bonus,
};

UENUM(BlueprintType) // Warning : Finally, this priority is high
enum class EProtectType : uint8
{
	VE_None									UMETA(DisplayName = "None"),
	VE_HP									UMETA(DisplayName = "HP"),
	VE_Count								UMETA(DisplayName = "Count"),
	VE_ImmuneNormal							UMETA(DisplayName = "ImmuneNormal"),
	VE_ImmuneRange							UMETA(DisplayName = "ImmuneRange"),
	VE_Immune								UMETA(DisplayName = "Immune"),
	VE_ImmuneUnActableAbnormal				UMETA(DisplayName = "ImmuneUnActableAbnormal"),
	VE_ImmuneGigantAbnormal					UMETA(DisplayName = "ImmuneGigantAbnormal"),
	VE_ImmuneAkhanAbnormal					UMETA(DisplayName = "ImmuneAkhanAbnormal"),
	VE_ImmuneDebuff							UMETA(DisplayName = "ImmuneDebuff"),
	VE_ImmuneWithHP							UMETA(DisplayName = "ImmuneWithHP"),
	VE_ImmuneRangeWithHPAndReturnDamage		UMETA(DisplayName = "ImmuneRangeWithHPAndReturnDamage"),
	VE_MissRange							UMETA(DisplayName = "MissRange"),
	VE_ImmunePhysical						UMETA(DisplayName = "ImmunePhysical"),
	VE_ImmuneMagic							UMETA(DisplayName = "ImmuneMagic"),
	VE_ImmuneAbnormal						UMETA(DisplayName = "ImmuneAbnormal"),
	VE_ImmuneAbnormalIce					UMETA(DisplayName = "ImmuneAbnormalIce"),
	VE_ImmuneAbnormalFire					UMETA(DisplayName = "ImmuneAbnormalFire"),
	VE_ImmuneAbnormalLightning				UMETA(DisplayName = "ImmuneAbnormalLightning"),
	VE_ImmuneAbnormalPoison					UMETA(DisplayName = "ImmuneAbnormalPoison"),
	VE_ImmuneAbnormalHoly					UMETA(DisplayName = "ImmuneAbnormalHoly"),
	VE_ImmuneAbnormalDark					UMETA(DisplayName = "ImmuneAbnormalDark"),
	VE_ImmuneDebuff_NotDown					UMETA(DisplayName = "ImmuneDebuff_NotDown"),
};


//=====================================================================================================
// Battle
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFriendshipTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Friend 			UMETA(DisplayName = "Friend"),
	VE_Enemy 			UMETA(DisplayName = "Enemy"),
	VE_FriendMagic 		UMETA(DisplayName = "FriendMagic"),
	VE_FriendPhysical 	UMETA(DisplayName = "FriendPhysical"),
	VE_MySummon			UMETA(DisplayName = "MySummon"),
	VE_SummonFriend		UMETA(DisplayName = "SummonFriend"),
	VE_FriendExceptMe	UMETA(DisplayName = "FriendExceptMe"),
	VE_FriendLowestHP	UMETA(DisplayName = "FriendLowestHP"),
	VE_EnemyLowestHP	UMETA(DisplayName = "EnemyLowestHP"),
	VE_FriendParty		UMETA(DisplayName = "FriendParty"),
	VE_All				UMETA(DisplayName = "All"),
};

UENUM(BlueprintType)
enum class EEnemySearchTargetTypeEnum : uint8
{
	VE_Close				UMETA(DisplayName = "Close"),
};



//=====================================================================================================
// Damage Result
//=====================================================================================================
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBattleDamageResultEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Hit					UMETA(DisplayName = "Hit"),
	VE_Critical				UMETA(DisplayName = "Critical"),
//	VE_MagicCritical		UMETA(DisplayName = "MagicCritical"),
	VE_Miss					UMETA(DisplayName = "Miss"),
	VE_Immune				UMETA(DisplayName = "Immune"),
	VE_Counter				UMETA(DisplayName = "Counter"),
	VE_JustInput			UMETA(DisplayName = "JustInput"),
	VE_Dodge				UMETA(DisplayName = "Dodge"),
	VE_Parry				UMETA(DisplayName = "Parry"),
	VE_Dead					UMETA(DisplayName = "Dead"),
	VE_Pierce				UMETA(DisplayName = "Pierce"),
	VE_JustInputAttack		UMETA(DisplayName = "JustInputAttack"),

	VE_Heal					UMETA(DisplayName = "Heal"),
	VE_NoneHitDamage		UMETA(DisplayName = "NoneHitDamage"),	// ex) dot...
	VE_ResistantDamage		UMETA(DisplayName = "ResistantDamage"),

	VE_ReceiveHit			UMETA(DisplayName = "ReceiveHit"),
	VE_ReceiveCritical		UMETA(DisplayName = "ReceiveCritical"),
// 	VE_ReceiveMagicCritical	UMETA(DisplayName = "ReceiveMagicCritical"),
	VE_ReceiveMiss			UMETA(DisplayName = "ReceiveMiss"),
	VE_ReceiveImmune		UMETA(DisplayName = "ReceiveImmune"),
	VE_ReceiveCounter		UMETA(DisplayName = "ReceiveCounter"),
	VE_ReceiveDodge			UMETA(DisplayName = "ReceiveDodge"),
// 	VE_ReceiveParry			UMETA(DisplayName = "ReceiveParry"),
	VE_ReceiveDead			UMETA(DisplayName = "ReceiveDead"),
	VE_ReceivePierce		UMETA(DisplayName = "ReceivePierce"),
	VE_ReceiveResistantDamage		UMETA(DisplayName = "ReceiveResistantDamage"),

	//-----------------------------------------------------------------------------------
	//Customizing
	VE_BuffText				UMETA(DisplayName = "BuffText"),
	VE_Dodge_Parry			UMETA(DisplayName = "Dodge_Parry"),
};


//=====================================================================================================
// Item
//=====================================================================================================
//UENUM(BlueprintType)
//enum class EGemItemPropertyTypeEnum : uint8
//{
//	VE_None								UMETA(DisplayName = "None"),
//	VE_PhysicalAttackPower				UMETA(DisplayName = "PhysicalAttackPower"),
//	VE_MagicAttackPower					UMETA(DisplayName = "MagicAttackPower"),
//	VE_Armor							UMETA(DisplayName = "Armor"),
//	VE_MagicResistance					UMETA(DisplayName = "MagicResistance"),
//	VE_CriticalRate_Scale				UMETA(DisplayName = "CriticalValue"),
//	VE_CriticalDamageRate_Scale			UMETA(DisplayName = "CriticalDamageValue"),
//	VE_AccuracyValue					UMETA(DisplayName = "AccuracyValue"),
//	VE_MaxHP 							UMETA(DisplayName = "MaxHp"),
//	VE_DamageBonus						UMETA(DisplayName = "DamageBonus_Scale"),
//
//};

// gone with item options change

//UENUM(BlueprintType)
//enum class EItemOptionIncreaseType : uint8
//{
//	VE_Prob		UMETA(DisplayName = "Prob"),
//	VE_Num		UMETA(DisplayName = "Num"),
//};
//
//UENUM(BlueprintType)
//enum class EItemOptionType : uint8
//{
//	//Weapon
//	VE_AttackSpeed				UMETA(DisplayName = "AttackSpeed"),
//	VE_PierceValue				UMETA(DisplayName = "VE_PierceValue"),
//	VE_CriticalDamage_Scale		UMETA(DisplayName = "VE_CriticalDamage_Scale"),
//	VE_AccuracyRate				UMETA(DisplayName = "VE_AccuracyRate"),
//	VE_MoveSpeed				UMETA(DisplayName = "VE_MoveSpeed"),
//	VE_DamageBonus_Scale		UMETA(DisplayName = "VE_DamageBonus_Scale"),
//	VE_PDamageBonus_Scale		UMETA(DisplayName = "VE_PDamageBonus_Scale"),
//
//	//Armor
//	VE_MaxHP					UMETA(DisplayName = "VE_MaxHP"),
//	VE_BlockRate_Scale			UMETA(DisplayName = "VE_BlockRate_Scale"),
//	VE_BlockValue				UMETA(DisplayName = "VE_BlockValue"),
//	VE_PDefBonusValue			UMETA(DisplayName = "VE_PDefBonusValue"),
//	VE_MDefBonusValue			UMETA(DisplayName = "VE_MDefBonusValue"),
//	VE_ParryValue				UMETA(DisplayName = "VE_ParryValue"),
//};


//UENUM(BlueprintType)
//enum class EItemGenerateOptionType : uint8
//{
//	VE_None								UMETA(DisplayName = "None"),
//	//Weapon
//	VE_AttackSpeed						UMETA(DisplayName = "AttackSpeed"),
//	VE_MoveSpeed						UMETA(DisplayName = "MoveSpeed"),
//	VE_PierceValue						UMETA(DisplayName = "PierceValue"),
//	VE_MaxHP							UMETA(DisplayName = "MaxHP"),
//	VE_ParryValue						UMETA(DisplayName = "ParryValue"),
//
//	VE_PhysicalAttackPower				UMETA(DisplayName = "PhysicalAttackPower"),
//	VE_MagicAttackPower					UMETA(DisplayName = "MagicAttackPower"),
//	VE_Armor							UMETA(DisplayName = "Armor"),
//	VE_MagicResistance					UMETA(DisplayName = "MagicResistance"),
//	VE_DodgeValue						UMETA(DisplayName = "DodgeValue"),
//	VE_CriticalValue					UMETA(DisplayName = "CriticalValue"),
//	VE_AccuracyValue					UMETA(DisplayName = "AccuracyValue"),
//	VE_CriticalDamageRate_Scale			UMETA(DisplayName = "CriticalDamageRate_Scale"),
//	VE_ParryDamageReduceRate_Scale		UMETA(DisplayName = "ParryDamageReduceRate_Scale"),
//	VE_PierceDefenceBonus				UMETA(DisplayName = "PierceDefenceBonus"),
//};

// 오버로드_장비아이템_변경안.docx.2.(1) DT_CharacterItem
// DT_CharacterItem.ItemType
// TODO: only VE_EquipItem is valid
UENUM(BlueprintType)
enum class EItemTypeEnum : uint8
{
	VE_None, // TODO: Not designed
	VE_EquipItem		UMETA(DisplayName = "EquipItem"),
	VE_MaterialItem		UMETA(DisplayName = "MaterialItem"), // no longer an item
	VE_GemItem			UMETA(DisplayName = "GemItem"), // no longer an item
	VE_ConsumableItem	UMETA(DisplayName = "Consumable"), // no longer an item
};

// 오버로드_장비아이템_변경안.docx.2.(1) DT_CharacterItem
// DT_CharacterItem.EquipItemPropertyType
UENUM(BlueprintType)
enum class EEquipItemPropertyType : uint8
{
	VE_PhysicalWeapon	UMETA(DisplayName = "PhysicalWeapon"),
	VE_MagicWeapon		UMETA(DisplayName = "MagicWeapon"),
	VE_PhysicalGuard	UMETA(DisplayName = "PhysicalGuard"),
	VE_MagicGuard		UMETA(DisplayName = "MagicGuard"),
	VE_HPGuard			UMETA(DisplayName = "HPGuard"),
};

// 오버로드_장비아이템_변경안.docx.1.(2).B.ii 장비별 장착 타입
// DT_CharacterItem.EquipType
// DT_Character.EquipType
// Table CSVs come with DisplayName of the enum values; no 'VE_'
UENUM(BlueprintType)
enum class EItemEquipTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"), // TODO: Not designed

	// close-ranged physical
	VE_Sword			UMETA(DisplayName = "Sword"),
	VE_Dagger			UMETA(DisplayName = "Dagger"),
	VE_Hammer			UMETA(DisplayName = "Hammer"),
	VE_Rod				UMETA(DisplayName = "Rod"),

	// long-ranged physical
	VE_Bow				UMETA(DisplayName = "Bow"),
	VE_Crossbow			UMETA(DisplayName = "Crossbow"),
	VE_Gun				UMETA(DisplayName = "Gun"),

	// magic
	VE_Staff			UMETA(DisplayName = "Staff"),
	VE_Magic			UMETA(DisplayName = "Magic"), // "Orb"
	VE_Wand				UMETA(DisplayName = "Wand"), // TODO: not on the Changes Document

	// armor, upper
	VE_HeavyArmor		UMETA(DisplayName = "HeavyArmor"),
	VE_LightArmor		UMETA(DisplayName = "LightArmor"),
	VE_FabricArmor		UMETA(DisplayName = "FabricArmor"),

	// armor, lower
	VE_HeavyPants		UMETA(DisplayName = "HeavyPants"),
	VE_LightPants		UMETA(DisplayName = "LightPants"),
	VE_FabricPants		UMETA(DisplayName = "FabricPants"),

	// accessories
	VE_Necklace			UMETA(DisplayName = "Necklace"),
	VE_Ring				UMETA(DisplayName = "Ring"),
};

// DT_Character.EquipArmorType
UENUM(BlueprintType)
enum class EItemEquipArmorTypeEnum : uint8
{
	VE_EveryOne			UMETA(DisplayName = "EveryOne"), // from design (Kratos)
	VE_Heavy			UMETA(DisplayName = "Heavy"),
	VE_Light			UMETA(DisplayName = "Light"),
	VE_Leather			UMETA(DisplayName = "Leather"),
	VE_Fabric			UMETA(DisplayName = "Fabric"),
};

// 오버로드_장비아이템_변경안.docx.2.(1) DT_CharacterItem
// DT_CharacterItem.EquipPosition
UENUM(BlueprintType)
enum class EItemEquipPositionEnum : uint8
{
	VE_Weapon			UMETA(DisplayName = "Weapon"),
	VE_Armor			UMETA(DisplayName = "Armor"),
	VE_Pants			UMETA(DisplayName = "Pants"),
	VE_Ring				UMETA(DisplayName = "Ring"),
	VE_Necklace			UMETA(DisplayName = "Necklace"),
	

	VE_Max
};

// 2018 Reboot
UENUM(BlueprintType)
enum class EItemGradeEnum : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
	Rare			UMETA(DisplayName = "Rare"),
	Epic			UMETA(DisplayName = "Epic"),
	Unique			UMETA(DisplayName = "Unique"),
	Tower			UMETA(DisplayName = "Tower"),

	Max,

	//temp - del table
	VE_Orange,
};

// DT_ItemRandomOption.xlsm.Description
// Option stat parameters
// Refer FCharacterBattleData
UENUM(BlueprintType)
enum class EItemStatOption : uint8
{
	MaxHP							UMETA(DisplayName = "MaxHP"),
	PhysicalAttackPower				UMETA(DisplayName = "PhysicalAttackPower"),
	MagicAttackPower				UMETA(DisplayName = "MagicAttackPower"),
	Armor							UMETA(DisplayName = "Armor"),
	MagicResistance					UMETA(DisplayName = "MagicResistance"),
	AttackRange						UMETA(DisplayName = "AttackRange"),
	MoveSpeed						UMETA(DisplayName = "MoveSpeed"),
	AttackSpeed						UMETA(DisplayName = "AttackSpeed"),
	DodgeValue						UMETA(DisplayName = "DodgeValue"),
	CriticalValue					UMETA(DisplayName = "CriticalValue"),
	CriticalDamageRate_Scale		UMETA(DisplayName = "CriticalDamageRate_Scale"),
	PierceValue						UMETA(DisplayName = "PierceValue"),
	AccuracyValue					UMETA(DisplayName = "AccuracyValue"),
	CriticalResistValue				UMETA(DisplayName = "CriticalResistValue"),
	Max
};

UENUM(BlueprintType)
enum class EItemEnchantStateEnum : uint8
{
	NoEnchant,
	HaveEnchant,
	MaxEnchant,
	CantEnchant
};

UENUM(BlueprintType)
enum class EItemUseStateEnum : uint8
{
	Using,
	Locking,
	EquipedRune
};

//=====================================================================================================
// Quest
//=====================================================================================================
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EQuestGuideTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_ROAComplete			UMETA(DisplayName = "ROAComplete"),
	VE_CampaignPlay			UMETA(DisplayName = "CampaignPlay"),
	VE_ColosseumPlay		UMETA(DisplayName = "ColosseumPlay"),
	VE_ArenaPlay			UMETA(DisplayName = "ArenaPlay"),
	VE_RaidPlay				UMETA(DisplayName = "RaidPlay"),
	VE_HeroSkillLevel		UMETA(DisplayName = "HeroSkillLevel"),
	VE_ItemEnchant			UMETA(DisplayName = "ItemEnchant"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMissionCategoryType : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Normal			UMETA(DisplayName = "Normal"),
	VE_Hero				UMETA(DisplayName = "Hero"),
	VE_Item				UMETA(DisplayName = "Item"),
	VE_Adventure		UMETA(DisplayName = "Adventure"),
	VE_Combat			UMETA(DisplayName = "Combat"),
	VE_Guild			UMETA(DisplayName = "Guild"),
	VE_Growth			UMETA(DisplayName = "Growth"),
	VE_Max,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_SKILL
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
		int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
		bool		activated;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
		int32		slotNo;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
		FString		skillId;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FHERO_SKILL& Data)
	{
		Ar << Data.level;
		Ar << Data.activated;
		Ar << Data.slotNo;
		Ar << Data.skillId;

		return Ar;
	}
};


//=====================================================================================================
// Avatar Ability
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAvatarAbilityType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_PhysicalDefence		UMETA(DisplayName = "PhysicalDefence"),
	VE_MaxHP				UMETA(DisplayName = "MaxHP"),
	VE_MagicalDefence		UMETA(DisplayName = "MagicalDefence"),
	VE_CriticalRate			UMETA(DisplayName = "CriticalRate"),
	VE_CriticalDamage		UMETA(DisplayName = "CriticalDamage"),
	VE_Dodge				UMETA(DisplayName = "Dodge"),
	VE_Parry				UMETA(DisplayName = "ParryValue"),
	VE_Pierce				UMETA(DisplayName = "PierceValue"),
	VE_MagicalAttack		UMETA(DisplayName = "MagicalAttack"),
	VE_MagicCritical		UMETA(DisplayName = "MagicCritical"),
	VE_CriticalResist		UMETA(DisplayName = "CriticalResist"),
	VE_ReduceCoolTime		UMETA(DisplayName = "ReduceCoolTime"),
	VE_SkillCostBonus		UMETA(DisplayName = "SkillCostBonus"),
	VE_MoveSpeed			UMETA(DisplayName = "MoveSpeed"),
	VE_ReceivedHeal			UMETA(DisplayName = "ReceivedHeal"),
	VE_TypeMax				UMETA(DisplayName = "TypeMax"),
};

UENUM(BlueprintType)
enum class ECrewSpotType : uint8
{
	VE_None,
	VE_Front_01,
	VE_Front_02,
	VE_Vicinity_01,
	VE_Vicinity_02,
	VE_Back_01,
	VE_Back_02,
};

UENUM(BlueprintType)
enum class EHeroInventoryType : uint8
{
	VE_TypeHero,
	VE_TypeSpirit,
	VE_InventoryTypeAll,
};

UENUM(BlueprintType)
enum class EAnimMontageEventDie : uint8
{
	VE_Die01				UMETA(DisplayName = "Die01"),
	VE_Die02				UMETA(DisplayName = "Die02"),
	VE_AnimMax				UMETA(DisplayName = "AnimMax"),
};


// raid boss enum start
UENUM(BlueprintType)
enum class ERaidBossClassType : uint8
{
	TANK,
	RANGE,
	SUPPORT,
	MELEE,
	MAGIC,
};

UENUM(BlueprintType)
enum class ERaidDifficultyType : uint8
{
	EASY,
	NORMAL,
	HARD,
	VERYHARD,
};

UENUM(BlueprintType)
enum class ERaidDayType : uint8
{
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT,
	SUN,
	WEEK,
};
// raid boss enum end

#define ScaleNumber										10000

//combat stat constants
#define AttackParameterConstant		8.0f
#define AutoSkillWeightConstant		4.0f
#define ActiveSkillWeightConstant	6.0f
#define UltimateSkillWeightConstant 8.0f
#define PassiveSkillWeightConstant	4.0f

#define SKILL_FIRECOOLTIME		0.3f
#define MAX_Skill_LEVEL			10

#define INVALID_INDEXNUM		-1

// PlayerUK
#define PLAYER_UK_UNIT			1000
#define PLAYER_UK_NONE			0

const int8 MAX_HERO_POS_COUNT = 9;
const int8 MAX_HERO_PLACE_COUNT = 5;
