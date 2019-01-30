// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETrailWidthModeEnum : uint8
{
	VE_TCentre 		UMETA(DisplayName = "Centre") ,
	VE_First 		UMETA(DisplayName = "First"),
	VE_Second		UMETA(DisplayName = "Second")
};


/**
 * 
 */
UCLASS()
class PROJECTR_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
	
public:

	AWeaponBase( const FObjectInitializer& ObjectInitializer);
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	UStaticMeshComponent* WeaponMeshRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	UParticleSystemComponent*	TrailRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	float						TrailWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	ETrailWidthModeEnum			TrailWidthMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	FName						TrailFirstSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	FName						TrailSecondSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	UArrowComponent*			ShotPointRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Normal)
	TArray<UMaterialInstanceDynamic *>		OriginalMaterialList;

	/*
	Blueprint Callable Functions
	*/
	// Start trail effect
	UFUNCTION(BlueprintCallable, Category = Battle)
	void	StartTrail();

	// End trail effect
	UFUNCTION(BlueprintCallable, Category = Battle)
	void	EndTrail();

	UFUNCTION(BlueprintCallable, Category = Battle)
	void	SetCastShadow(bool Active);

	/*
	Overrided functions
	*/	
	void BeginPlay() override;
	void Destroyed() override;

	ETrailWidthMode GetEngineTrailWidthMode(ETrailWidthModeEnum GameTrailWidthMode);
	void SetVisibility(bool bVisible);

};
