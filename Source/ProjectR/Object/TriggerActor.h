// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "TriggerActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnOverlapEvent, int32, Int);


UCLASS()
class PROJECTR_API ATriggerActor : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerActor(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATriggerActor)
	int32							ActorID = 0;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ATriggerActor)
	FDelegate_OnOverlapEvent	OnOverlapEvent;

	void			SetBoxExtent(FVector extent);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
