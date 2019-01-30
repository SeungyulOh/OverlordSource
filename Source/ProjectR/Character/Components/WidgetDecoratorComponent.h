// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "WidgetDecoratorComponent.generated.h"

class ABasePlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UWidgetDecoratorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetDecoratorComponent();

	void InitializeComponent() override;
	void UninitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateWidgetDecorator();
	void DeleteWidgetDecorator();

	void SetActiveWidgetDecorator(bool InActive);
	void RefreshWidgetZOrder(bool bForce = false);
	void RefreshWidgetPosition();
	void RefreshComponent();
	
	FORCEINLINE void	SetWidgetID(FName WidgetID) { WidgetDecoratorID = WidgetID; }
	FORCEINLINE void	SetCharacterCategory(ECharacterCategoryTypeEnum InCharacterCategory) { CharacterCategory = InCharacterCategory; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WidgetDecoratorComponent)
	FName			WidgetDecoratorID;	
	
protected:
	UPROPERTY()
	TWeakObjectPtr< UUserWidget >	WidgetDecorator;

	UPROPERTY()
	TWeakObjectPtr< AActor >		OwnerActor;

	UPROPERTY()
	bool							ActiveWidgetDecorator = false;
	UPROPERTY()
	ECharacterCategoryTypeEnum		CharacterCategory = ECharacterCategoryTypeEnum::VE_None;
	UPROPERTY()
	int32							WidgetZOrder = -9999;
	UPROPERTY()
	FVector2D						WidgetVisibleSizeMin = FVector2D::ZeroVector;
	UPROPERTY()
	FVector2D						WidgetVisibleSizeMax = FVector2D::ZeroVector;
	UPROPERTY()
	FVector2D						GameViewportSize = FVector2D::ZeroVector;
	UPROPERTY()
	FVector2D						GameResolution = FVector2D::ZeroVector;
	UPROPERTY()
	ABasePlayerController*			PlayerController = nullptr;

	bool Init = false;
};
