// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IAnimState.h"
#include "AbnormalAnimState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAbnormalAnimState : public UIAnimState
{
	GENERATED_BODY()
		
public:
	UAbnormalAnimState();

	void Initialize() override;
	void Enter(ECharacterCommandEnum Command, FVariantData varParam) override;
	void Leave() override;
	bool Update(float DeltaTime) override;
	void SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam) override;
	bool IsValidState() override;
	bool IsCancelAnim() override;
	EAnimStateEnum DoCommand(ECharacterCommandEnum Command, bool OnlyChecked = false) override;

	//////////////////////////////////////////////////////////////////////////

	EAbonormalStateFlagEnum GetCurrentAbnormalType();
	void SetAbnormalType(EAbonormalStateFlagEnum InAbonormalStateFlag);

private:

	EAbonormalStateFlagEnum	AbonormalStateFlag = EAbonormalStateFlagEnum::BM_None;

	float ElapsedTime = 0.f;
	float AnimTransitionTime = 0.f;
};
