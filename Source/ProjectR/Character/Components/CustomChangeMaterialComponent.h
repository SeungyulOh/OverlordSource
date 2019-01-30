// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CustomChangeMaterialComponent.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UCustomChangeMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomChangeMaterialComponent();

	// Called when the game starts
	void InitializeComponent() override;
		

	UFUNCTION(BlueprintCallable, Category = CustomChangeMaterialComponent)
	void			Active(bool InActive, UMaterialInterface* InCustomMaterial);
	
	void			ActivateDeathMaterials();

	void			ChangeOpacity(float Opacity);
	void			SetCustomMaterial(UMaterialInterface* InCustomMaterial);

private:
	// Restore to original materials
	void			ResetMaterials();

	// Returns Death effect material corresponding to the given material if any, null otherwise
	// Synchronous load
	UMaterial*		GetDeathMaterial(UMaterial* InMaterial);
	
	// Helper: Character material Path to corresponding death material
	// ex. /Game/Shaders/Character/PC/PC_Hair_MAT.PC_Hair_MAT => /Game/Shaders/Character/PC/Death/PC_Hair_MAT.PC_Hair_MAT
	static bool			GetDeathMaterialPath(const FString& InPath, FString& OutPath);

	void			ApplyWeaponImp(AWeaponBase* Weapon, UMaterialInterface* InCustomMaterial);
	//void			ApplyStaticMeshCompImp(UStaticMeshComponent* InTargetMesh, TArray<UMaterialInstanceDynamic *> InOriginArray, UMaterial* InCustomMaterial);
	void			ApplySkeletalMeshCompImp(USkeletalMeshComponent* InTargetMesh, TArray< UMaterialInstanceDynamic* > InOriginArray, UMaterialInterface* InCustomMaterial);
	
	// Like ApplySkeletalMeshCompImp but applies materials death-corresponding to the original materials
	void			ApplySkeletalMeshCompDeathMaterials(USkeletalMeshComponent* InTargetMesh, TArray< UMaterialInstanceDynamic* > InOriginArray);


	// MaterialScalar
public:

	void			SetCustomMaterialScalarParameterValue(FName ParamName, float Value);
	void			ApplyCustomMaterialScalarParameterValue(TArray<UMaterialInterface*> MaterialList, FName ParamName, float Value);


private:
	bool			IsActive = false;
	float			ActiveTime = 0.5f;
	float			ElapsedActiveTime = 0.0f;

	TWeakObjectPtr< AActor >				OwnerActor;
	//TArray<UMaterialInterface*>				OriginalMaterials;
	TWeakObjectPtr< UMaterialInterface >	CurrentCustomMaterial;
};
