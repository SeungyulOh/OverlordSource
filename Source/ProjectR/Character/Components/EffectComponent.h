// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "EffectComponent.generated.h"

class UCustomAmbientComponent;
class UCustomChangeMaterialComponent;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEffectTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_AppearEffect			UMETA(DisplayName = "AppearEffect"),
	VE_DisappearEffect		UMETA(DisplayName = "DisappearEffect"),
	VE_ChangeOpacity		UMETA(DisplayName = "ChangeOpacity"),
	VE_HitHightlight		UMETA(DisplayName = "HitHightlight"),
	VE_LightFlicker			UMETA(DisplayName = "LightFlicker"),

	VE_Max,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UEffectComponent();

	// Called when the game starts
	void InitializeComponent() override;
	void UninitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void AppearEffectStart(float InDuration);
	void DisappearEffectStart(float InDuration);
	void HitHightlightStart();

	UFUNCTION(BlueprintCallable, Category = UEffectComponent)
	void ChangeOpacity(float Opacity);

	UFUNCTION(BlueprintCallable, Category = UEffectComponent)
	void OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = UEffectComponent)
	void SetProperty(float InInterval, float InDuration);

	UFUNCTION(BlueprintCallable, Category = UEffectComponent)
	void LightFlickerStart();

private:
	void SetAppearEffectMaterialParam(float ProgressRatio);
	void SetDisappearEffectMaterialParam(float ProgressRatio);
	void ApplyScalarMaterialParam(TArray< UMaterialInterface* > InMaterialInstances, FName ParamName, float InParam);

	void AppearEffectTick(float DeltaTime);
	void DisappearEffectTick(float DeltaTime);
	void LightFlickerTick(float DeltaTime);


	//CustomAmbientComponent
	void SetCustomAmbient(FColor InColor);
	FColor GetModifiedColor(float InIntensity);

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >		OwnerActor;

	UPROPERTY()
	TWeakObjectPtr< UCustomAmbientComponent >			CustomAmbientComponent;

	UPROPERTY()
	TWeakObjectPtr< UCustomChangeMaterialComponent >	CustomChangeMaterialComponent;

	UPROPERTY()
	TWeakObjectPtr< UParticleSystemComponent >			SpawnEffect;

	UPROPERTY()
	UMaterialInstance*									OpacityMaterialInstance = nullptr;

	//UPROPERTY()
	//FVectorParameterValue								DisappearVectorParamValue;
	//bool												OnDisappearVectorParamValue = false;;

	EEffectTypeEnum					CurEffectType = EEffectTypeEnum::VE_None;

	bool			bActive = false;
	float			Duration;
	float			ElapsedTime;

	float			FlickingInterval = 0.1f;
	float			FlickingDuration = 0.5f;

	//HitHightlight
	float			HitHightlightDuration = 0.1f;
	FColor			HitHightlightColor = FColor::White;
	FColor			HitCriticalHightlightColor = FColor::Red;


	UPROPERTY()
	FVector			MaxHeight;
	UPROPERTY()
	FVector			BaseHeight;

	bool			bHitHight = false;

	UPROPERTY()
	FTimerHandle	EffectActiveHandler;

	UPROPERTY()
	FTimerHandle	MaterialActiveHandler;
};
