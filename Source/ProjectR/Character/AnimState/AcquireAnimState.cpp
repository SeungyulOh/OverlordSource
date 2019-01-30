// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AcquireAnimState.h"


UAcquireAnimState::UAcquireAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Acquire;
}

void UAcquireAnimState::Initialize()
{
	Super::Initialize();
}

void UAcquireAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);
}

void UAcquireAnimState::Leave()
{
	Super::Leave();
}

bool UAcquireAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	return true;
}

void UAcquireAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	Super::SetAnimParam(Command, varParam);
}

bool UAcquireAnimState::IsValidState()
{
	return true;
}

EAnimStateEnum UAcquireAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_Acquire;

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
	case ECharacterCommandEnum::VE_StopAttack:
	{
		ReturnAimState = EAnimStateEnum::VE_Idle;
	}break;
	case ECharacterCommandEnum::VE_StartMove:
	{
		ReturnAimState = EAnimStateEnum::VE_Run;
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
