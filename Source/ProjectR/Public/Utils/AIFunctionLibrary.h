// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "AI/AIDefs.h"
#include "AI/RAI_FSM.h"

#include "SharedConstants/GlobalConstants.h"
#include "AIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static URAI_FSM* MakeFSM( AActor* InOwnerActor, EAIFSMTypeEnum InFSMType);



	/*
	service
	*/
	static AActor* SearchEnemyInAttackSector(AActor* OwnerCharacter, bool SearchInSightRange);
	static AActor* SearchEnemy(AActor* OwnerCharacter, bool bForceToUseAttackRange = false, EPriorityTypeEnum InAIPreferType = EPriorityTypeEnum::VE_None);
	static AActor* SearchFriend(AActor* OwnerCharacter, bool bForceToUseAttackRange = false, EPriorityTypeEnum InAIPreferType = EPriorityTypeEnum::VE_None);
	static AActor*	SearchLinkedMemberEnemy(AActor* OwnerCharacter);
	static AActor* GetSpawnMemberAttackTarget(AActor* OwnerCharacter);
	
	static FVector	GetNextWayPoint(AActor* InOwnerCharacter);
	static AActor*	CheckFocusTarget(AActor* InOwnerCharacter);
	static void		MoveInsideTheWayPointAuto(AActor* InOwnerCharacter);
	
	/*
	decorator
	*/
	static bool IsCloseEnoughToActor(AActor* SrcActor, AActor* TargetActor, float ExtraDist);
	static bool IsCloseEnoughToLocation(AActor* SrcActor, FVector TargetLocation, float AcceptableDistance);
	static bool IsCloseEnoughForAttack(AActor* SrcActor, AActor* ResultEnemy);

	static bool IsDead(AActor* SrcActor);
	static bool IsInAbnormalStateFlag(AActor* SrcActor, EAbonormalStateFlagEnum InTargetStateFlag );
	static bool IsInUnActableState(AActor* SrcActor);
	static bool IsInState(AActor* SrcActor, ECharacterStateTypeEnum TargetState);

	static bool IsGoNextWayPoint(AActor* InOwnerCharacter, FVector TargetPos);

	/*
	task
	*/
	static bool	Stop(AActor* InOwnerCharacter);
	static bool	Attack(AActor* InOwnerCharacter, AActor* InTarget);
	static bool	StopAttack(AActor* InOwnerCharacter);
	static bool ScanEnemyOutsideSightRange(AActor* SrcActor );
	static bool CheckNextWayPoint(AActor* SrcActor);
	static bool CheckFollow(AActor* SrcActor, bool InCheckIdleState = true );

	/*
	Avoidance
	*/		
	static bool AvoidOtherCharacter( AActor* InOwnerActor );
	static bool PushOverlappedCharacter(AActor* InOwnerActor);
	static FVector GetAvoidVectorFromOtherCharacter(AActor* InOwnerActor, AActor* InNearestActor);

	/*
	Move helper
	*/
	static bool CanMoveToTargetPosition(AActor* InActor, FVector InTargetPos);
	static bool CanMoveUp(AActor* InActor, const float& GridHeight);

	static void SetCrewLocation(AActor* InActor, FVector& InLocation);
};