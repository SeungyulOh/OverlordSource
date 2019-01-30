// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WaypointComponent.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FWaypointInfo
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = WaypointInfo)
	FVector			TargetPos = FVector::ZeroVector;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = WaypointInfo)
	float			DelayTime = 0.0f;
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UWaypointComponent : public UActorComponent
{
	GENERATED_BODY()

	enum eState
	{
		eNoState,
		eDelayState,
		eDelayFinishState,
	};

public:	
	// Sets default values for this component's properties
	UWaypointComponent();

	// Called when the game starts
	void InitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FORCEINLINE bool IsLoop()					{ return bLoop; }
	FORCEINLINE void SetLoop(bool bSet)			{ bLoop = bSet; }
	FORCEINLINE float GetIgnoreMinDistance()	{ return IgnoreMinDistance; }
	FORCEINLINE void SetIgnoreMinDistance(float distance) { IgnoreMinDistance = distance; }
	
	void AddWaypoint(FVector pos, float fDelay = 0.0f);
	void ClearWaypoint();

	UFUNCTION(BlueprintCallable, Category = PhaseComponent)
	FVector GetNextWayPoint();
		
private:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	bool						bLoop = false;
	UPROPERTY()
	TArray< FWaypointInfo >		WayPointList;
	UPROPERTY()
	TArray< FWaypointInfo >		QueueWayPoint;

	UPROPERTY()
	FVector					LastWayPoint = FVector::ZeroVector;
	float					IgnoreMinDistance = 150.0f;
	float					DelayTime = 0.0f;
	eState					CurState = eNoState;	
};
