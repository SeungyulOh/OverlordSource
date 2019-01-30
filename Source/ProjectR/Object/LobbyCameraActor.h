// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LobbyCameraActor.generated.h"



UCLASS()
class PROJECTR_API ALobbyCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyCameraActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

public:
	void SetLobbyCharacter(FName InLobbyCharacterKey);
	void SetLobbyPostProcessSetting();
	void SetLobbyCharacterClickArea(FVector2D InLeftTop, FVector2D InRightBottom);
	void LobbyCameraPinch(float InPinchDistance);
	void RestoreLobbyCamera(float DeltaTime);
	void ResetLobbyCamera();

	//////////////////////////////////////////////////////////////////////////
	// Input

	bool IsInClickArea(FVector2D& CheckPosition);

	UFUNCTION()
	void OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType);
	UFUNCTION()
	void OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType);
	UFUNCTION()
	void OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType);

private:
	UPROPERTY()
	TWeakObjectPtr<UCameraComponent> CameraComponent = nullptr;

	struct FLobbyCharacterTableInfo* LobbyCharacterTableInfo;

	FVector2D ClickAreaLeftTop;
	FVector2D ClickAreaRightBottom;

	bool InputSwipe = false;
	FVector2D SwipeAnchor = FVector2D::ZeroVector;

	bool InputPinch = false;
	float MaxPinchToCameraDistance = 0.0f;
	float CurPinchToCameraDistance = 0.0f;
	float PrevPinchDistance = 0.0f;
	bool RestoringPinch = false;

	TWeakObjectPtr<class URGameInstance> RGameInstance;
};
