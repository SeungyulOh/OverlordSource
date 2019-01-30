// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ScrollEnvActor.generated.h"

/**
* used by prop BP_DarkLand_BG_ScrollWall in DarkLand_DungeonD_P
* The "scroll" in the name seems to mean elevator..
*/

UCLASS()
class PROJECTR_API AScrollEnvActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScrollEnvActor();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

		

	void PrepareScroll();	
	
	void StartScroll(float InDuration);
	void PlayMatinee();
	void PlaySequence();
	UFUNCTION(BlueprintCallable, Category = ScrollEnvActor)
	void BlendToMatineeCamera(bool InBlendToMatinee);

	void ScrollTick(float DeltaSeconds );
	void SwapScrollComponent();

	UFUNCTION(BlueprintCallable, Category = ScrollEnvActor)
	void GoToNextFloor(float InDuration);

	UFUNCTION()
	void OnSequenceEnd(FName name);



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	UPrimitiveComponent*				FirstGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	UPrimitiveComponent*				SecondGroup = nullptr;

		
	FVector				OriginFirstPos;	
	FVector				OriginSecondPos;	
	FVector				ScrollDistance;

	
	bool				bFirstStep = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	float				Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	FString				MatineeActorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	FString				MatineeCameraName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	float				BeforeDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollEnvActor)
	float				AfterDelay = 1.0f;

	bool				bActive = false;
	float				ElapsedScrollTime = 0.0f;


};
