// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "BattleObjectAnimInstance.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"



void UBattleObjectAnimInstance::NativeInitializeAnimation()
{
	this->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

	AddRotatorValue = FRotator::ZeroRotator;
}



void UBattleObjectAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::BlueprintUpdateAnimation(DeltaTimeX);

}

ABattleBaseObject* UBattleObjectAnimInstance::GetOwnerCharacter()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn))
	{
		ABattleBaseObject* BattleObject = Cast< ABattleBaseObject >(OwnerPawn);
		return BattleObject;
	}

	return nullptr;
}

void UBattleObjectAnimInstance::BeginSkillApply()
{
	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TryGetPawnOwner());
	if (IsValid(SkillComp))
		SkillComp->StartSkillApply();
}

void UBattleObjectAnimInstance::EndSkillApply()
{
	IsFireAni = false;
	IsCastingAni = false;

	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TryGetPawnOwner());
	if (IsValid(SkillComp))
		SkillComp->EndSkillAction_V2();
}

void UBattleObjectAnimInstance::BeginSkillAniState()
{

}

void UBattleObjectAnimInstance::EndSkillAniState()
{
	IsFireAni = false;
	IsCastingAni = false;

	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TryGetPawnOwner());
	if (IsValid(SkillComp))
		SkillComp->EndSkillAction_V2();
}
