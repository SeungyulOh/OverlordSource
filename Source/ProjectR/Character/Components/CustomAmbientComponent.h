// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CustomAmbientComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UCustomAmbientComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomAmbientComponent();

	// Called when the game starts
	void InitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = CustomAmbientcomponent)
	void			Active(float activeTime, float StartIntensity = 1.0f, FColor CustomLightColor = FColor::White, bool bIsForceColor = false);

	void			SetCustomAmbient( FColor InColor );

	FColor			GetModifiedColor(float InIntensity);

	// 리더 영웅이 공격하는 대상 하이라이트
	// When being targeted by Leader, assume TargetEffectLightColor, TargetEffectIntensity when not Active
	void EnterTargetEffect();
	void ExitTargetEffect();

private:
	bool			IsActive = false;
	float			ActiveTime = 0.5f;
	float			ElapsedActiveTime = 0.0f;
	UPROPERTY()
	FColor			LightColor = FColor::White;
	
	//test
	bool			IsForceActive = false;
	UPROPERTY()
	FColor			ForceLightColor = FColor::White;
	float			ForceActiveTime = 0.5f;
	float			ForceStartIntensity = 0.0f;

	bool UnderTargetEffect = false;
		
	UPROPERTY()
	TWeakObjectPtr< AActor >		OwnerActor;
};
