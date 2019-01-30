// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BattleBaseCharacter.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "CustomStructures/SharedStructures/CharacterBattleProperty.h"
#include "CustomStructures/BattleObjectInfo.h"
#include "BattleBaseObject.generated.h"

class USelectionIndicatorMeshComponent;
class UObjectBattleEntityComponent;
/**
 * 
 */
UCLASS()
class PROJECTR_API ABattleBaseObject : public ABattleBaseCharacter
{
	GENERATED_BODY()

	friend class UObjectBattleEntityComponent;

public:
	enum STATE
	{
		SEARCH_STATE,
		EXECUTE_STATE,
	};
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	EBattleObjectTypeEnum	BattleObjectType = EBattleObjectTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	FBattleObjectInfo		BattleObjectInfo;

	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*	StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UStaticMeshComponent*	BodyMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UStaticMeshComponent*	HeadMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	float					RotateSpeed = 500.0f;

public:
	ABattleBaseObject(const FObjectInitializer& ObjectInitializer);

	void OnConstruction(const FTransform& Transform) override;
	void Tick(float DeltaSecond) override;
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = BattleObject)
	void OnReset();

	UFUNCTION(BlueprintNativeEvent, Category = BattleObject)
	void OnPlay(AActor* Target = nullptr);

	UFUNCTION(BlueprintNativeEvent, Category = BattleObject)
	void OnApply();

	UFUNCTION(BlueprintImplementableEvent, Category = BattleObject)
	void OnDead();

	UFUNCTION(BlueprintCallable, Category = BattleObject)
	void Apply();

	UFUNCTION(BlueprintCallable, Category = BattleObject)
	UStaticMeshComponent* GetStaticMesh();

	UFUNCTION(BlueprintCallable, Category = BattleObject)
	UISkill_V2* GetActiveSkill();

	UFUNCTION(BlueprintCallable, Category = BattleObject)
	void SetRotateComponent(USceneComponent* component, FVector addPos = FVector(0.0f, 0.0f, 0.0f));

	UFUNCTION(BlueprintCallable, Category = BattleObject)
	void SetFollowComponent(AActor* Target, USceneComponent* component, FVector addPos = FVector(0.0f, 0.0f, 0.0f));

	void			OnReset_Implementation();
	void			OnPlay_Implementation(AActor* Target = nullptr);
	void			OnApply_Implementation();

	void AttachUI() override;

	//FVector	GetShotPointLocation(FName SocketName = EXCEPTION_NONE) override;

	void SkillFire();
	void EffectInit();

	USceneComponent* GetSocketSceneComponent(FName socketName);
	FVector GetSocketLocation(FName socketName);
	FVector GetBattleObjectForward();
	FVector GetBattleObjectRight();
	FVector GetBattleObjectUp();
	FRotator GetBattleObjectRotator();

	FORCEINLINE void SetSkillUseing(bool bUse) { IsSkillUseing = bUse; }
	FORCEINLINE bool GetSkillUseing() { return IsSkillUseing; }

	UFUNCTION()
	void OnDeadActor(AActor* DeadActor, AActor* FromActor);
	UFUNCTION(BlueprintImplementableEvent, Category = BattleObject)
	void BP_OnDeadActor();

private:
	void EndSkillActionExe() override;
	void InitHeroAfterCreationExe() override;
	void BaseObjectInit();	
			
	void GetSimpleCollisionCylinder(float& CollisionRadius, float& CollisionHalfHeight) const override;

	void OnAttackTick();
	void OnTotemTick();
	void OnFindTargetTick();

private:
	bool						IsSkillUseing = false;
	float						CurrentTime = 0.0f;	
	bool						IsFindTarget = false;
	STATE						BattleBaseObjectState;
	
	UPROPERTY()
	UShapeComponent*			ShapeComponent = nullptr;
	
	UPROPERTY()
	TMap<AActor*, AActor*>		OverlapMap;
	
	UPROPERTY()
	TArray< TWeakObjectPtr<AActor> >			OverlapList;
	
	UPROPERTY()
	AActor*						TargetAttackTarget = nullptr;	

	UPROPERTY()
	FTimerHandle				AttackTickHandler;
	UPROPERTY()
	FTimerHandle				FindTargetTickHandler;
};
