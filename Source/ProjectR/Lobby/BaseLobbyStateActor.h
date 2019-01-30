// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseLobbyStateActor.generated.h"

class UUserWidget;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETowerLobbySubstateEnum : uint8
{
	None = 0, // at entry
	NormalView,
	PortraitView, // only for Outdoor
	HeroManagementView, // only for Indoor
	HeroShop, // No transition In,Out 
	RaidTowerBoss,
	Max,
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETowerLobbyLighting : uint8
{
	Day = 0,
	Night,

	Max,
};

/*
 * Represents state/location of the lobby (outside, inside)
 * currently coincides with sublevel...
*/
UCLASS()
class PROJECTR_API ABaseLobbyStateActor : public AActor {

	GENERATED_BODY()

public:
	// Substate
	UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
	ETowerLobbySubstateEnum GetCurrentSubstate() { return CurrentSubstate; }
	UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
	virtual void ChangeState(ETowerLobbySubstateEnum InSubstate) { };
	
	DECLARE_MULTICAST_DELEGATE(FOnStateChanged);
	FOnStateChanged OnStateChanged;

	/** starts loading sublevel. sublevels are loaded to invisible state for controlling lighting scenario */
	virtual void EnterSublevel();
	/** unloads sublevel */
	virtual void LeaveSublevel();

	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveEnterSublevel();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveLeaveSublevel();

	////Callback
	UFUNCTION()
	void OnLeaveSublevelComplete();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterSublevelComplete);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = ABaseLobbyStateActor)
	FOnEnterSublevelComplete OnEnterSublevelComplete;

	// called after transition animation
	virtual void BeginSublevel();
	virtual void EndSublevel();
	
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveBeginSublevel();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveEndSublevel();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginSublevelComplete);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = ABaseLobbyStateActor)
	FOnBeginSublevelComplete OnBeginSublevelComplete;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSublevelComplete);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = ABaseLobbyStateActor)
	FOnEndSublevelComplete OnEndSublevelComplete;
// 	
// 
// 	// Cannot change this while the lobby state actor is active
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ABaseLobbyStateActor)
// 	ETowerLobbyLighting CurrentLighting = ETowerLobbyLighting::Day;
	
	UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
		virtual void SetVisibilityAll(bool IsVisible);

	//temp latentactioninfo instantiate
	// < executionFunction : function name after callback
	// < callbackTarget : target object
	// > initialize FLatentActionInfo
	UFUNCTION()
		FLatentActionInfo InitLatentActionInfo(FName executionFunction, UObject* callbackTarget, int32 uid, int32 linkage);


protected:
	ETowerLobbySubstateEnum CurrentSubstate = ETowerLobbySubstateEnum::None;

	// shims

	UFUNCTION()
	void Nop();

};
