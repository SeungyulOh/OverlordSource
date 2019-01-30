// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_AbnormalState.h"
#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_AbnormalState::Update( float DeltaTime )
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (IsValid(BattleEntityComponent) == false)
		return EAIStateTypeEnum::VE_None;

	if(!BattleEntityComponent->IsInUnActableState() && !BattleEntityComponent->IsInUnMovableState())
		return EAIStateTypeEnum::VE_Idle;

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_AbnormalState::OnEnter()
{
	UAIFunctionLibrary::Stop(OwnerActor.Get());

	//when target is trying to use skill, if the target get knockback attack, skill should be stopped. this means getting out of skillacting AI State.
	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillComp)
	{
		if (SkillComp->IsSkillActing())
			SkillComp->EndSkillActionForced_V2();
		if (SkillComp->IsSkillCasting())
			SkillComp->EndSkillActionForced_V2();
	}
}

void URAI_State_AbnormalState::OnLeave()
{

}