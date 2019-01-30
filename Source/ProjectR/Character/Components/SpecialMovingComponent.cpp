
#include "ProjectR.h"
#include "SpecialMovingComponent.h"

#include "Character/BattleBaseCharacter.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"

USpecialMovingComponent::USpecialMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
#ifdef RULE_TIMERACTOR
	PrimaryComponentTick.bCanEverTick = false;
#else
	PrimaryComponentTick.bCanEverTick = true;
#endif

	// ...

}

void USpecialMovingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerCharacter = Cast<ABattleBaseCharacter>(GetOwner());
}

void USpecialMovingComponent::UninitializeComponent()
{

	Super::UninitializeComponent();
}

void USpecialMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	switch (MoveType)
	{
	case ESpecialMoveTypeEnum::VE_Knockback:
		UpdateKnockBack(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Fear:
		UpdateFear(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Captivate:
		UpdateCaptivate(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Dash:
		UpdateDash(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Pull:
		UpdatePull(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Blink:
		UpdateBlink(DeltaTime);
		break;
	case ESpecialMoveTypeEnum::VE_Push:
		UpdatePush(DeltaTime);
		break;
	}
}



void USpecialMovingComponent::StartKnockBack(const FVector& InDirection, float InDuration)
{
	Duration = InDuration;
	TargetDirection = InDirection;

	MoveType = ESpecialMoveTypeEnum::VE_Knockback;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_KnockBack, true);
	}

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;
}

void USpecialMovingComponent::UpdateKnockBack(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
		StopKnockBack();
	else
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		if (BattleEntityComponent)
		{
			UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
			if (BattleInfo && ElapsedTime < Duration * 0.4f)
			{
				MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
				FVector TargetLoc = OwnerCharacter->GetActorLocation() + TargetDirection * MoveSpeed * delta_time;

				UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OwnerCharacter->GetWorld());
				if (navSystem)
				{
					FNavLocation navLocation;
					bool bSucess = navSystem->ProjectPointToNavigation(TargetLoc, navLocation);
					if (bSucess)
					{
						TargetLoc.X = navLocation.Location.X;
						TargetLoc.Y = navLocation.Location.Y;
						OwnerCharacter->SetActorLocation(TargetLoc);
					}
						
				}

				
			}
		}
	}
}

void USpecialMovingComponent::StopKnockBack()
{
	ElapsedTime = 0.f;
	Duration = 0.f;
	MoveType = ESpecialMoveTypeEnum::VE_None;
	TargetDirection = FVector::ZeroVector;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_KnockBack, false);
	}

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}


void USpecialMovingComponent::StartPush(const FVector& InDirection, float InDuration)
{
	Duration = InDuration;
	TargetDirection = InDirection;

	MoveType = ESpecialMoveTypeEnum::VE_Push;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Push, true);
	}
	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;
}

void USpecialMovingComponent::StopPush()
{
	ElapsedTime = 0.f;
	Duration = 0.f;
	MoveType = ESpecialMoveTypeEnum::VE_None;
	TargetDirection = FVector::ZeroVector;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Push, false);
	}
	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}

void USpecialMovingComponent::StopSpecialMoving()
{
	MoveType = ESpecialMoveTypeEnum::VE_None;

	if (CachedEntityAIComponent)
		CachedEntityAIComponent->AIRestart();

	if (CachedEntityAIComponent)
		CachedEntityAIComponent->StopMove();
}

void USpecialMovingComponent::StartFear(AActor* InTargetActor, float InDuration)
{
	if (!IsValid(InTargetActor))
		return;

	MoveType = ESpecialMoveTypeEnum::VE_Fear;

	TargetActor = InTargetActor;
	StartPos = TargetActor->GetActorLocation();
	Duration = InDuration;
	ElapsedTime = 0.0f;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->GetBattleInfo())
		{
			MoveSpeed = BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
		}
	}

	CachedMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerCharacter.Get());
	CachedEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (CachedEntityAIComponent)
		CachedEntityAIComponent->AIStop();

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;
}

void USpecialMovingComponent::StopFear()
{
	StopSpecialMoving();

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}

void USpecialMovingComponent::StartCaptivate(AActor* InTargetActor, float InDuration)
{
	if (!IsValid(InTargetActor))
		return;

	MoveType = ESpecialMoveTypeEnum::VE_Captivate;

	TargetActor = InTargetActor;
	Duration = InDuration;
	ElapsedTime = 0.0f;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->GetBattleInfo())
		{
			MoveSpeed = BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
		}
	}

	CachedMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerCharacter.Get());
	CachedEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (CachedEntityAIComponent)
		CachedEntityAIComponent->AIStop();

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;

}

void USpecialMovingComponent::StopCaptivate()
{
	StopSpecialMoving();


	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}

void USpecialMovingComponent::UpdateFear(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
	{
		StopFear();
	}
	else
	{
		if (OwnerCharacter.IsValid() && CachedMovementComponent != nullptr && CachedEntityAIComponent != nullptr)
		{
			if (TargetActor.IsValid())
			{
				StartPos = TargetActor->GetActorLocation();
			}

			FVector MoveDir = OwnerCharacter->GetActorLocation() - StartPos;
			MoveDir.Normalize();
			MoveDir.Z = 0.0f;
			MoveDir = MoveDir * delta_time * MoveSpeed;

			FVector TargetLoc = OwnerCharacter->GetActorLocation() + MoveDir;
			CachedMovementComponent->Velocity = MoveDir;
			CachedMovementComponent->RequestDirectMove(MoveDir, true);
			CachedEntityAIComponent->StartMove(TargetLoc);
		}
	}
}

void USpecialMovingComponent::UpdateCaptivate(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
	{
		StopCaptivate();
	}
	else
	{
		if (OwnerCharacter.IsValid() && CachedMovementComponent != nullptr && CachedEntityAIComponent != nullptr)
		{
			if (TargetActor.IsValid())
			{
				StartPos = TargetActor->GetActorLocation();
			}

			FVector MoveDir = StartPos - OwnerCharacter->GetActorLocation();
			MoveDir.Normalize();
			MoveDir.Z = 0.0f;
			MoveDir = MoveDir * MoveSpeed * 0.2f;

			FVector TargetLoc = OwnerCharacter->GetActorLocation() + MoveDir;
			CachedMovementComponent->Velocity = MoveDir;
			CachedMovementComponent->RequestDirectMove(MoveDir, false);
			CachedEntityAIComponent->StartMove(TargetLoc);
		}
	}

}



void USpecialMovingComponent::StartDash(const FVector& InTargetLocation, float InDuration, float InDistance)
{

	if (false == OwnerCharacter.IsValid())
	{
		return;
	}
	FVector CurrPos = OwnerCharacter->GetActorLocation();
	FVector Dir = InTargetLocation - CurrPos;
	Dir.Z = 0.0f;
	float Dist = (0.0f < InDistance ? InDistance : Dir.Size2D());
	Dir.Normalize();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UGridMapManager* grid_mgr = (RGameInstance != nullptr) ? RGameInstance->GridMapManager : nullptr;
	if (nullptr == grid_mgr)
	{
		return;
	}

	Duration = InDuration;
	ElapsedTime = 0.0f;

	if (3000.0f < InDistance)
	{
		InDistance = 3000.0f;
	}

	if (0.0f < InDistance && false == Dir.IsZero())
	{
		MoveType = ESpecialMoveTypeEnum::VE_Dash;

		CachedMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerCharacter.Get());
		CachedEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
		if (CachedEntityAIComponent)
			CachedEntityAIComponent->AIStop();

		StartPos = OwnerCharacter.Get()->GetActorLocation();
		TargetDistance = grid_mgr->GetFarthestMovableDistance(StartPos, Dir, InDistance);

		if (TargetDistance <= 0)
		{
			StopDash();
			return;
		}

		TargetDirection = Dir.GetSafeNormal2D();
		TargetDirection *= TargetDistance;
	}

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;



}

void USpecialMovingComponent::StopDash()
{
	StopSpecialMoving();

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
	{
		RemoteMoveComponent->bMoveActive = true;
		RemoteMoveComponent->TargetPosition = OwnerCharacter->GetActorLocation();
		UMapFunctionLibrary::AdjustActorLocation(OwnerCharacter.Get(), RemoteMoveComponent->TargetPosition);
	}
		
}


void USpecialMovingComponent::StartPull(const FVector& InTargetLocation, float InDuration)
{
	TargetLocation = InTargetLocation;
	Duration = InDuration;
	StartPos = OwnerCharacter->GetActorLocation();

	MoveType = ESpecialMoveTypeEnum::VE_Pull;
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Pull, true);
	}

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;
}

void USpecialMovingComponent::StopPull()
{
	ElapsedTime = 0.f;
	TargetLocation = FVector::ZeroVector;
	Duration = 0.f;
	MoveType = ESpecialMoveTypeEnum::VE_None;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (BattleEntityComponent)
	{
		BattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_Pull, false);
	}

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}

void USpecialMovingComponent::StartBlink(const FVector& InTargetLocation, const FRotator& InTargetRotation, float InDuration)
{
	TargetLocation = InTargetLocation;
	TargetRotator = InTargetRotation;
	Duration = InDuration;
	ElapsedTime = 0.f;
	MoveType = ESpecialMoveTypeEnum::VE_Blink;

	UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerCharacter.Get());
	if (RenderComponent)
		RenderComponent->SetVisible(false);

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = false;
}

void USpecialMovingComponent::StopBlink()
{
	Duration = 0.f;
	ElapsedTime = 0.f;
	MoveType = ESpecialMoveTypeEnum::VE_None;

	UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerCharacter.Get());
	if (RenderComponent)
		RenderComponent->SetVisible(true);

	OwnerCharacter->SetActorLocation(TargetLocation);
	OwnerCharacter->SetActorRotation(TargetRotator);

	TargetLocation = FVector::ZeroVector;
	TargetRotator = FRotator::ZeroRotator;

	URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerCharacter->GetComponentByClass(URemoteMoveComponent::StaticClass()));
	if (RemoteMoveComponent)
		RemoteMoveComponent->bMoveActive = true;
}

void USpecialMovingComponent::UpdateDash(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
	{
		StopDash();
	}
	else
	{
		if (OwnerCharacter.IsValid())
		{
			FVector curPos = FMath::Lerp<FVector>(StartPos, StartPos + TargetDirection, ElapsedTime / Duration);

			if(UMapFunctionLibrary::IsProjectionPointOnNavyMesh(OwnerCharacter.Get(), curPos))
			{
				UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(OwnerCharacter.Get());
				if (ActorCapsuleComponent)
				{
					curPos.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
				}

				UE_LOG(LogGamePlay, Log, TEXT("character pos : x-%f , y-%f"), curPos.X, curPos.Y);
				UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OwnerCharacter->GetWorld());
				if (navSystem)
				{
					FNavLocation navLocation;
					bool bSucess = navSystem->ProjectPointToNavigation(curPos, navLocation);
					if (bSucess)
					{
						curPos.X = navLocation.Location.X;
						curPos.Y = navLocation.Location.Y;
						OwnerCharacter->SetActorLocation(curPos);
					}

				}
			}
		}
	}
}

void USpecialMovingComponent::UpdatePull(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
	{
		StopPull();
	}
	else
	{
		if (OwnerCharacter.IsValid())
		{
			FVector curPos = FMath::Lerp<FVector>(StartPos, TargetLocation , ElapsedTime / Duration);

			if (UMapFunctionLibrary::IsProjectionPointOnNavyMesh(OwnerCharacter.Get(), curPos))
			{
				UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(OwnerCharacter.Get());
				if (ActorCapsuleComponent)
				{
					curPos.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
				}

				UE_LOG(LogGamePlay, Log, TEXT("character pos : x-%f , y-%f"), curPos.X, curPos.Y);
				OwnerCharacter->SetActorLocation(curPos);
			}
		}
	}
}

void USpecialMovingComponent::UpdateBlink(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
		StopBlink();
}

void USpecialMovingComponent::UpdatePush(float delta_time)
{
	ElapsedTime += delta_time;
	if (ElapsedTime >= Duration)
		StopPush();
	else
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
		if (BattleEntityComponent)
		{
			UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
			if (BattleInfo && ElapsedTime < Duration * 0.4f)
			{
				MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
				FVector TargetLoc = OwnerCharacter->GetActorLocation() + TargetDirection * MoveSpeed * delta_time;
				UNavigationSystem* navSystem = UNavigationSystem::GetNavigationSystem(OwnerCharacter->GetWorld());
				if (navSystem)
				{
					FNavLocation navLocation;
					bool bSucess = navSystem->ProjectPointToNavigation(TargetLoc, navLocation);
					if (bSucess)
					{
						TargetLoc.X = navLocation.Location.X;
						TargetLoc.Y = navLocation.Location.Y;
						OwnerCharacter->SetActorLocation(TargetLoc);
					}
				}
			}
		}
	}
}
