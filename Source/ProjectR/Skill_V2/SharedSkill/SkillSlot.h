// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SkillApplierBase.h"
#include "SharedConstants/GlobalConstants.h"
#include "Skill_V2/SkillSyncApplyData.h"

#include "SkillSlot.generated.h"

struct FSkillTableInfo_V2;
struct FRuneWordSkillTable;
struct FTowerSkillTable;
struct FSpiritSkillTableInfo;
class UBattleEntityComponent;
class USkillEntityComponent;
class UISkill_V2;

#define MAX_PERCENT_HP		1000



/**
 * 
 */
UCLASS()
class PROJECTR_API USkillSlot : public UObject
{
	GENERATED_BODY()
	
public:
	USkillSlot();

	FORCEINLINE void					SetSlotType(EAbnormalSlotTypeEnum InSlotType) { SlotType = InSlotType; }
	FORCEINLINE EAbnormalSlotTypeEnum	GetSlotType() { return SlotType;  }
	FORCEINLINE int32					GetSlotLevel() { return SlotLevel; }

	FORCEINLINE bool					IsActivated() { return bActivated; }
	
	bool					IsInBlockCancelTime();

	virtual bool			Update( float DeltaTime );	
		
	virtual void			SetSkillSyncData(FSkillSyncApplyData* SyncData) { ; } // for RemoteSkillSlot Interface
// 	virtual bool			OpenSlot( FSkillTableInfo_V2* InSkillTableInfo, bool InIsCaster, AActor* InOwner, AActor* InCaster, UISkill_V2* AppliedSkill );
	virtual bool			OpenSlot(FName InSkillTableID, bool InIsCaster, AActor* InOwner, AActor* InCaster, UISkill_V2* AppliedSkill);


	void					CloseSlot();

	FSkillApplierBase&		GetPersistentApplier(ESkillApplierTypeEnum InApplierType);
	bool					AccumulateApplier(ESkillApplierTypeEnum InApplierType, FSkillApplierBase& OutSkillApplied);
	
	FSkillApplierBase*		ContainApplier(ESkillApplierTypeEnum InApplierType);

	void					AttachSlotEffect();
	void					DetachSlotEffect();
	void					SetForceDetach() { bForceDetach = true; }

	void					SetAbnormalState(bool bAttach);


	FORCEINLINE FName					GetSkillID() { return SkillID; }
	FORCEINLINE UISkill_V2*				GetSkillInst() { return SkillInst;  }
	FORCEINLINE FName					GetSkillResourceID() { return SkillResourceID; }
	/*
	For debugging
	*/
	float					GetDuration();
	float					GetDurationElapsedTime();

	FORCEINLINE void					ResetElapsedTime() { ElapsedTime = 0.0f; }

	FORCEINLINE TArray< FSkillApplierBase > GetApplierList() { return SkillApplierList; }


protected:

	/*
	Apply applier
	*/	
		
	virtual bool			AttachPersistentApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	virtual bool			AttachContinousApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach );
	virtual bool			AttachConditionalApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	
	void					CheckContinuousApplier(float DeltaTime);
	
	virtual void			AttachPersistentStatUpDownApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	virtual void			AttachPersistentAbnormalApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	virtual void			AttachAdditionalDamageProperty(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	virtual void			AttachSpiritProperty(FSkillApplierBase& InSkillApplier, bool InIsAttach);
	virtual void			AttachPersistentStateApplier(FSkillApplierBase& InSkillApplier, bool InIsAttach);


	virtual void			ApplyContinousApplier(FSkillApplierBase& InSkillApplier, bool Updated = true);
	virtual void			SetApplierInfo();
	virtual void			SetRuneApplierInfo();
	virtual void			SetTowerApplierInfo();
	virtual void			SetSpiritApplierInfo();
	bool					IsPassiveSlot();


	bool								bActivated = false;
	int32								SlotLevel = 0;
	float								Duration = 0.0f;
	float								IntervalTime = 0.0f;
	EAbnormalGroupTypeEnum				GroupType = EAbnormalGroupTypeEnum::VE_None;
	EAbnormalSlotTypeEnum				SlotType = EAbnormalSlotTypeEnum::VE_None;
	
	FName								SkillResourceID = NAME_None;

	float								BlockCancelTime = 0.5f;

	float								ElapsedTime = 0.0f;	
	float								IntervalElapsedTime = 0.0f;

	bool								IsCasterSlot = false;	
	FName								SkillID;
	
	FName								SkillTableID = NAME_None;
	FName								RuneSkillTableID = NAME_None;
	FName								TowerSkillTableID = NAME_None;
	FName								SpiritSkillTableID = NAME_None;

// 	FSkillTableInfo_V2*					SkillTableInfo = nullptr;
// 	FRuneWordSkillTable*				RuneSkillTableInfo = nullptr;
// 	FTowerSkillTable*					TowerSKillTableInfo = nullptr;
// 	FSpiritSkillTableInfo*				SpiritSkillTableInfo = nullptr;

	UPROPERTY()
	UISkill_V2*							SkillInst = nullptr;

	TArray< FSkillApplierBase >			SkillApplierList;

	TWeakObjectPtr< AActor >			OwnerActor = nullptr;
	TWeakObjectPtr<AActor>				CasterActor = nullptr;

	/*
	Cache component
	*/
	UPROPERTY()
	TWeakObjectPtr< UBattleEntityComponent >	CasterBattleEntityComponent;
	UPROPERTY()
	TWeakObjectPtr< USkillEntityComponent >		CasterSkillEntityComponent;
	UPROPERTY()
	TWeakObjectPtr< UBattleEntityComponent >	OwnerBattleEntityComponent;

	/*
	Effects
	*/	
	UPROPERTY()
	TWeakObjectPtr< UParticleSystemComponent >		SlotEffect;

	bool								bForceDetach = false;
};