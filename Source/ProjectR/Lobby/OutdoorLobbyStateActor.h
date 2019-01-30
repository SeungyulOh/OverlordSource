// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BaseLobbyStateActor.h"
#include "OutdoorLobbyStateActor.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EOutdoorLobbyStateEnum : uint8
{
	NormalView = 0,
	PortraitMode,

	Max,
};

/**
*/
UCLASS()
class PROJECTR_API AOutdoorLobbyStateActor : public ABaseLobbyStateActor
{
	GENERATED_BODY()
public:
	void EnterSublevel() override;
	UFUNCTION()
	void LeaveSublevel() override;


	void BeginSublevel() override;
	void EndSublevel() override;

	//// Interface

	////Callback
	UFUNCTION()
	void LoadLobbyOutLevelCompleted();

	////

	UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
	virtual void SetVisibilityAll(bool IsVisible);

	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void ChangeState(ETowerLobbySubstateEnum InSubstate) override;

	// WIP
	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void GoToPortraitMode();
	//UFUNCTION(BlueprintImplementableEvent, Category = AOutdoorLobbyStateActor)
	//void ReceiveOnGoToPortraitMode();
	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void GoToPortraitModeComplete();
	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void GoToNormalView();
	//UFUNCTION(BlueprintImplementableEvent, Category = AOutdoorLobbyStateActor)
	//void ReceiveOnGoToNormalView();
	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void GoToNormalViewComplete();

	/*
	* Touch interaction
	* * swipe horizontally to rotate tower
	* * swipe vertically to scroll tower
	* * pinch to zoom?
	* * also tilt camera from gyrosensor input
	*/

	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void RotateTower(float YawDelta);

	// Vertical
	UFUNCTION(BlueprintCallable, Category = AOutdoorLobbyStateActor)
	void ScrollTower(float HeightDelta);

	// > Day : Lobby_Out_DayTime_Ls, Night : Lobby_Out_NightTime_Ls
	UFUNCTION()
		FName GetCurrentLightingInFName();

	//// Level BP Config

	/** for RotateTower */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
	FVector TowerPivotLocation;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
	//float MaxAbsOrbitYaw = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
	TWeakObjectPtr<ACameraActor> OutdoorCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
	TWeakObjectPtr<ACameraActor> OutdoorPortraitModeCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
		TWeakObjectPtr<ACameraActor> CameraActor;


private:


	EOutdoorLobbyStateEnum CurrentOutdoorLobbyState = EOutdoorLobbyStateEnum::NormalView;

	void UpdateOutdoorCameraTransform();

	void ResetTouchInteraction();

	//TWeakObjectPtr<AActor> TowerActor;
	
	float CurrentCameraOrbitYaw = 0.0f;

	// Saved
	FTransform DefaultOutdoorCameraTransform;
};
