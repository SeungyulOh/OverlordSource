// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"

#include "Skill_Passive_V2.generated.h"

struct FRuneWordSkillTable;
struct FTowerSkillTable;

/**
 * 
 */
UCLASS()
class PROJECTR_API USkill_Passive_V2 : public UISkill_V2
{
	GENERATED_BODY()
	
public:
	USkill_Passive_V2() {}

	bool	IsPassiveSkill() override { return true; } 

	bool	AttachPassiveSkill();	// animation skill event, bullet or nonbullet
	bool	FireCoditionCheckSkill(bool bCasterSlot = false, bool bNoneTarget = true);

private:
	bool	bApplying= false;
};

