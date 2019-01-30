// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "BasePlayerController.h"
#include "LeaderControlHelper.h"
#include "StagePlayerController.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FVirtualJoyStickHelper
{
	GENERATED_USTRUCT_BODY()

public:
	void Calculate(SVirtualJoystick* InVirtualJoystick);
};

UCLASS()
class PROJECTR_API AStagePlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	
	friend class ULeaderSkillControl;
	friend class ULeaderMoveControl;
	friend class UCombineSkillHelper;
public:
	AStagePlayerController(const FObjectInitializer& ObjectInitializer);


	virtual void				BeginPlay() override;
	virtual void				TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void				EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	ULeaderControlHelper*		GetLeaderControlHelper();

	
	UFUNCTION()
	void						OnLeaderMove();

	void						SetCrewPosition(int32 InCrewIndex);
	AActor*						SpawnHeroAtLocation(int32 SelectedCommandSlot, const FVector& WorldLocation, const FRotator& CharRotation, bool IsSpawnEffectFromCrew = false);
	void						SetSelectedCharacter(AActor* InSelectedCharacter);
	void						CheckOwnerAbnormal();
protected:
	UPROPERTY()
	ULeaderControlHelper*		LeaderControlHelper;
	UPROPERTY()
	FVirtualJoyStickHelper		VirtualJoystickHelper;
	UPROPERTY()
	TWeakObjectPtr<class URSkillSet_New>	DPadSkillInterface = nullptr;
};

