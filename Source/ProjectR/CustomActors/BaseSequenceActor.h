// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "SharedConstants/GlobalConstants.h"
#include "BaseSequenceActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_SequenceEvent, FName, SequenceKey);


UCLASS()
class PROJECTR_API ABaseSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSequenceActor(const FObjectInitializer& Init);

	// Called when the game starts or when spawned
	void BeginPlay() override;
	void BeginDestroy() override;

	// Event Delegate
	UFUNCTION()
	void OnLevelSequencePlayerPlay();
	UFUNCTION()
	void OnLevelSequencePlayerStop();
	UFUNCTION()
	void OnLevelSequencePlayerPause();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	void		Play(bool bLoop = false, bool bFirstFrame = true);

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	void		Stop();

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	void		Pause();

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	void		Resume();

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	bool		IsPlaying();

	UFUNCTION(BlueprintCallable, Category = ABaseSequenceActor)
	void		GotoEndStop();

public:
	UPROPERTY(BlueprintAssignable, Category = ABaseSequenceActor)
	FDelegate_SequenceEvent			OnSequenceEvent;

	UPROPERTY(BlueprintAssignable, Category = ABaseSequenceActor)
	FDelegate_SequenceEvent			OnSequenceEndEvent;

	UPROPERTY(BlueprintAssignable, Category = ABaseSequenceActor)
	FDelegate_SequenceEvent			OnSequencePauseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ABaseSequenceActor)
	FName							SequenceTableKey;

	UPROPERTY()
	ESequenceActorType				SequenceActorType = ESequenceActorType::VE_General;
};
