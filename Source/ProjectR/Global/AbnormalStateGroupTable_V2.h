// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "AbnormalStateGroupTable_V2.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API UAbnormalStateGroupTable_V2 : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	void BeginDestroy() override;

	// Is buff or debuff?
	bool		IsInBuff(EAbnormalSlotTypeEnum InSlotType);

	// Is in specific group?
	bool		IsInGroup(EAbnormalGroupTypeEnum InGroupType, EAbnormalSlotTypeEnum InSlotType);

private:

	// Is buff or debuff?
	TSet< EAbnormalSlotTypeEnum >				BuffSlotTypes;

	// groups
	TArray< TSet< EAbnormalSlotTypeEnum > >		SlotGroupTypes;
};
