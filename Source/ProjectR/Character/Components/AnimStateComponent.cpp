// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimStateComponent.h"

#include "Character/AnimState/IAnimState.h"
#include "Character/AnimState/DieAnimState.h"
#include "Character/AnimState/AbnormalAnimState.h"
#include "Character/AnimState/RunAnimState.h"
#include "Character/AnimState/SkillAnimState.h"
#include "Character/AnimState/AttackAnimState.h"
#include "Character/AnimState/IdleAnimState.h"
#include "Character/AnimState/SpawnAnimState.h"
#include "Character/AnimState/AcquireAnimState.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/BattleBaseAnimInstance.h"

#include "UtilFunctionIntegrated.h"





// Sets default values for this component's properties
UAnimStateComponent::UAnimStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAnimStateComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();

	if (UCharacterHelpFunctionLibrary::IsRemoteEntity(OwnerActor.Get()))
	{
		RemoteEntity = true;
	}
	
	InitAnimComponentStates();
}

void UAnimStateComponent::UninitializeComponent()
{
	if (CurrentState)
	{
		CurrentState->Leave();
		CurrentState = nullptr;
	}
	for (auto StateItor = AnimStates.CreateConstIterator(); StateItor; ++StateItor)
	{
		UIAnimState* State = StateItor->Value;
		if (IsValid(State))
		{
			State->Uninitialize();
			State->RemoveFromRoot();
		}
	}
	//AnimStates.Empty();

	Super::UninitializeComponent();
}

// Called every frame
void UAnimStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	CurrentState->Update(DeltaTime);
	
}

void UAnimStateComponent::InitAnimComponentStates()
{
	AnimStates.Empty(8);

	// Add AnimState order of priority
	AnimStates.Emplace(EAnimStateEnum::VE_Die, NewObject< UDieAnimState >((UObject*)GetTransientPackage(), UDieAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_Abnormal, NewObject< UAbnormalAnimState >((UObject*)GetTransientPackage(), UAbnormalAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_Run, NewObject< URunAnimState >((UObject*)GetTransientPackage(), URunAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_SkillAttack, NewObject< USkillAnimState >((UObject*)GetTransientPackage(), USkillAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_NormalAttack, NewObject< UAttackAnimState >((UObject*)GetTransientPackage(), UAttackAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_Idle, NewObject< UIdleAnimState >((UObject*)GetTransientPackage(), UIdleAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_Spawn, NewObject< USpawnAnimState >((UObject*)GetTransientPackage(), USpawnAnimState::StaticClass()));
	AnimStates.Emplace(EAnimStateEnum::VE_Acquire, NewObject< UAcquireAnimState >((UObject*)GetTransientPackage(), UAcquireAnimState::StaticClass()));

	for (auto StateItor = AnimStates.CreateConstIterator(); StateItor; ++StateItor)
	{
		UIAnimState* State = StateItor->Value;
		State->AddToRoot();
		State->SetOwnerData(OwnerActor);
		State->Initialize();
	}

	if (UCharacterHelpFunctionLibrary::IsPossibleToSpawn(OwnerActor.Get()))
	{
		SetAnimComponentState(EAnimStateEnum::VE_Spawn);
	}
	else
	{	
		SetAnimComponentState(EAnimStateEnum::VE_Idle);
	}
	
}

void UAnimStateComponent::SetAnimComponentState(EAnimStateEnum NewState, ECharacterCommandEnum Command, FVariantData varParam)
{
	EAnimStateEnum CurState = EAnimStateEnum::VE_None;
	if (CurrentState)
	{
		CurState = CurrentState->GetAnimStateType();
	}

	/*int32 ActorCasting = 9999;
	int32 ActorActing = 9999;
	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillEntityComponent)
	{
		ActorCasting = SkillEntityComponent->IsSkillCasting();
		ActorActing = SkillEntityComponent->IsSkillActing();
	}*/
	
	if (CurrentState)
	{
		if (CurrentState->GetAnimStateType() == NewState)
		{
			CurrentState->SetAnimParam(Command, varParam);
			return;
		}

		CurrentState->Leave();
	}

	UIAnimState* NewAnimState = nullptr;
	if (AnimStates.Contains(NewState))
	{
		NewAnimState = AnimStates[NewState];
		if (NewAnimState)
		{
			NewAnimState->Enter(Command, varParam);
		}
	}

	CurrentState = NewAnimState;

	
}

void UAnimStateComponent::RemoteAnimComponentState(EAnimStateEnum AnimState, ECharacterCommandEnum Command, FVariantData varParam)
{
	if (CurrentState)
	{
		if (CurrentState->GetAnimStateType() == EAnimStateEnum::VE_Die)
		{
			return;
		}
	}

	SetAnimComponentState(AnimState, Command, varParam);
}

void UAnimStateComponent::DoCommand(ECharacterCommandEnum Command, FVariantData varParam)
{
	if (CurrentState)
	{
		
		IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (OwnerActor_BaseProperty)
		{
			if (false == OwnerActor_BaseProperty->IsReady())
				return;
		}

		EAnimStateEnum ResultState	= CurrentState->DoCommand(Command);
		UIAnimState* ResultAnimState = AnimStates[ResultState];
		if (ResultAnimState)
		{
			if (ResultAnimState->IsValidState())
			{
				if(RemoteEntity)
					RemoteAnimComponentState(ResultState, Command, varParam);
				else
					SetAnimComponentState(ResultState, Command, varParam);
			}
		}
	}
}

bool UAnimStateComponent::IsCancelToCurrentAnim()
{
	if (CurrentState)
	{
		IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (OwnerActor_BaseProperty)
		{
			if (OwnerActor_BaseProperty->IsReady())
			{
				return CurrentState->IsCancelAnim();
			}
		}
	}

	return false;
}

bool UAnimStateComponent::IsAbleToAttackAnim()
{
	if (CurrentState)
	{
		IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (OwnerActor_BaseProperty)
		{
			if (false == OwnerActor_BaseProperty->IsReady())
				return false;
		}

		EAnimStateEnum ResultState = CurrentState->DoCommand(ECharacterCommandEnum::VE_StartAttack, true);
		if (EAnimStateEnum::VE_NormalAttack == ResultState)
		{
			return true;
		}
	}
	return false;
}

bool UAnimStateComponent::IsAbleToRunAnim()
{
	if (CurrentState)
	{
		IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (OwnerActor_BaseProperty)
		{
			if (false == OwnerActor_BaseProperty->IsReady())
				return false;
		}

		EAnimStateEnum ResultState = CurrentState->DoCommand(ECharacterCommandEnum::VE_StartMove, true);
		if (EAnimStateEnum::VE_Run == ResultState)
		{
			return true;
		}
	}
	return false;
}

UIAnimState* UAnimStateComponent::GetAnimState(EAnimStateEnum InState)
{
	if (AnimStates.Contains(InState))
	{
		UIAnimState* AnimState = AnimStates[InState];
		if (IsValid(AnimState))
		{
			return AnimState;
		}
	}
	return nullptr;
}

EAnimStateEnum UAnimStateComponent::GetCurrentAnimStateType()
{
	EAnimStateEnum AnimStateType = EAnimStateEnum::VE_None;
	if (CurrentState)
	{
		AnimStateType = CurrentState->GetAnimStateType();
	}

	return AnimStateType;
}



//////////////////////////////////////////////////////////////////////////
