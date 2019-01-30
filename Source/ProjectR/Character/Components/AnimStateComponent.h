// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OnlineKeyValuePair.h"
#include "SharedConstants/GlobalConstants.h"
#include "AnimStateComponent.generated.h"

class UIAnimState;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTR_API UAnimStateComponent : public UActorComponent
{
	GENERATED_BODY()
	
	
public:
	UAnimStateComponent();

	// Called when the game starts
	void InitializeComponent() override;
	void UninitializeComponent() override;

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitAnimComponentStates();
	void SetAnimComponentState(EAnimStateEnum State, ECharacterCommandEnum Command = ECharacterCommandEnum::VE_None, FVariantData varParam = FVariantData());
	
	// Remove Character Only
	void RemoteAnimComponentState(EAnimStateEnum AnimState, ECharacterCommandEnum Command = ECharacterCommandEnum::VE_None, FVariantData varParam = FVariantData());

	// Update Character Command
	void DoCommand(ECharacterCommandEnum Command, FVariantData varParam = FVariantData());

	bool IsCancelToCurrentAnim();
	bool IsAbleToAttackAnim();
	bool IsAbleToRunAnim();

	FORCEINLINE UIAnimState* GetCurrentAnimState() { return CurrentState; }
	UIAnimState* GetAnimState(EAnimStateEnum InState);
	EAnimStateEnum GetCurrentAnimStateType();
	

private:
	UPROPERTY()
	TMap< EAnimStateEnum, UIAnimState* >	AnimStates;
	
	UPROPERTY()
	UIAnimState*							CurrentState = nullptr;

	UPROPERTY()
	TWeakObjectPtr< AActor >				OwnerActor;
	
	UPROPERTY()
	bool									RemoteEntity = false;
};
