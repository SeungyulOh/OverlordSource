// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Skill_V2/SkillApplierBase.h"
#include "SkillSlot.h"

#include "SkillSlotContainer.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API USkillSlotContainer : public UObject
{
	GENERATED_BODY()
	
public:
	USkillSlotContainer();

	void			Init(AActor* InOwner);
	void			PrepareSlots();
	void			Update( float DeltaTime );	

	void			AddSkillSlot(UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster);

	void			SkillTable(FSkillTableInfo_V2* SkillTableInfo, UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster);

	bool			CanAddSkillSlot(FSkillTableInfo_V2* SkillInfo, bool bCasterSlot);
	bool			CanOpenSlot(FSkillTableInfo_V2* InSkillTableInfo, bool InIsCaster);
	bool			CanAttachApplier(FSkillTableInfo_V2* SkillTableInfo, bool IsCasterSlot);

	bool			IsInBlockSlot(EAbnormalSlotTypeEnum SlotType);
	bool			IsOnlyInstanceApplier(ESkillApplierTypeEnum* ApplierTypeList);

	USkillSlot*		GetSlotThatContainApplier(ESkillApplierTypeEnum InApplierType);

	TArray< USkillSlot* >&	GetAllSlots();

	bool			GatherPersistentAppilerValue(ESkillApplierTypeEnum InSkillApplierType, FSkillApplierBase& OutSkillApplier);

	bool			IsInSlot(EAbnormalSlotTypeEnum InSlotType);
	bool			IsInGroup(EAbnormalGroupTypeEnum InGroupType);
	bool			IsInBuff(EAbnormalApplyTypeEnum InBuffType);

	void			RemoveAllSlotsByBuffType(EAbnormalApplyTypeEnum InBuffType, int32 InSlotLevel);
	void			RemoveAllSlotsByGroup(EAbnormalGroupTypeEnum InGroupType, int32 InSlotLevel);
	void			RemoveSlot(EAbnormalSlotTypeEnum InSlotType);
	void			RemoveAllSlots();
	void			RemoveSlotbyResourceID(FName InID);

protected:
	void			AddSkillSlotForRealTime(UISkill_V2* AppliedSkill, bool InIsCasterSlot, AActor* InCaster);


protected:
	// index is ESkillSlotTypeEnum
	UPROPERTY()
	TArray< USkillSlot* >				SkillSlotList;

	TWeakObjectPtr< AActor >			OwnerActor = nullptr;

	bool								bRealTimeMode = false;
};