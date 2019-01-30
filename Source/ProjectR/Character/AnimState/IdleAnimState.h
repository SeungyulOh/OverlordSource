// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IAnimState.h"
#include "IdleAnimState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UIdleAnimState : public UIAnimState
{
	GENERATED_BODY()
		
public:
	UIdleAnimState();

	void Initialize() override;
	void Enter(ECharacterCommandEnum Command, FVariantData varParam) override;
	void Leave() override;
	bool Update(float DeltaTime) override;
	void SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam) override;
	bool IsValidState() override;
	EAnimStateEnum DoCommand(ECharacterCommandEnum Command, bool OnlyChecked = false) override;

	//////////////////////////////////////////////////////////////////////////

	void SetIdleType(EIdleTypeEnum InIdleType);

private:
	UPROPERTY()
	EIdleTypeEnum IdleType = EIdleTypeEnum::VE_NormalIdle;

	// SpecialIdle
	UPROPERTY()
	bool bCalcSpecialIdle = false;
	UPROPERTY()
	float SpecialIdleElapsedTime = 0.0f;
	UPROPERTY()
	float SpecialIdleCheckTime = 3.0f;
	UPROPERTY()
	bool bRealTimeMode = false;

};
