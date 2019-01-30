// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AbnormalAnimState.h"

#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "Utils/CharacterHelpFunctionLibrary.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillSlot.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "Utils/AIFunctionLibrary.h"


UAbnormalAnimState::UAbnormalAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Abnormal;
}

void UAbnormalAnimState::Initialize()
{
	Super::Initialize();
}

void UAbnormalAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);

	if (SkillEntityComponent.IsValid())
	{
		ElapsedTime = 0.f;
		AnimTransitionTime = 0.f;

		USkillSlotContainer* SlotContainer = SkillEntityComponent->GetSkillSlotContainer();
		if (SlotContainer)
		{
			USkillSlot* Slot = SlotContainer->GetSlotThatContainApplier(ESkillApplierTypeEnum::VE_p_airborne);
			if (Slot)
			{
				float Duration = Slot->GetDuration();
				UBattleBaseAnimInstance* BattleBaseAnim = GetBaseAnimInstance();
				UAnimMontage* Montage = BattleBaseAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
				if (Montage)
				{
					int32 SectionIndex = Montage->GetSectionIndex(ANIM_MONTAGE_ABNORMAL_AIREND);
					float Length = Montage->GetSectionLength(SectionIndex);
					AnimTransitionTime = Duration - Length;
				}
			}

			USkillSlot* Slot2 = SlotContainer->GetSlotThatContainApplier(ESkillApplierTypeEnum::VE_p_down);
			if (Slot2)
			{
				float Duration = Slot2->GetDuration();
				UBattleBaseAnimInstance* BattleBaseAnim = GetBaseAnimInstance();
				UAnimMontage* Montage = BattleBaseAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
				if (Montage)
				{
					int32 SectionIndex = Montage->GetSectionIndex(ANIM_MONTAGE_ABNORMAL_DOWNEND);
					float Length = Montage->GetSectionLength(SectionIndex);
					AnimTransitionTime = Duration - Length;
				}
			}
		}
	}
	
}

void UAbnormalAnimState::Leave()
{
	Super::Leave();

	SetAbnormalType(EAbonormalStateFlagEnum::BM_None);
}

bool UAbnormalAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (AbonormalStateFlag == EAbonormalStateFlagEnum::BM_Airborne || AbonormalStateFlag == EAbonormalStateFlagEnum::BM_Down)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= AnimTransitionTime)
		{
			UBattleBaseAnimInstance* BattleBaseAnim = GetBaseAnimInstance();
			UAnimMontage* Montage = BattleBaseAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
			if (Montage)
			{
				ElapsedTime = 0.f;
				AnimTransitionTime = Montage->GetPlayLength();
				float AnimPlayRate = 1.f;
				if(AbonormalStateFlag == EAbonormalStateFlagEnum::BM_Airborne)
					BattleBaseAnim->PlayAnimMontageToSectionName(Montage, ANIM_MONTAGE_ABNORMAL_AIREND, AnimPlayRate);
				else if(AbonormalStateFlag == EAbonormalStateFlagEnum::BM_Down)
					BattleBaseAnim->PlayAnimMontageToSectionName(Montage, ANIM_MONTAGE_ABNORMAL_DOWNEND, AnimPlayRate);
			}
		}
	}
	return IsValidState();
}

void UAbnormalAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	Super::SetAnimParam(Command, varParam);
	
	if (Command == ECharacterCommandEnum::VE_Abnormal)
	{
		if (varParam.GetType() == EOnlineKeyValuePairDataType::Int32)
		{
			int32 intParam;
			varParam.GetValue(intParam);
			SetAbnormalType((EAbonormalStateFlagEnum)intParam);
		}
	}
}

bool UAbnormalAnimState::IsValidState()
{
	AbonormalStateFlag = GetCurrentAbnormalType();
	
	if (AbonormalStateFlag != EAbonormalStateFlagEnum::BM_None)
	{
		return true;
	}
	else
		UAIFunctionLibrary::Stop(OwnerActor.Get());

	return false;
}

bool UAbnormalAnimState::IsCancelAnim()
{
	EAbonormalStateFlagEnum CurrentStateFlag = GetCurrentAbnormalType();

	if (EAbonormalStateFlagEnum::BM_None == CurrentStateFlag ||
		EAbonormalStateFlagEnum::BM_Fear == CurrentStateFlag ||
		EAbonormalStateFlagEnum::BM_Hold == CurrentStateFlag)
	{
		return true;
	}

	return false;
}

EAbonormalStateFlagEnum UAbnormalAnimState::GetCurrentAbnormalType()
{
	EAbonormalStateFlagEnum CheckFlag = EAbonormalStateFlagEnum::BM_None;

	if (BattleEntityComponent.IsValid())
	{
		if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Stun))
			CheckFlag = EAbonormalStateFlagEnum::BM_Stun;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Down))
			CheckFlag = EAbonormalStateFlagEnum::BM_Down;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Frozen))
			CheckFlag = EAbonormalStateFlagEnum::BM_Frozen;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Hold))
			CheckFlag = EAbonormalStateFlagEnum::BM_Hold;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Fear))
			CheckFlag = EAbonormalStateFlagEnum::BM_Fear;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Airborne))
			CheckFlag = EAbonormalStateFlagEnum::BM_Airborne;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_KnockBack))
			CheckFlag = EAbonormalStateFlagEnum::BM_KnockBack;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Push))
			CheckFlag = EAbonormalStateFlagEnum::BM_Push;
		else if (BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Pull))
			CheckFlag = EAbonormalStateFlagEnum::BM_Pull;
	}

	return CheckFlag;
}


EAnimStateEnum UAbnormalAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_Abnormal;

	switch (Command)
	{
	case ECharacterCommandEnum::VE_Die:
	{
		ReturnAimState = EAnimStateEnum::VE_Die;
	}break;
	case ECharacterCommandEnum::VE_Abnormal:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Idle;
		}
	}break;
	case ECharacterCommandEnum::VE_ActionSkill:
	case ECharacterCommandEnum::VE_CastingSkill:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_SkillAttack;
		}
	}break;
	case ECharacterCommandEnum::VE_StartAttack:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_NormalAttack;
		}
	}break;
	case ECharacterCommandEnum::VE_StartMove:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Run;
		}
	}break;
	case ECharacterCommandEnum::VE_StopMove:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Idle;
		}
	}break;
	default:
		break;
	}

	return ReturnAimState;
}

//////////////////////////////////////////////////////////////////////////


void UAbnormalAnimState::SetAbnormalType(EAbonormalStateFlagEnum InAbonormalStateFlag)
{
	AbonormalStateFlag = InAbonormalStateFlag;

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetAbnormalType(AbonormalStateFlag);
	}

}