// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "EntityAIComponent.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/BaseAIController.h"
#include "Character/Components/MovingStateCheckComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/Components/CustomCharacterMovementComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "GameInfo/PlayStateInfo.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/RAI_FSM.h"
#include "Object/EmptyBasicActor.h"

// Sets default values for OwnerCharacter.Get() component's properties
UEntityAIComponent::UEntityAIComponent()
{
	// Set OwnerCharacter.Get() component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEntityAIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = Cast< AActor >(GetOwner());

	AIControlMode = RGAMEINSTANCE(this)->GameEnvironmentVar.AIControlMode;
}

void UEntityAIComponent::UninitializeComponent()
{
	if (WaypointComponent)
	{
		WaypointComponent->UnregisterComponent();
		WaypointComponent = nullptr;
	}

	Super::UninitializeComponent();
}

// Called every frame
void UEntityAIComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
// 	if (URealTimeModManager::IsControlWithRTS())
// 		return;
	if (IsValid(AIFSM))
		AIFSM->Update(DeltaTime);	

}

void UEntityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginPosition = OwnerActor->GetActorLocation();
}

void UEntityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ForcePauseHandler.IsValid())
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(ForcePauseHandler);
	}

	if (OwnerActor->GetWorldTimerManager().IsTimerActive(IgnoreForcePauseHandler))
	{
		IgnoreForcePauseTimeEnd();
	}
	if (OwnerActor->GetWorldTimerManager().IsTimerActive(IgnoreBlackOutHandler))
	{
		IgnoreBlackOutTimeEnd();
	}

	Super::EndPlay(EndPlayReason);
}

void UEntityAIComponent::StartMove(const FVector& Dest)
{
	MoveDest = Dest;
	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartMove);
	}
}

void UEntityAIComponent::StopMove()
{
	auto CharacterMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
	if (IsValid(CharacterMovementComponent))
	{
		CharacterMovementComponent->StopMovementImmediately();
	}
	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
	}
}

void UEntityAIComponent::ForceSearchTarget()
{
	if (UUtilFunctionLibrary::GetGamePlayModeEnum() == EGamePlayModeTypeEnum::VE_Arena ||
		UUtilFunctionLibrary::GetGamePlayModeEnum() == EGamePlayModeTypeEnum::VE_MirrorDungeon ||
		URealTimeModManager::IsControlWithRTS())
	{
		return;
	}

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(BattleEntityComponent) == false)
		return;

	AActor*	ResultCharacter = GetNearestSearchTarget();

	if (IsValid(ResultCharacter))
	{
		ForceDirection(ResultCharacter->GetActorLocation());		
	}
}

AActor* UEntityAIComponent::GetNearestSearchTarget()
{
	if (false == OwnerActor.IsValid())
		return nullptr;

	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityProperty == nullptr)
		return nullptr;

	if (false == EntityProperty->IsReady())
		return nullptr;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(BattleEntityComponent) == false)
		return nullptr;

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(EntityProperty->GetCharacterCategory(), EntityProperty->GetCharacterID());
	if (CharacterInfo == nullptr)
		return nullptr;

	float					SightRange = URenewal_BattleFunctionLibrary::GetCustomizedSightRange(GetWorld(), BattleEntityComponent->GetBattleInfo());
	EPriorityTypeEnum		SearchPreferType = BattleEntityComponent->GetBattleInfo()->CharacterProperty.AIPriorityType;

	AActor*	ResultCharacter = URenewal_BattleFunctionLibrary::SearchEnemy(OwnerActor.Get(), OwnerActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, SightRange, 200.0f, SearchPreferType, EPriorityTypeEnum::VE_None );

	return ResultCharacter;
}

bool UEntityAIComponent::IsInForceTargetLocation()
{
	if (TargetLocation.IsZero())
	{
		return false;
	}
	return true;
}
	 
void UEntityAIComponent::SetIdleLocation(FVector InIdleLocation)
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(OwnerActor.Get());
	if (Blackboard)
	{
		if (InIdleLocation.IsNearlyZero())
		{
			Blackboard->ClearValue(FName(TEXT("IdleLocation")));
		}
		else
		{
			Blackboard->SetValueAsVector(FName(TEXT("IdleLocation")), InIdleLocation);
		}
	}
}

void UEntityAIComponent::ForceSetStateIdle()
{
	if (AIFSM)
	{
		AIFSM->SetState(EAIStateTypeEnum::VE_Idle);
	}

	UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (AnimStateComponent)
	{
		AnimStateComponent->SetAnimComponentState(EAnimStateEnum::VE_Idle);
	}
}

void UEntityAIComponent::SetForcePauseBySkill(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee /*= true*/)
{
	EGamePlayModeTypeEnum modeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
	switch (modeType)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
		return;
 	case EGamePlayModeTypeEnum::VE_BattleField:
 		return;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
		return;
	}

	//if (UUtilFunctionLibrary::IsSelectedCharacter(OwnerActor.Get()))
	//	return;

	//if (RGameInstance->PlayStateInfo->GetSlotIdx(OwnerActor.Get()) == -1)
	//	return;

	if (false == RGAMEINSTANCE(this)->OptionManager->IsUltimateMatineeView())
		return;

	SetForcePauseProcess(SlowTime, SlowRate, BlackOutTime, IsPlayMatinee);
}

void UEntityAIComponent::SetForcePauseProcess(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee /*= true*/)
{
	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityProperty == nullptr)
		return;

	if (false == URealTimeModManager::IsRealTimeGameMode())
	{
		// Slow
		OwnerActor->GetWorldTimerManager().ClearTimer(IgnoreForcePauseHandler);
		if (SlowTime > 0.0f)
		{
			OwnerActor->GetWorldTimerManager().SetTimer(IgnoreForcePauseHandler, this, &UEntityAIComponent::IgnoreForcePauseTimeEnd, SlowTime, false);

			AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
			if (pGameMode)
			{
				pGameMode->ObjectHelper.SetAllObjectPause(SlowTime, SlowRate, OwnerActor.Get());
			}
		}

	}
}

void UEntityAIComponent::SetForcePause(bool bPause, float time )
{
	if (bPause)
	{
		ForcePauseStart(time);
	}
	else
	{
		ForcePauseEnd();
	}
}
	 
float UEntityAIComponent::GetIgnoreForcePauseTime()
{
	return OwnerActor->GetWorldTimerManager().GetTimerRemaining(IgnoreForcePauseHandler);
}
	 
void UEntityAIComponent::ForcePauseStart(float time)
{
	SetPauseSetting(true);

	OwnerActor->GetWorldTimerManager().ClearTimer(ForcePauseHandler);

	if (time > 0.0f)
	{
		OwnerActor->GetWorldTimerManager().SetTimer(ForcePauseHandler, this, &UEntityAIComponent::ForcePauseEnd, time, false);
	}
}

void UEntityAIComponent::ForcePauseEnd()
{
	SetPauseSetting(false);

	OwnerActor->GetWorldTimerManager().ClearTimer(ForcePauseHandler);
}

void UEntityAIComponent::SetPauseSetting(bool bPause)
{
	bool bEnable = !bPause;
	OwnerActor->SetActorTickEnabled(bEnable);

	TArray<UActorComponent*> components;
	OwnerActor->GetComponents(components);

	for (UActorComponent* component : components)
	{
		if (IsValid(component))
		{
			if (UWidgetDecoratorComponent::StaticClass() == component->GetClass())
			{
#if LOG_CHARACTER
				UE_LOG(LogBattle, Log, TEXT("UWidgetDecoratorComponent"));
#endif
			}
			else
			{
				component->PrimaryComponentTick.SetTickFunctionEnable(bEnable);
			}
		}
	}
			
	if (IsValid(AIFSM))
		AIFSM->PauseMove(bPause);
}
	 
void UEntityAIComponent::IgnoreForcePauseTimeEnd()
{
	OwnerActor->GetWorldTimerManager().ClearTimer(IgnoreForcePauseHandler);

	UltimateSkillActor = RGAMEINSTANCE(this)->SequenceManager->GetCurrentSequenceActor();
	if ( UltimateSkillActor.IsValid() )
	{
		UltimateSkillActor->CustomTimeDilation = 1.0f;
		UltimateSkillActor->PlaybackSettings.PlayRate = 1.f;
		UltimateSkillActor = nullptr;
	}

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.CheckIgnoreForcePauseTimeEnd(OwnerActor.Get());
	}
}

void UEntityAIComponent::BlackOutBySeconds(float time)
{
	if (OwnerActor.IsValid())
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(IgnoreBlackOutHandler);
		if (time > 0.0f)
		{
			OwnerActor->GetWorldTimerManager().SetTimer(IgnoreBlackOutHandler, this, &UEntityAIComponent::IgnoreBlackOutTimeEnd, time, false);
			AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
			if (pGameMode)
			{
				pGameMode->ObjectHelper.SetAllObjectBlackOut(OwnerActor.Get());
			}
		}
	}
}

void UEntityAIComponent::IgnoreBlackOutTimeEnd()
{
	OwnerActor->GetWorldTimerManager().ClearTimer(IgnoreBlackOutHandler);
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.CheckIgnoreBlackOutTimeEnd(OwnerActor.Get());
	}
}

void UEntityAIComponent::SetBlackboardData()
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());	
	if ( BattleEntityComponent)
	{
		UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
		if (BattleInfo)
			BattleEntityComponent->BoundingRadius = BattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_UnitRadius);
	}
}

void UEntityAIComponent::SetBlackboardForceTarget(AActor* InForceTarget)
{	
	/*
	Rai fsm
	*/
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{			
			if (IsValid(InForceTarget))
			{
				SetBlackboardForceTargetLocation(FVector::ZeroVector);
				Rai_Blackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget, InForceTarget);
				Rai_Blackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, InForceTarget);
			}
			else
				Rai_Blackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		}
	}
}

void UEntityAIComponent::SetBlackboardForceTargetLocation(FVector ForceTargetLocation /*= FVector::ZeroVector*/)
{
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillEntityComponent == nullptr)
		return;

	/*
	Rai fsm
	*/
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{			
			if (ForceTargetLocation.IsNearlyZero() == false)
			{
				Rai_Blackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
				Rai_Blackboard->SetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_TargetLocation, ForceTargetLocation);
			}
			else
			{
				SetBlackboardForceTarget(nullptr);
				Rai_Blackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_TargetLocation);
			}				
		}
	}
}

void UEntityAIComponent::SetBlackboardLeader(AActor* InTarget)
{	
	/*
	Rai fsm
	*/
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			if (IsValid(InTarget))
				Rai_Blackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_LeaderToFollow, InTarget);
			else
				Rai_Blackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_LeaderToFollow);
		}
	}
}

void UEntityAIComponent::SetBlackboardAttackLocation(FVector InAttackLocation, bool InSet /*= true*/)
{
	/*
	Rai fsm
	*/
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			if (InSet)
			{
				Rai_Blackboard->SetBlackboardVectorValue(ERAIBalckboardValueEnum::VE_AttackLocation, InAttackLocation);
			}
			else
			{
				Rai_Blackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_AttackLocation);
			}
		}
	}
}


void UEntityAIComponent::ClearAllBlackboard()
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->ClearAllBlackboardValue();
		}
	}
}

bool UEntityAIComponent::IsSetBlackboardValue(ERAIBalckboardValueEnum InBlackboardValueType)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			return Rai_Blackboard->IsSetBlackboardValue(InBlackboardValueType);
		}
	}

	return false;
}

void UEntityAIComponent::ClearBlackboardValue(ERAIBalckboardValueEnum InValueType)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->ClearBlackboardValue(InValueType);
		}
	}
}

void UEntityAIComponent::SetBlackboardActorValue(ERAIBalckboardValueEnum InBlackboardValueType, AActor* InValue)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->SetBlackboardActorValue(InBlackboardValueType, InValue);
		}
	}
}

void UEntityAIComponent::SetBlackboardVectorValue(ERAIBalckboardValueEnum InBlackboardValueType, FVector InValue)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->SetBlackboardVectorValue(InBlackboardValueType, InValue);
		}
	}
}

void UEntityAIComponent::SetBlackboardFloatValue(ERAIBalckboardValueEnum InBlackboardValueType, float InValue)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->SetBlackboardFloatValue(InBlackboardValueType, InValue);
		}
	}
}

void UEntityAIComponent::SetBlackboardBoolValue(ERAIBalckboardValueEnum InBlackboardValueType, bool InValue)
{
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			Rai_Blackboard->SetBlackboardBoolValue(InBlackboardValueType, InValue);
		}
	}
}

AActor* UEntityAIComponent::GetBlackboardActorValue(ERAIBalckboardValueEnum InBlackboardValueType )
{	
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			return Rai_Blackboard->GetBlackboardActorValue(InBlackboardValueType);
		}
	}

	return nullptr;
}

FVector UEntityAIComponent::GetBlackboardVectorValue(ERAIBalckboardValueEnum InBlackboardValueType)
{	
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			return Rai_Blackboard->GetBlackboardVectorValue(InBlackboardValueType);
		}
	}

	return FVector::ZeroVector;
}

float UEntityAIComponent::GetBlackboardFloatValue(ERAIBalckboardValueEnum InBlackboardValueType)
{	
	if (IsValid(AIFSM))
	{
		URAI_Blackboard* Rai_Blackboard = AIFSM->GetBlackboard();
		if (IsValid(Rai_Blackboard))
		{
			return Rai_Blackboard->GetBlackboardFloatValue(InBlackboardValueType);
		}
	}

	return 0.0f;
}

void UEntityAIComponent::AIStop()
{
	StopMove();

	if (IsValid(AIFSM))
		AIFSM->ActivateAI(false);

}

void UEntityAIComponent::AIRestart()
{
	if (GetAIControlMode() == EAIControlModeEnum::VE_ManualMode)
		return;

	if (IsValid(AIFSM))
		AIFSM->ActivateAI(true);



}

bool UEntityAIComponent::IsActiveAI()
{
	if (IsValid(AIFSM))
	{
		return AIFSM->IsActivateAI();
	}

	return false;
}
	 
	 
void UEntityAIComponent::AttackManualMode()
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		auto CurrentTarget = BattleEntityComponent->GetCurrentAttackTarget();
		if (CurrentTarget)
		{
			ForceDirection(CurrentTarget->GetActorLocation());
		}
	}

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartAttack);
	}
}
	 
void UEntityAIComponent::ForceDirection(FVector InTargetLocation)
{
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (BaseProperty)
	{
		if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_BattleObject)
			return;
	}

	//rotate
	FVector DirectionVector = InTargetLocation - OwnerActor->GetActorLocation();
	DirectionVector.Z = 0;
	if (DirectionVector.IsNearlyZero())
		return;

 	FVector LookVector = OwnerActor->GetActorForwardVector();
 	LookVector.Z = 0;
	LookVector = LookVector.GetSafeNormal2D();
	DirectionVector = DirectionVector.GetSafeNormal2D();

	if (LookVector == FVector::ZeroVector || DirectionVector == FVector::ZeroVector)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Normalized Vector is too small!"));
		return;
	}

	float Degree = UUtilFunctionLibrary::GetBetweenDegree(LookVector, DirectionVector);
	FRotator rot = FRotator(0.f, Degree, 0.f);

	if (!rot.ContainsNaN())
		OwnerActor->AddActorWorldRotation(rot);
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("rot cantainsNan"));
		return;
	}
}

bool UEntityAIComponent::IsUsePathfinding()
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		UBossBaseComponent* BossComponent = BattleEntityComponent->GetBossComponent();
		if (IsValid(BossComponent))
		{
			return BossComponent->IsUsePathfinding();
		}
	}

	return true;
}

void UEntityAIComponent::PrepareWayPoint()
{

}

void UEntityAIComponent::CapsuleCollision(bool Enabled)
{
	UCapsuleComponent* CapsuleComponent = Cast< UCapsuleComponent >( OwnerActor->GetComponentByClass(UCapsuleComponent::StaticClass()) );
	if (CapsuleComponent)
	{
		if (Enabled)
		{
			CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		}
		else
		{
			CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
	}
}

float UEntityAIComponent::GetModifiedAnimSpeed()
{
	if( ModifiedAnimSpeed > 0.1f )
		return ModifiedAnimSpeed;

	return 0.1f;
}


//----------------------------------------------------------------------------------------
// Waypoint
//----------------------------------------------------------------------------------------
void UEntityAIComponent::AttachWaypointComponent()
{
	WaypointComponent = NewObject< UWaypointComponent >(this);

	WaypointComponent->OnComponentCreated();
	WaypointComponent->RegisterComponent();
}


AAIController* UEntityAIComponent::GetEntityAIController()
{
	APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
	if (OwnerPawn)
	{
		return Cast< AAIController >( OwnerPawn->GetController() );
	}
	return nullptr;
}

bool UEntityAIComponent::SearchNearAttackTarget(bool SearchInSightRange)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (!IsValid(GameInstance))
		return false;


	if (GameInstance->PlayStateInfo->GetbLeaderhasTarget() == false) // 180degree
	{
		AActor* MyOwner = GetOwner();
		AActor* ResultActor = UAIFunctionLibrary::SearchEnemyInAttackSector(MyOwner, SearchInSightRange);
		if (ResultActor)
		{
			auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(MyOwner);
			if (false == IsValid(BattleEntityComponent))
				return false;

			SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor);
			BattleEntityComponent->SetCurrentAttackTarget(ResultActor);
			return true;
		}
	}
	return false;
}

ABaseAIController* UEntityAIComponent::GetEntityBaseAIController()
{
	APawn* OwnerPawn = Cast< APawn >(OwnerActor.Get());
	if (OwnerPawn)
	{
		return Cast< ABaseAIController >(OwnerPawn->GetController());
	}
	return nullptr;
}


USphereComponent* UEntityAIComponent::GetNaviCollisionSphere()
{
	//return NaviCollisionSphere;
	return nullptr;
}

void UEntityAIComponent::InitAIFSM()
{
	if (URenewal_BattleFunctionLibrary::IsControlCharacter(OwnerActor.Get()))
	{
		AIFSM = UAIFunctionLibrary::MakeFSM(OwnerActor.Get(), EAIFSMTypeEnum::VE_ControlledHero);
	}
	else if ( URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::NONE) && URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get()))
	{
		AIFSM = UAIFunctionLibrary::MakeFSM(OwnerActor.Get(), EAIFSMTypeEnum::VE_Remote);
	}
	else
	{
		AIFSM = UAIFunctionLibrary::MakeFSM(OwnerActor.Get(), EAIFSMTypeEnum::VE_Normal);
	}
	
	AIFSM->ChangeState(EAIStateTypeEnum::VE_Idle);
}

EAIStateTypeEnum UEntityAIComponent::GetCurrentAIState()
{
	if (IsValid(AIFSM))
	{
		return AIFSM->GetCurrentState();
	}

	return EAIStateTypeEnum::VE_None;
}




void UEntityAIComponent::SetAIControlMode(EAIControlModeEnum InAIControlMode)
{
	AIControlMode = InAIControlMode;

	if (AIControlMode == EAIControlModeEnum::VE_ManualMode)
	{
		if (IsValid(AIFSM))
			AIFSM->ActivateAI(false);
	}
	else
	{
		AIRestart();
	}
}

void UEntityAIComponent::SetPause(bool bPause)
{
	if (IsValid(AIFSM))
		AIFSM->PauseMove(bPause);
}