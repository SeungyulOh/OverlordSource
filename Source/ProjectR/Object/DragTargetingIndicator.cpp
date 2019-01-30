// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "DragTargetingIndicator.h"

#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GlobalIntegrated.h"


// Sets default values
ADragTargetingIndicator::ADragTargetingIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADragTargetingIndicator::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ADragTargetingIndicator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
FVector ADragTargetingIndicator::GetBeamSourceLocation()
{
	if (DragOwner.IsValid())
	{
		auto Target = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(DragOwner.Get());
		if(Target)
		{
			auto SourcePoint = Target->GetSocketLocation(SOCKET_ROOT);
			return SourcePoint;
		}
	}

	return FVector::ZeroVector;
}

void ADragTargetingIndicator::SetBeamSourceLocation()
{
	auto SourcePoint = GetBeamSourceLocation();
	if( IsValid( PickUIBeam_Comp ) )
		PickUIBeam_Comp->SetBeamSourcePoint(0, SourcePoint, 0);
}

void ADragTargetingIndicator::SpawnBeamParticle(AActor* InDragOwner)
{
	if (IsValid(InDragOwner))
	{
		DragOwner = InDragOwner;

		//Create Beam Particle
		auto SourcePoint	= GetBeamSourceLocation();
		PickUIBeam_Comp		= UUtilFunctionLibrary::CustomSpawnEmitterAtLocation(this->GetWorld(), PickUIBeam, SourcePoint, FRotator::ZeroRotator, true);
		
		if (IsValid(PickUIBeam_Comp))
		{
			//Set Source
			SetBeamSourceLocation();
			
			//set Target
			PickUIBeam_Comp->SetActorParameter(FName(TEXT("Socket_Root_T")), this);
		}
	}
}

void ADragTargetingIndicator::DespawnBeamParticle()
{
	if (IsValid(PickUIBeam_Comp))
	{
		PickUIBeam_Comp->Deactivate();
		PickUIBeam_Comp = nullptr;
	}

}

void ADragTargetingIndicator::SetActivateSnapMode(bool IsActive, FVector TargetLocation, bool bAutoRelease /* = true*/, float Gap /*= 100.f*/)
{
	IsSnapMode			= IsActive;
	SnapTarget			= TargetLocation;
	SnapGap				=  Gap;
	IsAutoRelease		= bAutoRelease;
}

void ADragTargetingIndicator::ActivateDragTarget(AActor* InDragOwner, bool InActivate, FVector CurPos, bool IsBlackOutTime /*=true*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance != nullptr)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

		if (BaseStageInfo->GetGameMode() == EGamePlayModeTypeEnum::VE_Arena || 
			BaseStageInfo->GetGameMode() == EGamePlayModeTypeEnum::VE_MirrorDungeon)
			return;
	}

	if (InActivate)
	{
		if (InDragOwner)
		{
			//Create Beam Particle
			SpawnBeamParticle(InDragOwner);

			auto SourcePoint = GetBeamSourceLocation();

			//Set Initial Location Target
			this->SetActorLocation(SourcePoint);

			//SetBeamSourceLocation();
		
			//Set AreaParticle Location
			if( IsValid( PickUIBeam_Comp ) )
				PickUIBeam_Comp->SetWorldLocation(this->GetActorLocation());
		}
	
		SetActorScale3D(FVector(2.0f, 2.0f, 1.0f));

		SetDragTargetType(EDragTargetTypeEnum::VE_Noraml);
		CurTargetActor = nullptr;

		if(IsBlackOutTime)
			BlackOutTimeStart();
	}
	else
	{
		if (IsBlackOutTime)
			BlackOutTimeEnd();
		/*
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (RGameInstance != nullptr)
			RGameInstance->GameObjectMgr->SetAllObjectBreak();

		DespawnBeamParticle();
		RGameInstance->GameObjectMgr->SetAllObjectBreak();
		RGameInstance->GameObjectMgr->SetBlackOut(false);
		*/
	}

	this->SetActorHiddenInGame(!InActivate);	
}

void ADragTargetingIndicator::BlackOutTimeStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance != nullptr && DragOwner.IsValid())
	{
// 		UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 		if (!pGamePlayMode)
// 			return;
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (!pGameMode)
			return;

		pGameMode->ObjectHelper.SetAllObjectPause(MovePauseLimitSecond, 0.1f);
// 		RGameInstance->GameObjectMgr->SetAllObjectPause(MovePauseLimitSecond, 0.1f);
		URenderFunctionLibrary::IgnoreDarkenTarget(DragOwner.Get(), true);
		pGameMode->ObjectHelper.SetBlackOut(true);
// 		RGameInstance->GameObjectMgr->SetBlackOut(true);
		DragOwner->GetWorldTimerManager().SetTimer(DragTargetDarkEndHandle, this, &ADragTargetingIndicator::BlackOutTimeEnd, MovePauseLimitSecond, false);
	} 
}

void ADragTargetingIndicator::BlackOutTimeEnd()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (!pGamePlayMode)
// 		return;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;
	
// 	RGameInstance->GameObjectMgr->SetAllObjectBreak();
// 	DespawnBeamParticle();
// 	RGameInstance->GameObjectMgr->SetAllObjectBreak();
// 	RGameInstance->GameObjectMgr->SetBlackOut(false);
	
	pGameMode->ObjectHelper.SetAllObjectBreak();
	DespawnBeamParticle();
	pGameMode->ObjectHelper.SetAllObjectBreak();
	pGameMode->ObjectHelper.SetBlackOut(false);


	if (DragOwner.IsValid())
	{
		URenderFunctionLibrary::IgnoreDarkenTarget(DragOwner.Get(), false);
		DragOwner->GetWorldTimerManager().ClearTimer(DragTargetDarkEndHandle);
	}
	
	if (PrevTargetActor.IsValid())
		URenderFunctionLibrary::IgnoreDarkenTarget(PrevTargetActor.Get(), false);

	if (CurTargetActor.IsValid())
		URenderFunctionLibrary::IgnoreDarkenTarget(CurTargetActor.Get(), false);
}

void ADragTargetingIndicator::DragTarget(FVector curPos)
{
	if (DragOwner.IsValid() == false || curPos.Equals(FVector::ZeroVector))
	{
		ActivateDragTarget(nullptr, false);
		return;
	}

	auto bFindSuccesCheckTarget = CheckTarget(curPos);
	if (bFindSuccesCheckTarget == false)
	{
		SetDragTargetType(EDragTargetTypeEnum::VE_Noraml);
		SetBeamSourceLocation();

		FVector ActorLocation = curPos;
		if (IsSnapMode)
		{
			float Dist = FVector::Dist(curPos, SnapTarget);
			if (Dist < SnapGap)
			{
				ActorLocation = SnapTarget;
				if (IsAutoRelease)
				{
					SetActivateSnapMode(false, FVector::ZeroVector);
				}
			}

		}
		//beam target
		SetActorLocation( ActorLocation );

		//area effect
		if( IsValid( PickUIBeam_Comp ) )
			PickUIBeam_Comp->SetWorldLocation(this->GetActorLocation());

		
	}

	if (PrevTargetActor.IsValid())
		URenderFunctionLibrary::IgnoreDarkenTarget(PrevTargetActor.Get(), false); 
	
	if (CurTargetActor.IsValid())
		URenderFunctionLibrary::IgnoreDarkenTarget(CurTargetActor.Get(), true);
}															   

bool ADragTargetingIndicator::CheckTarget(FVector curPos)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return false;

	TArray< TWeakObjectPtr<AActor> > outActors;
	AActor* NearestActor = nullptr;// RGameInstance->GameObjectMgr->GetOverlapTargetFromPosition(outActors, DragOwner.Get(), curPos, 100.0f, EFriendshipTypeEnum::VE_None);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		NearestActor = pGamePlayMode->GetOverlapTargetFromPosition(outActors, DragOwner.Get(), curPos, 100.0f, EFriendshipTypeEnum::VE_None);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		NearestActor = pGameMode->ObjectHelper.GetOverlapTargetFromPosition(outActors, DragOwner.Get(), curPos, 100.0f, EFriendshipTypeEnum::VE_None);
	}
	if (NearestActor)
	{
		// enemy
		if (URenewal_BattleFunctionLibrary::IsEnemyActor( DragOwner.Get(), NearestActor))
			SetDragTargetType(EDragTargetTypeEnum::VE_EnemyTarget);
		else
			SetDragTargetType(EDragTargetTypeEnum::VE_FriendTarget);

		FVector FloorPos;
		if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, NearestActor->GetActorLocation(), FloorPos) == false)
			return false;

		//FVector FloorPos = URenewal_BattleFunctionLibrary::GetFloorPosition(GetWorld(), NearestActor->GetActorLocation(), 0.0f, NearestActor );
		
		SetActorLocation(FloorPos);
		if( IsValid( PickUIBeam_Comp ) )
			PickUIBeam_Comp->SetWorldLocation(this->GetActorLocation());
		
		PrevTargetActor = CurTargetActor;
		CurTargetActor	= NearestActor;

		return true;
	}
	else
	{
		PrevTargetActor = CurTargetActor;
		CurTargetActor = nullptr;
	}

	return false;
}

void ADragTargetingIndicator::SetDragTargetType(EDragTargetTypeEnum InDragTargetType)
{
	CurTargetType = InDragTargetType;
	OnSetDragTargetType(InDragTargetType);
}

EDragTargetTypeEnum ADragTargetingIndicator::GetDragTargetType()
{
	return CurTargetType;
}