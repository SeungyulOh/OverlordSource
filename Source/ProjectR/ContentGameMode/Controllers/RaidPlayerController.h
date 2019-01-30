// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "StagePlayerController.h"
#include "RaidPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API ARaidPlayerController : public AStagePlayerController
{
	GENERATED_BODY()
	
	friend class ULeaderSkillControl;
	friend class ULeaderMoveControl;
public:
	ARaidPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void				BeginPlay() override;

	virtual void				TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	UFUNCTION()
	void						OnRTSCrewDead(int32 InSlotIndex);

};

