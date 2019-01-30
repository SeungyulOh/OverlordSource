// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "SharedConstants/GlobalConstants.h"
#include "BattleObjectAnimInstance.generated.h"


class ABattleBaseObject;
/**
 * 
 */
UCLASS()
class PROJECTR_API UBattleObjectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleObject)
	FRotator		AddRotatorValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleObject)
	bool			IsFireAni = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleObject)
	bool			IsCastingAni = false;

	void NativeUpdateAnimation(float DeltaTimeX) override;
	void NativeInitializeAnimation() override;

	void BeginSkillApply();
	void EndSkillApply();

	void BeginSkillAniState();
	void EndSkillAniState();

	ABattleBaseObject* GetOwnerCharacter();
};
