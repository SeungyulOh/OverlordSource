// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_SkillActing.h"

#include "UtilFunctionIntegrated.h"

EAIStateTypeEnum URAI_State_SkillActing::Update( float DeltaTime )
{
	EAIStateTypeEnum AbnormalState = CheckAbnormalState();
	if (AbnormalState == EAIStateTypeEnum::VE_AbnormalState)
		return EAIStateTypeEnum::VE_AbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_SkillActing)
	{
		if (ResultSkillCheck == EAIStateTypeEnum::VE_None)
			return EAIStateTypeEnum::VE_Idle;
		else
			return ResultSkillCheck;
	}

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_SkillActing::OnEnter()
{
	UAIFunctionLibrary::Stop(OwnerActor.Get());
}

void URAI_State_SkillActing::OnLeave()
{

}