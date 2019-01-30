// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Components/SkillCastingDecalComponent.h"
#include "SkillCastingDecal.generated.h"

class UDecalComponent;

/**
 * Progresses SkillRangeProgress param of the decal material
 */
UCLASS()
class PROJECTR_API ASkillCastingDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillCastingDecal();

	void OnConstruction(const FTransform& Transform) override;

	//// Interface

	UFUNCTION(BlueprintCallable, Category = ASkillCastingDecal)
	void StartAnimation(float InDuration);

	//// BP config

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	TEnumAsByte<EEasingFunc::Type> InterpFunction = EEasingFunc::Type::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	float InterpBlendExp = 2.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	ESkillDecalColorType SkillDecalColorType = ESkillDecalColorType::VE_HeroOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	FLinearColor DecalColor_HeroOwner = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	FLinearColor DecalColor_Npc_Enemy = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	FLinearColor DecalColor_Npc_Friend = FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	FLinearColor DecalColor_Npc_Akhan = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	FLinearColor DecalColor_Npc_Gigant = FLinearColor::Blue;

	//// BP testing purposes
	
	// enabled and loops
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	bool bTesting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ASkillCastingDecal)
	float Duration = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:

	float CurrentAlpha = 0.0f;

	bool bCurrentlyProgressing = false;
};
