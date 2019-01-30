// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI/AIDefs.h"
#include "AI/RAI_Blackboard.h"
#include "SharedConstants/GlobalConstants.h"
#include "EntityAIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOneParamTacticMove, float, Duration);



class AAIController;
class ABaseAIController;
class URAI_FSM;
class ALevelSequenceActor;
class UWaypointComponent;
class UMovingStateCheckComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UEntityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEntityAIComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		
	virtual void			StartMove(const FVector& Dest);
	virtual void			StopMove();
	virtual void			ForceSearchTarget();
	virtual AActor*			GetNearestSearchTarget();
	virtual bool			IsInForceTargetLocation();

	virtual void			SetIdleLocation(FVector InIdleLocation);
	virtual void			ForceSetStateIdle();

	virtual void			SetForcePauseBySkill(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee = true);
	virtual void			SetForcePauseProcess(float SlowTime, float SlowRate, float BlackOutTime, bool IsPlayMatinee = true);
	virtual void			SetForcePause(bool bPause, float time = 0.0f);

	virtual float			GetIgnoreForcePauseTime();

	virtual void			ForcePauseStart(float time = 0.0f);
	virtual void			ForcePauseEnd();
	virtual void			SetPauseSetting(bool bPause);

	virtual void			BlackOutBySeconds(float time = 0.0f);
	virtual void			IgnoreForcePauseTimeEnd();
	virtual void			IgnoreBlackOutTimeEnd();

	
	/*
	Blackboard-related
	*/
	virtual void			SetBlackboardData();
	virtual void			SetBlackboardForceTarget(AActor* InForceTarget);
	virtual void			SetBlackboardForceTargetLocation(FVector ForceTargetLocation = FVector::ZeroVector);
	virtual void			SetBlackboardAttackLocation(FVector InAttackLocation, bool InSet = true);
	virtual void			SetBlackboardLeader(AActor* InTarget);	
	virtual void			ClearAllBlackboard();

	virtual bool			IsSetBlackboardValue(ERAIBalckboardValueEnum InBlackboardValueType);
	virtual void			ClearBlackboardValue(ERAIBalckboardValueEnum InValueType);

	virtual void			SetBlackboardActorValue(ERAIBalckboardValueEnum InBlackboardValueType, AActor* InValue);
	virtual void			SetBlackboardVectorValue(ERAIBalckboardValueEnum InBlackboardValueType, FVector InValue);
	virtual void			SetBlackboardFloatValue(ERAIBalckboardValueEnum InBlackboardValueType, float InValue );
	virtual void			SetBlackboardBoolValue(ERAIBalckboardValueEnum InBlackboardValueType, bool InValue);
	
	virtual AActor*			GetBlackboardActorValue(ERAIBalckboardValueEnum InBlackboardValueType);
	virtual FVector			GetBlackboardVectorValue(ERAIBalckboardValueEnum InBlackboardValueType);
	virtual float			GetBlackboardFloatValue(ERAIBalckboardValueEnum InBlackboardValueType);

	/*
	RAI_FSM-related
	*/
	virtual void				InitAIFSM();
	void						SetAIControlMode(EAIControlModeEnum InAIControlMode);
	FORCEINLINE EAIControlModeEnum			GetAIControlMode() { return AIControlMode; }

	virtual EAIStateTypeEnum	GetCurrentAIState();
	
	
	virtual void			AIStop();
	virtual void			AIRestart();
	bool					IsActiveAI();

	virtual void			AttackManualMode();
	virtual void			ForceDirection(FVector InTargetLocation);
	virtual bool			IsUsePathfinding();
	virtual void			PrepareWayPoint();
	virtual void			CapsuleCollision(bool Enabled);

	virtual float			GetModifiedAnimSpeed();
	FORCEINLINE void		SetModifiedAnimSpeed(float InSpeed) { ModifiedAnimSpeed = InSpeed; }
	FORCEINLINE void		AddModifiedAnimSpeed(float InSpeed) { ModifiedAnimSpeed += InSpeed; }
	FORCEINLINE URAI_FSM*	GetFSM() { return AIFSM; }

	FORCEINLINE FVector		GetTargetLocation() { return TargetLocation; }
	
	FORCEINLINE ECharacterDirectionEnum GetIdleDirection() { return IdleDirection; }
	FORCEINLINE void		SetIdleDirection(ECharacterDirectionEnum InIdleDirection) { IdleDirection = InIdleDirection; }
		
	virtual USphereComponent* GetNaviCollisionSphere();

	UPROPERTY()
	FDelegate_OnOneParamTacticMove	OnStart_TacticsMove;

	//----------------------------------------------------------------------------------------
	// Waypoint
	//----------------------------------------------------------------------------------------
	virtual void			AttachWaypointComponent();	
	FORCEINLINE UWaypointComponent* GetWaypointComponent() { return WaypointComponent; }

	
	ABaseAIController*		GetEntityBaseAIController();


	

	void SetPause(bool bPause);

protected:
	AAIController*				GetEntityAIController();
	
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;

	UPROPERTY()
	FTimerHandle			ForcePauseHandler;
	UPROPERTY()
	FTimerHandle			IgnoreForcePauseHandler;
	UPROPERTY()
	FTimerHandle			IgnoreBlackOutHandler;
	
	UPROPERTY()
	float					ModifiedAnimSpeed = 1.0f;
	UPROPERTY()
	EPriorityTypeEnum		PriorityType = EPriorityTypeEnum::VE_None;
	UPROPERTY()
	FVector					OriginPosition;
	
	UPROPERTY()
	ECharacterDirectionEnum IdleDirection = ECharacterDirectionEnum::VE_None;

	UPROPERTY()
	TWeakObjectPtr< ALevelSequenceActor >	UltimateSkillActor;

	UPROPERTY()
	UWaypointComponent*		WaypointComponent = nullptr;

	UPROPERTY()
	URAI_FSM*				AIFSM = nullptr;

	UPROPERTY()
	EAIControlModeEnum		AIControlMode = EAIControlModeEnum::VE_AutoMode;

	UPROPERTY()
	UMovingStateCheckComponent* MovingStateCheckComponent = nullptr;

	
	

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = EntityAI )
	FVector					TargetLocation;
	
	UPROPERTY()
	FVector					MoveDest = FVector::ZeroVector;

	bool TempCheckComp = true;


	//From BasePlayerController
	bool SearchNearAttackTarget(bool SearchInSightRange);
};
