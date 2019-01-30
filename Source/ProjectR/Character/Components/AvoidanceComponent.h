// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AvoidanceComponent.generated.h"


class UEntityAIComponent;
class UBattleEntityComponent;
/**
 * 
 */
UCLASS()
class PROJECTR_API UAvoidanceComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
		
public:
	UAvoidanceComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the game starts
	void InitializeComponent() override;
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void StopMovementImmediately() override;

	FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	
	void CalcAvoidanceVelocity(float DeltaTime) override;
	bool MoveProcess_From_BasePlayerController();
	bool Exception_LeaderMoveProcess();
	bool SearchNearAttackTarget(bool SearchInSightRange);

	void					InitAvoidanceMovement(UEntityAIComponent* EntityAIComp);
	void					UpdateSelectCharacter(float DeltaTime);
	FORCEINLINE bool		IsEnableStopSearch()				{ return bEnableStopSearch; }
	FORCEINLINE void		SetEnableStopSearch(bool bEnable)	{ bEnableStopSearch = bEnable; }

	FORCEINLINE bool		IsEnableAIMovementCondition() { return bEnableAIMovementCondition; }
	FORCEINLINE void		SetEnableAIMovementCondition(bool bEnable) { bEnableAIMovementCondition = bEnable; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = avoidfff)
	bool		IsCheckTest = false;


protected:
	void					CheckAIMovementCondition(float DeltaTime);	

protected:
	bool							bEnableStopSearch = false;
	bool							bEnableAIMovementCondition = true;
	bool							bSelectCharacterRVOOff = false;
	bool							bSamePosSearch = false;
	mutable int32					FrameCount = 0;
	mutable FVector					PrevNormal = FVector::ZeroVector;
	mutable FVector					PrevSlideDelta = FVector::ZeroVector;
	float							SameSearchTime = 0.0f;
	float							RVOOffTime = 0.0f;
	UPROPERTY()
	FVector							PrevSelectCharacterPos = FVector::ZeroVector;

	float							LockTimeAfterAvoid = 0.5f;

	UPROPERTY()
	UEntityAIComponent*				EntityAIComponent = nullptr;
	UPROPERTY()
	UBattleEntityComponent*			BattleEntityComponent = nullptr;

	UPROPERTY()
	FVector							PrevLocation = FVector::ZeroVector;
	float							SameLocTime = 0.0f;

};
