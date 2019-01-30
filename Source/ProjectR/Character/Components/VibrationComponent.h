// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "VibrationComponent.generated.h"

class UMeshComponent;
class UAnimStateComponent;
class ABasePlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UVibrationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVibrationComponent();

	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = VibrationComponent)
	void OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor);
	
	void ClearEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VibrationComponent)
	UCurveFloat*	CurveVibration = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VibrationComponent)
	bool			IsActivateComponent = true;


private:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;

	float					EllapsedTime = 0;
	float					DurationTime = 1.0f;
	float					TimeRatio = 1.0f;
	bool					IsVibration	= false;
	
	UPROPERTY()
	UMeshComponent*			MeshComponent = nullptr;
	
	UPROPERTY()
	UAnimStateComponent*	AnimStateComponent = nullptr;
	
	UPROPERTY()
	ABasePlayerController*  BasePlayerController = nullptr;

	UPROPERTY()
	FVector					InitLocation;
	
	UPROPERTY()
	FVector					ForwardVector;

};
