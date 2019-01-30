// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "SharedConstants/GlobalConstants.h"
#include "Table/CharacterTableInfo.h"
#include "BattleBaseAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_AnimInstance_NoParm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_AnimInstance_Attack, ENormalAttackTypeEnum, CheckAttackType, bool, bState);

UENUM(BlueprintType)
enum class ESkillAnimStateType : uint8
{
	VE_SkillCasting,
	VE_SkillAction,
	VE_SkillActionEnd,
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UBattleBaseAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()
	
public:

	virtual void PostLoad() override;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageIdle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageRun;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageAttack;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageAbnormal;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TAssetPtr< UAnimMontage >		AnimMontageDamageResult;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AnimMontageAsset)
	TMap<ESkillAniTypeEnum, TAssetPtr< UAnimMontage >>		AnimMontageSkillMap;

	UAnimMontage*	GetAnimMontageFromAnimMontageType(EAnimMontageType AnimMontageType);
	UAnimMontage*	GetSkillAnimMontage();


	void			CacheAnimMontageFile();
	
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UBattleBaseAnimInstance)
	FDelegate_AnimInstance_Attack			OnAttackStateDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UBattleBaseAnimInstance)
	FDelegate_AnimInstance_Attack			OnnUnconditionalExecuteAttackStateDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UBattleBaseAnimInstance)
	FDelegate_AnimInstance_NoParm			OnEndSkillAniDelegate;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	EAnimStateEnum AnimState = EAnimStateEnum::VE_None;

	EAbonormalStateFlagEnum AbonormalStateFlag = EAbonormalStateFlagEnum::BM_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	ESkillAniTypeEnum		SkillAniType = ESkillAniTypeEnum::VE_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	ENormalAttackTypeEnum	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	EIdleTypeEnum			IdleType = EIdleTypeEnum::VE_NormalIdle;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	float		AnimPlayRate = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	float		AttackAnimPlayRate = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	float		MoveAnimPlayRate = 1.0f;

	UPROPERTY()
	bool		FlyState = false;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	UAnimSequenceBase*	StateMachineAnimSequence = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	UAnimSequenceBase*	DieAnimSequence = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UBattleBaseAnimInstance)
	bool		StateMachineAnimLoop = true;

	UPROPERTY()
	FName		StateMachineAnimSectionName;
	

	//////////////////////////////////////////////////////////////////////////
	bool	SetAnimState(EAnimStateEnum InAnimState);
	void	SetIdleType(EIdleTypeEnum InIdleType) { IdleType = InIdleType; }
	void	SetNormalAttackType(ENormalAttackTypeEnum InNormalAttackType);
	void	SetNormalAttackAnim(int32 InNormalAttackAnim);
	void	SetSkillAniType(ESkillAniTypeEnum InSkillAniType) { SkillAniType = InSkillAniType; }
	void	SetSkillCasting(bool InSkillCasting);
	void	SetSkillActing(bool InSkillActing);
	void	SetSkillActingEnd(ESkillAniTypeEnum InSkillAniType);
	void	SetAbnormalType(EAbonormalStateFlagEnum InAbonormalStateFlag);

	UFUNCTION(BlueprintCallable, Category = UBattleBaseAnimInstance)
	void	SetFlyState(bool InState);
	void	SetAnimPlayRate(float InAnimPlayRate);
	void	SetAttackAnimPlayRate(float InAnimPlayRate);
	void	SetMoveAnimPlayRate(float InAnimPlayRate);
	FORCEINLINE void	SetIsLimitPlayRate(bool InValue) { IsLimitPlayRate = InValue; }
	FORCEINLINE bool	GetIsLimitPlayRate()			{ return IsLimitPlayRate; }
	FORCEINLINE void	SetIsForceDie2(bool InValue)	{ IsForceDie2 = InValue; }

	void	SetStateMachineAnimSequence(UAnimMontage* AnimMontage, FName InSlotName, FName InSectionName, bool InLoop);
	void	SetBasicIdleAnimSequence();
	void	SetDieAnimSequence(UAnimMontage* AnimMontage, FName InSlotName, FName InSectionName);

	//////////////////////////////////////////////////////////////////////////
	// AnimMontage
	
	virtual void PlayAnimMontageIdle();
	virtual void PlayAnimMontageRun();
	virtual void PlayAnimMontageNormalAttack();
	virtual void PlayAnimMontageSkillAttack(ESkillAnimStateType InSkillAnimState, ESkillAniTypeEnum InSkillAniType);
	virtual void PlayAnimMontageAbnormal(EAbonormalStateFlagEnum NewFlag, EAbonormalStateFlagEnum OldFlag);
	virtual void PlayAnimMontageFromDamageResultType(EBattleDamageResultEnum DamageResultType);
	virtual void PlayAnimMontageEvent();

	void	PauseAnimMontageNormalAttack();

	bool	PlayAnimMontageEventSection(const FName& SectionName, FName NextSectionName = FName());
	bool	IsPlayingAnimMontageEventSection(const FName& SectionName);

	//////////////////////////////////////////////////////////////////////////

	bool	IsPlayingSkillActingAnimMontage();
	void	AddStringFromMontageState(FString& InString);
	bool	PlayAnimMontageToSectionString(UAnimMontage* AnimMontage, const FString& SectionString, float& PlayRate, bool ActionMontage = true);
	bool	PlayAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName, float& PlayRate, bool ActionMontage = true);
	bool	StopAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName);
	bool	IsPlayingAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName);
	
	float	GetAnimMontageSectionLength(EAnimMontageType AnimMontageType, const FName& SectionName);
	float	GetAnimMontageSectionLength(UAnimMontage* AnimMontage, const FName& SectionName);

	//////////////////////////////////////////////////////////////////////////
	// Delete List
	//TODO: make Casting Type
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	bool		IsCasting = false;
	bool		bSkillActing = false;


	bool	bEnterCasting = false;
	// Delete List
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// BattleObject & Gigant
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	FRotator		AddRotatorValue;
	
protected:
	AActor*	GetOwnerCharacter();
	void CheckOwnerCharacterInfo(AActor* InOwnerActor);
	
protected:
	FCharacterTableInfo* OwnerCharacterInfo = nullptr;

	UPROPERTY()
	EAnimStateEnum PrevAnimState = EAnimStateEnum::VE_None;

	UPROPERTY()
	FName PlayActionMontageName;

public:

	//////////////////////////////////////////////////////////////////////////

	/*
	Override functions
	*/
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTimeX) override;
	void NativeUninitializeAnimation() override;

	//////////////////////////////////////////////////////////////////////////
	// AnimNotify

	// NormalAttack
	void	BeginAttackState(ENormalAttackTypeEnum CheckAttackType);
	void	EndAttackState(ENormalAttackTypeEnum CheckAttackType);

	void	BeginUnconditionalExecuteAttackState(ENormalAttackTypeEnum CheckAttackType);
	void	EndUnconditionalExecuteAttackState(ENormalAttackTypeEnum CheckAttackType);

	// SkillAttack
	void	BeginSkillAniState();
	void	EndSkillAniState();

	// SkillApply
	void	BeginSkillApply();
	void	EndSkillApply();

	// AbnormalState
	void	EnterAbnormalStateAni();
	void	EndAbnormalStateAni();

	// On hit target
	void	HitTarget();
	void	HitTargetDummy();
	// Shot projectile
	void	ShotProjectile(EBulltSocketType ShootPoint);
	// Skill Casting Notify
	void	SkillCastingNotify();
	// Skill Fire Notify
	void	SkillFireNotify();
	// Skill Dummy Notify
	void	SkillDummyNotify();

	// End spawn ani
	void	EndSpawnAni();
	// End dead ani
	void	EndDeadAni();
	
	
	//////////////////////////////////////////////////////////////////////////
	// TestMode
	void WangRealMode();

private:
	bool IsLimitPlayRate = true;
	bool IsForceDie2 = false;
	bool RenewalNormalAttack = false;
};
