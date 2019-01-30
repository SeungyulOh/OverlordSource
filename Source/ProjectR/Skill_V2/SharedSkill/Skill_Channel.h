// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ISkill_V2.h"
#include "Skill_Channel.generated.h"


class AContinuousSkillApplierActor;

/**
 * 
 */
UCLASS()
class PROJECTR_API USkill_Channel : public UISkill_V2
{
	GENERATED_BODY()
	
public:
	USkill_Channel() {};

	virtual void					InitSkill(FName InSkillID, FName InSkillTableID, AActor* InCaster, int32 InLevel, FSkillTableInfo_V2* SkillInfo);

	virtual void					Update(float DeltaTime);

	virtual bool					Casting(AActor* InTarget, const FVector& InTargetLocation) override;
	virtual void					DoAction();
	virtual bool					Fire();
	virtual void					End();

	//virtual void					StartCoolTime();

	virtual bool					IsChannelingSkill() { return true; }

	bool							CheckCasterHoldOn();

protected:
	void							UpdateCasterDirection();
	bool							CheckAimTarget();
	void							CreateAreaApplierActor();

private:
	float							ChannelingElapsedTime = 0.0f;
	float							ChannelingDurationTime = 0.0f;
	float							IntervalElapsedTime = 0.0f;
	float							IntervalTime = 0.0f;

	FVector							CasterCastingPos = FVector::ZeroVector;
};
