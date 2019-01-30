// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IAnimState.h"
#include "AttackAnimState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAttackAnimState : public UIAnimState
{
	GENERATED_BODY()
		
public:
	UAttackAnimState();

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
	FORCEINLINE int32 GetCurrentAttackAnimIdx() { return CurrentAttackAnim; }

public:
	TQueue<int32> AttackQueue;

private:
	UPROPERTY()
	ECharacterCategoryTypeEnum CharacterCategoryType = ECharacterCategoryTypeEnum::VE_None;

	UPROPERTY()
	ECharacterCommandEnum NextDoCommand = ECharacterCommandEnum::VE_None;

	bool NetSyncAttackAnim = false;
	bool bRealTimeMode = false;
	

	//////////////////////////////////////////////////////////////////////////
	// RenewalNormalAttack
private:
	void StartNormalAttack();
	void EndNormalAttack();
	void HitNormalAttack();
	
	void NextNormalAttackAnim();

private:
	bool RenewalNormalAttack = false;
	bool MeleeAttack = true;
	bool ReservedStopAttack = false;

	float NormalAttackTime = 0.0f;
	float NormalAttackHitTime = 0.0f;
	float NormalAttackDelayTime = 0.0f;
	int32 NormalAttackAnimCount = 0;

	int32 CurrentAttackAnim = 0;
	bool NextNormalAttack = false;

	float RemainAttackTime = 0.0f;
	float RemainAttackHitTime = 0.0f;

};
