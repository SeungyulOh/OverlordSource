// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SpecialMovingComponent.generated.h"

class ABattleBaseCharacter;
class UEntityAIComponent;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESpecialMoveTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Knockback			UMETA(DisplayName = "Knockback"),
	VE_Fear					UMETA(DisplayName = "Fear"),
	VE_Captivate			UMETA(DisplayName = "Captivate"),
	VE_Dash					UMETA(DisplayName = "Dash"),
	VE_Pull					UMETA(DisplayName = "Pull"),
	VE_Blink				UMETA(DisplayName = "Blink"),
	VE_Push					UMETA(DisplayName = "Push"),

	VE_Max,
};


const float KNOCK_BACK_TIME = 0.7f;
const float KNOCK_BACK_DISTANCE_DEFAULT = 1000.0f;
const float DASH_DISTANCE_DEFAULT = KNOCK_BACK_DISTANCE_DEFAULT;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API USpecialMovingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpecialMovingComponent();

	// Called when the game starts
	void InitializeComponent() override;
	void UninitializeComponent() override;

	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	ESpecialMoveTypeEnum GetSpecialMoveType() { return MoveType; }

	void StartKnockBack(const FVector& InDirection, float InDuration);
	void StopKnockBack();

	void StartPush(const FVector& InDirection, float InDuration);
	void StopPush();

	void StartFear(AActor* InTargetActor, float InDuration);
	void StopFear();

	void StartCaptivate(AActor* InTargetActor, float InDuration);
	void StopCaptivate();

	void StartDash(const FVector& InTargetLocation, float InDuration, float InDistance);
	void StopDash();

	void StartPull(const FVector& InTargetLocation, float InDuration);
	void StopPull();

	void StartBlink(const FVector& InTargetLocation, const FRotator& InTargetRotation , float InDuration);
	void StopBlink();

private:
	void UpdateKnockBack(float delta_time);
	void UpdateFear(float delta_time);
	void UpdateCaptivate(float delta_time);
	void UpdateDash(float delta_time);
	void UpdatePull(float delta_time);
	void UpdateBlink(float delta_time);
	void UpdatePush(float delta_time);

	
	void StopSpecialMoving();


private:
	UPROPERTY()
	TWeakObjectPtr<ABattleBaseCharacter>	OwnerCharacter;

	UPROPERTY()
	TWeakObjectPtr< AActor >	TargetActor;

	float						Duration;
	float						ElapsedTime = 0.0f;

	FVector						StartPos = FVector::ZeroVector;
	float						MoveSpeed = 500.0f;

	float						TargetDistance = 0.0f;
	FVector						TargetDirection = FVector::ZeroVector;
	FVector						TargetLocation;
	FRotator					TargetRotator;


	UPROPERTY()
	UCharacterMovementComponent*	CachedMovementComponent = nullptr;
	UPROPERTY()
	UEntityAIComponent*				CachedEntityAIComponent = nullptr;

	ESpecialMoveTypeEnum	MoveType = ESpecialMoveTypeEnum::VE_None;

};
