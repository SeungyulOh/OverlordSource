// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "IAnimState.h"

#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "UtilFunctionIntegrated.h"

void UIAnimState::Initialize()
{
	BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
}

void UIAnimState::Uninitialize()
{

}

void UIAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
// 	if (URenewal_BattleFunctionLibrary::IsControlCharacter(OwnerActor.Get()) == LOGFLAG_HERO)
// 	{
// 		FText	AnimStateName = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAnimStateEnum"), (int32)AnimStateType);
// 		UE_LOG(LogBattle, Log, TEXT("UIAnimState::Enter - %s AnimStateType %d"), *(OwnerActor->GetName()), *(AnimStateName.ToString()));
// 	}

	ActiveState = true;

	ChangeAniminstance(AnimStateType);
	
	SetAnimParam(Command, varParam);
}

void UIAnimState::Leave()
{
// 	if (URenewal_BattleFunctionLibrary::IsControlCharacter(OwnerActor.Get()) == LOGFLAG_HERO)
// 	{
// 		FText	AnimStateName = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAnimStateEnum"), (int32)AnimStateType);
// 		UE_LOG(LogBattle, Log, TEXT("UIAnimState::Leave - %s AnimStateType %s"), *(OwnerActor->GetName()), *(AnimStateName.ToString()));
// 	}

	ActiveState = false;
}

bool UIAnimState::Update(float DeltaTime)
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		if (BattleBaseAnimInstance->GetIsLimitPlayRate() == false)
			return true;

		if (EntityAIComponent.IsValid())
		{
			// Attack Anim Speed
			BattleBaseAnimInstance->SetAttackAnimPlayRate(EntityAIComponent->GetModifiedAnimSpeed());
		}
		if (BattleEntityComponent.IsValid())
		{
			// Move Speed
			BattleBaseAnimInstance->SetMoveAnimPlayRate(BattleEntityComponent->GetMoveAniSpeed());
		}
	}

	return true;
}

void UIAnimState::ChangeAniminstance(EAnimStateEnum InAnimState)
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetAnimState(InAnimState);
	}
}

void UIAnimState::SetOwnerData(TWeakObjectPtr< AActor > InOwner)
{
	OwnerActor = InOwner;
}

UBattleBaseAnimInstance* UIAnimState::GetBaseAnimInstance()
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = nullptr;

	if (OwnerActor.IsValid())
	{
		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if (OwnerCharacter)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				BattleBaseAnimInstance = Cast< UBattleBaseAnimInstance >(AnimInstance);
			}
		}
	}

	return BattleBaseAnimInstance;
}