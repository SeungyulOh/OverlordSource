// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BattleBaseAnimInstance.h"
#include "AkhanAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UAkhanAnimInstance: public UBattleBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	void PlayAnimMontageNormalAttack() override;
	void PlayAnimMontageAbnormal(EAbonormalStateFlagEnum NewFlag, EAbonormalStateFlagEnum OldFlag) override;
};
