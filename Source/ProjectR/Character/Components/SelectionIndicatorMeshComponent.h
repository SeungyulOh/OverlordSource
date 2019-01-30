// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
//#include "CharacterDecoratorComponentInterface.h"
#include "SelectionIndicatorMeshComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTR_API USelectionIndicatorMeshComponent : public UStaticMeshComponent//, public ICharacterDecoratorComponentInterface
{
	GENERATED_BODY()
	
public:
	void	SetOwner(AActor* TargetActor);
	void	SetScaleFactor(float InScaleFactor);	

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerActor;
	
	UPROPERTY()
	float						MeshScaleFactor = 1.0f;
};
