// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RunAnimState.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

URunAnimState::URunAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Run;
}

void URunAnimState::Initialize()
{
	Super::Initialize();

	if (OwnerActor.IsValid())
	{
		bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();		
	}
}

void URunAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);
}

void URunAnimState::Leave()
{
	Super::Leave();
}

bool URunAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (bRealTimeMode)
	{
		if (RGAMEINSTANCE(this)->RealTimeModManager)
		{
			RGAMEINSTANCE(this)->RealTimeModManager->REQ_BT_ACTOR_MOVE(OwnerActor.Get(), false, DeltaTime);
		}
	}
	return true;
}

void URunAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	Super::SetAnimParam(Command, varParam);
}

bool URunAnimState::IsValidState()
{
	return true;
}

EAnimStateEnum URunAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_Run;

	switch (Command)
	{
	case ECharacterCommandEnum::VE_Die:
	{
		ReturnAimState = EAnimStateEnum::VE_Die;
	}break;
	case ECharacterCommandEnum::VE_Abnormal:
	{
		ReturnAimState = EAnimStateEnum::VE_Abnormal;
	}break;
	case ECharacterCommandEnum::VE_ActionSkill:
	case ECharacterCommandEnum::VE_CastingSkill:
	{
		ReturnAimState = EAnimStateEnum::VE_SkillAttack;
	}break;
	case ECharacterCommandEnum::VE_StartAttack:
	{
		ReturnAimState = EAnimStateEnum::VE_NormalAttack;
	}break;
	case ECharacterCommandEnum::VE_StopMove:
	{
		ReturnAimState = EAnimStateEnum::VE_Idle;
	}break;
	default:
		break;
	}

	return ReturnAimState;
}