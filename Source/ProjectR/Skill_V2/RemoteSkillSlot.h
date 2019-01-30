// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedSkill/SkillSlot.h"

#include "RemoteSkillSlot.generated.h"


struct FSkillTableInfo_V2;
class UBattleEntityComponent;


/**
 * 
 */
UCLASS()
class PROJECTR_API URemoteSkillSlot : public USkillSlot
{
	GENERATED_BODY()
	
public:
	URemoteSkillSlot();

	virtual bool				Update(float DeltaTime) override;

	virtual void				SetSkillSyncData(FSkillSyncApplyData* SyncData) { TargetSkillSyncData = SyncData; }
	virtual bool				OpenSlot(FName InSkillID, bool InIsCaster, AActor* InOwner, AActor* InCaster, UISkill_V2* AppliedSkill) override;

protected:
	virtual bool				AttachContinousApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach) { return false; }
	virtual void				ApplyContinousApplier(FSkillApplierBase& InSkillApplier, bool Updated = true) override { ; }

	virtual void				SetApplierInfo() override;

private:
	bool						IsMyOwnerActor = false;
	FSkillSyncApplyData*		TargetSkillSyncData = nullptr;
	FSkillSyncSlotApplierData*	CurrentSyncApplierData = nullptr;
};