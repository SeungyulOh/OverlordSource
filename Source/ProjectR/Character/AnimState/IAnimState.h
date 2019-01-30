// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "OnlineKeyValuePair.h"
#include "SharedConstants/GlobalConstants.h"
#include "IAnimState.generated.h"

/**
 * 
 */

class UBattleBaseAnimInstance;
class UBattleEntityComponent;
class USkillEntityComponent;
class UEntityAIComponent;

UCLASS(BlueprintType)
class PROJECTR_API UIAnimState : public UObject
{
	GENERATED_BODY()
		
public:	
	UIAnimState(){};
	virtual ~UIAnimState() {};
	
	virtual		void Initialize();
	virtual		void Uninitialize();
	virtual		void Enter(ECharacterCommandEnum Command, FVariantData varParam);
	virtual		void Leave();
	virtual		bool Update(float DeltaTime);
	virtual		void SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam) {};
	virtual		bool IsValidState() { return false; };
	virtual		bool IsCancelAnim() { return true; };
	virtual		EAnimStateEnum DoCommand(ECharacterCommandEnum Command, bool OnlyChecked = false) { return EAnimStateEnum::VE_None; };

	//////////////////////////////////////////////////////////////////////////

	void ChangeAniminstance(EAnimStateEnum InAnimState);
	void SetOwnerData(TWeakObjectPtr< AActor > InOwner);
	FORCEINLINE EAnimStateEnum	GetAnimStateType() { return AnimStateType; }
	UBattleBaseAnimInstance* GetBaseAnimInstance();

protected:	

	UPROPERTY()
	TWeakObjectPtr< AActor > OwnerActor;

	UPROPERTY()
	TWeakObjectPtr< UBattleEntityComponent > BattleEntityComponent;
	UPROPERTY()
	TWeakObjectPtr< USkillEntityComponent > SkillEntityComponent;
	UPROPERTY()
	TWeakObjectPtr< UEntityAIComponent > EntityAIComponent;
	
	UPROPERTY()
	EAnimStateEnum	AnimStateType = EAnimStateEnum::VE_None;

	bool ActiveState = false;
};
