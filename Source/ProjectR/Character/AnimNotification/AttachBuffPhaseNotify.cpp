// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AttachBuffPhaseNotify.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "UtilFunctionIntegrated.h"

void UAttachBuffPhaseNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
}
