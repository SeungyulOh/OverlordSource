// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_SkillCasting.h"

#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_SkillCasting::Update( float DeltaTime )
{
	EAIStateTypeEnum AbnormalState = CheckAbnormalState();
	if (AbnormalState == EAIStateTypeEnum::VE_AbnormalState)
		return EAIStateTypeEnum::VE_AbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_SkillCasting)
	{
		if (ResultSkillCheck == EAIStateTypeEnum::VE_None)
			return EAIStateTypeEnum::VE_Idle;
		else
			return ResultSkillCheck;
	}

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_SkillCasting::OnEnter()
{
	UAIFunctionLibrary::Stop(OwnerActor.Get());
}

void URAI_State_SkillCasting::OnLeave()
{

}