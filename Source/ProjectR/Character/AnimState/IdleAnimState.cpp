// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "IdleAnimState.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/BattleBaseAnimInstance.h"

UIdleAnimState::UIdleAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Idle;
}

void UIdleAnimState::Initialize()
{
	Super::Initialize();
	if (OwnerActor.IsValid())
		bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();
	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (OwnerActor_BaseProperty)
	{
		// SpecialIdle
		if (OwnerActor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Npc ||
			OwnerActor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Minion ||
			OwnerActor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_BattleObject)
		{
			bCalcSpecialIdle = true;

			// Special Idle
			SpecialIdleCheckTime = ((FMath::Rand() % 400) * 0.01f) + 4.0f;

			if (FMath::RandBool())
			{
				IdleType = EIdleTypeEnum::VE_SpecialIdle;
			}
			else
			{
				IdleType = EIdleTypeEnum::VE_NormalIdle;;
			}
		}
	}
}

void UIdleAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);

	SetIdleType(IdleType);
}

void UIdleAnimState::Leave()
{
	Super::Leave();
}

bool UIdleAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	// SpecialIdle
	if (bCalcSpecialIdle)
	{
		SpecialIdleElapsedTime += DeltaTime;
		if (SpecialIdleElapsedTime > SpecialIdleCheckTime)
		{
			SpecialIdleElapsedTime = 0.0f;

			if (FMath::RandBool())
			{
				SetIdleType(EIdleTypeEnum::VE_SpecialIdle);
			}
			else
			{
				SetIdleType(EIdleTypeEnum::VE_NormalIdle);
			}
		}
	}
	if (bRealTimeMode)
	{
		auto RGameInstance = RGAMEINSTANCE(this);
		if (RGameInstance->RealTimeModManager)
			RGameInstance->RealTimeModManager->REQ_BT_ACTOR_MOVE(OwnerActor.Get(), true, DeltaTime);
	}
	return true;
}

void UIdleAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	Super::SetAnimParam(Command, varParam);
}

bool UIdleAnimState::IsValidState()
{
	return true;
}

EAnimStateEnum UIdleAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_Idle;

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
	case ECharacterCommandEnum::VE_StartMove:
	{
		ReturnAimState = EAnimStateEnum::VE_Run;
	}break;
	case ECharacterCommandEnum::VE_Acquire:
	{
		ReturnAimState = EAnimStateEnum::VE_Acquire;
	}break;
	default:
		break;
	}

	return ReturnAimState;
}

//////////////////////////////////////////////////////////////////////////

void UIdleAnimState::SetIdleType(EIdleTypeEnum InIdleType)
{
	IdleType = InIdleType;

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetIdleType(IdleType);
	}

	
}