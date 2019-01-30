// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "SkillEventReactor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API USkillEventReactor : public UObject
{
	GENERATED_BODY()
	
public:
	USkillEventReactor();

	void			Attach(AActor* InOwner);
	void			Detach();
	


	UFUNCTION()
	void			OnHitTarget( AActor* Invoker, EBattleDamageResultEnum DamageResult, AActor* TargetActor, bool bBullet );

	UFUNCTION()
	void			OnReceiveDamage( AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor);

	UFUNCTION()
	void			OnNotifySkillCasting(AActor* InCaster, UISkill_V2* UseSkill);

	void			AddPassiveSkillConditionCheckList(ESkillApplierTypeEnum ConditionType);
	void			CheckPassiveSkillCondition(USkillPassiveSlotContainer* PassiveSkillContainer);
	bool			IsNeedToCheckPassiveSkillCondition() { return (0 < PassiveConditionCheckList.Num()); }

private:

	void			CheckKillEvent( AActor* FromActor, AActor* TargetActor);
	void			CheckDeadEvent(AActor* FromActor, AActor* TargetActor);
	void			ApplyDeadCharacterWithPassive(USkillEntityComponent* Target_SkillEntityComponent, ESkillApplierTypeEnum SkillApplierType, AActor* TargetActor);

	void			CheckCriticalEvent(AActor* FromActor, AActor* TargetActor);
	void			CheckParryEvent(AActor* FromActor, AActor* TargetActor);
	void			CheckDodgeEvent(AActor* FromActor, AActor* TargetActor);
	void			CheckPierceEvent(AActor* FromActor, AActor* TargetActor);
	void			CheckHitEvent(AActor* FromActor, AActor* TargetActor);
	void			CheckAttackPileUp(AActor* FromActor, AActor* TargetActor);
	void			CheckDamageCalcState(AActor* Invoker, AActor* TargetActor);
	void			ApplyTargetWithPassiveByDamageResult(AActor* FromActor, AActor* TargetActor, ESkillApplierTypeEnum SkillApplierType, bool bMustApply = false);
	void			FirePassiveSkill(USkillEntityComponent* Target_SkillEntityComponent, AActor* TargetActor, ESkillApplierTypeEnum InSkillApplierType, bool bMustApply = false);
	void			DamageResultCheckEvent(EBattleDamageResultEnum InDamageResult, AActor* FromActor, AActor* TargetActor);
	void			CloseActiveSlotCheck(USkillEntityComponent* Target_SkillEntityComponent, ESkillApplierTypeEnum InSkillApplier);
protected:

	TWeakObjectPtr<AActor>		OwnerActor;

	/*
	Cache
	*/
	TWeakObjectPtr< USkillEntityComponent >			SkillEntityComponent;
	TWeakObjectPtr< UBattleEntityComponent >		BattleEntityComponent;

	TSet<ESkillApplierTypeEnum>						PassiveConditionCheckList;
};