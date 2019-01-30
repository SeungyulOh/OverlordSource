// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Global/GridMapManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMovementStateEnum : uint8
{
	VE_Stop				UMETA(DisplayName = "Stop"),
	VE_Moving			UMETA(DisplayName = "Moving"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMovementTickAITypeEnum : uint8
{
	VE_Normal				UMETA(DisplayName = "Normal"),
	VE_MoveToAttack			UMETA(DisplayName = "MoveToAttack"),
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
		
public:
	UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{}

	// Called when the game starts
	void InitializeComponent() override;
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StopMovementImmediately() override;
	
	/*
	Custom movement
	*/
	bool		MoveToLocation(FVector InTargetLocation, bool InUsePathfinding = true, EMovementTickAITypeEnum InMovementTickAIType = EMovementTickAITypeEnum::VE_Normal );
	bool		MoveToActor(AActor* InTargetActor, bool InUsePathfinding = true, EMovementTickAITypeEnum InMovementTickAIType = EMovementTickAITypeEnum::VE_Normal);	
	bool		bAlreadyArrived(FVector InTargetLocation);
	FORCEINLINE EMovementStateEnum GetCurrentMovementState() { return CurrentMovementState; }
	void		Stop();

	FORCEINLINE bool		IsReachedAtGoal() { return ReachedAtGoal; }

	//for test
	bool		bActive = true;
private:

	void		FollowPath();
	bool		FindResultPaths(const FVector& InTargetLocation);

	void		InitMovement();
	void		SetNextPath();
	bool		CheckNextPathPoint();
	bool		SameNewTargetCellAndSearchTargetCell(FVector& InTargetLocation);

	bool		CanMoveToNextTarget( FVector& InNextTargetPos );

	/*
	Tick Ai
	*/
	bool		MoveTickAIProcess();
	void		AttackTarget();

	UPROPERTY()
	FVector						TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	EMovementTickAITypeEnum		MoveTickAIType = EMovementTickAITypeEnum::VE_Normal;	

	UPROPERTY()
	TArray< FVector >			ResultPaths;
	UPROPERTY()
	int32						CurrentPathPointIndex = 0;
	UPROPERTY()
	FVector						CurrentTargetLocation;
	UPROPERTY()
	FVector						PrevTargetLocation;
	UPROPERTY()
	FVector						CurrentMoveDirection;
	UPROPERTY()
	float						CurrentPathDistance = 0.0f;

	bool						ReachedAtGoal = false;

	UPROPERTY()
	EMovementStateEnum			CurrentMovementState = EMovementStateEnum::VE_Stop;
	UPROPERTY()
	float						AcceptableRaidus = 0.1f;
	UPROPERTY()
	bool						bUsePathfinding = true;
	
	UPROPERTY()
	FCellIndex					SearchTargetCell;

	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
};
