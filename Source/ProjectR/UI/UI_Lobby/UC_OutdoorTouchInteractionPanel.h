// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "UC_OutdoorTouchInteractionPanel.generated.h"

class AOutdoorLobbyStateActor;
class ALobbyPlayerController;
class UUP_Lobby_Outdoor;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_OutdoorTouchInteractionPanel : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_OutdoorTouchInteractionPanel)
	bool PortraitMode = false;

	//// Delegates
	
	UFUNCTION()
	void OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType);
	UFUNCTION()
	void OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType);
	//UFUNCTION()
	//void OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType);

private:

	UPROPERTY()
	TWeakObjectPtr<AOutdoorLobbyStateActor> OutdoorLobbyStateActor;
	UPROPERTY()
	TWeakObjectPtr<ALobbyPlayerController> LobbyPlayerController;
	UPROPERTY()
	//TWeakObjectPtr<UUP_Lobby_Outdoor> OutdoorLobbyUMG;
	UUP_Lobby_Outdoor* OutdoorLobbyUMG;

	// helpers
	bool IsInClickArea(const FVector2D& Position);

	// Saved values
	FVector2D ClickAreaLeftTop = FVector2D::ZeroVector;
	FVector2D ClickAreaRightBottom = FVector2D::ZeroVector;

	// state variables
	bool Swiping = false;
	FVector2D PrevScreenPosition = FVector2D::ZeroVector;
	float SwipeScreenPositionToYawMultiplier = 0.2f;

	bool Pinching = false;
	//float PrevPinchingDistance = 0.0f;
	//float PinchScreenDistanceToAlphaMultiplier = 0.005f;
};
