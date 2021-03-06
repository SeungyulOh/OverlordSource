// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IAnimState.h"
#include "SkillAnimState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API USkillAnimState : public UIAnimState
{
	GENERATED_BODY()
		
public:
	USkillAnimState();

	void Initialize() override;
	void Uninitialize() override;
	void Enter(ECharacterCommandEnum Command, FVariantData varParam) override;
	void Leave() override;
	bool Update(float DeltaTime) override;
	void SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam) override;
	bool IsValidState() override;
	bool IsCancelAnim() override;
	EAnimStateEnum DoCommand(ECharacterCommandEnum Command, bool OnlyChecked = false) override;

	//////////////////////////////////////////////////////////////////////////

	void SetSkillAniType(ESkillAniTypeEnum	InSkillAniType);
	void SetSkillCasting(bool InSkillCasting);
	void SetSkillActing(bool InSkillAction);
	void SetSkillActingEnd(ESkillAniTypeEnum	InSkillAniType);
	
	UFUNCTION()
	void EndSkillAni();

private:
	void EndSkillDelay();


private:
	UPROPERTY()
	ESkillAniTypeEnum SkillAniType = ESkillAniTypeEnum::VE_None;
	UPROPERTY()
	bool bInSkillDelay = false;
	UPROPERTY()
	FTimerHandle SkillDelayHandler;
	UPROPERTY()
	TArray<float> DelayTimeSkill;
};
