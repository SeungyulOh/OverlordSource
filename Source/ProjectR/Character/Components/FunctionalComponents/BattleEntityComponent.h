// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Network/PacketDataStructures.h"
#include "BattleEntityComponent.generated.h"


class AActor;
class USkillEntityComponent;
class UBattleInfoBlackboard;
class UBattleOperator;
class UCharacterBattleInfo;
class UBattleEntityComponent;
class UBossBaseComponent;
class UPhaseComponent;
class AWeaponBase;

struct FWorkedBuffTableInfo;
struct FSkillSyncApplyData;



const int32 NET_SYNC_ATTACK = 100;

USTRUCT()
struct FCSNormalAttackSyncData
{
	GENERATED_USTRUCT_BODY()

	TWeakObjectPtr<AActor>	Target;

	EBattleDamageResultEnum	HitResult = EBattleDamageResultEnum::VE_None;
	float					HitDamage = 0.0f;
	int32					SyncHP = 0;

	void Clear()
	{
		Target = nullptr;
		HitResult = EBattleDamageResultEnum::VE_None;
		HitDamage = 0.0f;
		SyncHP = 0;
	}

	FCSNormalAttackSyncData& operator = (const FCSNormalAttackSyncData& other)
	{
		Target = other.Target;
		HitResult = other.HitResult;
		HitDamage = other.HitDamage;
		SyncHP = other.SyncHP;

		return *this;
	}

	FCSNormalAttackSyncData()
	{
		Clear();
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnReceiveDamage, AActor*, Invoker, float, Damage, EBattleDamageResultEnum, DamageResultType, AActor*, FromActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnHit, AActor*, Invoker, EBattleDamageResultEnum, DamageResult, AActor*, TargetActor, bool, bBullet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnReceiveHeal, float, Heal, AActor*, FromActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDelegate_OnNotifyDamage, AActor*, Attacker, AActor*, Victim, int32, Damage, bool, SkillAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnRefreshRealTimeParam, int32, ChangeParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnSetBlockAbnormalMsg, ESkillApplierTypeEnum, InBlockApplierType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UBattleEntityComponent : public UActorComponent
{
	GENERATED_BODY()
		 
protected:
	// Sets default values for this component's properties
	UBattleEntityComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	TWeakObjectPtr<UParticleSystemComponent> HitEffect;

	USkillEntityComponent*			GetOwnerSkillComponent();


	// Set battleInfo from table	
	virtual void					SetBattleInfoFromTable() ;


	virtual void					SetAvatarAbilityProperty();
	bool							GetAvatarAbilityPoints(TArray<int32>& AbilityPoints);

	// set initial battle property
	virtual void					InitBattleProperty();
	

	virtual void					CreateBattleInfo();
	virtual void					CreateBattleInfoBlackboard();
	virtual void					CreateBattleOperator();


	virtual UBattleInfoBlackboard*	GetBattleInfoBlackboard();
	virtual UBattleOperator*		GetBattleOperator();
	virtual UCharacterBattleInfo*	GetBattleInfo();

	void							ChangeAIPriorityType(EPriorityTypeEnum PriorityType);

	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual AActor*					GetCurrentAttackTarget();
	virtual void					SetCurrentAttackTarget(AActor* TargetActor);
	

	virtual void					HitTarget(bool bPhysicalAttack = true, bool bSkillAttack = false );
	virtual EBattleDamageResultEnum	HitTargetActor(AActor* TargetActor, bool bPhysicalAttack = true, bool bSkillAttack = false);
	virtual EBattleDamageResultEnum	HitTargetWithBullet( AActor* TargetActor, bool bPhysicalAttack = true, bool bSkillAttack = false);
		
	virtual void					SpawnHitEffect(EBattleDamageResultEnum DamageResult, AActor* TargetActor, bool bBullet);


	virtual bool					IsAttacking() ;
	virtual void					EnterAttack();
	virtual void					EndAttack();		
	
	UFUNCTION()
	void LateUpdate();

	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual float					GetCurrentHPPercent();

	FORCEINLINE float				GetCurrentHP() { return (float)CharacterBattleInfo->GetHP(); }

	// Is dead?
	FORCEINLINE void				SetDying(bool Dying) { bDyingState = Dying; }
	FORCEINLINE bool				IsDying() { return bDyingState; }
	virtual bool					IsDead();

	virtual void					DeadClear();

	virtual void					SetHP(float Value) ;	
	virtual int32					GetHP() ;	
	virtual int32					GetMaxHP() ;

	virtual void					AddHP(float Value, AActor* FromActor, bool IsDisplayValue = false);	
	virtual void					NotifyReceiveHeal(float value, AActor* FromActor = nullptr) ;

	// Is searchSkip
	virtual bool					IsSearchSkip();	
	virtual bool					IsEnemy(AActor* TargetActor);
	bool							GetIsPauseState() { return IsPauseState; }
	void							SetIsPauseState(bool InValue) { IsPauseState = InValue; }

	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual bool					AmIEnemyWithMyPlayer();
	


	// shot the projectile
	virtual void					ShotProjectile();

	virtual FVector					GetShotPointLocation(FName SocketName = EXCEPTION_NONE);
	virtual EBulltSocketType		GetShotPointType();
	virtual void					SetShootSocketPoint(EBulltSocketType Type);
	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual void					AssignShotPoint(UArrowComponent* InShotPointRef) ;
	virtual void					CreateShotPoint() ;

	/*
	Damage
	*/		

	
	virtual void					SearchTargetFromHitOnIdle(AActor* Offender);


	virtual EBattleDamageResultEnum	OnDamageFromCharacter(AActor* FromActor, bool bPhysicalAttack, bool bSkillAttack = false );
	virtual EBattleDamageResultEnum	OnDamage(AActor* FromActor, float FinalDamage, EBattleDamageResultEnum DamageResultType, bool bSkillAttack = false );
	virtual EBattleDamageResultEnum ApplyFinalDamage(float FinalDamage, AActor* FromActor, EBattleDamageResultEnum DamageResultType);
	bool							RecoverHPForTutorial();


	virtual void					Dead(bool InIsDestroy = false);
	virtual void					OnReceiveDamageEvent(float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor) ;


	virtual void					ValidateFinalCharacterProperty();


	/*
	Character & Abnormal state
	*/
	virtual bool					IsInState(ECharacterStateTypeEnum CharacterState) ;	
	virtual bool					SetAbnormalState(EAbonormalStateFlagEnum AbnormalStateFlag, bool bSet);
	
	


	/*
	New skill system
	*/	
	virtual int32					AttachAbnormalStateApplier(ESkillApplierTypeEnum InAbnormalApplyType, bool bSet);
	virtual bool					IsInAbnormalStateApplier(ESkillApplierTypeEnum InAbnormalApplyType);
	virtual bool					IsInAbnormalState( EAbonormalStateFlagEnum InAbnormalStateFlag );
	virtual bool					IsInUnActableState();
	virtual bool					IsInUnMovableState();
	

	virtual bool					IsInBlockBuffType(EAbnormalSlotTypeEnum InSlotType );
	virtual bool					IsInBlockGroupType(EAbnormalSlotTypeEnum InSlotType);
	virtual bool					IsInBlockSlotType(EAbnormalSlotTypeEnum InSlotType);

	virtual bool					IsInExceptBlockGroupType(EAbnormalSlotTypeEnum InSlotType);
	virtual bool					IsInExceptBlockSlotType(EAbnormalSlotTypeEnum InSlotType);

	virtual void					ClearAllAbnormalState() ;

	bool							IsAbleToUseSkill();

	UFUNCTION(BlueprintCallable, Category = BattleEntity)
	virtual ECharacterSituationState GetSituationState() ;
	UFUNCTION(BlueprintCallable, Category = BattleEntity)
	virtual void					SetSituationState(ECharacterSituationState InSituationState) ;

	UFUNCTION(BlueprintCallable, Category = BattleEntity)
	virtual void					ChangeSituation(ECharacterSituationState ChangeState) ;
	UFUNCTION(BlueprintCallable, Category = BattleEntity)
	virtual bool					IsInSituationState(ECharacterSituationState CheckState) ;

	virtual float					GetMoveAniSpeed() ;

	virtual UBossBaseComponent*		GetBossComponent() ;
	virtual void					SetBossComponent(UBossBaseComponent* InBossComponent ) ;

	
	
	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual int32					GetCommandSlotIndex();

	virtual void					SetCommandSlotIndex( int32 InCommandSlotIndex );

	/*
	Weapon
	*/
	AWeaponBase*					GetWeaponActor(bool bRight) ;
	AWeaponBase*					AttachWeaponBase(TSubclassOf<AWeaponBase> weaponClass, FName TargetSocket) ;
	virtual void					PrepareWeaponsBase() ;
	virtual void					DestroyWeaponsBase() ;

	virtual FVector					GetForwardVector() ;
	virtual void					EquipItem(EItemEquipPositionEnum Position, const FString& WeaponItemKey) ;
	virtual void					EquipItemFITEM(EItemEquipPositionEnum Position, FITEM& Item) ;
	void							EquipItemFITEMTest(EItemEquipPositionEnum Position, FITEM& Item);
	virtual void					UnEquipItem(EItemEquipPositionEnum Position) ;
	virtual void					AttachWeaponInit(UClass* WeaponClassType, bool bLeft = false) ;
	virtual bool					IsCloseEnoughForAttack(AActor* InTargetActor, float SpecialAttackRange = 0.0f, float ExtraDist = 0.0f ) ;
	virtual bool					IsCloseEnoughToActor(AActor* InTargetActor, float ExtraDist = 0.0f ) ;
	virtual bool					IsTargetInRange(AActor* InActor, float Range);
	virtual bool					IsCurrentTargetInNormalAttackRange();



	virtual void					SetSummonOwner(AActor* InOwner) ;
	virtual AActor*					GetSummonOwner() ;

	UFUNCTION(BlueprintCallable, Category = BattleEntityComponent)
	virtual bool					IsBoss() ;

	virtual void					SetBoss(bool InBoss) ;
	virtual bool					IsKeyMonster() ;
	virtual void					SetKeyMonster(bool InKeyMonster) ;

	virtual float					GetBoundingRadius() ;
	virtual void					SetBoundingRadius(float InBoundingRadius) ;
	virtual float					GetCapsuleComponentHalfHeightValue() ;

	ECharacterPropertyTypeEnum		GetAttackPowerType();


	// CSNormalAttack
	virtual void					SendRTS_AttackNow(bool bAttack, int32 AnimSequence);
	virtual void					RecvRTS_AttackNow(AActor* TargetActor, bool bAttack);
	virtual void					RecvRTS_NormalAttack(AActor* TargetActor, EBattleDamageResultEnum HitDamageResult, int32 HitDamage, uint32 TargetHP, uint8 sequence);
	virtual void					CSHitTarget(bool HitWithBullet = false);
	virtual void					CSOnDamage(AActor* Attacker, FCSNormalAttackSyncData& AttackData);

	void							StopBattleEntityForced();

	virtual void					ForceDead(AActor* FromActor);
	void							OnDeadEvent(AActor* FromActor);

	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnReceiveDamage			OnReceiveDamageDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnHit						OnHitDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnReceiveHeal				OnReceiveHealDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnNotifyDamage			OnNotifyDamageDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnRefreshRealTimeParam	OnRefreshRealTimeParam;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Battle)
	FDelegate_OnSetBlockAbnormalMsg		OnSetBlockAbnormalMsg;
	
public:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	// cache owner skill component
	UPROPERTY()
	UBattleEntityComponent*		OwnerBattleComponent = nullptr;
	UPROPERTY()
	USkillEntityComponent*		OwnerSkillComponent = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UCharacterBattleInfo*	CharacterBattleInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	TWeakObjectPtr< AActor >	CurrentAttackTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	TWeakObjectPtr< AActor >	DeadFromActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UArrowComponent*		ShotPoint = nullptr;

	
	UPROPERTY()
	EBulltSocketType		CheckShootPoint = EBulltSocketType::VE_None;

	UPROPERTY()
	UBattleInfoBlackboard*	BattleInfoBlackboard = nullptr;

	UPROPERTY()
	UBattleOperator*		BattleOperator = nullptr;
		
		
	bool					bAttackAni = false;
	bool					bShouldNextAttack = false;
	uint8					Attack_RTS_Sequence = 0;
	UPROPERTY()
	int32										AbnormalStateBit = 0;
	UPROPERTY()
	TMap< ESkillApplierTypeEnum, int32 >		AttachedAbnormalStateApplierCount;
	
	UPROPERTY()
	ECharacterSituationState	SituationState = ECharacterSituationState::VE_Ground;
	
	// Cache value
	UPROPERTY()
	FName					NormalAttackBuff;

	//Chracter FrontAttacked is true when receive damage from Actor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	bool					IsFrontAttacked = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	bool					IsInValidAttackPeriod = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	TWeakObjectPtr< AWeaponBase >			RWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle)
	TWeakObjectPtr< AWeaponBase >			LWeapon;

	UPROPERTY()
	TArray< AWeaponBase* >					WeaponObjectList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	float					BoundingRadius = 100.0f;

	/*
	Character Component
	*/
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterComponent)
	UBossBaseComponent*		BossComponent = nullptr;

	/*
	Character Flag
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleFlag)
	bool					bBoss = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleFlag)
	bool					bKeyMonster = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BattleFlag)
	bool					IsLeader = false;


	//
	bool					IsNonFriend = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	int32					CommandSlotIndex = -1;

	UPROPERTY()
	TWeakObjectPtr< AActor >			SummonerCharacter;
	
	// OwnerActor attack other actor
	bool							IsCSNormalAttackReceived = false;
	FCSNormalAttackSyncData			CSNormalAttackerChangeHPData;
	UPROPERTY()
	TArray<FCSNormalAttackSyncData>	CSNormalAttackSyncData;

	bool							IsCustomState = false; //only tuto
	bool							IsPauseState = false;
	bool							IsCreateShotPoint = false;

protected:
	bool							bAttacking = false;
	bool							bDyingState = true;
	bool							bRealTimeMode = false;

	bool							bForceTutorialing = false;
	bool							bForceTutorialColosseum = false;
	bool							bOwnerIsAccountChar = false;
	bool							IsDeadClearCall = true;

private:
	bool	IsGoalActorCellAdjacent(AActor* InTargetActor);
	void	AdjustForceTutorialColosseumDamage(float& FinalDamage);

	UPROPERTY()
	UClass* DamageFloater_BPClass;
};
