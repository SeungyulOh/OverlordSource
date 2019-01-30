// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/SkillResourceTableInfo.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/SharedBattle/BattleInfoBlackboard.h"
#include "Skill_V2/SkillSyncApplyData.h"		
#include "Skill_V2/ContinuousSkillApplierActor.h"

#include "ISkill_V2.generated.h"


class ABulletBase;
struct FSkillTableInfo_V2;

enum class ESkillType : uint8
{
	eSkillType_Normal,
};

/**
*
*/
UCLASS()
class PROJECTR_API UISkill_V2 : public UObject
{
	GENERATED_BODY()

public:
	UISkill_V2() {};

	virtual void					BeginDestroy() override;
	virtual void					ClearSkillEffectForced();

	/*	Effect	*/
	bool							AttachCastingEffect(AActor* InTargetActor);
	bool							AttachActionEffect(AActor* InTargetActor, ESkillStateEnum InActionEffectTiming);
	bool							AttachHitEffect(AActor* InTargetActor);
	bool							AttachHitEffectLocation(AActor* InTargetActor, FVector InHitLocation);
	bool							AttachBulletHitEffect(AActor* InTargetActor);
	bool							AttachScreenEffect(AActor* InTargetActor, ESkillStateEnum InScreenEffectTiming);

	virtual void					ShowUltimateSkillEffect();

	UParticleSystemComponent*		GetHitEffect();

	virtual void					InitSkill(FName InSkillID, FName InSkillTableID, AActor* InCaster, int32 InLevel, FSkillTableInfo_V2* SkillInfo);

	virtual void					Update(float DeltaTime);

	virtual void					SetTarget(AActor* InTarget, const FVector& InTargetLocation);
	virtual bool					Casting(AActor* InTarget, const FVector& InTargetLocation);
	virtual void					DoAction();
	virtual bool					Fire();		// animation skill event, bullet or nonbullet
	virtual bool					DummyFire();// animation skill event, bullet or nonbullet
	virtual bool					Apply();	// apply skill buff, if this is bullet skill, this event is called on hit the target.	
	virtual void					ApplySkillBullet();
	virtual void					End();
	
	// 캐스팅중에 해야하는 동작
	void							CastingAction();

	bool IsIncludeApplierTypeEnum(ESkillApplierTypeEnum InType);

	void DeleteContinuousActor();

	//int32							GetSkillUseCost();
	virtual bool					CanCastSkill(const bool bCheckCoolling=true);
	virtual bool					RTS_CanCastSkill();
	virtual void					GetApplyTarget();



	virtual void					StartCoolTime();
	virtual void					CancelCasting();

	void							StartCrewSkillCoolTime();

	void							UpdateCooling(float DeltaTime);
	void							UpdateCrewSkillCooling(float DeltaTime);

	bool							IsValidCaster();
	ESkillAttributeEnum				GetSkillAttributeType();

	void							GetAppliedTargetSkillComponentList(TArray<USkillEntityComponent*> &TargetSkillComponentList);

	virtual bool					IsChannelingSkill() { return false; }

	virtual bool					IsPassiveSkill() { return false; }


	FORCEINLINE FName						GetSkillID() { return SkillID; }
	FORCEINLINE int32						GetSkillLevel() { return SkillLevel; }
	FORCEINLINE FName						GetSkillTableID() { return SkillTableID; }
	FORCEINLINE FSkillTableInfo_V2*			GetSkillInfo() { return SkillInfo_V2; }
	FORCEINLINE TArray< TWeakObjectPtr<AActor> >& GetApplyTargetArray() { return ApplyTargetActors; }

	FORCEINLINE FSkillResourceTableInfo*	GetSkillResourceInfo() { return SkillResourceInfo; }
	FORCEINLINE void						SetSkillCategoryType(ESkillCategoryTypeEnum Category) { SkillCategoryType = Category; }
	FORCEINLINE ESkillCategoryTypeEnum		GetSkillCategoryType() { return SkillCategoryType; }

	FORCEINLINE AActor*						GetCaster() { return Caster.Get(); }
	FORCEINLINE AActor*						GetTarget() { return Target.Get(); }
	FORCEINLINE void						SetTarget(AActor* TempTarget) { Target = TempTarget; }

	FORCEINLINE FVector						GetTargetLocation() { return TargetLocation; }
	FORCEINLINE void						SetTargetLocation(FVector Location) { TargetLocation = Location; }

	FORCEINLINE ESkillStateEnum				GetSkillState() { return SkillState; }
	FORCEINLINE void						SetSkillState(ESkillStateEnum InSkillState) { SkillState = InSkillState; }

	FORCEINLINE bool						IsSkillCooling() { return (0.0f < CurrentCoolTime); }
	FORCEINLINE float						GetCurrentCoolTime() { return CurrentCoolTime; }
	
	FORCEINLINE bool						IsCrewSkillCooling() { return (0.0f < CurrentCrewSkillCoolTime); }
	FORCEINLINE float						GetCurrentCrewSkillCoolTime() { return CurrentCrewSkillCoolTime; }

	void									DecreaseCoolTime(float InDecreaseCoolTime);
	void									IncreaseCoolTime(float InIncreaseCoolTime);

	void							SetSkillSyncData(const FSkillSyncApplyData& CasterData, const TArray<FSkillSyncApplyData>& TargetDataList);
	FSkillSyncApplyData*			GetTargetSkillSyncData(AActor* Target);
	void							ForceApplySkillSyncData();
	void							ReserveEnd() { ReservedSkillEnd = true; } // for Skill_Channel End By Network

	/*
	Summon..
	*/
	bool							SummonMinion(FSkillApplierBase& InSkillApplier);

	void							GetSummonLocation(FVector InTargetLocation, TArray< FVector >& OutResult, int32 InRequestCount );

	AActor*							SummonCharacter(FName InSummonNPCID, FVector& SpawnLocation);
	void							ApplySummonedMinionProperty(AActor* InMinion, float InPropertyPercent);

//	void							CheckNextComboSkill();

protected:
	void							CacheCasterComponent();


public:
	FSkillSyncData								SkillSyncData;

	UPROPERTY()
	FTimerHandle								ComboDelayTimeHandler;

protected:
	FName										SkillID;
	int32										SkillLevel;

	FName										SkillTableID;
	ESkillCategoryTypeEnum						SkillCategoryType = ESkillCategoryTypeEnum::VE_None;

	TWeakObjectPtr<AActor>						Caster;
	TWeakObjectPtr<AActor>						Target;

	FVector										TargetLocation = FVector::ZeroVector;

	FSkillTableInfo_V2*							SkillInfo_V2 = nullptr;
	FSkillResourceTableInfo*					SkillResourceInfo = nullptr;

	TWeakObjectPtr< AContinuousSkillApplierActor >	ContinuousApplierActor = nullptr;

	TWeakObjectPtr< UParticleSystemComponent >	CastingEffect;
	TWeakObjectPtr< UParticleSystemComponent >	CastingBeamEffect;
	TWeakObjectPtr< UParticleSystemComponent >	ActionEffect;
	TWeakObjectPtr< UParticleSystemComponent >	BulletHitEffect;
	TWeakObjectPtr< UParticleSystemComponent >	HitEffect;
		
	TWeakObjectPtr< UEntityAIComponent >		CasterAIEntityComponent;

	bool										ReservedSkillEnd = false;

	TWeakObjectPtr< UEntityRenderComponent >	CasterEntityRenderComponent = nullptr;
	
	TArray< TWeakObjectPtr<AActor> >			ApplyTargetActors;

	//Cache Caster
	TWeakObjectPtr<UBattleEntityComponent>		CasterBattleEntityComponent = nullptr;
	TWeakObjectPtr<USkillEntityComponent>		CasterSkillEntityComponent = nullptr;
	
	TWeakObjectPtr<UBattleInfoBlackboard>		CasterBattleInfoBlackboard = nullptr;


	float										ElapsedCastingTime = 0.0f;
	ESkillStateEnum								SkillState = ESkillStateEnum::VE_None;
	float										CurrentCoolTime = 0.0f;
	float										CurrentCrewSkillCoolTime = 0.0f;

	bool										bRealTimeMode = false;

public:
	ESkillType									eSkillType = ESkillType::eSkillType_Normal;
};
