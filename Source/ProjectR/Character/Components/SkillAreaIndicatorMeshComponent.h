// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterDecoratorComponentInterface.h"
#include "SkillAreaIndicatorMeshComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTR_API USkillAreaIndicatorMeshComponent : public UStaticMeshComponent, public ICharacterDecoratorComponentInterface
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable, Category = CharacterDecorator )
	void	SetOwner(AActor* TargetActor) override;


	UFUNCTION(BlueprintCallable, Category = CharacterDecorator)
	void	ShowSkillArea(float InRangeX, float InRangeY);

	void	SetLocationOriginal();
	void	SetSkillAreaMeshType(ESkillAreaMeshTypeEnum InSkillAreaMeshType);
	void	SetTintColor(FLinearColor InColor);
	
	void	SkillAreaMeshUpdate(FVector& InWorldPos);
	void	ShowAreaSkillTarget(const FVector& InTargetLocation);
	void	ClearAreaSkillTarget();
	FORCEINLINE ESkillAreaMeshTypeEnum GetSkillAreaMeshType() { return SkillAreaMeshType; }

	void	SetShapeType(ESearchShapeTypeEnum InType, float InShapeParam);
	FORCEINLINE ESearchShapeTypeEnum	GetSkillShapeType() { return SkillShapeType; }

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >				OwnerActor;
	UPROPERTY()
	TArray< TWeakObjectPtr< AActor > >		Prev_TargetActors;
	UPROPERTY()
	UMaterialInstanceDynamic*				SelectionMaterialInstance = nullptr;
	UPROPERTY()
	ESkillAreaMeshTypeEnum					SkillAreaMeshType = ESkillAreaMeshTypeEnum::VE_None;
	UPROPERTY()
	ESearchShapeTypeEnum					SkillShapeType = ESearchShapeTypeEnum::VE_None;
	UPROPERTY()
	float									SkillShapeParam = 0.0f;
	
};
