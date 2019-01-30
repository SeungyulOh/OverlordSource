// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Skill_Remote_V2.generated.h"


/**
*
*/
UCLASS()
class PROJECTR_API USkill_Remote_V2 : public UISkill_V2
{
	GENERATED_BODY()

public:
	USkill_Remote_V2() {};
	
	virtual bool					Fire() override;			
};
