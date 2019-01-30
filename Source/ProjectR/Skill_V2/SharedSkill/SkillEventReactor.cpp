// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectR.h"
#include "SkillEventReactor.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/SkillSlot.h"
#include "Skill_V2/SkillApplierBase.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Skill_V2/SharedSkill/Skill_Passive_V2.h"


USkillEventReactor::USkillEventReactor()
{
	PassiveConditionCheckList.Empty();
}


void USkillEventReactor::Attach(AActor* InOwner)
{
	checkf(InOwner != nullptr, TEXT("USkillEventReactor::Attach[InOwner Is Empty]"));
// 	if (IsValid(InOwner) == false)
// 		return;

	OwnerActor = InOwner;

	SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillEntityComponent.IsValid())
	{
		SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &USkillEventReactor::OnNotifySkillCasting);
		SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &USkillEventReactor::OnNotifySkillCasting);
	}

	BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent.IsValid())
	{
		BattleEntityComponent->OnHitDelegate.RemoveDynamic(this, &USkillEventReactor::OnHitTarget);
		BattleEntityComponent->OnHitDelegate.AddDynamic(this, &USkillEventReactor::OnHitTarget);

		BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &USkillEventReactor::OnReceiveDamage);
		BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &USkillEventReactor::OnReceiveDamage);
	}
}

void USkillEventReactor::Detach()
{
	if (SkillEntityComponent.IsValid())
	{
		SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &USkillEventReactor::OnNotifySkillCasting);
	}

	if (BattleEntityComponent.IsValid())
	{
		BattleEntityComponent->OnHitDelegate.RemoveDynamic(this, &USkillEventReactor::OnHitTarget);
		BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &USkillEventReactor::OnReceiveDamage);
	}
}

void USkillEventReactor::OnHitTarget(AActor* Invoker, EBattleDamageResultEnum DamageResult, AActor* TargetActor, bool bBullet)
{
	/*
	Release hide
	*/
	checkf(Invoker != nullptr, TEXT("USkillEventReactor::OnHitTarget[Invoker Is Empty]"));
// 	if (IsValid(Invoker) == false)
// 	{
// 		return;
// 	}

	USkillEntityComponent* Target_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Invoker);
	if (IsValid(Target_SkillEntityComponent) == false)
	{
		return;
	}
	CloseActiveSlotCheck(Target_SkillEntityComponent, ESkillApplierTypeEnum::VE_p_hide);

	switch (DamageResult)
	{
		case EBattleDamageResultEnum::VE_Dead:
			CheckKillEvent(Invoker, TargetActor);
			CheckDeadEvent(Invoker, TargetActor);
			break;
	}
}

void USkillEventReactor::CloseActiveSlotCheck(USkillEntityComponent* Target_SkillEntityComponent, ESkillApplierTypeEnum InSkillApplier)
{
	checkf(Target_SkillEntityComponent != nullptr, TEXT("USkillEventReactor::CloseActiveSlotCheck[Target_SkillEntityComponent Is Empty]"));
	if (IsValid(Target_SkillEntityComponent) == false)
	{
		return;
	}

	USkillSlotContainer* ActiveSkillContainer = Target_SkillEntityComponent->GetSkillSlotContainer();
	USkillPassiveSlotContainer* PassiveSkillContainer = Target_SkillEntityComponent->GetPassiveSkillSlotContainer();
	if (ActiveSkillContainer && PassiveSkillContainer)
	{
		USkillSlot* ActiveSlot = ActiveSkillContainer->GetSlotThatContainApplier(InSkillApplier);
		if (ActiveSlot)
		{
			if (ActiveSlot->IsInBlockCancelTime() == false)
				ActiveSlot->CloseSlot();
		}
	}
}

void USkillEventReactor::DamageResultCheckEvent(EBattleDamageResultEnum InDamageResult, AActor* FromActor, AActor* TargetActor)
{
	switch (InDamageResult)
	{

	case EBattleDamageResultEnum::VE_Critical:
// 	case EBattleDamageResultEnum::VE_MagicCritical:
		CheckAttackPileUp(FromActor, TargetActor);
		CheckCriticalEvent(FromActor, TargetActor);
		break;

// 	case EBattleDamageResultEnum::VE_Parry:
// 		CheckParryEvent(FromActor, TargetActor);
// 		break;

// 	case EBattleDamageResultEnum::VE_Dodge:
// 		CheckDodgeEvent(FromActor, TargetActor);
// 		break;

	case EBattleDamageResultEnum::VE_Pierce:
		CheckAttackPileUp(FromActor, TargetActor);
		CheckPierceEvent(FromActor, TargetActor);
		break;

	case EBattleDamageResultEnum::VE_Hit:
		CheckAttackPileUp(FromActor, TargetActor);
		CheckHitEvent(FromActor, TargetActor);
		break;

	default:
		break;
	}
}
void USkillEventReactor::CheckDamageCalcState(AActor* Invoker, AActor* TargetActor)
{
	checkf(BattleEntityComponent.IsValid(), TEXT("USkillEventReactor::CheckDamageCalcState[BattleEntityComponent Is Empty]"));
// 	if (BattleEntityComponent.IsValid() == false)
// 		return;
	
	auto DamageCalcStateInfos = BattleEntityComponent->GetBattleInfoBlackboard()->GetDamageCalcStateInfos();
	
	for (auto DamageResultElem : DamageCalcStateInfos)
	{
		DamageResultCheckEvent(DamageResultElem, Invoker, TargetActor);
	}
}
  
void USkillEventReactor::OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	if (Damage > 0)
	{
		/*
		Release hide
		*/
		CloseActiveSlotCheck(SkillEntityComponent.Get(), ESkillApplierTypeEnum::VE_p_hide);

		if (IsValid(FromActor))
		{
			DamageResultCheckEvent(DamageResultType, FromActor, Invoker);
		}
	}
	else
	{
		// for dodge 
		switch (DamageResultType)
		{
			case EBattleDamageResultEnum::VE_Dodge:
				CheckDodgeEvent(FromActor, Invoker);
				break;
		}
	}
}

void USkillEventReactor::OnNotifySkillCasting(AActor* InCaster, UISkill_V2* UseSkill)
{
	CloseActiveSlotCheck(SkillEntityComponent.Get(), ESkillApplierTypeEnum::VE_p_hide);
}

void USkillEventReactor::AddPassiveSkillConditionCheckList(ESkillApplierTypeEnum ConditionType)
{
	if (false == PassiveConditionCheckList.Contains(ConditionType))
	{
		PassiveConditionCheckList.Emplace(ConditionType);
	}
}

void USkillEventReactor::CheckPassiveSkillCondition(USkillPassiveSlotContainer* PassiveSkillContainer)
{
	checkf(PassiveSkillContainer != nullptr, TEXT("USkillEventReactor::CheckPassiveSkillCondition[PassiveSkillContainer Is Empty]"));
// 	if (false == IsValid(PassiveSkillContainer))
// 		return;

	for (auto ConditionType : PassiveConditionCheckList)
	{
		TArray< USkillSlot* > OutSlots;
		if (PassiveSkillContainer->GetSlotThatContainAppliers(ConditionType, OutSlots, true))
		{
			for (USkillSlot* SkillSlot : OutSlots)
			{
				auto SkillInstance = SkillSlot->GetSkillInst();

				FVector SkillTargetLocation = FVector::ZeroVector;
				AActor* TargetActor = nullptr;
				USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
				if (SkillComp->CheckSkillTarget(SkillInstance, OwnerActor.Get(), TargetActor, SkillTargetLocation))
				{
					SkillInstance->SetTarget(TargetActor, SkillTargetLocation);
					auto PassiveSkillInstance = Cast<USkill_Passive_V2>(SkillInstance);
					if (IsValid(PassiveSkillInstance))
					{
						PassiveSkillInstance->FireCoditionCheckSkill();
					}
				}
			}
		}
	}
	PassiveConditionCheckList.Empty();
}

//this method is called , when I kill someone.
void USkillEventReactor::CheckKillEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_kill_hide);
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_kill);
}

void USkillEventReactor::CheckDeadEvent(AActor* FromActor, AActor* TargetActor)
{
	checkf(TargetActor != nullptr, TEXT("USkillEventReactor::CheckDeadEvent[TargetActor Is Empty]"));
// 	if (IsValid(TargetActor) == false)
// 	{
// 		return;
// 	}

	USkillEntityComponent* Target_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetActor);
	if (IsValid(Target_SkillEntityComponent))
	{
		ApplyDeadCharacterWithPassive(Target_SkillEntityComponent, ESkillApplierTypeEnum::VE_t_guts, TargetActor);
		ApplyDeadCharacterWithPassive(Target_SkillEntityComponent, ESkillApplierTypeEnum::VE_t_die, TargetActor);
	}
}

void USkillEventReactor::ApplyDeadCharacterWithPassive(USkillEntityComponent* Target_SkillEntityComponent, ESkillApplierTypeEnum SkillApplierType, AActor* TargetActor)
{
	checkf(Target_SkillEntityComponent != nullptr, TEXT("USkillEventReactor::ApplyDeadCharacterWithPassive[Target_SkillEntityComponent Is Empty]"));
// 	if (IsValid(Target_SkillEntityComponent) == false)
// 	{
// 		return;
// 	}

	USkillPassiveSlotContainer* Target_PassiveSkillContainer = Target_SkillEntityComponent->GetPassiveSkillSlotContainer();
	if (Target_PassiveSkillContainer)
	{
		bool bCheckRate = true;
		// already checked UBattleOperator::CheckPassiveDamageOnDead() in UBattleEntityComponent::ApplyFinalDamage()
		if(ESkillApplierTypeEnum::VE_t_guts == SkillApplierType)
			bCheckRate = false;

		if (Target_PassiveSkillContainer->ContainOneTimeValidApplier(SkillApplierType, bCheckRate))
		{
			TArray< USkillSlot* > OutSlots;
			bool bResult = Target_PassiveSkillContainer->GetSlotThatContainAppliers(SkillApplierType, OutSlots, bCheckRate);
			if (bResult)
			{
				for (USkillSlot* SkillSlot : OutSlots)
				{
					auto SkillInstance = SkillSlot->GetSkillInst();
					SkillInstance->SetTarget(TargetActor, TargetActor->GetActorLocation());

					auto PassiveSkillInstance = Cast<USkill_Passive_V2>(SkillInstance);
					if (IsValid(PassiveSkillInstance))
					{
						if (ESkillApplierTypeEnum::VE_t_guts == SkillApplierType)
						{
							Target_SkillEntityComponent->EndSkillAction_V2();
						}
						PassiveSkillInstance->FireCoditionCheckSkill();
					}
				}
			}
		}

		Target_PassiveSkillContainer->ClearOneTimeValidApplier(SkillApplierType);
	}
}

void USkillEventReactor::CheckCriticalEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_critical);

	if (BattleEntityComponent.IsValid() && (BattleEntityComponent->GetBattleInfoBlackboard()->GetBlackBoardSkillAttack() == false))
	{
		ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_p_split_shot);
	}

	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_accuracy);

	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_magic_critical);
}

void USkillEventReactor::CheckParryEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult( TargetActor, FromActor, ESkillApplierTypeEnum::VE_t_parry);
}

void USkillEventReactor::CheckDodgeEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult(TargetActor, FromActor, ESkillApplierTypeEnum::VE_t_avoid);
}

void USkillEventReactor::CheckPierceEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_accuracy);
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_pierce);
}

void USkillEventReactor::CheckHitEvent(AActor* FromActor, AActor* TargetActor)
{
	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_p_split_shot);

	if (BattleEntityComponent.IsValid() )
	{
		if(!BattleEntityComponent->GetBattleInfoBlackboard()->GetBlackBoardSkillAttack())
			ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_p_split_shot);
	}

	ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_accuracy);
}

void USkillEventReactor::CheckAttackPileUp(AActor* FromActor, AActor* TargetActor)
{
	UBattleEntityComponent* FromBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);
	if (IsValid(FromBattleComp) && IsValid(FromBattleComp->GetBattleInfoBlackboard()))
	{
		if (FromBattleComp->GetBattleInfoBlackboard()->GetBlackBoardSkillAttack())
			return;

		if (FromBattleComp->GetBattleInfoBlackboard()->IsAttachedPileupSlot())
		{
			if (FromBattleComp->GetBattleInfoBlackboard()->IncreaseAttackCount(FromActor))
			{
				ApplyTargetWithPassiveByDamageResult(FromActor, TargetActor, ESkillApplierTypeEnum::VE_t_attack_pile_up, true);
				FromBattleComp->GetBattleInfoBlackboard()->ResetAttackCountStack(FromActor);
			}
		}
	}
}

void USkillEventReactor::ApplyTargetWithPassiveByDamageResult(AActor* FromActor, AActor* TargetActor, ESkillApplierTypeEnum SkillApplierType, bool bMustApply /*= false*/)
{
	checkf(FromActor != nullptr, TEXT("USkillEventReactor::ApplyTargetWithPassiveByDamageResult[FromActor Is Empty]"));
// 	if (IsValid(FromActor) == false)
// 	{
// 		return;
// 	}

	USkillEntityComponent* From_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(FromActor);
	if (IsValid(From_SkillEntityComponent))
	{
		FirePassiveSkill(From_SkillEntityComponent, TargetActor, SkillApplierType, bMustApply);
	}
}

void USkillEventReactor::FirePassiveSkill(USkillEntityComponent* From_SkillEntityComponent, AActor* TargetActor, ESkillApplierTypeEnum InSkillApplierType, bool bMustApply /*= false*/)
{
	checkf(From_SkillEntityComponent != nullptr, TEXT("USkillEventReactor::FirePassiveSkill[From_SkillEntityComponent Is Empty]"));
// 	if (IsValid(From_SkillEntityComponent) == false)
// 	{
// 		return;
// 	}

	USkillPassiveSlotContainer* PassiveSkillContainer = From_SkillEntityComponent->GetPassiveSkillSlotContainer();
	if (PassiveSkillContainer)
	{
		TArray< USkillSlot* > OutSlots;
		bool bResult = PassiveSkillContainer->GetSlotThatContainAppliers(InSkillApplierType, OutSlots, !bMustApply);
		if (bResult)
		{
			for (USkillSlot* SkillSlot : OutSlots)
			{
				auto SkillInstance = SkillSlot->GetSkillInst();
				SkillInstance->SetTarget(TargetActor, TargetActor->GetActorLocation());

				auto PassiveSkillInstance = Cast<USkill_Passive_V2>(SkillInstance);
				if (IsValid(PassiveSkillInstance))
				{
					bool bCasterSlot = bMustApply;
					bool bNoneTarget = !bMustApply;
					PassiveSkillInstance->FireCoditionCheckSkill(bCasterSlot, bNoneTarget);
				}
			}
		}
	}
}