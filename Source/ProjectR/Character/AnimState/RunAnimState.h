// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IAnimState.h"
#include "RunAnimState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URunAnimState : public UIAnimState
{
	GENERATED_BODY()
		
public:
	URunAnimState();

	void Initialize() override;
	void Enter(ECharacterCommandEnum Command, FVariantData varParam) override;
	void Leave() override;
	bool Update(float DeltaTime) override;
	void SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam) override;
	bool IsValidState() override;
	EAnimStateEnum DoCommand(ECharacterCommandEnum Command, bool OnlyChecked = false) override;

private:	
	class UCharacterBattleInfo * BattleInfo = nullptr;
	bool bRealTimeMode = false;	
};
