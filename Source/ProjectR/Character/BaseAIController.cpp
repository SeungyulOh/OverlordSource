// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "BaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Character/Components/MovingStateCheckComponent.h"
#include "Character/Components/CustomCharacterMovementComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"

#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/GridMapInspectorComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"




#include "BrainComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"

#include "UtilFunctionIntegrated.h"

//#define PATH_FIND_GRID
//#define DEBUG_PATH
//#define PATH_FIND_ONCE

ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer) : 
#ifdef USE_DETOUR_CROWD
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
#else
	Super(ObjectInitializer)
#endif

{

}


void ABaseAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	OwnerCharacter = Cast< AActor >(InPawn);
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	

	
}

/*
Service related..
*/
AActor* ABaseAIController::SearchEnemy(float InRangeThreshold)
{
	return UAIFunctionLibrary::SearchEnemy(OwnerCharacter.Get() );
}

AActor* ABaseAIController::SearchFriend(float InRangeThreshold)
{
	return UAIFunctionLibrary::SearchFriend(OwnerCharacter.Get() );
}

void ABaseAIController::SetBlackboardWithEnemySearchResult( FName BalckboardKey, AActor* ResultEnemy)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(BalckboardKey, ResultEnemy);
	}
}

FVector	ABaseAIController::GetNextWayPoint()
{
	return UAIFunctionLibrary::GetNextWayPoint(OwnerCharacter.Get());
}

bool ABaseAIController::IsGoNextWayPoint(FVector gopos)
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	if (gopos.IsNearlyZero())
		return false;

	FVector diff = OwnerCharacter->GetActorLocation() - gopos;

	float fLength = diff.SizeSquared2D();
	float fDefaultAcceptRadius = 0.0f;
	auto PathFollowingComp = GetPathFollowingComponent();
	if (IsValid(PathFollowingComp))
	{
		fDefaultAcceptRadius = PathFollowingComp->GetDefaultAcceptanceRadius();
	}
	
	if (fLength > fDefaultAcceptRadius * fDefaultAcceptRadius)
		return true;

	return false;
}

bool ABaseAIController::IsInState(ECharacterStateTypeEnum TargetState)
{
	return UAIFunctionLibrary::IsInState(OwnerCharacter.Get(), TargetState);
}

bool ABaseAIController::IsMaxHP(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return true;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (IsValid(BattleEntityComponent) == false)
		return false;

	if (BattleEntityComponent->GetCurrentHPPercent() >= 1.0f)
		return true;

	return false;
}

bool ABaseAIController::IsOverrideHero()
{
	if (OwnerCharacter.IsValid())
	{
		float CharacterOverlapScale = RGAMEINSTANCE(this)->GameEnvironmentVar.CharacterOverlapScale;
		TArray< TWeakObjectPtr<AActor> > ResultActors;
		AActor* NearestActor = nullptr;
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pGameMode)
		{
			NearestActor = pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, OwnerCharacter.Get(), EFriendshipTypeEnum::VE_None, CharacterOverlapScale);
		}
		if (NearestActor)
		{
			return true;
		}
	}
	return false;
}

bool ABaseAIController::CanOccupyGridCell()
{
	if (OwnerCharacter.IsValid())
	{		
		UGridMapInspectorComponent* GridMapInspectorComponent = UCharacterHelpFunctionLibrary::GetGridMapInspectorComponent(OwnerCharacter.Get());
		if (GridMapInspectorComponent == nullptr)
			return true;

		FCellIndex CurrentIndex = GridMapInspectorComponent->GetCurrentGridIndex();
		FCellIndex PosCellIndex;
		RGAMEINSTANCE(this)->GridMapManager->GetCellIndexFromWorldPos(OwnerCharacter->GetActorLocation(), PosCellIndex);
		if (CurrentIndex.XIdx != PosCellIndex.XIdx || CurrentIndex.YIdx != PosCellIndex.YIdx)
		{
			auto TargetCellDataState = RGAMEINSTANCE(this)->GridMapManager->GetMapInfo(PosCellIndex.XIdx, PosCellIndex.YIdx);
			if (TargetCellDataState.CellInfo == EMapCellStateEnum::VE_Occupied)
				return false;
		}
	}

	return true;
}

//none use
void ABaseAIController::CheckForceTarget()
{
	//if (OwnerCharacter.IsValid() == false)
	//	return;
}

bool ABaseAIController::DoesHaveWaypoint()
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (OwnerActor_EntityAIComponent)
		return (OwnerActor_EntityAIComponent->GetWaypointComponent() != nullptr);

	return false;
}

bool ABaseAIController::IsEnemy(AActor* TargetActor)
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (OwnerActor_BattleEntityComponent)
	{
		return OwnerActor_BattleEntityComponent->IsEnemy(TargetActor);
	}

	return false;
}

bool ABaseAIController::IsReachedAtGoal()
{
#ifdef PATH_FIND_GRID
	if (OwnerCharacter.IsValid() == false)
		return false;

	UCustomCharacterMovementComponent* CustomCharacterMovementComponent = UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent( OwnerCharacter.Get());
	if (CustomCharacterMovementComponent)
	{
		return CustomCharacterMovementComponent->IsReachedAtGoal();
	}

	return false;
#else
	return true;
#endif
}

bool ABaseAIController::CheckAttackType(EAttackTypeEnum InTargetAttackType)
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (OwnerActor_BattleEntityComponent)
	{
		EAttackTypeEnum OwnerAttackType = OwnerActor_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<EAttackTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackType);
		if (OwnerAttackType == InTargetAttackType)
			return true;
	}

	return false;
}

bool ABaseAIController::HasLinkedMebmerAttackTarget()
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	return false;
}

bool ABaseAIController::IsFollowLeaderActivated()
{
	return false;
}

/*
Task related..
*/
bool ABaseAIController::Attack(AActor* InTarget)
{
	return UAIFunctionLibrary::Attack(OwnerCharacter.Get(), InTarget);
}

bool ABaseAIController::ClearBlackboard(FName KeyName)
{
	if (Blackboard)
	{
		Blackboard->ClearValue( KeyName );
		return true;
	}
	return false;
}

bool ABaseAIController::CustomMoveToActor(AActor* Goal,					
										EMovementTickAITypeEnum InMovementTickAIType,
										bool bStopOnOverlap,
										bool bUsePathfinding,
										bool bCanStrafe,
										TSubclassOf<UNavigationQueryFilter> FilterClass,
										bool bAllowPartialPath)
{
	if (Goal == nullptr)
		return false;

#ifdef PATH_FIND_ONCE
	if ((CurrentTarget == Goal) && (GetMoveStatus() == EPathFollowingStatus::Moving))
		return true;
#endif

	if (OwnerCharacter.IsValid() == false)
		return false;

	if (IsInSkillRelatedAction())
		return false;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (IsValid(BattleEntityComponent) == false)
		return false;
	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (IsValid(EntityAIComponent) == false)
		return false;

	if (BattleEntityComponent->IsInUnMovableState())
		return false;

	EPathFollowingRequestResult::Type rtType = EPathFollowingRequestResult::Failed;

	if (BattleEntityComponent->BossComponent != nullptr )
	{
		if (!BattleEntityComponent->BossComponent->IsMoveEnable())
			return false;
	}

	float AcceptanceRadius = 100.0f;
	UEntityAIComponent* GoalEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Goal);
	UBattleEntityComponent* GoalBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Goal);
	if (IsValid(GoalEntityAIComponent) && IsValid(GoalBattleEntityComponent))
	{
		bUsePathfinding = GoalEntityAIComponent->IsUsePathfinding();
		AcceptanceRadius = GoalBattleEntityComponent->BoundingRadius;
	}
	
	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerCharacter.Get());
	if (IsValid(MovementComponent) && MovementComponent->IsFlying())
	{
		bUsePathfinding = false;
	}


#ifdef DEBUG_PATH
	TArray< FVector > ResultPaths = UUtilFunctionLibrary::GetNavigationPaths(OwnerCharacter.Get(), OwnerCharacter->GetActorLocation(), Goal->GetActorLocation());
	DrawDebugLines(ResultPaths);
#endif

	
	rtType = MoveToActor(Goal, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPath);
		
	
	if (rtType == EPathFollowingRequestResult::Failed)
		return false;

	EntityAIComponent->StartMove(Goal->GetActorLocation());

	CurrentTarget = Goal;

	return true;	
}

bool ABaseAIController::CustomMoveToNearestMovableCellFromActor(AActor* Goal,
	EMovementTickAITypeEnum InMovementTickAIType,
	bool bStopOnOverlap,
	bool bUsePathfinding,
	bool bCanStrafe,
	TSubclassOf<UNavigationQueryFilter> FilterClass,
	bool bAllowPartialPath)
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	if (IsInSkillRelatedAction())
		return false;

	if (Goal == nullptr)
		return false;

	FVector Destination;
#ifdef PATH_FIND_GRID
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( RGameInstance->GridMapManager )
	{
		switch ( InMovementTickAIType )
		{
			case EMovementTickAITypeEnum::VE_MoveToAttack:
				// If target is in adjacent cell, just stop or attack according to movement tick ai type
				if (RGameInstance->GridMapManager->AreTwoCellsAdjacentFromWorldPos(OwnerCharacter->GetActorLocation(), Goal->GetActorLocation()))
				{
					Attack();
					return true;
				}
				break;
			default:
				{
					
				}
				break;
		}

		Destination = RGameInstance->GridMapManager->GetNearestPointToTargetCellFromSourceWorldPos(OwnerCharacter->GetActorLocation(), Goal->GetActorLocation());
		CustomMoveToLocation(Destination, InMovementTickAIType, bStopOnOverlap, bUsePathfinding, true, bCanStrafe, FilterClass, bAllowPartialPath);
				
		return true;
	}
#else
	return CustomMoveToActor(Goal, InMovementTickAIType, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPath);
#endif

	return false;
}
 
bool ABaseAIController::IsMovingToTargetLocation()
{
	UCustomCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent(OwnerCharacter.Get());
	if (!IsValid(MovementComponent))
		return false;

	if (OwnerCharacter.IsValid() == false)
		return false;

	UBlackboardComponent* BlackboardComp = UAIBlueprintHelperLibrary::GetBlackboard(OwnerCharacter.Get());
	if (!IsValid(BlackboardComp))
		return false;
	  
	auto TargetLocation = BlackboardComp->GetValueAsVector(TEXT("TargetLocation"));

	if (CurrentTargetLocation.Equals(TargetLocation, 10.0f) == false)
		return false;

	if (TargetLocation == FVector::ZeroVector)
		return false;

	if (MovementComponent->GetCurrentMovementState() == EMovementStateEnum::VE_Moving)
		return true;

	return false;
}

void ABaseAIController::DrawDebugLines(TArray< FVector > Points)
{
	if (Points.Num() < 2)
		return;

	for (int i = 0; i < Points.Num() - 1; i++)
	{
		DrawDebugLine(OwnerCharacter->GetWorld(), Points[i], Points[i+1], FColorList::VioletRed, false, 1.0f, 0, 3.0f );
	}
}


bool ABaseAIController::CustomMoveToLocation( const FVector& Dest,
	EMovementTickAITypeEnum InMovementTickAIType,
	bool bStopOnOverlap,
	bool bUsePathfinding,
	bool bProjectDestinationToNavigation,
	bool bCanStrafe,
	TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPath)
{

#ifdef PATH_FIND_ONCE 
	if (IsDestinationNearEnoughButKeepGoing(Dest) == true)
		return true;

	if (IsMovingToTargetLocation())
		return true;
#endif

	if (OwnerCharacter.IsValid() == false)
		return false;

	if (IsInSkillRelatedAction())
		return false;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (IsValid(BattleEntityComponent) == false)
		return false;
	if (BattleEntityComponent->IsInUnMovableState())
		return false;

	
	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (IsValid(EntityAIComponent) == false)
		return false;

#ifdef PATH_FIND_GRID
	UCustomCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent(OwnerCharacter.Get());
	if (!IsValid(MovementComponent))
	{
		return false;		
	}
	
	if (MovementComponent->bAlreadyArrived(Dest) == true)
	{
		StopMove();
	}
	else
	{
		bUsePathfinding = !(MovementComponent->IsFlying());

		bool bSuccessMoveToLocation = MovementComponent->MoveToLocation(Dest, bUsePathfinding, InMovementTickAIType );
		if (bSuccessMoveToLocation == false)
			return true;
	}
#else	

	EPathFollowingRequestResult::Type rtType = MoveToLocation(Dest, 0.0f, bStopOnOverlap, bUsePathfinding);

	if (rtType == EPathFollowingRequestResult::Failed)
		return false;
#endif

#ifdef DEBUG_PATH
	TArray< FVector > ResultPaths = UUtilFunctionLibrary::GetNavigationPaths(OwnerCharacter.Get(), OwnerCharacter->GetActorLocation(), Dest);
	if (ResultPaths.Num() == 0)
		return false;

	DrawDebugLines(ResultPaths);
#endif

	EntityAIComponent->StartMove(Dest);

	CurrentTargetLocation = Dest;

	return true;
}

bool ABaseAIController::IsDestinationNearEnoughButKeepGoing(const FVector& InDestination)
{
#ifdef PATH_FIND_GRID
	UCustomCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent(OwnerCharacter.Get());
	if (IsValid(MovementComponent))
	{
		auto MoveStatus = MovementComponent->GetCurrentMovementState();
		if (MovementComponent->bAlreadyArrived(InDestination) && (MoveStatus == EMovementStateEnum::VE_Moving))
			return true;
	}
#else
	auto MoveStatus = GetMoveStatus();
	if (CurrentTargetLocation.Equals(InDestination, 10.0f) && (MoveStatus == EPathFollowingStatus::Moving))
		return true;
#endif

	return false;
}

bool ABaseAIController::CustomSimpleMoveToActor(AActor* Goal)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	if (NavSys)
	{
		NavSys->SimpleMoveToActor(this, Goal);

		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
		if (IsValid(EntityAIComponent) == false)
			return false;

		EntityAIComponent->StartMove(Goal->GetActorLocation());
	}
	return true;
}

bool ABaseAIController::CustomSimpleMoveToLocation(const FVector& Dest)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	if (NavSys)
	{
		NavSys->SimpleMoveToLocation(this, Dest);

		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
		if (IsValid(EntityAIComponent) == false)
			return false;

		EntityAIComponent->StartMove(Dest);
	}
	return true;
}

bool ABaseAIController::StopMove()
{
	return UAIFunctionLibrary::Stop(OwnerCharacter.Get());
}

bool ABaseAIController::MoveNearestAdjacentCell(AActor* TargetActor, FName BalckboardKey)
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) )
	{
		UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		EAttackTypeEnum OwnerAttackType = EAttackTypeEnum::VE_Melee;
		if (Owner_BattleEntityComponent)
			OwnerAttackType = Owner_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;

		//
		FVector TargetLocation = OwnerCharacter->GetActorLocation();
		if( IsValid( TargetActor ) && OwnerAttackType == EAttackTypeEnum::VE_Melee)		
			TargetLocation = TargetActor->GetActorLocation();
		
		FVector NearestAdjacentPos = RGameInstance->GridMapManager->GetNearestPointToTargetCellFromSourceWorldPos( OwnerCharacter->GetActorLocation(), TargetLocation);
#ifdef PATH_FIND_ONCE
		if (CurrentTargetLocation.Equals(NearestAdjacentPos) && (GetMoveStatus() == EPathFollowingStatus::Moving))
			return true;
#endif

		if (Blackboard)
		{
			CurrentTargetLocation = NearestAdjacentPos;
			Blackboard->SetValueAsVector(BalckboardKey, NearestAdjacentPos );
		}
		return true;
	}

	return false;
}

void ABaseAIController::CheckIdle()
{
	if (IsTryIdleState == false)
	{
		IsTryIdleState = true;
#ifdef RULE_TESTDELETE
		float TryIdle = ((FMath::Rand() % 400) * 0.01f) + 2.0f;
#else
		float TryIdle = ((FMath::Rand() % 400) / 100.0f) + 2.0f;
#endif
		
		SetTryIdle(TryIdle);
	}
}

void ABaseAIController::SearchIdleLocation()
{
	if (OwnerCharacter.IsValid() == false)
		return;
	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (IsValid(EntityAIComponent) == false)
		return;	
		
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());

	FNavLocation RandomPointNavLocation;
	float Range = FMath::Rand() % 200 + 100;
	FVector WorldLocation;
	if(BattleEntityComponent)
		WorldLocation = (BattleEntityComponent->GetForwardVector() * -1 * Range) + OwnerCharacter->GetActorLocation();

	UMapFunctionLibrary::GetMovePawnRandomReachablePointInRadius( Cast< APawn >( OwnerCharacter.Get() ), 100.0f, WorldLocation, RandomPointNavLocation);
	WorldLocation = RandomPointNavLocation.Location;

	EntityAIComponent->SetIdleLocation(WorldLocation);
	float MoveIdle = ((FMath::Rand() % 1300) / 100.0f) + 2.0f;
	SetIdle(MoveIdle);
	IsIdleState = true;
}


void ABaseAIController::SetIdle(float InTime)
{
	GetWorldTimerManager().ClearTimer(IdleHandler);
	GetWorldTimerManager().SetTimer(IdleHandler, this, &ABaseAIController::IdleDelay, InTime, false);
}

void ABaseAIController::SetTryIdle(float InTime)
{
	GetWorldTimerManager().ClearTimer(TryIdleHandler);
	GetWorldTimerManager().SetTimer(TryIdleHandler, this, &ABaseAIController::SearchIdleLocation, InTime, false);
}

void ABaseAIController::IdleDelay()
{
	if (OwnerCharacter.IsValid() == false)
		return;
	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (IsValid(EntityAIComponent) == false)
		return;

	IsIdleState = false;
	GetWorldTimerManager().ClearTimer(IdleHandler);
	GetWorldTimerManager().ClearTimer(TryIdleHandler);
	IsTryIdleState = false;
	EntityAIComponent->SetIdleLocation(FVector::ZeroVector);
}


void ABaseAIController::Suicide()
{
	if (OwnerCharacter.IsValid() == false)
		return;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (IsValid(BattleEntityComponent) == false)
		return;

	BattleEntityComponent->AddHP(-999999, OwnerCharacter.Get());
	BattleEntityComponent->ForceDead(OwnerCharacter.Get());
}


void ABaseAIController::SetAttackTarget(AActor* InTargetActor)
{
	if (OwnerCharacter.IsValid() == false)
		return;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (IsValid(BattleEntityComponent) == false)
		return;

	BattleEntityComponent->SetCurrentAttackTarget(InTargetActor);
}


void ABaseAIController::Escape()
{
	if (OwnerCharacter.IsValid() == false)
		return;

	
	FNavLocation RandomPointNavLocation;
	auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (IsValid(Character_AIEntityComponent))
	{
		FVector WorldLocation = (OwnerCharacter->GetActorForwardVector() * -1 * 500.0f) + OwnerCharacter->GetActorLocation();

		UMapFunctionLibrary::GetMovePawnRandomReachablePointInRadius(OwnerCharacter.Get(), 250.0f, WorldLocation, RandomPointNavLocation);
		WorldLocation = RandomPointNavLocation.Location;
		
		Character_AIEntityComponent->SetBlackboardForceTargetLocation(WorldLocation);
	}
}

void ABaseAIController::ChangeDummy()
{
	if (OwnerCharacter.IsValid() == false)
		return;
}

void ABaseAIController::FollowOrStop()
{
	StopMove();
}



bool ABaseAIController::CheckFocusAttack()
{
	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	if (IsValid(BasePlayerController) == false)
		return false;

	if (BasePlayerController->GetIsFocusAttack() == false)
		return false;
	
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	bool IsMyCharacter = (BattleEntityComponent && BattleEntityComponent->AmIEnemyWithMyPlayer() == false);
	if (IsMyCharacter)
	{
		//isforcetarget?
		auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
		if (IsValid(Character_AIEntityComponent))
		{
			auto ForceTargetActor = Character_AIEntityComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
			if (ForceTargetActor)
			{
				return false;
			}
		}

		AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
		UBattleEntityComponent* SelectBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectCharacter);
		bool IsNonSelectCharacter = (SelectCharacter != nullptr && OwnerCharacter != SelectCharacter && SelectBattleEntityComponent);
		if (IsNonSelectCharacter)
		{
			AActor* AttackTargetCharacter = SelectBattleEntityComponent->GetCurrentAttackTarget();
			if (AttackTargetCharacter == nullptr)
				return false;

			if(BattleEntityComponent->GetCurrentAttackTarget() != AttackTargetCharacter)
			{
				BattleEntityComponent->SetCurrentAttackTarget(AttackTargetCharacter);
			}
			return true;
		}
	}
	return false;
}


/*
Helper functions
*/
bool ABaseAIController::IsInSkillRelatedAction()
{
	if (OwnerCharacter.IsValid() == false)
		return false;

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (SkillEntityComponent)
	{
		if (SkillEntityComponent->IsSkillCasting() || SkillEntityComponent->IsSkillActing())
		{
			return true;
		}
	}

	return false;
}

void ABaseAIController::Destroyed()
{
	Super::Destroyed();
}