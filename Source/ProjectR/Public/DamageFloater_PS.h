// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharedGlobalConstants.h"
#include "DamageFloater_PS.generated.h"




UCLASS()
class PROJECTR_API UDamageFloaterManager : public UObject
{
	GENERATED_BODY()

public:
	UDamageFloaterManager();

	void Init();

	UFUNCTION()
	TArray<UTexture2D*>& GetCorrespondingNumberArray(EBattleDamageResultEnum DamageResultType, bool isEnemy);
	UFUNCTION()
	UTexture2D* GetCorrespondingTextTexture(EBattleDamageResultEnum DamageResultType);

	UFUNCTION()
	UParticleSystem* GetCorrespondingPS(EBattleDamageResultEnum DamageResultType, bool isEnemy);

public:
	UPROPERTY()
	TArray<UTexture2D*> WhiteSpriteArray;
	UPROPERTY()
	TArray<UTexture2D*> YelloSpriteArray;
	UPROPERTY()
	TArray<UTexture2D*> RedSpriteArray;
	UPROPERTY()
	TArray<UTexture2D*> GreenSpriteArray;
	UPROPERTY()
	UTexture2D* MissSprite;
	UPROPERTY()
	UTexture2D* CritSprite;

	UPROPERTY()
	UParticleSystem* WhitePS;
	UPROPERTY()
	UParticleSystem* YelloPS;
	UPROPERTY()
	UParticleSystem* RedPS;
	UPROPERTY()
	UParticleSystem* GreenPS;
};


UCLASS()
class PROJECTR_API ADamageFloater_PS : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageFloater_PS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(AActor* InFromActor, AActor* InOwnerActor , float Damage , EBattleDamageResultEnum DamageResultType);	
	
	UFUNCTION()
	void DestroyThisActor(UParticleSystemComponent* PSC);

public:
	UPROPERTY()
	USceneComponent* RootComp;

	UPROPERTY()
	UParticleSystemComponent* DamagePSC;
	UPROPERTY()
	UParticleSystem* SmokePS;

	UPROPERTY()
	TWeakObjectPtr<AActor> FromActor;
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;
	UPROPERTY()
	int32	FinalDamage;
	

	UPROPERTY()
	TArray<UTexture2D*> NumberArray;
	UPROPERTY()
	UTexture2D*	TextTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFloaterSpeed")
	int32 base = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFloaterSpeed")
	float InitialPowerOffSet = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFloaterSpeed | Deceleration")
	float Coefficient = 4;

private:
	UPROPERTY()
	TArray<int32> DamageNumberArray;

	FVector InitialPowerDirection;
	float InitialPowerIntensity;
	float InitialUpPowerIntensity;
	float decelerationIntensity;

	EBattleDamageResultEnum DamageResultEnum;
	
	
};