// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_State_Idle.h"

#include "RAI_Blackboard.h"
#include "RAI_FSM.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"


#include "CustomStructures/GameEnvironmentVar.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
EAIStateTypeEnum URAI_State_Idle::Update( float DeltaTime )
{
	if (OwnerBlackboard.IsValid() == false)
		return EAIStateTypeEnum::VE_None;

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor);
	if (BaseProperty)
	{
		if (false == BaseProperty->IsReady())
			return EAIStateTypeEnum::VE_None;
	}

	EAIStateTypeEnum ResultAbnormalState = CheckAbnormalState();
	if (ResultAbnormalState != EAIStateTypeEnum::VE_None)
		return ResultAbnormalState;

	EAIStateTypeEnum ResultSkillCheck = CheckSkillState();
	if (ResultSkillCheck != EAIStateTypeEnum::VE_None)
		return ResultSkillCheck;
	
	EAIStateTypeEnum ResultBlackCheck = CheckBlackboard();
	if (ResultBlackCheck != EAIStateTypeEnum::VE_None && ResultBlackCheck != GetStateType() )
		return ResultBlackCheck;
	
	/*
	Check force target, if force target is set, change current target actor to force target.
	*/	
	EAIStateTypeEnum CheckForceTargetResult = CheckForceTarget(nullptr);
	if (CheckForceTargetResult != EAIStateTypeEnum::VE_None)
		return CheckForceTargetResult;


	// search enemy	
	AActor* ResultActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get() );
	if (IsValid(ResultActor))
	{	
		OwnerBlackboard->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, ResultActor );
		
		auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (Owner_BattleEntityComponent)
		{
			if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(ResultActor))
			{
				return EAIStateTypeEnum::VE_Attack;
			}
		}

		return EAIStateTypeEnum::VE_MoveToTarget;				
	}


	/*
	Follow leader
	*/
	EAIStateTypeEnum FollowLeaderResult = FollowLeader();
	if (FollowLeaderResult != EAIStateTypeEnum::VE_None)
		return FollowLeaderResult;

	/*
	Check follow summoner
	*/
	EAIStateTypeEnum FollowSummonerResult = FollowSummoner();
	if (FollowSummonerResult != EAIStateTypeEnum::VE_None)
		return FollowSummonerResult;
	

	return EAIStateTypeEnum::VE_None;
}

void URAI_State_Idle::OnEnter()
{
	InitInternalData();

	UAIFunctionLibrary::Stop(OwnerActor.Get());

	if (OwnerBlackboard.IsValid())
	{
		// when avoid system is applied, jitterinng effect happens.
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
		OwnerBlackboard->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_TargetLocation);
	}

}

void URAI_State_Idle::OnLeave()
{

}