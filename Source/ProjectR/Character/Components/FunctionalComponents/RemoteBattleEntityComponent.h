// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleEntityComponent.h"
#include "RemoteBattleEntityComponent.generated.h"


const int32 MAX_SYNC_ATTACK_TRY_COUNT = 10;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API URemoteBattleEntityComponent : public UBattleEntityComponent
{
	GENERATED_BODY()
		 
protected:
	// Sets default values for this component's properties
	URemoteBattleEntityComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void			SetAvatarAbilityProperty() { ; }
		
	USkillEntityComponent*	GetOwnerSkillComponent();

	virtual void			Dead( bool InIsDestroy = false)  override;

	virtual void			SetHP(float Value) override;
	virtual void			AddHP(float Value, AActor* FromActor, bool IsDisplayValue = false)  override;

	// RTS_NormalAttack
	virtual void			RecvRTS_AttackNow(AActor* TargetActor, bool bAttack) override;
	virtual void			RecvRTS_NormalAttack(AActor* TargetActor, EBattleDamageResultEnum HitDamageResult, int32 HitDamage, uint32 TargetHP, uint8 sequence) override;

	// CSNormalAttack
	//virtual void			SendCSNormalAttack(ENetNormalAttackStateEnum AttackState, ENormalAttackTypeEnum AttackAnimType) override;
	//virtual void			RecvCSNormalAttack(Proud::PtNotiNormalAttack& attack_data) override;
	virtual void			CSOnDamage(AActor* Attacker, FCSNormalAttackSyncData& AttackData) override;

	virtual void			ForceDead(AActor* FromActor) override;
	//////////////////////////////////////////////////////////////////////////

	void Remote_Dead(AActor* FromActor);
	void Remote_SetHP(float Value);
	void Remote_AddHP(float Value, bool IsDisplayValue = false);
};
