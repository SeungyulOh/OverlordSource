// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "UtilFunctionIntegrated.h"
// #include "Utils/AIFunctionLibrary.h"


#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/CustomCharacterMovementComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Character/BaseAIController.h"
#include "GlobalIntegrated.h"

#include "GameInfo/PlayStateInfo.h"



//#include "GameInfo/StageInfo.h"


#include "Table/CharacterTableInfo.h"
#include "AI/RAI_FSM.h"

#include "AI/RAI_State_Idle.h"
#include "AI/RAI_State_MoveToTarget.h"
#include "AI/RAI_State_MoveToLocation.h"
#include "AI/RAI_State_MoveToAttackLocation.h"
#include "AI/RAI_State_FollowLeader.h"
#include "AI/RAI_State_Attack.h"
#include "AI/RAI_State_SkillCasting.h"
#include "AI/RAI_State_SkillActing.h"
#include "AI/RAI_State_AbnormalState.h"

#include "AI/Hero/RAI_State_Hero_Idle.h"
#include "AI/Hero/RAI_State_Hero_MoveToTarget.h"
#include "AI/Hero/RAI_State_Hero_MoveToLocation.h"
#include "AI/Hero/RAI_State_Hero_AvoidToLocation.h"
#include "AI/Hero/RAI_State_Hero_Attack.h"
#include "AI/Hero/RAI_State_Hero_ApproachToTarget.h"


#include "Network/RTSManager.h"
#include "RAI_State_TaticsMove.h"

#define MoveUpMaxHeight 100.0f

URAI_FSM* UAIFunctionLibrary::MakeFSM(AActor* InOwnerActor, EAIFSMTypeEnum InFSMType)
{
	URAI_FSM* AIFSM = NewObject< URAI_FSM >(InOwnerActor);
	AIFSM->Init(InOwnerActor);
	
	switch (InFSMType)
	{
		case EAIFSMTypeEnum::VE_Normal:
		{
			AIFSM->SetStateTableSize(10);

			URAI_State* CharState = NewObject< URAI_State_Idle>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_Idle, CharState);

			CharState = NewObject< URAI_State_MoveToTarget>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToTarget, CharState);

			CharState = NewObject< URAI_State_MoveToLocation>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToLocation, CharState);

			//CharState = NewObject< URAI_State_Hero_ApproachToTarget>(InOwnerActor);
			//CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			//AIFSM->AddState(EAIStateTypeEnum::VE_ApproachToTarget, CharState);

			CharState = NewObject< URAI_State_MoveToAttackLocation>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToAttackLocation, CharState);

			CharState = NewObject< URAI_State_FollowLeader>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_FollowLeader, CharState);

			CharState = NewObject< URAI_State_Attack>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_Attack, CharState);

			CharState = NewObject< URAI_State_SkillCasting>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_SkillCasting, CharState);

			CharState = NewObject< URAI_State_SkillActing>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_SkillActing, CharState);

			CharState = NewObject< URAI_State_AbnormalState>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_AbnormalState, CharState);
		}
		break;
		case EAIFSMTypeEnum::VE_Remote:
		case EAIFSMTypeEnum::VE_ControlledHero:
		{
			AIFSM->SetStateTableSize(11);

			URAI_State* CharState = NewObject< URAI_State_Hero_Idle>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_Idle, CharState);

			CharState = NewObject< URAI_State_Hero_MoveToTarget>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToTarget, CharState);

			CharState = NewObject< URAI_State_Hero_MoveToLocation>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToLocation, CharState);

// 			CharState = NewObject< URAI_State_Hero_ApproachToTarget>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_ApproachToTarget, CharState);			

			CharState = NewObject< URAI_State_MoveToAttackLocation>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToAttackLocation, CharState);

			CharState = NewObject< URAI_State_Hero_AvoidToLocation>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_AvoidToLocation, CharState);

			CharState = NewObject< URAI_State_FollowLeader>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_FollowLeader, CharState);

			CharState = NewObject< URAI_State_Hero_Attack>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_Attack, CharState);

			CharState = NewObject< URAI_State_SkillCasting>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_SkillCasting, CharState);

			CharState = NewObject< URAI_State_SkillActing>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_SkillActing, CharState);

			CharState = NewObject< URAI_State_AbnormalState>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_AbnormalState, CharState);

			CharState = NewObject< URAI_State_TaticsMove>(InOwnerActor);
			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
			AIFSM->AddState(EAIStateTypeEnum::VE_TaticsMove, CharState);
		}
		break;
// 		case EAIFSMTypeEnum::VE_Akhan:
// 		{
// 			AIFSM->SetStateTableSize(7);
// 
// 			URAI_State* CharState = NewObject< URAI_State_Akhan_Idle>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_Idle, CharState);
// 
// 			CharState = NewObject< URAI_State_Akhan_MoveToTarget>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_MoveToTarget, CharState);
// 
// 			CharState = NewObject< URAI_State_Akhan_CanSkillCasting>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_CanSkillCasting, CharState);
// 			
// 
// 			CharState = NewObject< URAI_State_Attack>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_Attack, CharState);
// 
// 			CharState = NewObject< URAI_State_SkillCasting>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_SkillCasting, CharState);
// 
// 			CharState = NewObject< URAI_State_SkillActing>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_SkillActing, CharState);
// 
// 			CharState = NewObject< URAI_State_Akhan_AbnormalState>(InOwnerActor);
// 			CharState->Init(InOwnerActor, AIFSM->GetBlackboard());
// 			AIFSM->AddState(EAIStateTypeEnum::VE_AbnormalState, CharState);
// 		}
// 		break;
	}

	return AIFSM;
}


AActor* UAIFunctionLibrary::SearchEnemyInAttackSector(AActor* OwnerCharacter, bool SearchInSightRange)
{
	if (IsValid(OwnerCharacter) == false)
		return nullptr;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter);
	if (IsValid(BattleEntityComponent) == false)
		return nullptr;

	if (IsValid(BattleEntityComponent->GetBattleInfo()) == false)
		return nullptr;

	float AttackRange = BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
	float OwnerBoundingRadius = BattleEntityComponent->BoundingRadius;
	float SightRange = AttackRange + OwnerBoundingRadius;
	float InRangeThreshold = BattleEntityComponent->GetBattleInfo()->CharacterProperty.RangeThreshold;

	AActor* ResultEnemy = nullptr;
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
	FVector SectorValue(SightRange, 90.0f, 0.0f);
	URenewal_BattleFunctionLibrary::FindTarget(CheckBattleCharacter, OwnerCharacter, OwnerLocation, EFriendshipTypeEnum::VE_Enemy, ESearchShapeTypeEnum::VE_Sector2D, SectorValue);
	if (CheckBattleCharacter.Num() == 0)
	{
		if(SearchInSightRange)
		{
			SightRange = URenewal_BattleFunctionLibrary::GetCustomizedSightRange(OwnerCharacter->GetWorld(), BattleEntityComponent->GetBattleInfo());
			ResultEnemy = URenewal_BattleFunctionLibrary::SearchEnemy(OwnerCharacter, OwnerLocation, ESearchShapeTypeEnum::VE_Circle, SightRange, InRangeThreshold, EPriorityTypeEnum::VE_None, EPriorityTypeEnum::VE_None);
			if (ResultEnemy == nullptr)
			{
				ResultEnemy = SearchLinkedMemberEnemy(OwnerCharacter);
			}
		}
	}
	else if (CheckBattleCharacter.Num() == 1)
	{
		ResultEnemy = CheckBattleCharacter[0].Get();
		if (false == URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerCharacter, ResultEnemy))
		{
			ResultEnemy = nullptr;
		}
	}
	else
	{
		float minDist = 10000000.0f;
		AActor* nearestCharacter = nullptr;
		for (auto& Character : CheckBattleCharacter)
		{
			if (false == URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerCharacter, Character.Get()))
				continue;

			float Dist = FVector::DistSquared(OwnerLocation, Character->GetActorLocation());
			if (Dist < minDist)
			{
				minDist = Dist;
				nearestCharacter = Character.Get();
			}
		}
		ResultEnemy = nearestCharacter;
	}

	return ResultEnemy;
}

AActor* UAIFunctionLibrary::SearchEnemy(AActor* InOwnerCharacter, bool bForceToUseAttackRange, EPriorityTypeEnum InAIPreferType )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	if ( IsValid(InOwnerCharacter) == false)
		return nullptr;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
	if (IsValid(BattleEntityComponent) == false)
		return nullptr;

	if (IsValid(BattleEntityComponent->GetBattleInfo()) == false)
		return nullptr;

	FHeroSlotInfo& LeaderSlot = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(0);
	bool bLeader = false;
	if (LeaderSlot.SlotSpawnState != EHeroSpawnStateEnum::VE_None)
	{
		if (LeaderSlot.SpawnedTargetActor == InOwnerCharacter)
		{
			bLeader = true;
		}
	}

	float SightRange = bLeader ?
		RGameInstance->GameEnvironmentVar.LeaderSightRange :
		URenewal_BattleFunctionLibrary::GetCustomizedSightRange(GameWorld, BattleEntityComponent->GetBattleInfo());

	EPriorityTypeEnum SearchPreferType = BattleEntityComponent->GetBattleInfo()->CharacterProperty.AIPriorityType;
	float InRangeThreshold = BattleEntityComponent->GetBattleInfo()->CharacterProperty.RangeThreshold;

	FVector OwnerLocation = InOwnerCharacter->GetActorLocation();
	AActor* ResultEnemy = URenewal_BattleFunctionLibrary::SearchEnemy(InOwnerCharacter, OwnerLocation, ESearchShapeTypeEnum::VE_Circle, SightRange, InRangeThreshold, SearchPreferType, InAIPreferType);
	if (ResultEnemy == nullptr)
	{
		ResultEnemy = SearchLinkedMemberEnemy(InOwnerCharacter);
	}

	if (bForceToUseAttackRange)
	{
		if (BattleEntityComponent->IsCloseEnoughForAttack(ResultEnemy, InRangeThreshold) == false )
			return nullptr;		
	}
	
	return ResultEnemy;
}

AActor* UAIFunctionLibrary::SearchFriend(AActor* InOwnerCharacter, bool bForceToUseAttackRange, EPriorityTypeEnum InAIPreferType)
{
	if (IsValid(InOwnerCharacter) == false)
		return nullptr;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
	if (IsValid(BattleEntityComponent) == false)
		return nullptr;
	
	float SightRange = BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_SightRange);
	EPriorityTypeEnum SearchPreferType = BattleEntityComponent->GetBattleInfo()->CharacterProperty.AIPriorityType;
	float InRangeThreshold = BattleEntityComponent->GetBattleInfo()->CharacterProperty.RangeThreshold;
	AActor* ResultFriend = URenewal_BattleFunctionLibrary::SearchFriend(InOwnerCharacter, InOwnerCharacter->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, SightRange, InRangeThreshold, SearchPreferType, InAIPreferType);

	if ( IsValid(ResultFriend) && bForceToUseAttackRange )
	{
		if (BattleEntityComponent->IsCloseEnoughForAttack(ResultFriend) == false)
			return nullptr;
	}

	return ResultFriend;
}

AActor*	UAIFunctionLibrary::SearchLinkedMemberEnemy( AActor* OwnerCharacter )
{
	return GetSpawnMemberAttackTarget(OwnerCharacter);
}


AActor* UAIFunctionLibrary::GetSpawnMemberAttackTarget( AActor* InOwnerCharacter)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	
// 	{
// 		//UIGamePlayMode* GamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 		//if (IsValid(GamePlayMode))
// 		//{
// 		//	AStageBase* StageBase = GamePlayMode->GetStageSpawner();
// 		//	if (StageBase)
// 		//	{
// 		//		UCharacterSpawner* OwnerSpawner = StageBase->GetOwnerSpawner(InOwnerCharacter);
// 		//		if (OwnerSpawner)
// 		//		{
// 		//			return OwnerSpawner->GetMemebersAttackTarget();
// 		//		}
// 		//	}
// 		//}
// 	}

	return nullptr;
}

FVector	UAIFunctionLibrary::GetNextWayPoint( AActor* InOwnerCharacter )
{
	if ( IsValid( InOwnerCharacter ) == false)
		return FVector::ZeroVector;

	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InOwnerCharacter );
	if (IsValid(EntityAIComponent) == false)
		return FVector::ZeroVector;

	UWaypointComponent* WayPointcomponent = EntityAIComponent->GetWaypointComponent();

	if (WayPointcomponent)
	{
		FVector Waypoint = WayPointcomponent->GetNextWayPoint();

		if (Waypoint.IsNearlyZero())
			return FVector::ZeroVector;

		return Waypoint;
	}
	return FVector::ZeroVector;
}

AActor* UAIFunctionLibrary::CheckFocusTarget(AActor* InOwnerCharacter)
{
	if (IsValid(InOwnerCharacter) == false)
		return nullptr;

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(InOwnerCharacter->GetWorld());
	if (IsValid(BasePlayerController) == false)
		return nullptr;

	if (BasePlayerController->GetIsFocusAttack() == false)
		return nullptr;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
	bool IsMyCharacter = (BattleEntityComponent && BattleEntityComponent->AmIEnemyWithMyPlayer() == false);
	if (IsMyCharacter)
	{
		//isforcetarget?
		auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InOwnerCharacter);
		if (IsValid(Character_AIEntityComponent))
		{
			auto ForceTargetActor = Character_AIEntityComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
			if (ForceTargetActor)
			{
				return nullptr;
			}
		}

		AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
		UBattleEntityComponent* SelectBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectCharacter);
		bool IsNonSelectCharacter = (SelectCharacter != nullptr && InOwnerCharacter != SelectCharacter && SelectBattleEntityComponent);
		if (IsNonSelectCharacter)
		{
			return SelectBattleEntityComponent->GetCurrentAttackTarget();			
		}
	}
	return nullptr;
}



// void UAIFunctionLibrary::IsMoveInsideTheWayPointCrash(AActor* InOwnerCharacter)
// {
// 	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
// 	if (IsValid(Target_BattleEntityComponent) == false)
// 		return;
// 	
// 	FSpawnerTransformInfo WayPoint = RGAMEINSTANCE(GEngine)->AutoPlayManager->GetNextWayPointTransform();
// 	float TargetBoundingRadius = Target_BattleEntityComponent->BoundingRadius;
// 	float Distance = InOwnerCharacter ? (WayPoint.Position - InOwnerCharacter->GetActorLocation()).Size() : 0.f;
// 
// 	//if ((TargetBoundingRadius + 500) >= (Distance))
// 	if (TargetBoundingRadius >= Distance)
// 	{
// 		UBaseStageInfo*			BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 		ALevelActor*			LevelActor = BaseStageInfo->GetLevelActor();
// 
// 		if (LevelActor)
// 			LevelActor->ConditionCheck();
// 	}
// }

void UAIFunctionLibrary::MoveInsideTheWayPointAuto(AActor* InOwnerCharacter)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->AutoPlayManager != nullptr, TEXT("%s[RGameInstance->AutoPlayManager is Empty]"), __FUNCTION__);

	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InOwnerCharacter);
	if (IsValid(EntityAIComponent) == false)
		return ;

	if (UUtilFunctionLibrary::IsLeaderCharacter(InOwnerCharacter) == false)
		return;

	UBaseStageInfo*			BaseStageInfo		=	UGamePlayBluePrintFunction::GetBaseStageInfo();
	ALevelActor*			LevelActor			=	BaseStageInfo->GetLevelActor();

	if (LevelActor == nullptr)
		return;

	ALevelActorPhase*		LevelActorPhase = LevelActor->GetLevelActorPhase();

	if (LevelActorPhase == nullptr)
		return;

	FSpawnerTransformInfo NextTransformInfo = RGameInstance->AutoPlayManager->GetNextWayPointTransform();

	if (NextTransformInfo.ID == 0xffff)
		return;

	if (LevelActorPhase->ClearCondition != EPhaseClearEnumType::VE_Move)
	{
		NextTransformInfo.ID = 0xffff;
		RGAMEINSTANCE(GEngine)->AutoPlayManager->SetResetNextWayPoint(NextTransformInfo);
	}
}


/*
Decorator..
*/


bool UAIFunctionLibrary::IsCloseEnoughToActor( AActor* SrcActor, AActor* TargetActor, float ExtraDist)
{
	if (IsValid(TargetActor) == false)
		return true;

	if ( IsValid(SrcActor) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (BattleEntityComponent)
		return BattleEntityComponent->IsCloseEnoughToActor(TargetActor, ExtraDist);

	return true;
}

bool UAIFunctionLibrary::IsCloseEnoughToLocation(AActor* SrcActor, FVector TargetLocation, float AcceptableDistance)
{
	if ( IsValid( SrcActor ) == false)
		return true;
	
	FVector DebugVector = SrcActor->GetActorLocation();
	float Dist = (SrcActor->GetActorLocation() - TargetLocation).SizeSquared2D();
	if (Dist < (AcceptableDistance*AcceptableDistance))
		return true;

	return false;
}


bool UAIFunctionLibrary::IsCloseEnoughForAttack(AActor* SrcActor, AActor* ResultEnemy)
{
	if (IsValid(ResultEnemy) == false)
		return true;

	if ( IsValid( SrcActor ) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (BattleEntityComponent)
		return BattleEntityComponent->IsCloseEnoughForAttack(ResultEnemy);

	return true;
}

bool UAIFunctionLibrary::IsDead(AActor* SrcActor)
{
	if (IsValid(SrcActor) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (IsValid(BattleEntityComponent) == false)
		return true;

	return BattleEntityComponent->IsDead();
}

bool UAIFunctionLibrary::IsInAbnormalStateFlag(AActor* SrcActor, EAbonormalStateFlagEnum InTargetStateFlag)
{
	if (IsValid(SrcActor) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (IsValid(BattleEntityComponent) == false)
		return false;

	return BattleEntityComponent->IsInAbnormalState(InTargetStateFlag);
}

bool UAIFunctionLibrary::IsInUnActableState(AActor* SrcActor)
{
	if (IsValid(SrcActor) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (IsValid(BattleEntityComponent) == false)
		return false;

	return BattleEntityComponent->IsInUnActableState();
}

bool UAIFunctionLibrary::IsInState(AActor* SrcActor, ECharacterStateTypeEnum TargetState)
{
	if (IsValid(SrcActor) == false)
		return true;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	if (IsValid(BattleEntityComponent) == false)
		return false;

	return BattleEntityComponent->IsInState(TargetState);
}

bool UAIFunctionLibrary::IsGoNextWayPoint(AActor* InOwnerCharacter, FVector TargetPos)
{
	if (IsValid(InOwnerCharacter) == false)
		return false;

	if (TargetPos.IsNearlyZero())
		return false;

	FVector diff = InOwnerCharacter->GetActorLocation() - TargetPos;

	float fLength = diff.SizeSquared2D();
	float fDefaultAcceptRadius = 50.0f;

	APawn* OwnerPawn = Cast< APawn >(InOwnerCharacter);
	if (OwnerPawn)
	{
		ABaseAIController* BaseAIController = Cast< ABaseAIController >(OwnerPawn->GetController());
		if (IsValid(BaseAIController))
		{
			auto PathFollowingComp = BaseAIController->GetPathFollowingComponent();
			if (IsValid(BaseAIController->GetPathFollowingComponent()))
			{
				fDefaultAcceptRadius = PathFollowingComp->GetDefaultAcceptanceRadius();
			}
		}
	}

	if (fLength > fDefaultAcceptRadius * fDefaultAcceptRadius)
		return true;

	return false;
}


/*
Task
*/

bool UAIFunctionLibrary::Stop(AActor* InOwnerCharacter)
{
	if (IsValid(InOwnerCharacter) == false)
		return false;

	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InOwnerCharacter);
	if (IsValid(EntityAIComponent) == false)
		return false;

	EntityAIComponent->StopMove();


	UCustomCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent(InOwnerCharacter);
	if (!IsValid(MovementComponent))
		return false;

	MovementComponent->StopMovementImmediately();

	return true;
}

bool UAIFunctionLibrary::Attack(AActor* InOwnerCharacter, AActor* InTarget)
{
	if (IsValid(InOwnerCharacter) == false)
		return false;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
	if (IsValid(BattleEntityComponent) == false)
		return false;

// 	Stop(InOwnerCharacter);

	if (IsValid(InTarget))
	{
		BattleEntityComponent->SetCurrentAttackTarget(InTarget);
	}

 	BattleEntityComponent->EnterAttack();

	return true;
}

bool UAIFunctionLibrary::StopAttack(AActor* InOwnerCharacter)
{
	if (IsValid(InOwnerCharacter) == false)
		return false;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerCharacter);
	if (IsValid(BattleEntityComponent) == false)
		return false;

	BattleEntityComponent->SetCurrentAttackTarget(nullptr);
	BattleEntityComponent->EndAttack();

	return true;
}

// if srcactor is controlled character, search long distance enemy on idle.
bool UAIFunctionLibrary::ScanEnemyOutsideSightRange(AActor* SrcActor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);

	if (IsValid(SrcActor) == false)
		return false;
	
	auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SrcActor);
	if (IsValid(Character_AIEntityComponent) /*&& bIsIdleState*/)
	{
		AActor* ResultEnemy = URenewal_BattleFunctionLibrary::SearchEnemy(SrcActor, SrcActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, 100000.0f, 10.0f, EPriorityTypeEnum::VE_None, EPriorityTypeEnum::VE_None);
		if (ResultEnemy)
		{
			Character_AIEntityComponent->SetBlackboardAttackLocation(ResultEnemy->GetActorLocation());
			return true;
		}
	}

	return false;
}

bool UAIFunctionLibrary::CheckNextWayPoint(AActor* SrcActor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->AutoPlayManager != nullptr, TEXT("%s[RGameInstance->AutoPlayManager is Empty]"), __FUNCTION__);

	auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SrcActor);
	
	FSpawnerTransformInfo NextTransformInfo = RGameInstance->AutoPlayManager->GetNextWayPointTransform();

	if (NextTransformInfo.ID == 0xffff)
		return false;

	float Dist = FVector::DistSquaredXY(SrcActor->GetActorLocation(), NextTransformInfo.Position);

	ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	float AcceptanceDist = BasePC->GetLeaderAIModeType() != EAIControlModeEnum::VE_ManualMode? 10000 : 250000;

	if (Dist > AcceptanceDist)
	{
		if (IsValid(Character_AIEntityComponent) /*&& bIsIdleState*/)
		{
			Character_AIEntityComponent->SetBlackboardAttackLocation(NextTransformInfo.Position);
			return true;
		}
	}
	else
	{
		MoveInsideTheWayPointAuto(SrcActor);
	}

	return false;
}


// follow leader
bool UAIFunctionLibrary::CheckFollow(AActor* SrcActor, bool InCheckIdleState /*=true*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);

	if (IsValid(SrcActor) == false)
		return false;

	/*UAIFunctionLibrary::DoICheckFollowLeader(SrcActor);
	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(InTargetActor->GetWorld());
	bool IsCheckDistFollowRule = (URenewal_BattleFunctionLibrary::IsControlCharacter(InTargetActor) &&
		IsValid(BasePlayerController) &&
		BasePlayerController->GetIsFollow());*/


	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SrcActor);
	bool IsMyCharacter = (BattleEntityComponent && BattleEntityComponent->AmIEnemyWithMyPlayer() == false && URenewal_BattleFunctionLibrary::IsControlCharacter(SrcActor));
	if (IsMyCharacter)
	{
// 		auto Character_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(SrcActor);
// 		if (Character_AnimStateComponent)
// 		{
// 			if (Character_AnimStateComponent->IsAbleToRunAnim() == false)
// 				return false;
// 		}

		ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(SrcActor->GetWorld());
		if (IsValid(BasePlayerController))
		{
// 			bool bIsIdleState = true;
// 			if (InCheckIdleState)
// 			{
// 				bIsIdleState = (BattleEntityComponent->GetCurrentAttackTarget() == nullptr);
// 			}
// 			else
// 			{
// 				if (BattleEntityComponent->IsAttacking())
// 					BattleEntityComponent->StopAttack();
// 			}


			auto Character_AIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SrcActor);
			if (IsValid(Character_AIEntityComponent) /*&& bIsIdleState*/)
			{
				bool IsSummonCharacter = (BattleEntityComponent->GetSummonOwner() != nullptr);
				if (IsSummonCharacter)
				{
					Character_AIEntityComponent->SetBlackboardLeader(BattleEntityComponent->GetSummonOwner());
					return true;
				}

				AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
				bool IsNonSelectCharacter = (SelectCharacter != nullptr && SrcActor != SelectCharacter);
				if (IsNonSelectCharacter)
				{
					float FollowDistance = RGameInstance->GameEnvironmentVar.FollowLeaderDistance;

					// Use some threshold, 50.0f
					if (BattleEntityComponent->IsCloseEnoughToActor(SelectCharacter, FollowDistance + 50.0f) == false)
					{
						Character_AIEntityComponent->ClearAllBlackboard();
						Character_AIEntityComponent->SetBlackboardLeader(SelectCharacter);							
						return true;
					}					
				}
			}
		}
	}
	return false;
}


bool UAIFunctionLibrary::AvoidOtherCharacter(AActor* InOwnerActor)
{
#ifdef USE_DETOUR_CROWD
	return false;
#endif
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(InOwnerActor);
	if (EntityBaseProperty == nullptr)
		return false;

	if ( IsValid(InOwnerActor) && EntityBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
	{
		ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
		AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
		if (InOwnerActor == SelectedCharacter)
			return false;

		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(EntityBaseProperty->GetCharacterID());
		if (CharacterTableInfo)
		{
			if (CharacterTableInfo->AttackType == EAttackTypeEnum::VE_Range)
				return false;
		}

		float CharacterOverlapScale = RGameInstance->GameEnvironmentVar.CharacterOverlapScale;
		TArray< TWeakObjectPtr<AActor> > ResultActors;
		// TODO, use team NO instead of friendship type

		AActor* NearestActor = nullptr;
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
		if (pGameMode)
			NearestActor = pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
		
		if (NearestActor)
		{			
			auto NearestActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(NearestActor);
			if (NearestActor_BattleEntityComponent)
			{
				FVector AvoidDir = GetAvoidVectorFromOtherCharacter(InOwnerActor, NearestActor);

#ifdef CHECK_NEXT_MOVE_VALIDATION
				auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerActor);
				float AvoidDist = NearestActor_BattleEntityComponent->GetBoundingRadius();
				if (OwnerActor_BattleEntityComponent)
					AvoidDist = OwnerActor_BattleEntityComponent->GetBoundingRadius() + NearestActor_BattleEntityComponent->GetBoundingRadius() + 100.0f;

				FVector AvoidLocation = NearestActor->GetActorLocation() + AvoidDir * AvoidDist;
				if (UAIFunctionLibrary::CanMoveToTargetPosition(InOwnerActor, AvoidLocation))
#endif
				{
					auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InOwnerActor);
					if (OwnerActor_EntityAIComponent)
					{

						OwnerActor_EntityAIComponent->SetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_AvoidanceVector, AvoidDir);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool UAIFunctionLibrary::PushOverlappedCharacter(AActor* InOwnerActor)
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
// 	checkf(RGameInstance->GameObjectMgr != nullptr, TEXT("%s[RGameInstance->GameObjectMgr is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(InOwnerActor);
	if (EntityBaseProperty == nullptr)
		return false;

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);

	if (IsValid(InOwnerActor) && EntityBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
	{
		float CharacterOverlapScale = RGameInstance->GameEnvironmentVar.CharacterOverlapScale;
		TArray< TWeakObjectPtr<AActor> > ResultActors;
		// TODO, use team NO instead of friendship type
		//RGameInstance->GameObjectMgr->GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
		
// 		UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 		if (pGamePlayMode)
// 		{
// 			pGamePlayMode->GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
// 		}
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
		}
		for( auto& overlappedActor : ResultActors )
		{
			if (BasePlayerController->IsSelectedCharacter(overlappedActor.Get()))
				continue;

			UEntityAIComponent* Target_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(overlappedActor.Get());
			if (Target_EntityAIComponent == nullptr)
				continue;

			if (Target_EntityAIComponent->GetCurrentAIState() == EAIStateTypeEnum::VE_Idle ||
				Target_EntityAIComponent->GetCurrentAIState() == EAIStateTypeEnum::VE_Attack)
			{
				
				FVector OwnerForward = InOwnerActor->GetActorForwardVector();
				FVector AvoidDir = overlappedActor->GetActorLocation() - InOwnerActor->GetActorLocation();
				if (UUtilFunctionLibrary::GetBetweenDirFrontBack(OwnerForward, AvoidDir) == ECharacterDirection4REnum::VE_Front)
				{
					FVector LaunchDir = AvoidDir - FVector::DotProduct(OwnerForward, AvoidDir) * OwnerForward;

					LaunchDir.Z = 0.0f;
					LaunchDir.Normalize();

					ACharacter* TargetActor = Cast< ACharacter >(overlappedActor.Get());
					if (IsValid(TargetActor))
					{
						TargetActor->LaunchCharacter(LaunchDir * 400.0f, true, false);						
					}
				}				
			}
		}
	}

	return false;
}

FVector UAIFunctionLibrary::GetAvoidVectorFromOtherCharacter(AActor* InOwnerActor, AActor* InNearestActor )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
// 	checkf(RGameInstance->GameObjectMgr != nullptr, TEXT("%s[RGameInstance->GameObjectMgr is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(InOwnerActor);
	if (EntityBaseProperty == nullptr)
		return FVector::ZeroVector;

	if (IsValid(InOwnerActor) && EntityBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
	{
		float CharacterOverlapScale = RGameInstance->GameEnvironmentVar.CharacterOverlapScale;
		TArray< TWeakObjectPtr<AActor> > ResultActors;
		// TODO, use team NO instead of friendship type
		AActor* NearestActor = InNearestActor;
		if(IsValid( InNearestActor) == false )		
		{
			//NearestActor = RGameInstance->GameObjectMgr->GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
// 			UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 			if (pGamePlayMode)
// 			{
// 				NearestActor = pGamePlayMode->GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
// 			}
			AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
			if (pGameMode)
			{
				NearestActor = pGameMode->ObjectHelper.GetOverlapTargets(ResultActors, InOwnerActor, EFriendshipTypeEnum::VE_Friend, CharacterOverlapScale);
			}
		}
		if (NearestActor)
		{
			FVector TargetDir = InOwnerActor->GetActorLocation() - NearestActor->GetActorLocation();
			TargetDir.Normalize();
/*
			ECharacterDirection4REnum DirectionValue = UUtilFunctionLibrary::GetBetweenDirLeftRight(InOwnerActor->GetActorForwardVector(), TargetDir);
			FVector AvoidDir = InOwnerActor->GetActorRightVector();
			AvoidDir.Z = 0.0f;
			// if target actor is placed on right side, avoid dir should be left direction.
			if (DirectionValue == ECharacterDirection4REnum::VE_Right )
				AvoidDir *= -1;
*/
			FVector AvoidDir = InOwnerActor->GetActorForwardVector() + TargetDir;			
			return AvoidDir;
		}
	}

	return FVector::ZeroVector;
}


bool UAIFunctionLibrary::CanMoveToTargetPosition( AActor* InActor, FVector InTargetPos)
{
	if (!IsValid(InActor))
		return false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(RGameInstance->GridMapManager != nullptr, TEXT("%s[RGameInstance->GridMapManager is Empty]"), __FUNCTION__);


	if (UMapFunctionLibrary::IsCollideInMoving(InActor, InTargetPos))
		return false;
	
	FVector ProjectionPoint;
	auto bProjection = UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, InTargetPos, ProjectionPoint);
	if (bProjection == false)
		return false;

	bool bSuccessMove = UMapFunctionLibrary::IsEnableNavigationPos(InActor,ProjectionPoint, FVector::ZeroVector, false);
	if (bSuccessMove == false)
		return false;

	auto MapState = RGameInstance->GridMapManager->GetMapInfoFromWorldPos(InTargetPos);
	if (MapState.CellInfo == EMapCellStateEnum::VE_Invalid || MapState.CellInfo == EMapCellStateEnum::VE_None)
		return true;

	if (CanMoveUp(InActor, MapState.Height) == false)
		return false;

	return true;
}

bool UAIFunctionLibrary::CanMoveUp( AActor* InActor, const float& GridHeight)
{
	if (!IsValid(InActor))
		return false;

	auto CharacterHeight = InActor->GetActorLocation().Z;
	auto DiffHeight = GridHeight - CharacterHeight;
	if (DiffHeight <= MoveUpMaxHeight)
	{
		return true;
	}

	return false;
}

void UAIFunctionLibrary::SetCrewLocation(AActor* InActor, FVector& InLocation)
{
	if (!IsValid(InActor))
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UPlayStateInfo* pStateInfo = RGameInstance->PlayStateInfo;
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("%s[RGameInstance is Empty]"), __FUNCTION__);
	checkf(pStateInfo != nullptr, TEXT("%s[pStateInfo is Empty]"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	InLocation = InActor->GetActorLocation();

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	AActor* LeaderActor = BasePlayerController->GetSelectedCharacter();
	if (IsValid(LeaderActor))
	{
		InLocation = LeaderActor->GetActorLocation();

		FHeroSlotInfo* slot = pStateInfo->GetSelectedCharacterSlotCharacter(InActor);
		if (slot == nullptr)
			return;
		FCharacterTableInfo* pCharacterTable = UTableManager::GetInstancePtr()->GetCharacterRow(slot->HeroIDName);
		checkf(pCharacterTable != nullptr, TEXT("%s[pCharacterTable is Empty]"), __FUNCTION__);
		FCrewSpotTableInfo* pCrewSpotTable = UTableManager::GetInstancePtr()->GetCrewSpotTableRow(pCharacterTable->CrewSpot);
		checkf(pCrewSpotTable != nullptr, TEXT("%s[pCrewSpotTable is Empty]"), __FUNCTION__);

		FVector2D spot2d;
		spot2d.Set(0, 0);
		if (slot->CharacterRole == ECharacterRoleEnum::VE_Crew1)
			spot2d = pCrewSpotTable->Crew1Spot;
		else if (slot->CharacterRole == ECharacterRoleEnum::VE_Crew2)
			spot2d = pCrewSpotTable->Crew2Spot;



		FTransform TargetTransform = LeaderActor->GetActorTransform();
		FSpawnerTransformInfo Element;

		Element.Position.Set(spot2d.X, spot2d.Y, 0);
		Element.Rotation = TargetTransform.Rotator();

		FVector SpawnPos = Element.Position;
		SpawnPos = SpawnPos.RotateAngleAxis(TargetTransform.Rotator().Yaw, FVector::UpVector);
		TargetTransform.AddToTranslation(SpawnPos);
			
		FRotator TargetRot = TargetTransform.Rotator();
		TargetTransform.SetRotation(TargetRot.Quaternion());
		Element.Position = TargetTransform.GetLocation();
		Element.Position.Z = 0;


		InLocation.X = Element.Position.X;
		InLocation.Y = Element.Position.Y;
	}
}
