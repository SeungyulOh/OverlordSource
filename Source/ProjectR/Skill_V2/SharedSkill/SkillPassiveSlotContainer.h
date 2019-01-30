// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Skill_V2/SkillApplierBase.h"
#include "SkillSlot.h"

#include "SkillPassiveSlotContainer.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTR_API USkillPassiveSlotContainer : public UObject
{
	GENERATED_BODY()
	
public:
	USkillPassiveSlotContainer();

	void			Init(AActor* InOwner);
	void			PrepareSlots();
	void			Update( float DeltaTime );	
	
	void			AddPassiveSkillSlot(UISkill_V2* AppliedSkill, AActor* InCaster);
	void			AddItemSkillSlot(UISkill_V2* AppliedSkill);

	bool			GetSlotThatContainAppliers(ESkillApplierTypeEnum InApplierType, TArray< USkillSlot* >& OutSkillSlots, bool CheckRate);
	int32			GetSlotArrayIndex();

	bool			GatherPersistentAppilerValue(ESkillApplierTypeEnum InSkillApplierType, FSkillApplierBase& OutSkillApplier);


	TArray< USkillSlot* >&	GetAllSlots();

	void			RemoveSlot(FName& ItemSkillID);
	void			RemoveAllSlots();

	void			AddOneTimeValidApplier(ESkillApplierTypeEnum InApplier, USkillSlot* SkillSlot);
	bool			ContainOneTimeValidApplier(ESkillApplierTypeEnum InApplier, bool CheckRate);
	void			ClearOneTimeValidApplier(ESkillApplierTypeEnum InApplier);

protected:

	// index is ESkillSlotTypeEnum
	UPROPERTY()
	TArray< USkillSlot* >						SkillSlotList;
	UPROPERTY()
	TWeakObjectPtr< AActor >					OwnerActor = nullptr;
	UPROPERTY()
	TMap<ESkillApplierTypeEnum, USkillSlot*>	OneTimeValidApplier;
};