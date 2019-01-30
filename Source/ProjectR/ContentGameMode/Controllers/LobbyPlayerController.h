// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SharedConstants/GlobalConstants.h"
#include "LobbyPlayerController.generated.h"

/*Delegates*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnOnePointInput, FVector2D, ScreenPosition, EInputEvent, InputType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnTwoPointInput, FVector2D, ScreenPosition1, FVector2D, ScreenPosition2, EInputEvent, InputType);

/**
 * 
 */
UCLASS()
class PROJECTR_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSecond) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION(BlueprintCallable)
	void CheckAndroidBack();
	UFUNCTION(BlueprintCallable)
	void OnQuitPopupOK();
	UFUNCTION(BlueprintCallable)
	void OnQuitPopupCancel();

	FORCEINLINE bool GetbTickTest() { return bTickTest; }
	FORCEINLINE void SetbTickTest(bool _input) { bTickTest = _input; }

	FORCEINLINE bool GetbPinchTest() { return bPinchTest; }
	FORCEINLINE void SetbPinchTest(bool _input) { bPinchTest = _input; }

	/** Input handlers. */
	void OnTapPressed(const FVector2D& ScreenPosition, float DownTime);
	void OnHoldPressed(const FVector2D& ScreenPosition, float DownTime);
	void OnHoldReleased(const FVector2D& ScreenPosition, float DownTime);
	void OnSwipeStarted(const FVector2D& AnchorPosition, float DownTime);
	void OnSwipeUpdate(const FVector2D& ScreenPosition, float DownTime);
	void OnSwipeReleased(const FVector2D& ScreenPosition, float DownTime);
	void OnSwipeTwoPointsStarted(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);
	void OnSwipeTwoPointsUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);
	void OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime);
	void OnPinchUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);
	void OnPinchReleased(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);

	//////////////////////////////////////////////////////////////////////////

	virtual FString ConsoleCommand(const FString& Command, bool bWriteToLog = true) override;

public:
	/*
	Delegates
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ALobbyPlayerController)
	FDelegate_OnOnePointInput	OnOnePointInput_Tap;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ALobbyPlayerController)
	FDelegate_OnOnePointInput	OnOnePointInput_Swipe;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ALobbyPlayerController)
	FDelegate_OnTwoPointInput	OnTwoPointInput_Pinch;

private:
	bool		bTickTest = false;
	bool		bPinchTest = false;

	float		GestureDelayTime = 0.0f;

	bool		Swiping = false;

	UPROPERTY()
	class UNormalTouchInput*	InputHandler = nullptr;
	UPROPERTY()
	class URGameInstance*		RGameInstance = nullptr;
	UPROPERTY()
	class UParticleSystem*		ClickParticle = nullptr;
};

