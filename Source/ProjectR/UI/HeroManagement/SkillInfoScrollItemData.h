// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "SkillInfoScrollItemData.generated.h"

class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API USkillInfoScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	EHERO_SKILL_SLOT	SkillSlot = EHERO_SKILL_SLOT::ActiveSkill01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	FName			SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	FName			SkillNameTableId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	FString			CurHeroUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	int32			SkillLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USkillInfoScrollItemData)
	bool			bSpiritSkill = false;

	uint8 SkillIndex;
};
