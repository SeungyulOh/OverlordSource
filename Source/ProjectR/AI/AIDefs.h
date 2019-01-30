// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIDefs.generated.h"



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIStateTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "0.None"),
	VE_Idle					UMETA(DisplayName = "1.Idle"),
	VE_MoveToTarget			UMETA(DisplayName = "2.MoveToTarget"),
	VE_MoveToLocation		UMETA(DisplayName = "3.MoveToLocation"),
	VE_MoveToAttackLocation	UMETA(DisplayName = "4.MoveToAttackLocation"),
	VE_Attack				UMETA(DisplayName = "5.Attack"),
	VE_SkillCasting			UMETA(DisplayName = "6.SkillCasting"),
	VE_SkillActing			UMETA(DisplayName = "7.SkillActing"),
	VE_AbnormalState		UMETA(DisplayName = "8.AbnormalState"),
	VE_FollowLeader			UMETA(DisplayName = "9.FollowLeader"),
	VE_AvoidToLocation		UMETA(DisplayName = "10.AvoidToLocation"),
	VE_ApproachToTarget		UMETA(DisplayName = "11.ApproachToTarget"),
	VE_CanSkillCasting		UMETA(DisplayName = "12.CanSkillCasting"),
	VE_TaticsMove			UMETA(DisplayName = "13.CanSkillCasting"),

	VE_Max,
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIFSMTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Normal				UMETA(DisplayName = "Normal"),
	VE_ControlledHero		UMETA(DisplayName = "Controlled Hero"),
//	VE_Akhan				UMETA(DisplayName = "Akhan"),
	VE_Remote				UMETA(DisplayName = "Remote"),
	VE_Max,
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIActionTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Move					UMETA(DisplayName = "Move"),
	VE_Skill				UMETA(DisplayName = "Skill"),
	VE_TargetChange			UMETA(DisplayName = "TargetChange"),	
	VE_Buff					UMETA(DisplayName = "Buff"),
	Ve_Roaming				UMETA(DisplayName = "Roaming"),
	VE_Max,
};

UENUM(BlueprintType)		// Apply target type
enum class EAIActionTargetTypeEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Me						UMETA(DisplayName = "Me"),
	VE_Friend					UMETA(DisplayName = "Friend"),
	VE_Enemy 					UMETA(DisplayName = "Enemy"),
	VE_FriendCorpse				UMETA(DisplayName = "FriendCorpse"),
	VE_EnemyCorpse				UMETA(DisplayName = "EnemyCorpse"),
	VE_FriendLowestHP			UMETA(DisplayName = "FriendLowestHP"),
	VE_FriendLowestMP			UMETA(DisplayName = "FriendLowestMP"),
	VE_EnemyLowestHP			UMETA(DisplayName = "EnemyLowestHP"),
	VE_EnemyLowestMP			UMETA(DisplayName = "EnemyLowestMP"),
	VE_EnemyFar					UMETA(DisplayName = "EnemyFar"),
	VE_EnemyCurrent				UMETA(DisplayName = "EnemyCurrent"),
	VE_EnemyHealer				UMETA(DisplayName = "EnemyHealer"),
	VE_FriendExceptMe			UMETA(DisplayName = "FriendExceptMe"),
	VE_CurrentTarget			UMETA(DisplayName = "CurrentTarget"),
	VE_FriendMagic				UMETA(DisplayName = "FriendMagic"),
	VE_FriendPhysical			UMETA(DisplayName = "FriendPhysical"),
	VE_FriendTeam				UMETA(DisplayName = "FriendTeam"),
	VE_MySummon					UMETA(DisplayName = "MySummon"),

};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIStrategyTypeEnum : uint8
{
	VE_None										UMETA(DisplayName = "None"),
	VE_Escape									UMETA(DisplayName = "Escape"),
	VE_ConditionalTargetChange					UMETA(DisplayName = "ConditionalTargetChange"),
	VE_KeepDistance								UMETA(DisplayName = "KeepDistance"),
	VE_Roaming									UMETA(DisplayName = "Roaming"),
	VE_Patrol									UMETA(DisplayName = "Patrol"),
	VE_Max,
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIActionMoveTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Distance				UMETA(DisplayName = "Distance"),
	VE_Actor				UMETA(DisplayName = "Actor"),
	VE_Second				UMETA(DisplayName = "Second"),	
	VE_Max,
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAIActionDirectionEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Forward				UMETA(DisplayName = "Forward"),
	VE_Backward				UMETA(DisplayName = "Backward"),
	VE_Left					UMETA(DisplayName = "Left"),
	VE_Right				UMETA(DisplayName = "Right"),
	VE_Max,
};


