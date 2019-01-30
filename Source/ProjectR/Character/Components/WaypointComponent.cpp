// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "WaypointComponent.h"

#include "Character/BaseAIController.h"

#include "UtilFunctionIntegrated.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"


// Sets default values for this component's properties
UWaypointComponent::UWaypointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaypointComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	OwnerActor = GetOwner();
	LastWayPoint = FVector::ZeroVector;
}


// Called every frame
void UWaypointComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}


void UWaypointComponent::AddWaypoint(FVector pos, float fDelay)
{
	FWaypointInfo info;
	info.TargetPos = pos;
	info.DelayTime = fDelay;
	WayPointList.Emplace(info);
	QueueWayPoint.Emplace(info);
}

void UWaypointComponent::ClearWaypoint()
{
	WayPointList.Empty();
	QueueWayPoint.Empty();
}

FVector UWaypointComponent::GetNextWayPoint()
{
	if (!OwnerActor.IsValid())
		return FVector::ZeroVector;

	if (!LastWayPoint.IsNearlyZero())
	{
		FVector diff = OwnerActor->GetActorLocation() - LastWayPoint;
		FVector curPos = OwnerActor->GetActorLocation();
		float fDistance = diff.Size2D();

		float fDefaultAcceptRadius = 0.0f;
		APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
		if (OwnerPawn)
		{
			AAIController* AIController = Cast< AAIController >(OwnerPawn->GetController());
			if (IsValid(AIController))
			{
				auto PathFollowingComp = AIController->GetPathFollowingComponent();
				if (IsValid(PathFollowingComp))
					fDefaultAcceptRadius = PathFollowingComp->GetDefaultAcceptanceRadius();
			}
		}

		if(fDistance - fDefaultAcceptRadius > IgnoreMinDistance)
			return LastWayPoint;
	}		

	LastWayPoint = FVector::ZeroVector;
	if (QueueWayPoint.Num() > 0)
	{
		FWaypointInfo info = QueueWayPoint[0];
		if (!info.TargetPos.IsNearlyZero())
		{
			LastWayPoint = info.TargetPos;
			//DrawDebugBox(OwnerActor.Get()->GetWorld(), LastWayPoint, FVector(50.f, 50.f, 50.f), FColor::Red, false, 10.f, 0, 10.f);
			QueueWayPoint.RemoveAt(0);

			CurState = eNoState;

			if (bLoop)
			{
				if (QueueWayPoint.Num() == 0)
					QueueWayPoint = WayPointList;
			}
		}
	}

	return FVector::ZeroVector;
}