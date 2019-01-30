// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimBuffPhaseState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"



void UAnimBuffPhaseState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimAttackState Begin"));
#endif

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAnimBuffPhaseState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimBuffPhaseState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimAttackState End"));
#endif

	Super::NotifyEnd(MeshComp, Animation);	
}