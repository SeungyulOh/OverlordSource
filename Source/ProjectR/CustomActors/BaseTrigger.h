// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_TriggerEnter, ABaseTrigger*, Trigger);

UCLASS()
class PROJECTR_API ABaseTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseTrigger)
	UShapeComponent*		CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseTrigger)
	FName					TriggerID;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = URaidListScrollItem)
	FDelegate_TriggerEnter	OnTriggerEnter_Delegate;

	// Sets default values for this actor's properties
	ABaseTrigger(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int32 GetTriggerID();


};
