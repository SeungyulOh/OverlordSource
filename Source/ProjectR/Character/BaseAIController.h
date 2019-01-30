// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "AIController.h"
#include "Character/Components/CustomCharacterMovementComponent.h"
#include "BaseAIController.generated.h"

//class UBehaviorTree;

//#define USE_DETOUR_CROWD

/**
 * 
 */
UCLASS()
class PROJECTR_API ABaseAIController : public AAIController
{
	GENERATED_UCLASS_BODY()
	
public:
	//ABaseAIController(const FObjectInitializer& ObjectInitializer);

	void Possess(class APawn* InPawn) override;
	void Tick(float DeltaTime) override;
	void Destroyed() override;

	/*
	Service related..
	*/
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	AActor* SearchEnemy(float InRangeThreshold);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	AActor* SearchFriend(float InRangeThreshold);
	
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void SetBlackboardWithEnemySearchResult(FName BalckboardKey, AActor* ResultEnemy);
	
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	FVector	GetNextWayPoint();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsGoNextWayPoint(FVector gopos);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsInState( ECharacterStateTypeEnum TargetState);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsMaxHP( AActor* TargetActor );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsOverrideHero();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CanOccupyGridCell();
	
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void CheckForceTarget();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool DoesHaveWaypoint();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsEnemy( AActor* TargetActor );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsReachedAtGoal();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CheckAttackType( EAttackTypeEnum InTargetAttackType );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool HasLinkedMebmerAttackTarget();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool IsFollowLeaderActivated();


	/*
	Task related..
	*/
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool Attack( AActor* InTarget = nullptr );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool ClearBlackboard( FName KeyName );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CustomMoveToActor( AActor* Goal,				
							EMovementTickAITypeEnum InMovementTickAIType,
							bool bStopOnOverlap,
							bool bUsePathfinding,
							bool bCanStrafe,
							TSubclassOf<UNavigationQueryFilter> FilterClass,
							bool bAllowPartialPath);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CustomMoveToNearestMovableCellFromActor(AActor* Goal,
			EMovementTickAITypeEnum InMovementTickAIType,
			bool bStopOnOverlap,
			bool bUsePathfinding,
			bool bCanStrafe,
			TSubclassOf<UNavigationQueryFilter> FilterClass,
			bool bAllowPartialPath);


	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CustomMoveToLocation(	const FVector& Dest,							
							EMovementTickAITypeEnum InMovementTickAIType,
							bool bStopOnOverlap,
							bool bUsePathfinding,
							bool bProjectDestinationToNavigation,
							bool bCanStrafe,
							TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPath);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CustomSimpleMoveToActor(AActor* Goal);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool CustomSimpleMoveToLocation(const FVector& Dest);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool	StopMove();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool	MoveNearestAdjacentCell( AActor* TargetActor, FName BalckboardKey);

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void	Suicide();

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void	SetAttackTarget( AActor* InTargetActor );

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	bool	IsMovingToTargetLocation();

	

	UFUNCTION(BlueprintCallable, Category = BaseAIController)
		void		Escape();
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void		ChangeDummy();
	UFUNCTION(BlueprintCallable, Category = BaseAIController)
	void		FollowOrStop();
	
private:
	bool IsDestinationNearEnoughButKeepGoing(const FVector& InDestination);
	bool CheckFocusAttack();

	void SearchIdleLocation();
	void IdleDelay();
	void CheckIdle();
	void SetIdle(float InTime);
	void SetTryIdle(float InTime);

	bool IsInSkillRelatedAction();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAIController)
	EAIControlModeEnum			AIControlMode = EAIControlModeEnum::VE_AutoMode;

protected:
	void						DrawDebugLines(TArray< FVector > Points );

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseAIController)
	TWeakObjectPtr< AActor >	OwnerCharacter;

	UPROPERTY()
	EPriorityTypeEnum			SearchPreferType = EPriorityTypeEnum::VE_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BaseAIController)
	bool						IsIdleState = false;
	
	UPROPERTY()
	bool						IsTryIdleState = false;

	UPROPERTY()
	FVector						CurrentTargetLocation;

	UPROPERTY()
	TWeakObjectPtr< AActor >	CurrentTarget;

private:
	UPROPERTY()
	FTimerHandle	IdleHandler;

	UPROPERTY()
	FTimerHandle	TryIdleHandler;

};
