// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "UC_HeroTouchInteractionPanel.generated.h"

class AIndoorLobbyStateActor;
class ALobbyPlayerController;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroTouchInteractionPanel : public URBaseWidget
{
	GENERATED_BODY()	
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;


	//// Delegates
	
	UFUNCTION()
	void OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType);
	UFUNCTION()
	void OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType);
	UFUNCTION()
	void OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType);

private:

	UPROPERTY()
	TWeakObjectPtr<AIndoorLobbyStateActor> IndoorLobbyStateActor;
	UPROPERTY()
	TWeakObjectPtr<ALobbyPlayerController> LobbyPlayerController;

	// helpers
	bool IsInHeroClickArea(const FVector2D& Position);

	// Saved values
	FVector2D HeroClickAreaLeftTop = FVector2D::ZeroVector;
	FVector2D HeroClickAreaRightBottom = FVector2D::ZeroVector;

	// state variables
	bool Swiping = false;
	FVector2D PrevScreenPosition = FVector2D::ZeroVector;
	float SwipeScreenPositionToYawMultiplier = -0.5f;

	bool Pinching = false;
	float PrevPinchingDistance = 0.0f;
	float PinchScreenDistanceToAlphaMultiplier = 0.005f;
};
