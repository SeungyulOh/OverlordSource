// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RemoteMoveComponent.h"

#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "Character/AnimState/IAnimState.h"
#include "Character/AnimState/DieAnimState.h"
#include "Character/AnimState/AbnormalAnimState.h"
#include "Character/AnimState/RunAnimState.h"
#include "Character/AnimState/SkillAnimState.h"
#include "Character/AnimState/AttackAnimState.h"
#include "Character/AnimState/IdleAnimState.h"
#include "Character/AnimState/SpawnAnimState.h"
#include "Character/AnimState/AcquireAnimState.h"


// Sets default values for this component's properties
URemoteMoveComponent::URemoteMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void URemoteMoveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();
	if (OwnerActor.IsValid())
	{
		auto BaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
		if (BaseProperty)
		{
			auto RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance->HttpClient->IsValidKID(BaseProperty->GetUserKID()))
				UE_LOG(LogRTS, Log, TEXT("TickComponent What"));
		}
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (IsValid(OwnerCharacter))
	{
		if (OwnerCharacter->GetCharacterMovement())
		{
			OwnerCharacter->GetCharacterMovement()->bUseRVOAvoidance = false;
		}

		StartPosition = OwnerCharacter->GetActorLocation();
		TargetPosition = StartPosition;
		Init();
	}
}

// Called every frame
void URemoteMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (OwnerActor.IsValid())
	{
		auto BaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
		if (BaseProperty)
		{
			auto RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance->HttpClient->IsValidKID(BaseProperty->GetUserKID()))
				UE_LOG(LogRTS, Log, TEXT("TickComponent What"));
		}
	}

	ARaidRGameMode* RaidGameMode = Cast<ARaidRGameMode>(UGameplayStatics::GetGameMode(OwnerActor->GetWorld()));
	if (RaidGameMode->GameModeState != EGameModeStateEnum::VE_Start)
		return;

	if (IsMoveActive())
	{
		if (IsArrive())
		{
			StopMoving();
		}
		else
		{
			MoveToTargetPosition(DeltaTime);
		}
	}
}

void URemoteMoveComponent::Init()
{
	bMoveActive = true;
}

bool URemoteMoveComponent::IsArrive()
{
	if (OwnerActor.IsValid() == false)
		return false;

	FVector CurrentPosition = TargetPosition - OwnerActor->GetActorLocation();
	CurrentPosition.Z = 0.f;
	if(CurrentPosition.IsNearlyZero())
	{
		return true;
	}

	return false;
}

bool URemoteMoveComponent::IsMoveActive() const
{
	return bMoveActive;
}


void URemoteMoveComponent::MoveToTargetPosition(float DeltaTime)
{
	if (IsMoveActive() == false)
		return;

	if (OwnerActor.IsValid() == false)
		return;

	UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComp)
	{
		FVector CurrPos = OwnerActor.Get()->GetActorLocation();
		FVector MoveDir = TargetPosition - CurrPos;
		float RemainDist = MoveDir.Size();
		MoveDir.Normalize();

		float MovableDist = 0;
		UCharacterBattleInfo* BattleInfo = BattleEntityComp->GetBattleInfo();
		if (BattleInfo)
		{
			float MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
			MovableDist = MoveSpeed * DeltaTime;
		}

		FVector NextPos = CurrPos;
		if (MovableDist >= RemainDist)
		{
			auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AnimStateComponent))
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
			}
		}
		else
		{
			NextPos = CurrPos + (MoveDir * MovableDist);

			auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AnimStateComponent))
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartMove);

				if (OwnerActor_AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run)
				{
					FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
					OwnerActor->SetActorRotation(lookatRotation);
					OwnerActor->SetActorLocation(NextPos);
				}
			}
		}
		
	}
}

void URemoteMoveComponent::StopMoving()
{
 	//bMoveActive = false;

	if (OwnerActor.IsValid() == false)
		return;

	auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (false == IsValid(EntityAIComponent))
		return;

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent) && OwnerActor_AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Run )
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
		EntityAIComponent->StopMove();
		OwnerActor->SetActorLocation(TargetPosition);
	}
}

void URemoteMoveComponent::Update(FRTS_ACTOR_UPDATE _update)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (IsValid(OwnerCharacter))
	{
		StartPosition = OwnerCharacter->GetActorLocation();
		FVector Diff = _update.location - StartPosition;
		if (Diff.IsNearlyZero() == false)
		{
			TargetPosition = _update.location;
			DirectionVector = TargetPosition - StartPosition;
			DirectionVector.Z = 0;

			//DrawDebugBox(OwnerActor->GetWorld(), TargetPosition, FVector(20.f, 20.f, 20.f), FColor::Red, false, 1.f, 0.f, 5.f);
		}
	}
}