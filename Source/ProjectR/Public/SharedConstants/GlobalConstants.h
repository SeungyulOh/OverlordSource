// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/WalletConstants.h"
#include "SharedConstants/SharedGlobalConstants.h"
#include "RTS_Sctruct.h"
#include "GlobalConstants.generated.h"


/*
GameInfo
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	//VE_Login 			UMETA(DisplayName = "Login"),
	VE_Lobby 			UMETA(DisplayName = "Lobby"),
	//VE_LobbyFloating 	UMETA(DisplayName = "LobbyFloating"),
	VE_Play				UMETA(DisplayName = "Play"),	
	VE_PlayTest			UMETA(DisplayName = "PlayTest"),
	VE_Shutdown			UMETA(DisplayName = "Shutdown"),
	VE_Tutorial			UMETA(DisplayName = "Tutorial"),
	VE_Max,
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameModeStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Init				UMETA(DisplayName = "Init"),	// 게임에 필요한 셋팅을 한다.[맵 아이디,리소스 기타 등등] 로딩화면이 켜짐
	VE_Prepare			UMETA(DisplayName = "Prepare"),	// 맵을 로딩하고 리소스를 캐싱한다
	VE_Ready			UMETA(DisplayName = "Ready"),	// Prepare 모드에서 캐싱 및 준비 과정이 끝나면 각종 셋팅별로 정리한다. 로딩화면이 여기서 꺼짐.
	VE_Start			UMETA(DisplayName = "Start"),	// 게임을 시작함
	VE_End				UMETA(DisplayName = "End"),		// 게임 결과 화면을 출력함
	VE_Result			UMETA(DisplayName = "Result"),	// 게임에서 사용한 리소스를 해제하고 메모리를 정리한다.
};

UENUM(BlueprintType)            //"BlueprintType" is essential to include
enum class EGamePlayModeTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_MainLobby			UMETA(DisplayName = "Defualt"),
	VE_Campaign				UMETA(DisplayName = "Campaign"),
	VE_Arena				UMETA(DisplayName = "Arena"),
	VE_Colosseum			UMETA(DisplayName = "Colosseum"), 
	VE_RaidKillBoss			UMETA(DisplayName = "RaidKillBoss"),
	VE_BattleField			UMETA(DisplayName = "BattleField"),
	VE_BattleGround			UMETA(DisplayName = "BattleGround"),
	VE_GuildRaid			UMETA(DisplayName = "GuildRaid"),
	VE_MirrorDungeon		UMETA(DisplayName = "MirrorDungeon"),
	VE_HeroEpisode			UMETA(DisplayName = "HeroEpisode"),
	VE_Challenge			UMETA(DisplayName = "Challenge"),
	VE_WeekDungeon			UMETA(DisplayName = "WeekDungeon"),
	VE_PlayTest				UMETA(DisplayName = "PlayTest"),
	VE_FloorStore			UMETA(DisplayName = "FloorStore"),
	VE_Guild				UMETA(DisplayName = "Guild"),
	VE_TimeTreasure			UMETA(DisplayName = "TimeTresure"),
	VE_SingleRaid			UMETA(DisplayName = "SingleRaid"),
	VE_MultiRaid			UMETA(DisplayName = "MultiRaid"),
	VE_CampaignHard			UMETA(DisplayName = "CampaignHard"),
	VE_HeroManagement		UMETA(DisplayName = "HeroManagement"),

	VE_Shot					UMETA(DisplayName = "VE_Shot"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESpawnResultTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Success				UMETA(DisplayName = "Success"),
	VE_CountLess			UMETA(DisplayName = "CountLess"),
	VE_NoReady				UMETA(DisplayName = "NoReady"),	
	VE_IncorrectLocation	UMETA(DisplayName = "IncorrectLocation"),
	VE_Fail					UMETA(DisplayName = "Fail"),
	VE_Moving				UMETA(DisplayName = "Moving"),
	VE_Attack				UMETA(DisplayName = "Attack"),
};

//=====================================================================================================
//CharacterInfo
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGradeEnum : uint8
{
	VE_Boss				UMETA(DisplayName = "Boss"),
	VE_Easy				UMETA(DisplayName = "Easy"),
	VE_Elite			UMETA(DisplayName = "Elite"),
	VE_Epic				UMETA(DisplayName = "Epic"),
	VE_Normal			UMETA(DisplayName = "Normal"),
	VE_Summon			UMETA(DisplayName = "Summon"),
};

//=====================================================================================================
//HeroFilter
//=====================================================================================================

UENUM()
enum class EFilterSortTypeEnum : uint8
{
	VE_Id				UMETA(DisPlayname = "Id"),
	VE_Level			UMETA(DisPlayname = "Level"),
	VE_AttackPower		UMETA(DisPlayname = "AttackPower"),
	VE_RuneCount		UMETA(DisPlayname = "RuneCount"),
	VE_SortingOrder		UMETA(DisPlayname = "SortingOrder"),
	VE_Grade				UMETA(DisplayName = "Grade"),
	VE_Damage				UMETA(DisplayName = "Damage"),
	VE_PhysicalDefense		UMETA(DisplayName = "PhysicalDefense"),
	VE_MagicDefense			UMETA(DisplayName = "MagicDefense"),
	VE_Health				UMETA(DisplayName = "Health"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EHeroListFilterEnum : uint8
{
	VE_All					UMETA(DisplayName = "All"),
	VE_HaveHero				UMETA(DisplayName = "HaveHero"),
	VE_Tanker				UMETA(DisplayName = "Tanker"),
	VE_RangeDealer 			UMETA(DisplayName = "RangeDealer"),
	VE_MeleeDealer 			UMETA(DisplayName = "MeleeDealer"),
	VE_MagicDealer			UMETA(DisplayName = "MagicDealer"),
	VE_Supporter			UMETA(DisplayName = "Supporter"),
	VE_Level				UMETA(DisplayName = "Level"),
	VE_AttackPower			UMETA(DisplayName = "AttackPower"),
	VE_Grade				UMETA(DisplayName = "Grade"),
	VE_Damage				UMETA(DisplayName = "Damage"),
	VE_PhysicalDefense		UMETA(DisplayName = "PhysicalDefense"),
	VE_MagicDefense			UMETA(DisplayName = "MagicDefense"),
	VE_Health				UMETA(DisplayName = "Health"),
	VE_Max,
};

/*
Check order of priority == 1
*/

UENUM(BlueprintType)
enum class ECharacterCommandEnum : uint8
{
	VE_None,
	VE_Die,
	VE_Abnormal,
	VE_ActionSkill,
	VE_ActionSkillEnd,
	VE_CastingSkill,
	VE_StopSkill,
	VE_StartAttack,		// Normal Attack
	VE_StopAttack,
	VE_StartMove,
	VE_StopMove,
	VE_Acquire,
};

UENUM(BlueprintType)
enum class EAnimMontageType : uint8
{
	VE_Idle,
	VE_Run,
	VE_Attack,
	VE_Skill,
	VE_Abnormal,
	VE_Event,
	VE_DamageResult,
};

UENUM(BlueprintType)
enum class EAnimStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Die				UMETA(DisplayName = "1.Die"),
	VE_Abnormal 		UMETA(DisplayName = "2.Abnormal"),
	VE_Run				UMETA(DisplayName = "3.Run"),
	VE_SkillAttack		UMETA(DisplayName = "4.SkillAttack"),
	VE_NormalAttack		UMETA(DisplayName = "5.NormalAttack"),
	VE_Idle				UMETA(DisplayName = "6.Idle"),
	VE_Spawn			UMETA(DisplayName = "7.Spawn"),
	VE_Acquire			UMETA(DisplayName = "8.Acquire"),
};

UENUM(BlueprintType)
enum class EIdleTypeEnum : uint8
{
	VE_NormalIdle		UMETA(DisplayName = "NormalIdle"),
	VE_SpecialIdle		UMETA(DisplayName = "SpecialIdle"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EApplyBuffConditionEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Hit				UMETA(DisplayName = "Hit"),
	VE_Critical			UMETA(DisplayName = "Critical"),
	VE_Hit_Critical		UMETA(DisplayName = "Hit_Critical"),
	VE_Dodge			UMETA(DisplayName = "Dodge"),
	VE_Parry			UMETA(DisplayName = "Parry"),
	VE_Immune			UMETA(DisplayName = "Immune"),
	VE_Miss				UMETA(DisplayName = "Miss"),
	VE_Dead				UMETA(DisplayName = "Dead"),
	VE_All				UMETA(DisplayName = "All"),
};

/*
Skill enumerations..
*/

UENUM(BlueprintType)
enum class ESkillAreaMeshTypeEnum : uint8
{
	VE_None,
	VE_SkillArea,
	VE_SkillPickArea,
};


UENUM(BlueprintType)
enum class ESkillIconColorEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Induce			UMETA(DisplayName = "Induce"),
	VE_Damage			UMETA(DisplayName = "Damage"),
};

UENUM(BlueprintType)
enum class ESkillIconShapeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Stun_Induce			UMETA(DisplayName = "Stun_Induce"),
	VE_Stun_Damage			UMETA(DisplayName = "Stun_Damage"),
	VE_Down_Induce			UMETA(DisplayName = "Down_Induce"),
	VE_Down_Damage			UMETA(DisplayName = "Down_Damage"),
	VE_Fear_Induce			UMETA(DisplayName = "Fear_Induce"),
	VE_Fear_Damage			UMETA(DisplayName = "Fear_Damage"),
	VE_Hold_Induce			UMETA(DisplayName = "Hold_Induce"),
	VE_Hold_Damage			UMETA(DisplayName = "Hold_Damage"),
	VE_Poison_Induce		UMETA(DisplayName = "Poison_Induce"),
	VE_Poison_Damage		UMETA(DisplayName = "Poison_Damage"),
	VE_Bleeding_Induce		UMETA(DisplayName = "Bleeding_Induce"),
	VE_Bleeding_Damage		UMETA(DisplayName = "Bleeding_Damage"),
	VE_Fire_Induce			UMETA(DisplayName = "Fire_Induce"),
	VE_Fire_Damage			UMETA(DisplayName = "Fire_Damage"),
	VE_Ice_Induce			UMETA(DisplayName = "Ice_Induce"),
	VE_Ice_Damage			UMETA(DisplayName = "Ice_Damage"),
};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillHoldingTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Now			UMETA(DisplayName = "Now"),
	VE_Duration 	UMETA(DisplayName = "Duration"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillOverrideTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_AfterHit		UMETA(DisplayName = "AfterHit"),
	VE_Free 		UMETA(DisplayName = "Free"),
	VE_WholeBlock 	UMETA(DisplayName = "WholeBlock"),
};


UENUM(BlueprintType)
enum class EBulltSocketType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Shoot				UMETA(DisplayName = "Shoot"),
	VE_ShootR				UMETA(DisplayName = "ShootR"),
	VE_ShootL				UMETA(DisplayName = "ShootL"),
};


/*
Skill Condition
*/
UENUM(BlueprintType)
enum class ESkillConditionTypeEnum : uint8
{
	VE_None							UMETA(DisplayName = "None"),
	VE_Equal 						UMETA(DisplayName = "Equal"),
	VE_Less 						UMETA(DisplayName = "Less"),
	VE_Greater 						UMETA(DisplayName = "Greater"),
	VE_LessEqual					UMETA(DisplayName = "LessEqual"),
	VE_GreaterEqual					UMETA(DisplayName = "GreatEqual"),
	VE_Percent 						UMETA(DisplayName = "Percent"),
	VE_Periodic						UMETA(DisplayName = "Periodic"),
	VE_Count						UMETA(DisplayName = "Count"),		
	VE_CloseByEnemies				UMETA(DisplayName = "CloseByEnemies"),
	VE_LastAppliedSkillCirtically	UMETA(DisplayName = "LastAppliedSkillCirtically"),
	VE_UnActableState				UMETA(DisplayName = "UnActableState"),
	VE_CountAndNotHide				UMETA(DisplayName = "CountAndNotHide"),  // Using count on dead..
	VE_CloseByNotTargetMeEnemies	UMETA(DisplayName = "NotTargetMeEnemy"),  
	VE_BetweenTargetDistance		UMETA(DisplayName = "BetweenDistance"),
	VE_Probability					UMETA(DisplayName = "ProbabilityOnAttacking"),	
	VE_ActiveOne					UMETA(DisplayName = "ActiveOne"),
	VE_InTheAbnormalState			UMETA(DisplayName = "InTheAbnormalState"),
	VE_NotInTheAbnormalState		UMETA(DisplayName = "NotInTheAbnormalState"),
	VE_InState						UMETA(DisplayName = "InState"),
	VE_Exploitation					UMETA(DisplayName = "Exploitation"),
	VE_BattleBlackboard				UMETA(DisplayName = "BattleBlackboard"),
	VE_CheckDeadCharacter			UMETA(DisplayName = "CheckDeadCharacter"),
	VE_TargetBack					UMETA(DisplayName = "TargetBack"),
	VE_TargetBackSourceFrontLeft	UMETA(DisplayName = "TargetBackSourceFrontLeft"),
	VE_TargetBackSourceFrontRight	UMETA(DisplayName = "TargetBackSourceFrontRight"),
	VE_ActiveAll					UMETA(DisplayName = "ActiveAll"),
	VE_CheckApplyCharacter			UMETA(DisplayName = "CheckApplyCharacter"),
	VE_InTheAbnormalStateGroup		UMETA(DisplayName = "InTheAbnormalStateGroup"),
	VE_CheckOwnerCharacter			UMETA(DisplayName = "CheckOwnerCharacter"),
};

/*
Raid Boss Attack Pattern Condition
*/

UENUM(BlueprintType)
enum class ERaidBossTermsTypeEnum : uint8
{
	VE_None							UMETA(DisplayName = "None"),
	VE_Loop_Start					UMETA(DisplayName = "Loop_Start"),
	VE_First_Skill_Own				UMETA(DisplayName = "First_Skill_Own"),
	VE_TargetSlotType_Hero 			UMETA(DisplayName = "TargetSlotType_Hero"),
	VE_Remain_HP 					UMETA(DisplayName = "Remain_HP"),
	VE_Spend_Time					UMETA(DisplayName = "Spend_Time"),
	VE_Akhan_Fly_Low				UMETA(DisplayName = "Akhan_Fly_Low"),
	VE_Akhan_Fly_High				UMETA(DisplayName = "Akhan_Fly_High"),
	VE_Akhan_Anger					UMETA(DisplayName = "Akhan_Anger"),
	VE_Attack_Repeat				UMETA(DisplayName = "Attack_Repeat"),
	VE_Phase_Repeat					UMETA(DisplayName = "Phase_Repeat"),
	VE_Phase						UMETA(DisplayName = "Phase"),
	VE_NoTerms						UMETA(DisplayName = "NoTerms"),
	VE_Loop_End						UMETA(DisplayName = "Loop_End"),
	VE_OutOfAttackRange				UMETA(DisplayName = "OutOfAttackRange"),
	VE_Attack_RepeatEndTerm			UMETA(DisplayName = "Attack_RepeatEndTerm"),
	VE_Slot_Over					UMETA(DisplayName = "Slot_Over"),
	VE_Slot_Destroy					UMETA(DisplayName = "Slot_Destroy"),
};



UENUM(BlueprintType)
enum class ERaidBossActionTypeEnum : uint8
{
	VE_None							UMETA(DisplayName = "None"),	
	VE_Ground						UMETA(DisplayName = "Ground"),
	VE_LowFlight					UMETA(DisplayName = "LowFlight"),
	VE_HighFligt					UMETA(DisplayName = "HighFligt"),
	VE_Anger			 			UMETA(DisplayName = "Anger"),	
	VE_OutOfAttackRange				UMETA(DisplayName = "OutOfAttackRange"),
	VE_Shield						UMETA(DisplayName = "Shield"),
	
};



UENUM(BlueprintType)
enum class ECharacterCheckPointEventEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Tick						UMETA(DisplayName = "Tick"),
	VE_Attack					UMETA(DisplayName = "Attack"),
	VE_EnemyDead 				UMETA(DisplayName = "EnemyDead"),
	VE_FriendDead 				UMETA(DisplayName = "FriendDead"),
	VE_ApplySkill				UMETA(DisplayName = "ApplySkill"),
	VE_Attack_Hit				UMETA(DisplayName = "Attack_Hit"),
	VE_Attack_Critical			UMETA(DisplayName = "Attack_Critical"),
	VE_Attack_Hit_Critical		UMETA(DisplayName = "Attack_Hit_Critical"),
	VE_Attack_Miss				UMETA(DisplayName = "Attack_Miss"),
	VE_Attack_Immune			UMETA(DisplayName = "Attack_Immune"),
	VE_Attack_Dodge				UMETA(DisplayName = "Attack_Dodge"),
	VE_Attack_Parry				UMETA(DisplayName = "Attack_Parry"),
	VE_Attack_Start				UMETA(DisplayName = "Attack_Parry"),
	VE_Attack_End				UMETA(DisplayName = "Attack_End"),
	VE_Attack_All				UMETA(DisplayName = "Attack_All"),
	VE_Attacked_Hit				UMETA(DisplayName = "Attacked_Hit"),
	VE_Attacked_Critical		UMETA(DisplayName = "Attacked_Critical"),
	VE_Attacked_Hit_Critical	UMETA(DisplayName = "Attacked_Hit_Critical"),
	VE_Attacked_Miss			UMETA(DisplayName = "Attacked_Miss"),
	VE_Attacked_Immune			UMETA(DisplayName = "Attacked_Immune"),
	VE_Attacked_Dodge			UMETA(DisplayName = "Attacked_Dodge"),
	VE_Attacked_Parry			UMETA(DisplayName = "Attacked_Parry"),
	VE_Attacked_All				UMETA(DisplayName = "Attacked_All"),
	VE_SelfDead 				UMETA(DisplayName = "SelfDead"),
	VE_SpawnTime 				UMETA(DisplayName = "SpawnTime"),
	VE_AdditionalDamage			UMETA(DisplayName = "AdditionalDamage"),
	VE_SkillEnd					UMETA(DisplayName = "SkillEnd"),
	VE_DestroyProtect			UMETA(DisplayName = "DestroyProtect"),
	VE_AttackAttribute_Fire		UMETA(DisplayName = "AttackAttribute_Fire"),
	VE_CheckPointMax,
};

UENUM(BlueprintType)
enum class ESkillDurationType : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Casting		UMETA(DisplayName = "Casting"),
	VE_Channeling	UMETA(DisplayName = "Channeling"),
	VE_All			UMETA(DisplayName = "All"),
};

UENUM(BlueprintType)
enum class EConditionCombineType : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_AND				UMETA(DisplayName = "And"),
	VE_OR				UMETA(DisplayName = "Or"),
};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EScaleValueTypeEnum : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Normal		UMETA(DisplayName = "Normal"),
	VE_100Rate 		UMETA(DisplayName = "100 percent rate"),
};

UENUM(BlueprintType)
enum class EBuffApplyPointEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_HitBullet		UMETA(DisplayName = "HitBullet"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBuffDetachPointEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_ReceiveDamage	UMETA(DisplayName = "ReceiveDamage"),
	VE_HitTarget		UMETA(DisplayName = "HitTarget"),
	VE_Dead				UMETA(DisplayName = "Dead"),
};


/*
Buff enumerations..
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESortTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_All				UMETA(DisplayName = "All"),
	VE_Hero				UMETA(DisplayName = "Hero"),
	VE_Minion			UMETA(DisplayName = "Minion"),
	VE_Continent		UMETA(Displayname = "Continent"),
};

UENUM(BlueprintType)
enum class EDetachBuffEventEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Attack_Hit				UMETA(DisplayName = "Attack_Hit"),
	VE_Attack_Critical			UMETA(DisplayName = "Attack_Critical"),
	VE_Attack_Hit_Critical		UMETA(DisplayName = "Attack_Hit_Critical"),
	VE_Attack_Miss				UMETA(DisplayName = "Attack_Miss"),
	VE_Attack_Immune			UMETA(DisplayName = "Attack_Immune"),
	VE_Attack_Dodge				UMETA(DisplayName = "Attack_Dodge"),
	VE_Attack_Parry				UMETA(DisplayName = "Attack_Parry"),
	VE_Attack_All				UMETA(DisplayName = "Attack_All"),
	VE_AttackerDetachBuffEventMAX,
	VE_Attacked_Hit				UMETA(DisplayName = "Attacked_Hit"),
	VE_Attacked_Critical		UMETA(DisplayName = "Attacked_Critical"),
	VE_Attacked_Hit_Critical	UMETA(DisplayName = "Attacked_Hit_Critical"),
	VE_Attacked_Miss			UMETA(DisplayName = "Attacked_Miss"),
	VE_Attacked_Immune			UMETA(DisplayName = "Attacked_Immune"),
	VE_Attacked_Dodge			UMETA(DisplayName = "Attacked_Dodge"),
	VE_Attacked_Parry			UMETA(DisplayName = "Attacked_Parry"),
	VE_Attacked_All				UMETA(DisplayName = "Attacked_All"),

};

UENUM(BlueprintType)
enum class EBuffPropensityEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Buff				UMETA(DisplayName = "Buff"),
	VE_DeBuff			UMETA(DisplayName = "DeBuff"),
};


//=====================================================================================================
//CharacterAnimNotify
//=====================================================================================================
UENUM(BlueprintType)		
enum class EAttachCharacterObjectEnum : uint8
{
	VE_None 			UMETA(DisplayName = "None"),
	VE_CharacterMesh 	UMETA(DisplayName = "CharacterMesh"),
	VE_AllWeapon		UMETA(DisplayName = "AllWeapon"),
	VE_LWeapon			UMETA(DisplayName = "LWeapon"),
	VE_RWeapon			UMETA(DisplayName = "RWeapon"),
	VE_DecoEffect		UMETA(DisplayName = "DecoEffect"),
	VE_All				UMETA(DisplayName = "All"),
};



//=====================================================================================================
//CharacterInfo
//=====================================================================================================


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EHeroSpawnStateEnum : uint8
{
	VE_Ready 		UMETA(DisplayName = "Ready"),
	VE_Spawned 		UMETA(DisplayName = "Spawned"),
	VE_Despawned 	UMETA(DisplayName = "Despawned"),
	VE_Dead			UMETA(DisplayName = "Dead"),
	VE_None			UMETA(DisplayName = "None")
};


//=====================================================================================================
//Spawner
//=====================================================================================================
UENUM(BlueprintType)
enum class ESpawnerTypeEnum : uint8
{	
	VE_None						UMETA(DisplayName = "None"),
	VE_BossSpawner 				UMETA(DisplayName = "BossSpawner"),
	VE_Normal					UMETA(DisplayName = "Normal"),
	VE_Formation				UMETA(DisplayName = "Formation"),
	VE_RaidBossSpawner 			UMETA(DisplayName = "RaidBossSpawner"),
	VE_BObjectSpawner			UMETA(DisplayName = "BObjectSpawner"),
	VE_ProtectionTargetObject	UMETA(DisplayName = "ProtectionTargetObject"),
	VE_ProtectTarget			UMETA(DisplayName = "ProtectTarget"),
};

UENUM(BlueprintType)
enum class ESpawnerFormationTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Right 			UMETA(DisplayName = "Right"),	
	VE_Left 			UMETA(DisplayName = "Left"),
	VE_Forward 			UMETA(DisplayName = "Forward"),
	VE_Back 			UMETA(DisplayName = "Back"),
	VE_Circle			UMETA(DisplayName = "Circle"),
	VE_SpawnerPos		UMETA(DisplayName = "SpawnerPos"),
	VE_RandomRange		UMETA(DisplayName = "Random range"),
	VE_Custom			UMETA(DisplayName = "Custom"),
};

UENUM(BlueprintType)
enum class ETouchStateTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Spawn			UMETA(DisplayName = "Spawn"),
	VE_CharacterMove	UMETA(DisplayName = "CharacterMove"),
	VE_Drag				UMETA(DisplayName = "Drag"),
};

/*
Item
*/

UENUM(BlueprintType)
enum class EItemInvenTypeEnum : uint8
{
	VE_WeaponItem		UMETA(DisplayName = "WeaponItem"),
	VE_ArmorItem		UMETA(DisplayName = "ArmorItem"),
	VE_MagicArmorItem	UMETA(DisplayName = "MagicArmorItem"),
	VE_AccessoriesItem	UMETA(DisplayName = "AccessoriesItem"),
	VE_JewelryItem		UMETA(DisplayName = "JewelryItem"),
	VE_ExpendablesItem	UMETA(DisplayName = "ExpendablesItem"),
	VE_ConsumableItem	UMETA(DisplayName = "ConsumableItem"),
	VE_GemItem			UMETA(DisplayName = "GemItem"),
	VE_None				UMETA(DisplayName = "None"),
	VE_All				UMETA(DisplayName = "All")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemInventoryTypeEnum : uint8
{
	VE_Normal			UMETA(DisplayName = "Normal"),
	VE_Enchant			UMETA(DisplayName = "Enchant"),
	VE_Grind			UMETA(DisplayName = "Grind"),
	VE_Manufacture		UMETA(DisplayName = "Manufacture"), 
	VE_Sell				UMETA(DisplayName = "Sell"),
	VE_Lock				UMETA(DisplayName = "Lock"),
	VE_GemFilter		UMETA(DisplayName = "GemFilter"),
	VE_BatchGrind		UMETA(DisplayName = "BatchGrind"),
	VE_LevelUp			UMETA(DisplayName = "LevelUp"),
	VE_None				UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EItemSortTypeEnum : uint8
{
	VE_ASC				UMETA(DisplayName = "Ascending"),
	VE_DESC				UMETA(DisplayName = "Descending"),
	VE_None				UMETA(DisplayName = "None"),
};




UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemUITypeEnum : uint8
{
	VE_Weapon			UMETA(DisplayName = "Weapon"),
	VE_Armor			UMETA(DisplayName = "Armor"),
	VE_Pants			UMETA(DisplayName = "Pants"),
	VE_Ring				UMETA(DisplayName = "Ring"),
	VE_Gem				UMETA(DisplayName = "Gem"),
};



//=====================================================================================================
//UI
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECommonPopupType : uint8
{
	VE_OK				UMETA(DisplayName = "OK"),
	VE_OKCANCEL			UMETA(DisplayName = "OKCANCEL"),
	VE_ITEMOK			UMETA(DisplayName = "ITEMOK"),
	VE_Error			UMETA(DisplayName = "Error"),
};

UENUM(BlueprintType)
enum class EProgressTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Layer1 				UMETA(DisplayName = "VE_Layer1"),
	VE_Layer2 				UMETA(DisplayName = "VE_Layer2"),
};

//chat type
UENUM(BlueprintType)
enum class EChatListEnum : uint8
{
	VE_GENERAL = 0			UMETA(DisplayName = "GeneralChannel"),
	VE_BATTLE = 1			UMETA(DisplayName = "BattleChannel"),
	VE_PARTY = 2			UMETA(DisplayName = "PartyChannel"),
	VE_GUILD = 3			UMETA(DisplayName = "GuildChannel"),
	VE_ENUM_MAX = 4,
	VE_NOTIFY = 5			UMETA(DisplayName = "Notify"),
	VE_WHISPER = 6			UMETA(DisplayName = "Whisper"),
	VE_NONE
};
//=====================================================================================================
//PlayerDungeon
//=====================================================================================================
UENUM(BlueprintType)
enum class EPDSocketTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_CharacterType	 	UMETA(DisplayName = "CharacterType"),
	VE_MinonType			UMETA(DisplayName = "MinonType"),
	VE_ObjectType 			UMETA(DisplayName = "ObjectType"),
	VE_BossType 			UMETA(DisplayName = "BossType"),
	VE_Special0ObjectType 	UMETA(DisplayName = "Special0ObjectType"),
	VE_SpawnArea			UMETA(DisplayName = "SpawnAreaType"),
	VE_PatternType			UMETA(DisplayName = "PatternType"),
	VE_Object1Type 			UMETA(DisplayName = "Object1Type"),
	VE_CameraType 			UMETA(DisplayName = "CameraType"),
	VE_BonusMinionType		UMETA(DisplayName = "BonusMinionType"),
	VE_HeroSpawnType		UMETA(DisplayName = "HeroSpawnType"),
};

UENUM(BlueprintType)
enum class EPDBlockMapTypeEnum : uint8
{
	VE_GateType		 		UMETA(DisplayName = "GateType"),
	VE_EventType 			UMETA(DisplayName = "EventType"),
	VE_DefenceType 			UMETA(DisplayName = "DefenceType"),
	VE_MainType 			UMETA(DisplayName = "MainType"),
	VE_GoldType 			UMETA(DisplayName = "GoldType"),
	VE_SmithyType			UMETA(DisplayName = "SmityType"),
	VE_MagicType			UMETA(DisplayName = "MagicType"),
	VE_ProductionMType		UMETA(DisplayName = "ProductionMType"),
	VE_ExitType 			UMETA(DisplayName = "ExitType"),

	VE_UMainType			UMETA(DisplayName = "UMainType"),
	VE_UAlchemyType			UMETA(DisplayName = "UAlchemyType"),
	VE_USmityType			UMETA(DisplayName = "USmityType"),
	VE_UMaterialMineType	UMETA(DisplayName = "UMaterialMineType"),
	VE_UMinionAcademyType	UMETA(DisplayName = "UMinionAcademyType"),

	VE_Max
};

UENUM(BlueprintType)
enum class EPDMapTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_NormalType		 	UMETA(DisplayName = "NormalType"),
	VE_BossType 			UMETA(DisplayName = "BossType"),
};

UENUM(BlueprintType)
enum class EPDSocketStateEnum : uint8
{
	VE_NoneState			UMETA(DisplayName = "NoneState"),
	VE_SpawnState 			UMETA(DisplayName = "SpawnState"),
};

UENUM(BlueprintType)
enum class EPDObjectInfoUIStateEnum : uint8
{
	VE_NoneState			UMETA(DisplayName = "NoneState"),
	VE_InfoState			UMETA(DisplayName = "InfoState"),
	VE_ConfirmState			UMETA(DisplayName = "ConfirmState"),
	VE_RemoveState			UMETA(DisplayName = "RemoveState"),
};

//=====================================================================================================
//Bit Flag
//=====================================================================================================

UENUM(BlueprintType)
enum class ENotifyStateFlagEnum : uint8
{
	BM_None								UMETA(DisplayName = "None"),
	BM_Hero								UMETA(DisplayName = "Hero"),
	BM_Item								UMETA(DisplayName = "Item"),
	BM_Skill							UMETA(DisplayName = "Skill"),
	BM_AllTask							UMETA(DisplayName = "AllTask"),
	BM_Achievement						UMETA(DisplayName = "Achievement"),
	BM_DailyMission						UMETA(DisplayName = "DailyMission"),
	BM_Topbar							UMETA(DisplayName = "Topbar"),
	BM_NewHeroCinematicAnimation		UMETA(DisplayName = "NewHeroCinematicAnimation"),
	BM_Quest_Guide						UMETA(DisplayName = "Quest guide"),
	BM_MailBox							UMETA(DisplayName = "MailBox"),
	BM_Friend							UMETA(DisplayName = "Friend"),
	BM_Notify							UMETA(DisplayName = "Notify"),
	BM_Campaign							UMETA(DisplayName = "Campaign"),
	BM_Raid								UMETA(DisplayName = "Raid"),
	BM_Crusader							UMETA(DisplayName = "Crusader"),
	BM_WeekDungeon						UMETA(DisplayName = "WeekDungeon"),
	BM_Arena							UMETA(DisplayName = "Arena"),
	BM_Colosseum						UMETA(DisplayName = "Colosseum"),
	BM_BattleField						UMETA(DisplayName = "BattleField"),
	BM_BattleGround						UMETA(DisplayName = "BattleGround"),
	//BM_UltimateBattle					UMETA(DisplayName = "UltimateBattle"),
	//BM_RankBattle						UMETA(DisplayName = "RankBattle"),
	// BEGIN Chat
	BM_Chat								UMETA(DisplayName = "Chat"),
	BM_Chat_General						UMETA(DisplayName = "Chat General"),
	BM_Chat_Battle						UMETA(DisplayName = "Chat Battle"),
	BM_Chat_Party						UMETA(DisplayName = "Chat Party"),
	BM_Chat_Guild						UMETA(DisplayName = "Chat Guild"),
	// END Chat
	BM_EquipItemWeapon					UMETA(DisplayName = "EquipItemWeapon"),
	BM_EquipItemArmor					UMETA(DisplayName = "EquipItemArmor"),
	BM_EquipItemPants					UMETA(DisplayName = "EquipItemPants"),
	BM_EquipItemRing					UMETA(DisplayName = "EquipItemRing"),
	BM_EquipItemAwaken					UMETA(DisplayName = "EquipItemAwaken"),
	BM_EquipItemTotal					UMETA(DisplayName = "EquipItemTotal"),
	BM_UpgradeSkill1					UMETA(DisplayName = "UpgradeSkill1"),
	BM_UpgradeSkill2					UMETA(DisplayName = "UpgradeSkill2"),
	BM_UpgradeSkill3					UMETA(DisplayName = "UpgradeSkill3"),
	BM_UpgradeSkill4					UMETA(DisplayName = "UpgradeSkill4"),
	BM_UpgradeSkill5					UMETA(DisplayName = "UpgradeSkill5"),
	BM_UpgradeSkill6					UMETA(DisplayName = "UpgradeSkill6"),
	BM_TotalUpgradeSkill				UMETA(DisplayName = "TotalUpgradeSkill"),
	BM_HeroLevelUp						UMETA(DisplayName = "HeroLevelUp"),
	BM_Shop								UMETA(DisplayName = "Shop"),
	BM_Guild							UMETA(DisplayName = "Guild"),
	BM_GuildAttendance					UMETA(DisplayName = "GuildAttendance"),
	BM_GuildDonate						UMETA(DisplayName = "GuildDonate"),
	BM_Mission							UMETA(DisplayName = "Mission"),
	BM_ItemCraft						UMETA(DisplayName = "ItemCraft"),
	BM_Lobby_Battle						UMETA(DisplayName = "Lobby_Battle"),
	BM_Lobby_Challenge					UMETA(DisplayName = "Lobby_Challenge"),
	BM_Max
};


UENUM(BlueprintType)
enum class ECharacterDecoTypeEnum : uint8
{
	VE_Hpbar				UMETA(DisplayName = "HP Bar"),
	VE_SkillIcon			UMETA(DisplayName = "SkillIcon"),
	VE_DebugPanel			UMETA(DisplayName = "DebugPanel"),
	VE_UIDamage				UMETA(DisplayName = "Damage"),	
	VE_SelectionArea		UMETA(DisplayName = "SelectionArea"),
	VE_SkillArea			UMETA(DisplayName = "SkillArea"),
};

//=====================================================================================================
//HeroList
//=====================================================================================================

UENUM(BlueprintType)
enum class EDeckUsingTypeEnum : uint8
{
	VE_Campaign				UMETA(DisplayName = "Campaign"),
	VE_Arena				UMETA(DisplayName = "Arena"),
	VE_WeekDungeon			UMETA(DisplayName = "Challenge"),
	VE_Raid					UMETA(DisplayName = "Raid"),
	VE_HeroCard				UMETA(DisplayName = "HeroCard"),
	VE_Crusader				UMETA(DisplayName = "Crusader"),
	VE_MirrorDungeon		UMETA(DisplayName = "MirrorDungeon"),
	VE_PlayAlone			UMETA(DisplayName = "PlayAlone"),
	VE_MultiRaid			UMETA(DisplayName = "MultiRaid"),
	VE_Max,
};

//=====================================================================================================
//Campaign 
//=====================================================================================================


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECampaignQuestType : uint8
{
	VE_CampClear	UMETA(DisplayName = "CampClear"),
	VE_ItemGet		UMETA(DisplayName = "ItemGet"),
};

UENUM(BlueprintType)
enum class EDungeonTypeEnum : uint8
{
	VE_Normal			UMETA(DisplayName = "Normal"),
	VE_HeroCard			UMETA(DisplayName = "HeroCard"),
	VE_WeekDungeon		UMETA(DisplayName = "DailyDungeon"),
	VE_None				UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EStringTableType : uint8
{
	VE_Hero					UMETA(DisplayName = "Hero"),
	VE_Npc					UMETA(DisplayName = "Npc"),
	VE_Item					UMETA(DisplayName = "Item"),
	VE_Skill				UMETA(DisplayName = "Skill"),
	VE_BattleObject			UMETA(DisplayName = "BattleObject"),
	VE_Campaign				UMETA(DisplayName = "Campaign"),
	VE_PD					UMETA(DisplayName = "PD"),
	VE_Common				UMETA(DisplayName = "Common"),
	VE_Unit					UMETA(DisplayName = "Unit"),
	VE_UI					UMETA(DisplayName = "UI"),
	VE_Error				UMETA(DisplayName = "Error"),
	VE_Shop					UMETA(DisplayName = "Shop"),
	VE_Dialogue				UMETA(DisplayName = "Dialogue"),
	VE_Tooltip				UMETA(DisplayName = "Tooltip"),
	VE_SystemMessage		UMETA(DisplayName = "SystemMessage"),
	VE_Name					UMETA(Displayname = "Name"),
	VE_Description			UMETA(Displayname = "DescriptionString"),
	VE_Subtitle				UMETA(Displayname = "Subtitle"),
};

UENUM(BlueprintType)
enum class EHeroOwnershipType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Me					UMETA(DisplayName = "Me"),
	VE_Arena				UMETA(DisplayName = "Arena"),	
	VE_Colosseum			UMETA(DisplayName = "Colosseum"),
	VE_Mirror				UMETA(DisplayName = "Mirror"),
	VE_Boss					UMETA(DisplayName = "Boss"),
};

//=====================================================================================================
//UI
//=====================================================================================================
UENUM(BlueprintType)
enum class ELanguageType : uint8
{
	VE_KOR		UMETA(DisplayName = "Korea"),
	VE_ENG		UMETA(DisplayName = "English"),
	VE_CHN		UMETA(DisplayName = "Chiana"),
	VE_TWN		UMETA(DisplayName = "Taiwan"),
	VE_MAX,
};


//=====================================================================================================
//Game Option
//=====================================================================================================
UENUM(BlueprintType)
enum class EOptionType : uint8
{
	VE_LANGUAGE		UMETA(DisplayName = "Laguage"),
	VE_GRAPHIC		UMETA(DisplayName = "Graphic"),
	VE_SOUND		UMETA(DisplayName = "Sound"),
	VE_MAX,
};

//=====================================================================================================
//AI
//=====================================================================================================
UENUM(BlueprintType)
enum class EWaypointMoveType : uint8
{
	VE_NormalMove		UMETA(DisplayName = "NormalMove"),
	VE_PatrolMove		UMETA(DisplayName = "PatrolMove"),
};

UENUM(BlueprintType)
enum class EAIControlModeEnum : uint8
{
	VE_ManualMode			UMETA(DisplayName = "ManualMode"),
	VE_SemiAutoMode			UMETA(DisplayName = "SemiAutoMode"),
	VE_AutoMode				UMETA(DisplayName = "AutoMode"),
};


//=====================================================================================================
//Tutorial
//=====================================================================================================

UENUM(BlueprintType) //check modify MakeIconArray
enum class EHudIcon : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	//VE_Hero_Ally			UMETA(DisplayName = "None"),//
	VE_HeroIcon1			UMETA(DisplayName = "HeroIcon1"),
	VE_HeroIcon2			UMETA(DisplayName = "HeroIcon2"),
	VE_HeroIcon3			UMETA(DisplayName = "HeroIcon3"),
	VE_HeroIcon4			UMETA(DisplayName = "HeroIcon4"),
	VE_HeroIcon5			UMETA(DisplayName = "HeroIcon5"),
	//VE_HeroSkill_Ally		UMETA(DisplayName = "None"),//
	/*VE_HeroSkillCoolIcon1	UMETA(DisplayName = "HeroSkillCoolIcon1"),
	VE_HeroSkillCoolIcon2	UMETA(DisplayName = "HeroSkillCoolIcon2"),
	VE_HeroSkillCoolIcon3	UMETA(DisplayName = "HeroSkillCoolIcon3"),
	VE_HeroSkillCoolIcon4	UMETA(DisplayName = "HeroSkillCoolIcon4"),
	VE_HeroSkillCoolIcon5	UMETA(DisplayName = "HeroSkillCoolIcon5"),*/
	//VE_HpBarSwitcher		UMETA(DisplayName = "None"),//
	VE_BossHPBar			UMETA(DisplayName = "BossHPBar"),
	VE_CameraIcon			UMETA(DisplayName = "CameraIcon"),
	VE_OptionMenu			UMETA(DisplayName = "OptionMenu"),
	VE_SkillSet				UMETA(DisplayName = "SkillSet"),//
	VE_SkillIcon1			UMETA(DisplayName = "SkillIcon1"),
	VE_SkillIcon2			UMETA(DisplayName = "SkillIcon2"),
	VE_SkillIcon3			UMETA(DisplayName = "SkillIcon3"),
	VE_CancelIcon			UMETA(DisplayName = "CancelIcon"),
	VE_FocusAttackIcon		UMETA(DisplayName = "FocusAttackIcon"),
	VE_SearchIcon			UMETA(DisplayName = "SearchIcon"),
	VE_AttackIcon			UMETA(DisplayName = "AttackIcon"),
	VE_AvoidIcon			UMETA(DisplayName = "AvoidIcon"),
	//VE_AutoIcon				UMETA(DisplayName = "AutoIcon"),
	VE_AutoSkillIcon		UMETA(DisplayName = "AutoSkillIcon"),
	//VE_MeetingIcon			UMETA(DisplayName = "MeetingIcon"),
	VE_MovePad				UMETA(DisplayName = "MovePad"),
	//VE_SpecialIcon			UMETA(DisplayName = "None"),
	VE_All,
	
	VE_HeroIcon_Skill1		UMETA(DisplayName = "HeroIcon_Skill1"),
	VE_HeroIcon_Skill2		UMETA(DisplayName = "HeroIcon_Skill2"),
	VE_HeroIcon_Skill3		UMETA(DisplayName = "HeroIcon_Skill3"),
	VE_HeroIcon_Skill4		UMETA(DisplayName = "HeroIcon_Skill4"),
	VE_HeroIcon_Skill5		UMETA(DisplayName = "HeroIcon_Skill5"),

	VE_Screen				UMETA(DisplayName = "Screen"),
	VE_Skip					UMETA(DisplayName = "Skip"),
};

UENUM(BlueprintType)
enum class ESubHUDState : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_PlayMovie			UMETA(DisplayName = "Right"),
	VE_IconHighLight		UMETA(DisplayName = "Both"),
	VE_Loading				UMETA(DisplayName = "Both"),
	VE_Hide,
	VE_OnDialog,
	VE_OffDialog,
};

UENUM(BlueprintType)
enum class ECommandSceneType : uint8
{
	VE_NoneChange 	UMETA(DisplayName = "NoneChange"),
	VE_Off		  	UMETA(DisplayName = "Off"),
	VE_On		  	UMETA(DisplayName = "On"),
};

UENUM(BlueprintType)
enum class EJudgeType : uint8
{
	VE_None 		UMETA(DisplayName = "None"),
	VE_Excellent	UMETA(DisplayName = "Excellent"),
	VE_Good		  	UMETA(DisplayName = "Good"),
	VE_Miss		  	UMETA(DisplayName = "Miss"),
};

UENUM(BlueprintType)
enum class EScenePhaseType : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_MakeScene				UMETA(DisplayName = "MakeScene"),
	VE_LoadingScene				UMETA(DisplayName = "LoadingScene"),
	VE_StartScene				UMETA(DisplayName = "StartScene"),
	VE_DelayScene				UMETA(DisplayName = "DelayScene"),
	VE_CommandScene				UMETA(DisplayName = "CommandScene"),
	//
	VE_InitCharacterCustom		UMETA(DisplayName = "InitCharacterCustom"),
	VE_InitSpawner				UMETA(DisplayName = "InitSpawner"),
	VE_InitSpawnSlot			UMETA(DisplayName = "InitSpawnSlot"),
	VE_InitIcon					UMETA(DisplayName = "InitIcon"),
	VE_InitEffect				UMETA(DisplayName = "InitEffect"),
	VE_InitNotiIndicator		UMETA(DisplayName = "InitNotiIndicator"),
	VE_InitNotiDialogue			UMETA(DisplayName = "InitNotiDialogue"),
	VE_InitForceSpawn			UMETA(DisplayName = "InitForceSpawn"),
	VE_InitWallSpawn			UMETA(DisplayName = "InitWallSpawn"),
	VE_InitCameraArm			UMETA(DisplayName = "InitCameraArm"),
	VE_InitTrigger				UMETA(DisplayName = "InitTrigger"),
	VE_InitViewImage			UMETA(DisplayName = "InitViewImage"),
	VE_InitCameraShake			UMETA(DisplayName = "InitCameraShake"),
	VE_InitJudge				UMETA(DisplayName = "InitJudge"),
	//
	VE_PlayMatinee				UMETA(DisplayName = "PlayMatinee"),
	VE_PlaySequence				UMETA(DisplayName = "PlaySequence"),
	VE_PlayMovie				UMETA(DisplayName = "PlayMovie"),
	VE_PlayCameraMove			UMETA(DisplayName = "PlayCameraMove"),
	VE_PlayMove					UMETA(DisplayName = "PlayMove"),
	VE_PlayAnimation			UMETA(DisplayName = "PlayAnimation"),
	VE_PlayHitAnimation			UMETA(DisplayName = "PlayHitAnimation"),
	VE_PlayBattle				UMETA(DisplayName = "PlayBattle"),
	VE_PlaySkill				UMETA(DisplayName = "PlaySkill"),
	VE_PlayTargetHP				UMETA(DisplayName = "PlayTargetHP"),
	VE_PlayJudgeResult			UMETA(DisplayName = "PlayJudgeResult"),
	VE_PlayTimeMotion			UMETA(DisplayName = "PlayTimeMotion"),
	//
	VE_InputIcon				UMETA(DisplayName = "InputIcon"),
	VE_InputLocation			UMETA(DisplayName = "InputLocation"),
	VE_InputDialogue			UMETA(DisplayName = "InputDialogue"),
	VE_InputSkillLocation		UMETA(DisplayName = "InputSkillLocation"),
	VE_InputSpawn				UMETA(DisplayName = "InputSpawn"),
	VE_EndScene					UMETA(DisplayName = "EndScene"),

	//temp
	VE_ForceCache				UMETA(DisplayName = "ForceCache"),
};

//=====================================================================================================
//Dialogue
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDialogueCharPosition : uint8
{
	VE_Left					UMETA(DisplayName = "Left"),
	VE_Right				UMETA(DisplayName = "Right"),
	VE_Center				UMETA(DisplayName = "Center"),
	VE_Both					UMETA(DisplayName = "Both"),
	VE_None					UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EDialogueAnimationType : uint8
{
	VE_None 				UMETA(DisplayName = "None"),
	VE_Idle 				UMETA(DisplayName = "Idle"),
	VE_LobbyIdle 			UMETA(DisplayName = "LobbyIdle"),
	VE_LobbySocial 			UMETA(DisplayName = "LobbySocial"),
	VE_LobbySpecial 		UMETA(DisplayName = "LobbySpecial"),
	VE_BattleReady01 		UMETA(DisplayName = "BattleReady"),
	VE_BattleWin01 			UMETA(DisplayName = "BattleWin"),
	VE_BattleEnd 			UMETA(DisplayName = "BattleEnd"),
};

//=====================================================================================================
//Attendance
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAttendanceType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Month				UMETA(DisplayName = "Month"),
	VE_Week					UMETA(DisplayName = "Week"),
	
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAttendanceUserType : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_New					UMETA(DisplayName = "New"),
	VE_Common				UMETA(DisplayName = "Common"),
	VE_Return				UMETA(DisplayName = "Return"),

};


//=====================================================================================================
// Controller
//=====================================================================================================

UENUM(BlueprintType)
enum class EDPadControllerStateFlagEnum : uint8
{
	VE_None = 0				UMETA(DisplayName = "None"),
	VE_Idle = 1				UMETA(DisplayName = "1.Idle"),
	VE_Move = 2				UMETA(DisplayName = "2.Move"),
	VE_Attack = 4			UMETA(DisplayName = "3.Attack"),
	VE_AttackHold = 8		UMETA(DisplayName = "4.AttackHold"),
	VE_Skill = 16			UMETA(DisplayName = "5.Skill"),
	VE_SkillMove = 32		UMETA(DisplayName = "6.SkillMove"),
};

//=====================================================================================================
// ContentsUnLock
//=====================================================================================================


//=====================================================================================================
//Input
//=====================================================================================================

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EInputCommandTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_LeftAxis				UMETA(DisplayName = "LeftAxis"),
	VE_RightAxis			UMETA(DisplayName = "RightAxis"),
	VE_Attack				UMETA(DisplayName = "Attack"),
	VE_AttackHold			UMETA(DisplayName = "AttackHold"),
	VE_AttackRelease		UMETA(DisplayName = "AttackRelease"),
	VE_SkillPress			UMETA(DisplayName = "SkillPress"),
	VE_SkillRelease			UMETA(DisplayName = "SkillRelease"),
	VE_ForceAttackStop		UMETA(DisplayName = "ForceAttackStop"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDragInfo
{
	GENERATED_USTRUCT_BODY()

	FVector		StartPos;
	FVector		LastPos;
	FVector		CurrentPos;
	float		DeltaDist;
	float		TotalDeltaDist;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPinchInfo
{
	GENERATED_USTRUCT_BODY()

	FDragInfo	FingerIndex1;
	FDragInfo	FingerIndex2;
	float		BothFingerDist;
	float		DeltaDist;	
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class  EVideoQualityEnum : uint8
{
	VE_LOW = 0,	
	VE_MID = 1,
	VE_HIGH = 2,
	VE_VERYHIGH = 3,
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class  EDayOfWeekTypeEnum : uint8
{	
	VE_Monday			UMETA(DisplayName = "VE_Monday"),
	VE_Tuesday			UMETA(DisplayName = "VE_Tuesday"),
	VE_Wednesday		UMETA(DisplayName = "VE_Wednesday"),
	VE_Thursday			UMETA(DisplayName = "VE_Thursday"),
	VE_Friday			UMETA(DisplayName = "VE_Friday"),
	VE_Saturday			UMETA(DisplayName = "VE_Saturday"),
	VE_Sunday			UMETA(DisplayName = "VE_Sunday"),
	VE_Max				UMETA(DisplayName = "VE_Max"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class  ESequenceActorType : uint8
{
	VE_General,
	VE_UltimateSkill,
};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class  ESkillShapedIconType : uint8
{
	VE_Target			UMETA(DisplayName = "Target"),
	VE_Circle			UMETA(DisplayName = "Circle"),
	VE_Box				UMETA(DisplayName = "Box"),
	VE_Sector			UMETA(DisplayName = "Sector"),
	VE_TargetLocation	UMETA(DisplayName = "TargetLocation"),
	VE_TargetCircle		UMETA(DisplayName = "TargetCircle"),
	VE_Me				UMETA(DisplayName = "Me"),
	VE_Friend			UMETA(DisplayName = "Friend"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class  EConstantType : uint8
{
	VE_INT,
	VE_FLOAT,
	VE_STRING,
};

// EDITOR
UENUM(BlueprintType)
enum class EReleaseEnv: uint8
{
	None, // No override
	DEV,
	DESIGN,
	QA,
	LIVE,
	LOCAL
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGroupStateType : uint8
{
	VE_ReSpawn		UMETA(DisplayName = "ReSpawn"),
	VE_Member		UMETA(DisplayName = "Member"),
	VE_Master		UMETA(DisplayName = "Master"),
	VE_PartyMove	UMETA(DisplayName = "PartyMove"),
	VE_Delegation	UMETA(DisplayName = "Delegation"),
};

UENUM(BlueprintType)
enum class EPartyRole : uint8
{
	Leader, Crew1_First, Crew1_Second, Crew2_First, Crew2_Second, None
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillIconState : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Normal			UMETA(DisplayName = "Normal"),
	VE_Locked			UMETA(DisplayName = "Locked"),
	VE_InCoolTime		UMETA(DisplayName = "InCoolTime"),
	VE_WaitTarget		UMETA(DisplayName = "WaitTarget"),
	VE_InCrewCoolTime	UMETA(DisplayName = "InCrewCoolTime"),
	VE_InSpiritCoolTime	UMETA(DisplayName = "InSpiritCoolTime"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESkillOrigin : uint8
{
	VE_Leader			UMETA(DisplayName = "Leader"),
	VE_Crew1			UMETA(DisplayName = "Crew1"),
	VE_Crew2			UMETA(DisplayName = "Crew2"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUpgradeCondition : uint8
{
	None				UMETA(DisplayName = "None"),
	Hero_Class			UMETA(DisplayName = "Hero_Class"),
	Spirit_Class		UMETA(DisplayName = "Spirit_Class"),
	Specific			UMETA(DisplayName = "Specific"),
};

// 컨텐츠 첫진입체크할때 인덱스로 사용할 enum
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFirstCheckContentsType : uint8
{
	FirstForceTutorial = 0,
	FirstHeroManagement,
	FirstArena,
	FirstColosseum,
	FirstSingleRaid,
	FirstStageHard1_1,
	FirstSingleRaid_DeckSetting,
	FirstStageFail,
};

UENUM(BlueprintType)
enum class ERaidType : uint8
{
	SingleRaid,
	MultiRaid,
};

/**
 *
 */

#define MovePauseLimitSecond							5.0f
#define DefenceRateMultiplyConstantWithLevel			10

//constant
#define MAX_FLOAT					100000000.0f
#define TIMER_ACTIVE				0.1f

//Socket
#define PARAMETER_ACTOR	FName(TEXT("Actor"))
#define SOCKET_ROOT		FName(TEXT("Socket_Root"))
#define SOCKET_DAMAGE	FName(TEXT("Socket_Damage"))
#define SOCKET_PARRY	FName(TEXT("Socket_Parry"))
#define SOCKET_SHOOT	FName(TEXT("Socket_Shoot"))
#define SOCKET_SHOOTR	FName(TEXT("Socket_ShootR"))
#define SOCKET_SHOOTL	FName(TEXT("Socket_ShootL"))
#define SOCKET_STATE	FName(TEXT("Socket_State"))
#define SOCKET_LWEAPON	FName(TEXT("Socket_LeftWeapon"))
#define SOCKET_RWEAPON	FName(TEXT("Socket_RightWeapon"))
#define EXCEPTION_ZERO	FName(TEXT("0"))
#define EXCEPTION_NONE	FName(TEXT("None"))
#define SOCKET_FLOOR	FName(TEXT("Socket_Floor"))

#define STATICM_CIRCLE		FName(TEXT("Circle"))
#define STATICM_CIRCLENEW	FName(TEXT("Circle_New"))
#define STATICM_SELECTION	FName(TEXT("SelectionMesh"))
#define STATICM_SYNERGY		FName(TEXT("SelectionGlowMesh"))
#define STATICM_TARGETING	FName(TEXT("EnemyGlowMesh"))

#define PARAMETER_BASE_TEXTURE		FName("BaseTexture")
#define PARAMETER_DIFFUSE_TEXTURE	FName("DiffuseTexture")
#define PARAMETER_OPACITY			FName("Opacity")
#define PARAMETER_RADIUS			FName("Radius")
#define PARAMETER_TINTCOLOR			FName("TintColor")
#define PARAMETER_COLORANDOPACITY	FName("ColorAndOpacity")
#define PARAMETER_TRANSLUCENT		FName("Translucent")
#define PARAMETER_EFFECT_AREA		FName("DecalRadius")
#define PARAMETER_EFFECT_PARTICLE	FName("ParticleRadius")
#define PARAMETER_EFFECT_BUFFCOUNT	FName("BuffCount")

#define BLACKBOARDKEY_TARGETLOCATION	FName(TEXT("TargetLocation"))
#define BLACKBOARDKEY_FORCETARGET		FName(TEXT("ForceTarget"))
#define BLACKBOARDKEY_LEADERTOFOLLOW	FName(TEXT("LeaderToFollow"))

#define CUSTOMCOLORHEX_GREEN	FString("44FF24FF")
#define CUSTOMCOLORHEX_RED		FString("FA2307FF")
#define CUSTOMCOLORHEX_BLUE		FString("2D61FDFF")
#define CUSTOMCOLORHEX_YELLOW	FString("FFEB83FF")
#define CUSTOMCOLORHEX_ORANGE	FString("FF872900")

#define SKILLAREA_MAX			4000.0f
#define COMMANDSLOT_MAX			5
#define APPLYBUFF_MAX			4
#define PERCENT_MAX				1
#define PERCENT_RATEMAX			100.0f
#define SKILLBUFF_MAX			4
#define RADIUSSIZE_DEFAULT		96.0f
#define RADIUSSIZE_BLACKHOLE	500
#define RADIUSSIZE_SHADOW		300
#define DEADEFFECT_TIME			2.0f
#define MAIL_MAX_REWARDCOUNT	8
#define NET_SEARCHSIZE			1600.0f
#define SYNERGYICON_MAX			3

//Log
#if WITH_EDITOR
	#define LOG_BATTLE			0
	#define LOG_SKILL			0
	#define LOG_BUFF			0
	#define LOG_BULLET			0
	#define LOG_CHARACTER		0
	#define LOG_CHARACTERTYPE	0
	#define LOG_ANIMNOTIFY		0
	#define LOG_ANIMSTATE		0
	#define LOG_INPUT			0
	#define LOG_UIINPUT			0
	#define LOG_UI				0
	#define LOG_AI				0
	#define LOG_SEQUENCE		0
	#define LOG_STATE			0
	#define LOG_SKILLPAD		0
	#define LOG_SKILLDEBUG		0
	#define LOG_TIME			1
	#define LOG_GAMESTATE		0
	#define LOG_COLOSSEUM		0
	#define LOG_BATTLEFIELD		0
	#define LOG_BATTLEGROUND	0
	#define LOG_NET_REALTIME	0
	#define LOG_NET_RTS			0
	#define LOG_FILE			1
#else
	#define LOG_BATTLE			0
	#define LOG_SKILL			0
	#define LOG_BUFF			0
	#define LOG_BULLET			0
	#define LOG_CHARACTER		0
	#define LOG_CHARACTERTYPE	0
	#define LOG_ANIMNOTIFY		0
	#define LOG_ANIMSTATE		0
	#define LOG_INPUT			0
	#define LOG_UIINPUT			0
	#define LOG_UI				0
	#define LOG_AI				0
	#define LOG_SEQUENCE		0
	#define LOG_STATE			0
	#define LOG_SKILLPAD		0
	#define LOG_SKILLDEBUG		0
	#define LOG_TIME			0
	#define LOG_GAMESTATE		0
	#define LOG_COLOSSEUM		0
	#define LOG_BATTLEFIELD		0
	#define LOG_BATTLEGROUND	0
	#define LOG_NET_REALTIME	0
	#define LOG_NET_RTS			0
	#define LOG_FILE			1
#endif

#define ENABLE_FORCE_GC			0

#define PLAYPD					1
#define LOGFLAG_HERO			1

//Hero
#define MAX_HERO_GRADE						5
//#define AWAKEN_LEVEL						50
#define MAX_HERO_LEVEL						100


// KID
#define KID_NONE				0
#define KID_NPC_MY				0
#define KID_NPC_ENEMY			-1
#define KID_TEST_MY				1
#define KID_COLOSSEUM_AI		-1

// GroupKey
#define GroupKey_SINGLE_ENEMY	-1
#define GroupKey_SINGLE_NONE	0
#define GroupKey_SINGLE_MY		1

#define GroupKey_COLOSSEUM_LEFT		1
#define GroupKey_COLOSSEUM_RIGHT	2

#define GroupKey_BATTLEGROUND_LEFT	1
#define GroupKey_BATTLEGROUND_RIGHT	2

#define GroupKey_REALTIME_NPC	-1

// PartyID
#define PartyID_NONE			-1
#define PartyID_PC_MY			0
#define PartyID_NPC_ENEMY		1
#define PartyID_NPC_HELP		2

#define PartyID_AUTO_1			PartyID_PC_MY

#define PartyID_COLOSSEUM_LEFT	1
#define PartyID_COLOSSEUM_RIGHT	11

// RealTime
#define REALTIME_PARTYID_NONE	0

// HeroUD
#define	HERO_UD_NONE			FString("")

//Item
#define MAX_ITEM_GRADE		5
#define MAX_ENCHANT_ITEM_MATERIAL	3
#define MAX_GRIND_COUNT		12
#define MAX_DROP_ITEM_NUMBER	12

//GameRule
//#define RULE_TARGETING_KEEPSAFETARGET 0
//#define RULE_USEITEMGEM 0
//#define RULE_AWAKEN	0
#define RULE_TWOTOUCHBUTTON	1
//#define RULE_TESTCONSTRUCT	0
//#define RULE_TESTCHANGE	0
//#define RULE_TESTWIDGET		0
//#define RULE_TESTDELETE		0
//#define RULE_TIMERACTOR		0
//#define RULE_TODOMEMORY		1

//#define RULE_SUBMARINE		0


//#pragma warning(disable: 4946)  //rts_generated.h ...


//Campaign
#define MAX_CAMPAIGN_CHAPTER_COUNT						16
#define MAX_CAMPAIGN_EPISODE_COUNT						4
#define MAX_CAMPAIGN_STAGE_COUNT						6
#define MAX_STAGE_COUNT_IN_EPISODE						6
#define STAGE_PROGRESSBAR_EACH_VALUE					0.25f
#define MAX_STAGE_MONSTER								5 
#define MAX_REWARD_ITEM_COUNT							5
#define CAMPAIGN_STAGE_LIMIT_TIME						300.0f
#define CAMPAIGN_STAGE_SUPER_CLEAR_SPARSE_TIME			30
#define CAMPAIGN_STAGE_ULTIMATE_CLEAR_SPARSE_TIME		60
#define CAMPAIGN_STAGE_GRADE2_CLEAR_ALIVE_HERO_COUNT	1
#define CAMPAIGN_STAGE_GRADE3_CLEAR_ALIVE_HERO_COUNT	0
#define CAMPAIGN_DIFFICULTLY_ADD_LEVEL					5
//#define KOKOMO_ASYNCCACHE								1

//ComponentTag
#define TAG_TARGETINGMESH				FName("TargetingMesh")
//#define TAG_ATTACKAREAMESH				FName("AttackAreaMesh")
#define TAG_HIDEMESH					FName("HideMesh")
#define TAG_NOSOUND						FName("TAG_NOSOUND")
#define TAG_KILLACTOR					FName("SequencerActor")
#define TAG_TRIGGER1					FName("TRIGGER1")
#define TAG_TRIGGER2					FName("TRIGGER2")

//CommonString
#define COMMONSTRING_WARNING			FName("UI_Warning")
#define COMMONSTRING_NOTIFY				FName("UI_Notify")
#define COMMONSTRING_ASK_BATTLE_QUIT	FName("UI_AskQuit")
#define COMMONSTRING_ASK_GAME_QUIT		FName("UI_EndAskPopup")

//MatineeString
#define MATINEE_NOTI_END			FName("EndMatinee")
#define MATINEE_NOTI_FINISHED		FName("Finished")
#define MATINEE_BOSS_SPAWN			FName("BossSpawnMatinee")

// Sequence String
#define SEQUENCE_ATTACH_GENETAL_ACTOR		FName("Sequence_Target_Actor")
#define SEQUENCE_ATTACH_ULTIMATE_ACTOR		FName("Ultimate_Actor")
#define SEQUENCE_ULTIMATE_POSTFIX			FString("_Ultimate_Sequence")
#define LOBBYMAP_CHARACTERMAP				FName("CharacterSet")

#define SEQUENCE_STAGE_CLEAR				FName("Stage_Clear_Sequence")
#define SEQUENCE_VICTORY					FName("Victory_Sequence")
#define SEQUENCE_GETNEWHERO					FName("GetNewHero_Sequence")

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameModeSequenceTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Arena			UMETA(DisplayName = "Arena"),
	VE_Colosseum		UMETA(DisplayName = "Colosseum"),
	VE_Crusader			UMETA(DisplayName = "Crusader"),
	VE_Akhan			UMETA(DisplayName = "Akhan"),
	VE_Gigant			UMETA(DisplayName = "Gigant"),
	VE_Mors				UMETA(DisplayName = "Mors"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTestItemInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FTestItemInfo)
		FName					ItemKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FTestItemInfo)
		TArray<FString>			GemList;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayTestDeck
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPlayTestDeck)
		FName				Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPlayTestDeck)
		int32				Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPlayTestDeck)
		TArray<int32>		SkillLevelList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPlayTestDeck)
		TArray<FTestItemInfo>		ItemList;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlayTestDecks
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPlayTestDecks)
		TArray<FPlayTestDeck>				DeckMemberList;
};

//=====================================================================================================
//Constant Info
//=====================================================================================================


// CustomActor Resource
#define RES_CUSTOMACTOR_ULTIMATE_RETURN_CAMERA		FName("UltimateReturnCamera")

//ArenaResult
#define ARENA_WIN_POINT			100
#define ARENA_LOSE_POINT		20
#define ARENA_CONT_POINT		10

//ColosseumResult
#define COLOSSEUM_CONT_POINT		2

//PD
#define PATTERN_ROW		5
#define PATTERN_COL		5
#define PATTERN_MAX		PATTERN_ROW * PATTERN_COL


//Game Info
#define PD_MAX_HERO_COUNT				10
#define CAMPAIGN_NORMAL_MAX_HERO_COUNT	5
#define CAMPAIGN_HARD_MAX_HERO_COUNT	5
#define CHALLANGE_MAX_HERO_COUNT		5
#define RAID_MAX_HERO_COUNT				5
#define ARENA_MAX_HERO_COUNT			5
#define COLOSSEUM_MAX_HERO_COUNT		5
#define CRUSADER_MAX_HERO_COUNT			5
#define REALTIME_MAX_HERO_COUNT			5
#define MIRRORDUNGEON_MAX_HERO_COUNT	5
#define PLAYALONE_MAX_HERO_COUNT		5
#define EPISODE_ALLCLEAR_STAR			18
#define DEF_MAX_HERO_LEVEL				150
#define	dMAIN_SLOT_HERO					0
#define DEF_MAX_HERO_COUNT				300
#define ITEMMAX_COUNT					100
// Party Role Index
#define ROLE_LEADER_INDEX				0
#define ROLE_CREW1_BATTLE_INDEX			1
#define ROLE_CREW1_REST_INDEX			2
#define ROLE_CREW2_BATTLE_INDEX			3
#define ROLE_CREW2_REST_INDEX			4

//Dialogue
#define WIDGETID_DIALOGUEUI				FName("WBP_Dialogue")
#define WIDGETID_MEDIAFRAMEWORKUI		FName("WBP_MediaFrameWork")
#define TEMP_RTS_SINGLE_RAID_STAGE_ID	FName("S_AKHAN_EASY")
#define TEMP_RTS_SINGLE_RAID_MAP		FName("35")

// Skill Target Search Type Mask
const int32 APPLY_TARGET_NONE = 0;
const int32 APPLY_TARGET_FRIEND = 0x01;
const int32 APPLY_TARGET_ENEMY = 0x02;
const int32 APPLY_TARGET_FRIEND_MAGIC = 0x04;
const int32 APPLY_TARGET_FRIEND_PHYSIC = 0x08;

// item gem
const int32 MAX_GEM_ITEM_COUNT = 3;

//#define	DEF_UI_AUTO_SIZE_FIT	1

#if UE_BUILD_SHIPPING
#define TEST_PROJECTR_MEMORY_SKIP_TEST 0
#define DISABLE_SPAWN_BGM_ACTOR 0
#else
#define TEST_PROJECTR_MEMORY_SKIP_TEST 0
#define DISABLE_SPAWN_BGM_ACTOR 0
#endif