// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CustomCharacterMovementComponent.h"

#include "Character/Components/FunctionalComponents/PathFindingComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"


// Called when the game starts
void UCustomCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();	

	TargetLocation = FVector::ZeroVector;
	SearchTargetCell = FCellIndex(-1, -1);
}


// Called every frame
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bActive == false)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentMovementState == EMovementStateEnum::VE_Moving)
	{
		FollowPath();
	}
}

void UCustomCharacterMovementComponent::StopMovementImmediately()
{
	Super::StopMovementImmediately();

	ResultPaths.Empty();
	SearchTargetCell = FCellIndex(-1, -1);
	CurrentPathPointIndex = 0;
	PrevTargetLocation = FVector::ZeroVector;
	CurrentTargetLocation = FVector::ZeroVector;;
	CurrentMoveDirection = FVector::ZeroVector;;
	CurrentPathDistance = 0.0f;	
	CurrentMovementState = EMovementStateEnum::VE_Stop;

	ReachedAtGoal = true;	
	MoveTickAIType = EMovementTickAITypeEnum::VE_Normal;

	//for debug
	//FlushPersistentDebugLines(this->GetWorld());
}


bool UCustomCharacterMovementComponent::MoveToLocation(FVector InTargetLocation, bool InUsePathfinding, EMovementTickAITypeEnum InMovementTickAIType)
{
	if (OwnerActor.IsValid() == false)
		return false;

	MoveTickAIType = InMovementTickAIType;
	
	if (CurrentMovementState == EMovementStateEnum::VE_Moving &&
		TargetLocation.Equals(InTargetLocation, 10.0f))
		return false;
	
	if (bAlreadyArrived(InTargetLocation) == true)
	{
		StopMovementImmediately();
		return true;
	}

	if (SameNewTargetCellAndSearchTargetCell(InTargetLocation) == true)
		return false;
	

	if (FindResultPaths(InTargetLocation) == false)
		return false;
	 
	CurrentMovementState = EMovementStateEnum::VE_Moving;
	AcceptableRaidus = 0.1f;
	bUsePathfinding = InUsePathfinding;

	InitMovement();
	return true;
}

bool UCustomCharacterMovementComponent::bAlreadyArrived(FVector InTargetLocation)
{
	if (OwnerActor.IsValid() == false)
		return false;

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	auto OwnerCharacterLocation = OwnerActor.Get()->GetActorLocation();
	FCellIndex DestinationCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(OwnerCharacterLocation, DestinationCell);
	if (DestinationCell == SearchTargetCell)
		return true;
	
	//if (OwnerCharacterLocation.Equals(InTargetLocation, 10.0f))
	//	return true;

	return false;
}

bool UCustomCharacterMovementComponent::CanMoveToNextTarget(FVector& InNextTargetPos)
{
	if (OwnerActor.IsValid() == false)
		return false;

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	FCellIndex DestinationCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(InNextTargetPos, DestinationCell);
	FMapCellData TargetCellDataState = RGameInstance->GridMapManager->GetMapInfo(DestinationCell.XIdx, DestinationCell.YIdx);
	if (TargetCellDataState.CellInfo == EMapCellStateEnum::VE_Movable)
		return true;

	return false;
}


bool UCustomCharacterMovementComponent::SameNewTargetCellAndSearchTargetCell(FVector& InTargetLocation)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->GridMapManager))
		return false;

	FCellIndex NewTargetCell;
	RGameInstance->GridMapManager->GetCellIndexFromWorldPos(InTargetLocation, NewTargetCell);

	if (SearchTargetCell == FCellIndex(-1, -1))
	{
		SearchTargetCell = NewTargetCell;
		return false;
	}
	else if (SearchTargetCell == NewTargetCell)
	{
		return true;
	}

	SearchTargetCell = NewTargetCell;

	return false;
}

bool UCustomCharacterMovementComponent::FindResultPaths(const FVector& InTargetLocation)
{	
	if (!OwnerActor.IsValid())
		return false;

	UPathFindingComponent* PathFindingComponent = Cast<UPathFindingComponent>(OwnerActor.Get()->GetComponentByClass(UPathFindingComponent::StaticClass()));
	if (IsValid(PathFindingComponent) == false)
		return false;

	if (PathFindingComponent->PathFind(InTargetLocation, ResultPaths) == false)
		return false;

	if (ResultPaths.Num() <= 1)
		return false;

	return true;
}

bool UCustomCharacterMovementComponent::MoveToActor(AActor* InTargetActor, bool InUsePathfinding, EMovementTickAITypeEnum InMovementTickAIType)
{
	if (OwnerActor.IsValid() == false)
		return false;

	MoveTickAIType = InMovementTickAIType;
		
	if (FindResultPaths(InTargetActor->GetActorLocation()) == false)
		return false;

	//DrawDebugLines(ResultPaths);

	bUsePathfinding = InUsePathfinding;

	float AcceptanceRadius = 0.1f;
	UEntityAIComponent* GoalEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InTargetActor);
	UBattleEntityComponent* GoalBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
	if (IsValid(GoalEntityAIComponent) && IsValid(GoalBattleEntityComponent))
	{
		bUsePathfinding = GoalEntityAIComponent->IsUsePathfinding();
		AcceptanceRadius = GoalBattleEntityComponent->BoundingRadius;
	}

	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
	if (IsValid(MovementComponent) && MovementComponent->IsFlying())
	{
		bUsePathfinding = false;
	}
	 
	CurrentMovementState = EMovementStateEnum::VE_Moving;

	InitMovement();

	return true;
}


void UCustomCharacterMovementComponent::InitMovement()
{
	CurrentPathPointIndex = 1;
	PrevTargetLocation = ResultPaths[0];
	CurrentTargetLocation = ResultPaths[CurrentPathPointIndex];
	CurrentMoveDirection = CurrentTargetLocation - PrevTargetLocation;
	CurrentPathDistance = CurrentMoveDirection.SizeSquared2D();
	CurrentMoveDirection.Normalize();

	ReachedAtGoal = false;


}

void UCustomCharacterMovementComponent::Stop()
{
	StopMovementImmediately();	
}

void UCustomCharacterMovementComponent::SetNextPath()
{
	++CurrentPathPointIndex;
	if (CurrentPathPointIndex >= ResultPaths.Num())
	{
		if (MoveTickAIProcess() == false)
		{
			StopMovementImmediately();
		}		
	}
	else
	{	
		if (MoveTickAIProcess() == false)
		{
			if (CanMoveToNextTarget(ResultPaths[CurrentPathPointIndex]))
			{
				PrevTargetLocation = CurrentTargetLocation;
				CurrentTargetLocation = ResultPaths[CurrentPathPointIndex];
				CurrentMoveDirection = CurrentTargetLocation - PrevTargetLocation;
				CurrentPathDistance = CurrentMoveDirection.SizeSquared2D();
				CurrentMoveDirection.Normalize();
			}else
				StopMovementImmediately();
		}		

	}
}

bool UCustomCharacterMovementComponent::CheckNextPathPoint()
{
	FVector CurrentLocation = OwnerActor->GetActorLocation();
	bool bUpdateNewPath = false;

	if ((CurrentLocation - CurrentTargetLocation).SizeSquared2D() <= (AcceptableRaidus * AcceptableRaidus))
		bUpdateNewPath = true;

	if (bUpdateNewPath == false)
	{
		float MoveDistanct = (CurrentLocation - PrevTargetLocation).SizeSquared2D();
		if (MoveDistanct >= CurrentPathDistance)
			bUpdateNewPath = true;
	}

	if (bUpdateNewPath)
	{
		SetNextPath();
	}

	return bUpdateNewPath;
}

void UCustomCharacterMovementComponent::FollowPath()
{
	if (CurrentMovementState == EMovementStateEnum::VE_Moving)
	{
		UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
		if (MovementComponent)
		{
			CheckNextPathPoint();
			{
				float CurrentVelocity = MovementComponent->GetMaxSpeed();
				//MovementComponent->Velocity = CurrentVelocity*CurrentMoveDirection;				
				MovementComponent->RequestDirectMove(CurrentVelocity*CurrentMoveDirection, true );
			}			
		}
	}
}

bool UCustomCharacterMovementComponent::MoveTickAIProcess()
{
	switch (MoveTickAIType)
	{
		case EMovementTickAITypeEnum::VE_Normal:
			break;
		case EMovementTickAITypeEnum::VE_MoveToAttack:
			{
				UBattleEntityComponent* OwnerEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
				if ( IsValid(OwnerEntityComponent) )
				{					
					if (OwnerEntityComponent->IsCloseEnoughForAttack(OwnerEntityComponent->GetCurrentAttackTarget()))
					{
						AttackTarget();
						return true;
					}
				}				
			}
			break;
	}

	return false;
}

void UCustomCharacterMovementComponent::AttackTarget()
{
	auto Owner_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (Owner_EntityAIComponent)
	{
		if (Owner_EntityAIComponent->GetEntityBaseAIController())
			Owner_EntityAIComponent->GetEntityBaseAIController()->Attack();
	}
}