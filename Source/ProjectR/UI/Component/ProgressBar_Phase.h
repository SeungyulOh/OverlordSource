// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Components/ProgressBar.h"
#include "ProgressBar_Phase.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UProgressBar_Phase : public UProgressBar
{
	GENERATED_BODY()
	
public:
	UProgressBar_Phase(const FObjectInitializer& ObjectInitializer);
	void RemoveFromParent() override;

	void InitPhaseList();

	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	void SetTargetActor(AActor* InTargetActor, EProgressTypeEnum InProgressType = EProgressTypeEnum::VE_None);
	
	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	void SetTargetActorList(TArray<AActor*> InTargetActors, EProgressTypeEnum InProgressType = EProgressTypeEnum::VE_None);

	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	void SetNeedle(USlider* InNeedle);

	// ProgressBar Percent Binding
	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	float GetPhasePercent();

	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	void ChangeProgressBarTint();
	
	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	void SetCustomPhaseColor(TArray<FLinearColor> InPhaseColor);

	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	int32 GetCurrentPhase();

	UFUNCTION(BlueprintCallable, Category = ProgressPhase)
	int32 GetCurrentHP();

public:
	/*
	override
	*/
	// UWidget interface
	void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	// Begin UObject interface
	void PostLoad() override;
	// End of UObject interface

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Style)
	FSlateBrush				PursuerImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProgressPhase)
	float					HPDecreaseValue = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProgressPhase)
	TArray<FLinearColor>	PhaseColor;

private:
	void ChangeProgressBarTintLayer1();
	void ChangeProgressBarTintLayer2();
	
	int32 GetSingleTargetCurrentHP();
	int32 GetMultiTargetCurrentHP();

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >	TargetActor;
	UPROPERTY()
	TArray<AActor*>				TargetActorsList;

	int32			HPPhaseValue = 1000;
	int32			MaxPhase;
	int32			MaxHP;

	int32			CurrentPhase;
	int32			CurrentHP;
	int32			PrevHP;

	float			CurrentPercent;
	float			PrevPercent;
	
	UPROPERTY()
	TArray<int32>	PhaseList;

	UPROPERTY()
	FColor Blue = FColor(0, 0, 255);
	UPROPERTY()
	FColor Cyan = FColor(0, 128, 255);
	UPROPERTY()
	FColor Orange = FColor(255, 128, 0);
	UPROPERTY()
	FColor Red = FColor(255, 0, 0);
	UPROPERTY()
	FColor Black = FColor(0, 0, 0);

	//boss color
	UPROPERTY()
	FColor Purple = FColor(80, 0, 80);
	//const FColor Blue = FColor(0, 0, 255);
	UPROPERTY()
	FColor Green	= FColor::Green;
	UPROPERTY()
	FColor Yellow = FColor::Yellow;
	//const FColor Red = FColor(255, 0, 0);
	
	UPROPERTY()
	USlider*				HPNeedle = nullptr;
	
	UPROPERTY()
	EProgressTypeEnum		ProgressType = EProgressTypeEnum::VE_None;

};
