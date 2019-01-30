// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SpawnAnimState.h"


USpawnAnimState::USpawnAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Spawn;
}

void USpawnAnimState::Initialize()
{
	Super::Initialize();
}

void USpawnAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);
}

void USpawnAnimState::Leave()
{
	Super::Leave();
}

bool USpawnAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	return true;
}

void USpawnAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	//Super::SetAnimParam(Command, varParam);
}

bool USpawnAnimState::IsValidState()
{
	return true;
}

EAnimStateEnum USpawnAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_Spawn;

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
