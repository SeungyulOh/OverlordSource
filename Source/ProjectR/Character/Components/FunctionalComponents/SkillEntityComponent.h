// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "SkillEntityComponent.generated.h"


class AActor;
class UBuffSkillController;
class UAutoSkillContainer;
class UInputSkillContainer;
class APassiveContinousSkill;
class UBattleEntityComponent;
//class UISkill;
class UPassiveSkillContainer;
class UCharacterSkill;
class UIBuff;

class USkill_Rune;
class USkill_Tower;
class UISkill_V2;
class USkillSlotContainer;
class USkillPassiveSlotContainer;
class URunePassiveSlotContainer;
class UTowerPassiveSlotContainer;
class URAI_SkillAICaster;
class USkillEventReactor;
class USkill_Passive_V2;
class ANormalBullet;

struct FSkillSyncApplyData;
struct FSkillTableInfo_V2;
struct FSkillApplierBase;
struct FSkillSyncSlotApplierData;
struct FRuneWordSkillTable;
struct FTowerSkillTable;
struct FRuneWordTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnFireAutoSkill, FName, CompareSkillID, int32, InCommandSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnFireActiveSkill, FName, InSkillName, UTexture2D*, InSkillPotrait, ESkillCategoryTypeEnum, InSkillCategoryType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnChangeActiveSkillIcon, FName, CompareSkillID, float, InCoolTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnEndSkillCoolTime, FName, CompareSkillID, ESkillCategoryTypeEnum, InCategoryType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnSetAbnormalIcon, EAbnormalSlotTypeEnum, InSlotType, bool, bSetAbnormal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnNotifySkillCasting, AActor*, Caster, UISkill_V2*, UseSkill);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API USkillEntityComponent : public UActorComponent
{
	GENERATED_BODY()
		 
public:
	// Sets default values for this component's properties
	USkillEntityComponent();

	// Called when the game starts
	void InitializeComponent() override;
	void UninitializeComponent() override;

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EEndPlayReason) override;




	virtual void		UpdateSkillComponent(float DeltaSecond);
	void				ClearSkillEntityComponent();
		
	
	
	virtual void		StartSkillApply();
	virtual void		EndSkillApply();

	void				SetNeedTargetSkillID(FName InSkillID);
	
	//void				SetVisibleSkillArea(bool bShow);
	FORCEINLINE bool	IsVisibleSkillArea()							{ return bVisibleSkillArea; }
	FORCEINLINE void	SetIsVisibleSkillArea(bool InVisibleSkillArea)	{ bVisibleSkillArea = InVisibleSkillArea; }

	/*
	flag
	*/
	bool				IsSkillCasting();
	UFUNCTION(BlueprintCallable, Category = Skill)
	bool				IsSkillActing();
	

	void				SetSkillCasting(bool InSkillCasting);
	void				SetSkillActing(bool InSkillActing);
	

	bool				IsAutoMode();
	FORCEINLINE bool	IsAttachSkill() { return bAttachSkill; }
	
	FORCEINLINE void	SetAttachSkill(bool InAttachSkill)	{ bAttachSkill = InAttachSkill; }
	

	FORCEINLINE bool	IsCreateSkillEnable() { return bCreateSkillEnable; }
	FORCEINLINE void	SetCreateSkillEnable(bool InCreateSkillEnable) { bCreateSkillEnable = InCreateSkillEnable; }
	

	bool				IsValidSkillRange_V2(const FVector& SkillTargetLocation, FName TargetSkillID = FName() );
	float				GetSkillRange(FName TargetSkillID = FName());


	//gettter/setter
	FORCEINLINE ESkillAniTypeEnum	GetSkillAniType() { return SkillAniType; }
	void				SetSkillAniType(ESkillAniTypeEnum InSkillAniType);

	FORCEINLINE void	SetNetSkillSending(bool send) { bNetSkillSending = send; }
	

	void				ClearLastAppliedSkillInformations();

	void				SetSkillAutoModeOnOff(TArray<bool>& OnOffList);


	FORCEINLINE void	ActivateSkill(bool bActivate) { bActivated = bActivate; }
	FORCEINLINE bool	IsActivated() { return bActivated;  }
	

	/*
	Utils
	*/
	void				SetBattleEntityComponent(UBattleEntityComponent* BttleEntityComp);

public:
	

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Skill)
	FDelegate_OnFireActiveSkill			OnFireActiveSkillDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Skill)
	FDelegate_OnSetAbnormalIcon			OnSetAbnormalIcon;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Skill)
	FDelegate_OnNotifySkillCasting		OnNotifySkillCasting;

protected:
	UPROPERTY()
	TWeakObjectPtr<UBattleEntityComponent>			OwnerBattleEntityComponent;


	TWeakObjectPtr< AActor >	OwnerActor;	
	
	UPROPERTY()
	ESkillAniTypeEnum			SkillAniType = ESkillAniTypeEnum::VE_None;

	UPROPERTY()
	FName						NeedTargetSkill;			// if skill need target or targetlocation. you should set the this memeber
	
	bool						bCreateSkillEnable = true;	
	bool						bVisibleSkillArea = false;

	bool						bSkillCasting = false;
	bool						bSkillActing = false;
	
	bool						bNetSkillSending = false; // for network

	bool						bAttachSkill = true;
	
	bool						bSkillApply = false;
	
	bool						bActivated = false;	
	

public:
	void						TryToAttachSkills_V2();
	void						AttachSkill_V2(FName SkillID, int32 SkillLevel, ESkillCategoryTypeEnum SkillCategoryType);

	void						SetVisibleSkillArea_V2(bool bShow);
	void						SetVisibleCrewSpawnArea(bool bShow, float InRange);

	bool						CastNeedTargetSkill_V2(FName InSkillID, AActor* ActorTarget, FVector LocationTarget);

	void						SetCurrentSkill_V2(UISkill_V2* InSkill);
	FORCEINLINE UISkill_V2*		GetCurrentSkill_V2() { return CurrentSkill_V2; }
	bool						IsCurrentSkillChanneling();
	void						FireCurrentSkill_V2();
	void						DummyCurrentSkill_V2();
	// 캐스팅중에 해야하는 동작 수행
	void						CastingActionSkill_V2();

	virtual void				EndSkillAction_V2();
	void						EndSkillActionForced_V2();
	void						ForceDeleteContinuousActor();
	void						ForceProcessCurrentSkill();

	void						ApplySkill(UISkill_V2* AppliedSkill, AActor* Caster, bool InbApplyCaster = false);

	UISkill_V2*					GetSkill_V2(FName SkillID);
	UISkill_V2*					GetSkillByCategory_V2(ESkillCategoryTypeEnum InSkillCategory);
	float						GetSkillCoolTime_V2(FName SkillID);
	float						GetCrewSkillCoolTime_V2(FName SkillID);
	void						DecreaseCoolTime(int32 TargetPercent, int32 TargetValue);

	

	
	UFUNCTION()
	void						EnterGlobalSkillCoolTime();

	void						UpdateRealTimeSkillSlot(FSkillSyncApplyData& SyncData);

	// not use cost : -1.0, enough cost : 1.0, else 0.0 ~ 1.0
	//float						GetSkillCostPercentage(FName SkillID);
	//bool						IsEnoughSkillCostToCastSkill(int32 SkillUseCost);
	//int32						GetFinalSkillCost(int32 BaseSkillUseCost);
	//void						CheckAccumulationSkillCost(UISkill_V2* ApplySkill);
	//void						CheckAccumulationSkillCost(ESkillApplierTypeEnum ApplierType);
	//void						AddSkillCostPoint(int32 CostPoint);
	//void						ResetSkillCostPoint() { AccumulatedSkillCostPoint = 0; }
	//float						GetSkillCostPoint() { return AccumulatedSkillCostPoint; }
	//float						GetAppliedSkillCostBonusRate();

	void						ChangeAISkillMode(EAIControlModeEnum InSkillModeType);
	void						PrepareAICastingSkills();

	void						ClearPassiveSkillAndSlot();
	void						ForceClearSkillEffect();

	void						RemoveItemSkill(FName& ItemSkillID);

	/*
	Check state
	*/
	bool						IsInSlot(EAbnormalSlotTypeEnum InSlotType);
	bool						IsInGroup(EAbnormalGroupTypeEnum InGroupType);
	bool						IsInBuff(EAbnormalApplyTypeEnum InBuffType);

	/*
	Dispell slots
	*/
	void						ClearAllSlots();
	void						ClearSlotsByBuffType(EAbnormalApplyTypeEnum InApplyType, int32 InSlotLevel );
	void						ClearSlotsByGroup(EAbnormalGroupTypeEnum InGroupType, int32 InSlotLevel);
	void						ClearSlot(EAbnormalSlotTypeEnum InSlotType);

	void						ReservePassiveSkillCondition(ESkillApplierTypeEnum ConditionType);
	void						CheckPassiveSkillCondition();

	

public:
	FORCEINLINE USkillSlotContainer*		GetSkillSlotContainer()			{ return SkillSlotContainer; }
	FORCEINLINE USkillPassiveSlotContainer*	GetPassiveSkillSlotContainer()	{ return SkillPassiveSlotContainer; }
protected:
	void						UpdateSkillInst(float DeltaTime);

	static void					GetPlayerHeroSkillInfos(TArray<FHERO_SKILL>& Infos, int32 kID, FString HeroUD, AActor* InTargetActor);
	static void					CreateDummySkills(TArray<FHERO_SKILL>& SkillData, AActor* InTargetActor);


	// from manager
private:
	static FVector GetSearchShapeInfobySkill(UISkill_V2* Skill);
	static FVector GetSearchShapeInfo(FSkillTableInfo_V2* SkillInfo);
	static AActor* FindEnemySkillTarget(FSkillTableInfo_V2* SkillInfo, AActor* Caster);
	static AActor* FindFriendSkillTarget(FSkillTableInfo_V2* SkillInfo, AActor* Caster);
	static AActor* FindNearestSkillTarget(AActor* caster, EFriendshipTypeEnum find_type);
	static FVector FindSkillTargetLocation(FSkillTableInfo_V2* SkillInfo, AActor* Caster);

	static bool BlockMoveingCatetoryApplierCheck(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier);

public:
	static UISkill_V2*			CreateSkill(AActor* Caster, FName SkillID, int32 Level);
	static USkill_Passive_V2*	CreatePassiveSkill(AActor* Caster, FName SkillID, int32 Level);
	static USkill_Passive_V2*	CreateSpiritSkill(AActor* Caster, FName SkillID, int32 Level);

	static bool	CastInputSkill(FName SkillNO, AActor* Caster, AActor* ActorTarget, FVector LocationTarget = FVector::ZeroVector);
	static bool CheckSkillTarget(UISkill_V2* CastSkill, AActor* Caster, AActor* &ActorTarget, FVector& LocationTarget);

	static void GetSkillTarget(TArray< TWeakObjectPtr<AActor> >& outArray, UISkill_V2* Skill);
	static void GetSkillApplyTarget(TArray< TWeakObjectPtr<AActor> >& outArray, UISkill_V2* Skill, AActor* ActorTarget, FVector LocationTarget);

	static ANormalBullet* FireTargetBullet(UISkill_V2* Skill, AActor* Caster, AActor* Target, bool IsRealBullet = true);
	static void FireMultiBullet(UISkill_V2* Skill, AActor* Caster, AActor* ActorTarget, FVector LocationTarget, bool IsRealBullet = true);
	static bool FireLocationBullet(UISkill_V2* Skill, AActor* Caster, FVector location, bool IsRealBullet = true);

	static void SetDelayAreaForAreaEffect(float RangeRadius, UParticleSystemComponent* InTargetEffect, bool IsSkipVisible, int32 InSkipIndex = 0);

	// 캐스팅중에 발동되야하는 스킬만 적용시킬 함수(ex. VE_i_dash)
	static bool ApplyCastingActionApplier(AActor* Caster, AActor* Target, ESkillApplierTypeEnum& InSkillApplier , float CastingDuration);
	
	static void ApplyInstanceApplierGroup(AActor* Caster, AActor* Target, FSkillTableInfo_V2* InSkillTableInfo, bool IsCasterSlot, UISkill_V2* AppliedSkill = nullptr, bool bNoneTargetApplier = false);

	static bool ApplyInstanceApplier_Delegator(bool bNoneTargetApplier, AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill = nullptr);

	static bool ApplyInstanceApplier(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill = nullptr);
	static bool ApplyInstanceNoneTargetApplier(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill = nullptr);

	static bool CS_ApplyInstanceApplier(AActor* Caster, AActor* Target, FSkillSyncSlotApplierData& SyncApplierData, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill = nullptr);
	static bool CS_ApplyInstanceNoneTargetApplier(AActor* Caster, AActor* Target, FSkillSyncSlotApplierData& SyncApplierData, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill = nullptr);

/*
Check synergy
*/
	static bool	GetSynergySkillTarget(AActor* InOwnerActor, TArray< TWeakObjectPtr<AActor> >& OutTargets, FName InSkillTableID, FSkillTableInfo_V2* InSkillTableInfo = nullptr);
	static bool IsSkillSynergyTarget(FSkillTableInfo_V2* InSkillTableInfo, AActor* InTargetActor);

protected:
	UPROPERTY()
	TMap<FName, UISkill_V2*>	EntitySkills_V2;

	UPROPERTY()
	TArray<bool>				AISkillAutoModeOn;

	UPROPERTY()
	USkillSlotContainer*		SkillSlotContainer = nullptr;

	UPROPERTY()
	USkillPassiveSlotContainer*		SkillPassiveSlotContainer = nullptr;

	UPROPERTY()
	USkillEventReactor*			SkillEventReactor = nullptr;

	UPROPERTY()
	UISkill_V2*					CurrentSkill_V2 = nullptr;

	//UPROPERTY()
	//int32						AccumulatedSkillCostPoint = 0;

	//bool						PrevSkillCostAccumulationOn = false;
	//bool						SkillCostAccumulationOn = false;

	EAIControlModeEnum			SkillModeType = EAIControlModeEnum::VE_AutoMode;

	/*
	Test ai skill caster
	*/
	UPROPERTY()
	URAI_SkillAICaster*			AISkillCaster = nullptr;
};
