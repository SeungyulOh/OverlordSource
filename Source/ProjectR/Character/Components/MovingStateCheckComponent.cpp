// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
#include "MovingStateCheckComponent.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"

//#define USE_DYNAMIC_NAVMESH
//#define USE_NAVI_COLLISION_SPHERE

// Sets default values for this component's properties
UMovingStateCheckComponent::UMovingStateCheckComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UMovingStateCheckComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();
	if ( OwnerActor.IsValid() )
	{		
		IsMoving = false;
		IngnoringNavimeshUpdate = false;

		auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (OwnerActor_EntityAIComponent)
		{
#ifdef USE_NAVI_COLLISION_SPHERE
			if (OwnerActor_EntityAIComponent->GetNaviCollisionSphere() )
			{
				OwnerActor_EntityAIComponent->GetNaviCollisionSphere()->SetCanEverAffectNavigation(true);
			}
#endif			
			ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
			if (OwnerCharacter)
			{
				if (OwnerCharacter->GetCapsuleComponent())
				{					
					OwnerCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(false);
					OwnerCharacter->GetCapsuleComponent()->bDynamicObstacle = false;
#ifdef USE_DYNAMIC_NAVMESH
					OwnerCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(true);
					OwnerCharacter->GetCapsuleComponent()->bDynamicObstacle = false;
#endif
				}
			}
		}
	}		
}

void UMovingStateCheckComponent::CheckMovingState()
{	
	if (OwnerActor.IsValid())
	{
		//todo : PathFinding Test		
		FVector CurrentVelocity = OwnerActor->GetVelocity();
		bool IsRun = CurrentVelocity.SizeSquared() > 0.1f;
				
		if (!IsRun)
		{
			if (AvoidOtherCharacter() == false)
			{
				StopMove();
			}
		}

		IsMoving = IsRun;
	}
}

bool UMovingStateCheckComponent::StopMove()
{		
	if (OwnerActor.IsValid() == false)
		return false;

	if ( IsMoving )
	{
		auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
#ifdef USE_NAVI_COLLISION_SPHERE		
		if (OwnerActor_EntityAIComponent)
		{
			if (OwnerActor_EntityAIComponent->GetNaviCollisionSphere() )
				OwnerActor_EntityAIComponent->GetNaviCollisionSphere()->SetCanEverAffectNavigation(bStop);
		}
#endif	
		
	}

#ifdef USE_DYNAMIC_NAVMESH
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetCapsuleComponent())
			OwnerCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	}
#endif

	IsMoving = false;
	IngnoringNavimeshUpdate = false;

	return true;
}

void UMovingStateCheckComponent::StartMove()
{	
	if (OwnerActor.IsValid() == false)
		return;

	IsMoving = true;

#ifdef USE_DYNAMIC_NAVMESH
	if( !IngnoringNavimeshUpdate )
	{		
		IngnoringNavimeshUpdate = true;			
		OwnerActor->GetWorldTimerManager().SetTimer(IgnoreNavimeshUpdateTimer, this, &UMovingStateCheckComponent::ResetIgnoreFlag, 0.5f);		
	}		

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetCapsuleComponent())
			OwnerCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(false);
	}

	//UE_LOG(LogBattle, Log, TEXT(">>>>11111 Start moving - %s "), *(OwnerCharacter->GetName()));
#endif

#ifdef USE_NAVI_COLLISION_SPHERE
	auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (OwnerActor_EntityAIComponent)
	{
		if (OwnerActor_EntityAIComponent->GetNaviCollisionSphere())
			OwnerActor_EntityAIComponent->GetNaviCollisionSphere()->SetCanEverAffectNavigation(false);
	}
#endif

	//UE_LOG(LogBattle, Log, TEXT(">>>>22222 Start moving - %s "), *(OwnerActor->GetName()));
}

void UMovingStateCheckComponent::ResetIgnoreFlag()
{
	if (OwnerActor.IsValid())
	{
#ifdef USE_DYNAMIC_NAVMESH
		IngnoringNavimeshUpdate = false;
		OwnerActor->GetWorldTimerManager().ClearTimer(IgnoreNavimeshUpdateTimer);		
#endif
	}	
}

bool UMovingStateCheckComponent::AvoidOtherCharacter()
{
		
	float CharacterOverlapScale = RGAMEINSTANCE(this)->GameEnvironmentVar.CharacterOverlapScale;
	TArray< TWeakObjectPtr< AActor > > ResultActors;

	// TODO, use team NO instead of friendship type
	AActor* NearestActor = nullptr;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		NearestActor = pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, OwnerActor.Get(), EFriendshipTypeEnum::VE_None, CharacterOverlapScale);
	}
	if (NearestActor)
	{
		auto NearestActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(NearestActor);
		if (NearestActor_BattleEntityComponent)
		{				
			FVector AvoidDir = OwnerActor->GetActorLocation() - NearestActor->GetActorLocation();
			AvoidDir.Z = 0.0f;
			AvoidDir.Normalize();

			auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			float AvoidDist = NearestActor_BattleEntityComponent->GetBoundingRadius();
			if (OwnerActor_BattleEntityComponent)
				AvoidDist = OwnerActor_BattleEntityComponent->GetBoundingRadius() + NearestActor_BattleEntityComponent->GetBoundingRadius();

			FVector AvoidLocation = NearestActor->GetActorLocation() + AvoidDir * AvoidDist;

			auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
			if (OwnerActor_EntityAIComponent)
			{
				OwnerActor_EntityAIComponent->SetBlackboardForceTargetLocation(AvoidLocation);
				return true;
			}				
		}
	}
	

	return false;
}