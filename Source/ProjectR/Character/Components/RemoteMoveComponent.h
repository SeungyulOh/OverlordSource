// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Network/SyncPeerDefs.h"
#include "RemoteMoveComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTR_API URemoteMoveComponent : public UActorComponent
{
	GENERATED_BODY()
	
	
public:
	URemoteMoveComponent();

	// Called when the game starts
	void InitializeComponent() override;

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();
	bool IsArrive();
	bool IsMoveActive() const;

	// S2C
// 	void CSMoveStart(FCharacterMoveData& CharacterMoveData);
// 	void CSMoveStop(FCharacterMoveData& CharacterMoveData);

 	void MoveToTargetPosition(float DeltaTime);
 	void StopMoving();
	void Update(FRTS_ACTOR_UPDATE _update);
	FCharacterMoveData				CharacterMoveData;
public:
// 	float		SyncInterval = 1.0f;
 	float		IgnoreDistance = 5.0f;
	bool		bMoveActive = false;

	UPROPERTY()
	FVector		StartPosition = FVector::ZeroVector;

	UPROPERTY()
	FVector		TargetPosition = FVector::ZeroVector;

	UPROPERTY()
	FVector		DirectionVector;

	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
};
