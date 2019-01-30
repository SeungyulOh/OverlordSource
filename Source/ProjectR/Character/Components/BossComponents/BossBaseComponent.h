// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "BossBaseComponent.generated.h"

//struct FBossProtectEffectInfo;

UCLASS( ClassGroup=(Boss), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UBossBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	


	// Sets default values for this component's properties
	UBossBaseComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = BossBaseComponent)
	virtual void SetOwner(AActor* character);

	UFUNCTION(BlueprintCallable, Category = BossBaseComponent)
	virtual FVector GetForwardVector();

	virtual bool	IsUsePathfinding() { return true; }
	virtual void	StartSkillApply();
	virtual void	EndSkillApply();
	virtual bool	IsMoveEnable() { return true; }
	virtual AActor*	GetOwner() { return BattleActor; }

	virtual void SetBossProtectType(EProtectType InProtectType);

	virtual bool CanAttachSlot(EAbnormalSlotTypeEnum SlotType) { return true;}

public:
	//Warning : Direct Input Data From CharacterBluePrint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossBaseComponent)
	FName		BossProtectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossBaseComponent)
	FName		BossAiName;
	
protected:
	UPROPERTY()
	AActor*										BattleActor = nullptr;
	bool										bSkillApply = false;

	
	TWeakObjectPtr< UParticleSystemComponent >	BossProtectEffect;
	EProtectType								BossProtectType;	
	float										DefaultBossProtectPower = 0.0f;
	const int32									EffectDepthMax = 3;
	bool										IsBossProtectDeapth1 = false;
	bool										IsBossProtectDeapth2 = false;
	
};
