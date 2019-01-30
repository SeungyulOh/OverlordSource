// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "CustomWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UCustomWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomWidgetComponent();

	// Called when the game starts
	void BeginPlay() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OnRegister() override;
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void SetWidgetSpace(bool bIsWorld);
	void SetBlendMode(EWidgetBlendMode InBlendMode);
	void SetWidgetAngleFromCameraView();
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomWidgetComponent)
	FName			WidgetDecoratorID;	

	UPROPERTY()
	UMaterialInterface* TranslucentMaterial_OneSided_NoDepth;

private:
	void			SetWidgetPos();
	void			CustomUpdateMaterialInstance();

private:
	UPROPERTY()
	TWeakObjectPtr< UUserWidget >	WidgetDecorator;
	UPROPERTY()
	TWeakObjectPtr< AActor >		OwnerActor;
	UPROPERTY()
	bool			bHasTranslucentDepthTest = false;
	UPROPERTY()
	float			UpdateCheck = 0;
};
