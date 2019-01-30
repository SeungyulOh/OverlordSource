// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"

#include "Engine/TriggerBox.h"

#include "EventTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOverlapEventTrigger, int32, Int);

UCLASS()
class PROJECTR_API AEventTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:	
	AEventTrigger(const FObjectInitializer& ObjectInitializer);
	~AEventTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AEventTrigger)
	int32							ActorID = 0;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = AEventTrigger)
	FDelegate_OnOverlapEventTrigger	OnOverlapEvent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
