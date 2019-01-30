// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Object.h"
#include "Skill_V2/SkillApplierBase.h"
#include "CustomStructures/SharedStructures/ProtectInfo.h"
#include "BattleOperator.generated.h"

class USkillEntityComponent;
class UBattleEntityComponent;




UCLASS()
class PROJECTR_API UBattleOperator : public UObject
{
	GENERATED_BODY()
	
public:
	float	CheckProtectState(float Damage, AActor* FromActor);
	float	ApplyAdditionalDamage(float InDamage, AActor* FromActor, EBattleDamageResultEnum& OutDamageResult);

	void	AttachProtect(FProtectInfo& ProtectInfo);
	void	DetachProtect(EProtectType ProtectType, bool InIsTimeOut);
	bool	IsApplyProtect(EProtectType ProtectType);
	float	GetProtectPower(EProtectType ProtectType);
	float	GetProtectPowerPrecent(EProtectType ProtectType);


	void	SetOwner(AActor* Owner);
	bool	CheckPassiveDamageOnDead(AActor* TargetActor);

	

private:
	void	CloseProtectSlot(const ESkillApplierTypeEnum InSkillApplierType);
	
private:
	UPROPERTY()
	TArray<FProtectInfo> ProtectArray;
	
	TWeakObjectPtr< AActor >						OwnerCharacter;

	// cache..
	TWeakObjectPtr< USkillEntityComponent >			OwnerSkillEntityComponent;
	TWeakObjectPtr< UBattleEntityComponent >		OwnerBattleEntityComponent;

};
