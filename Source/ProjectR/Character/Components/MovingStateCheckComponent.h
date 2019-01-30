// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MovingStateCheckComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UMovingStateCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovingStateCheckComponent();

	// Called when the game starts
	void InitializeComponent() override;
		
		
	void		StartMove();
	bool		StopMove();
	void		ResetIgnoreFlag();
	
	void		CheckMovingState();

	UFUNCTION(BlueprintCallable, Category = MovingStateCheckComponent)
	bool		AvoidOtherCharacter();

private:
	UPROPERTY()
	FTimerHandle				IgnoreNavimeshUpdateTimer;
	
	UPROPERTY()
	bool						IngnoringNavimeshUpdate = false;
	
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	
	UPROPERTY()
	bool						IsMoving = false;

	
};
