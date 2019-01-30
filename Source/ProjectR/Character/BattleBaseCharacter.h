// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SharedConstants/GlobalConstants.h"

#include "SharedBattle/BattleInfoBlackboard.h"
#include "Network/PacketDataStructures.h"
#include "Item/WeaponBase.h"
#include "Interfaces/EntityBaseProperty.h"


#include "CustomStructures/SharedStructures/CharacterBattleProperty.h"
#include "AI/AIDefs.h"

#include "Interfaces/EntityComponentGetterInterface.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "BattleBaseCharacter.generated.h"

class UAnimStateComponent;
class UIBuff;
class UBuffSkillController;
class UISkill;
class UAutoSkillContainer;
class UInputSkillContainer;
class UCharacterSkill;
class USkillActiveCondition;
class APassiveContinousSkill;
class AWeaponBase;
class UBattleInfoBlackboard;
class UBattleOperator;
class UPhaseComponent;
class UBossBaseComponent;
class UCustomChangeMaterialComponent;
class UCustomCharacterMovementComponent;
//class USelectionIndicatorMeshComponent;
class ULateUpdateComponent;
class UCustomAmbientComponent;
class UEffectComponent;
class UWaypointComponent;
//class U\CharacterSpawner;

//todo : test
class UPathFindingComponent;
class UFindValidLocationComponent;
class UVibrationComponent;
class UAvoidanceComponent;
class USpecialMovingComponent;
class UGridMapInspectorComponent;

/**
 * 
 */
UCLASS()
class PROJECTR_API ABattleBaseCharacter : public ACharacter, public IEntityBaseProperty, public IEntityComponentGetterInterface
{
	GENERATED_BODY()
	
public:

	ABattleBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

	//=========================================================================================
	// for test
	//=========================================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	FCharacterBattleProperty	TotalProperty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	FCharacterBattleProperty	BaseProperty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	FCharacterBattleProperty	ItemProperty;
	//=========================================================================================
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)//, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* HeadUpWidgetComp;
public:
	
	/*
	Overrided functions
	*/
	void OnConstruction(const FTransform& Transform) override;
	void Tick(float DeltaSecond) override;
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Destroyed() override;
	bool IsReadyForFinishDestroy() override;

	/*
	override EntityComponentGetterInterface
	*/
	FORCEINLINE UBattleEntityComponent* GetBattleEntityComponent() override { return BattleEntityComponent; }
	FORCEINLINE USkillEntityComponent*	GetSkillEntityComponent() override { return SkillEntityComponent; }
	FORCEINLINE UEntityAIComponent*		GetEntityAIComponent() override { return EntityAIComponent; }
	FORCEINLINE UEntityRenderComponent* GetEntityRenderComponent() override { return EntityRenderComponent; }
	FORCEINLINE UAnimStateComponent*	GetAnimStateComponent() override { return AnimStateComponent; }
	FORCEINLINE URemoteMoveComponent*	GetRemoteMoveComponent() override { return RemoteMoveComponent; }

	//todo : test
	//UPathFindingComponent*	GetPathFindingComponent() { return PathFindingComponent; }
	FORCEINLINE UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent() { return CustomCharacterMovementComponent; }

	FORCEINLINE UVibrationComponent*		GetVibrationComponent() {return VibrationComponent;}
	FORCEINLINE USpecialMovingComponent*	GetSpecialMovingComponent() { return SpecialMovingComponent; }
	FORCEINLINE UGridMapInspectorComponent* GetGridMapInspectorComponent() { return GridMapInspectorComponent; }
	FORCEINLINE UEffectComponent*			GetEffectComponent() { return EffectComponent; }
	UCustomAmbientComponent*				GetCustomAmbientComponent() { return CustomAmbientComponent; }

	FORCEINLINE void						SetFSMType(EAIFSMTypeEnum InAIFSMType) { Rai_FSMType = InAIFSMType; }

protected:
	virtual void		EndSkillActionExe();
	virtual void		InitHeroAfterCreationExe();
	
protected:
	UPROPERTY()
	UBattleEntityComponent* BattleEntityComponent;
	UPROPERTY()
	USkillEntityComponent*	SkillEntityComponent;
	UPROPERTY()
	UEntityAIComponent*		EntityAIComponent;
	UPROPERTY()
	UEntityRenderComponent* EntityRenderComponent;
	UPROPERTY()
	UAnimStateComponent*	AnimStateComponent;
	UPROPERTY()
	URemoteMoveComponent*	RemoteMoveComponent = nullptr;

	UPROPERTY()
	ULateUpdateComponent* LateUpdateComponent = nullptr;

	UPROPERTY()
	UCustomAmbientComponent* CustomAmbientComponent = nullptr;

	UPROPERTY()
	UEffectComponent*		EffectComponent = nullptr;

	//todo : test
	//UPathFindingComponent*			PathFindingComponent = nullpyt;
	//UFindValidLocationComponent*		FindValidLocationComponent = nullpyt;

	UPROPERTY()
	UCustomCharacterMovementComponent*	CustomCharacterMovementComponent = nullptr;

	UPROPERTY()
	UVibrationComponent*	VibrationComponent = nullptr;
	//UAvoidanceComponent*		AvoidanceComponent = nullptr;

	UPROPERTY()
	USpecialMovingComponent* SpecialMovingComponent = nullptr;

	UPROPERTY()
	UGridMapInspectorComponent* GridMapInspectorComponent = nullptr;

	bool						IsStopTick = false;

	float TimeAcc = 0.f;

public:
	UPROPERTY()
	bool				IsAlive;

	bool			bSpawnedFromCrewChange = false;

	UPROPERTY()
	EAIFSMTypeEnum		Rai_FSMType = EAIFSMTypeEnum::VE_Normal;

		
	//===========================================================================================================
	//
	// IBattleProperty interfaces
	//
	//===========================================================================================================	
	virtual EEntityTypeEnum				GetEntityType() override;
	virtual void						SetEntityType(EEntityTypeEnum InEntityType) override;
	
	virtual int32						GetLevel() override;
	virtual void						SetLevel(int32 InLevel) override;
	
	virtual int32						GetGrade() override;
	virtual void						SetGrade(int32 InGrade) override;

	EClassTypeEnum						GetJobClass() override;
	void								SetJobClass(EClassTypeEnum InJobClass) override;

	virtual bool						IsReady() override;	
	virtual void						SetReady(bool InReady) override;
		
	virtual FName						GetCharacterID() override;
	virtual void						SetCharacterID(FName InCharacterID) override;

	virtual ECharacterCategoryTypeEnum	GetCharacterCategory() override;
	virtual void						SetCharacterCategory(ECharacterCategoryTypeEnum InCharacterCategory) override;

	virtual int32						GetUserKID() override;
	virtual void						SetUserKID(int32 InUserKID) override;

	virtual	int32						GetPlayerUK() override;
	virtual	void						SetPlayerUK(int32 InPlayerUK) override;

	virtual int32						GetPartyID() override;
	virtual void						SetPartyID(int32 InPartyID) override;

	virtual int32						GetGroupKey() override;
	virtual void						SetGroupKey(int32 InGroupKey) override;

	virtual FString						GetHeroUD() override;
	virtual void						SetHeroUD(FString InHeroUD) override;
	virtual void						InitStartProperty();

	virtual ECharacterPropertyTypeEnum	GetAttachPropertyType() override;
	virtual void						SetAttachPropertyType(ECharacterPropertyTypeEnum InPropertyType) override;

	virtual bool						GetAccountCharacter() override;
	virtual void						SetAccountCharacter(bool InFlag) override;

	virtual int32						GetRealTimeParam() override;
	virtual void						SetRealTimeParam(int32 InParam) override;
	virtual void						ReSetRealTimeParam() override;

	virtual FEntityBasePropertyInfo*	GetEntityBasePropertyInfo() override;

	
	
	virtual void						InitMembers() override;
	virtual void						InitHeroBase() override;	
	virtual void						InitHeroAfterCreation() override;
	virtual void						SetSpawnEvent() override;
	virtual void						SetSpawnEventForArena() override;
	virtual void						AttachUI() override;

	virtual void						AttachComponents() override;

	void 								CacheInstance();

	// BlueprintImplementableEvent
	UFUNCTION(BlueprintImplementableEvent, Category = CharacterUI)
	void								OnInit() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Battle)
	void								OnInitHero() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Skill)
	void								OnInitHeroAfterCreation() override;

	//Change Situation
	UFUNCTION(BlueprintImplementableEvent, Category = Battle)
	void								OnChangeSituation() override;

	// when attackTarget is set.
	UFUNCTION(BlueprintImplementableEvent, Category = Battle)
	void								OnSetAttackTarget(AActor* TargetActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	FEntityBasePropertyInfo				EntityBasePropertyInfo;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};



