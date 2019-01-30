// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Lobby/BaseLobbyStateActor.h"
#include "TowerLobbyGameMode.generated.h"

class ACameraActor;
class ALevelSequenceActor;
class UCameraComponent;
class ABaseLobbyStateActor;
class AOutdoorLobbyStateActor;
class AIndoorLobbyStateActor;


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETowerLobbyStateEnum : uint8
{
	None = 0, // at entry
	Indoor,
	Outdoor,
	Campaign,

	Max,
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETowerLobbyContentStateEnum : uint8
{
	LobbyMain = 0,
	Campaign,
	SingleRaid,
	HeroInventory,

	Max,
};

/**
 * TowerLobbyGameMode manages user facing interaction from Lobby_NewDesign_P and its sublevels such as
 * * transition effect between indoor and outdoor
 * * day and night / lighting scenarios
 * * (future spec) portrait mode outdoor, energy save mode
 *
 * touching, swiping, pinching, and possibly gyrosensor tilt are handled by LobbyStateActors
 * 
 */
UCLASS()
class PROJECTR_API ATowerLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATowerLobbyGameMode(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//// BP Config

	/** ETowerLobbyLighting::Max for no override */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	ETowerLobbyLighting LobbyLightingOverride = ETowerLobbyLighting::Max;

	//// Interface to the level script

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
		void SetupIndoorLobbyStateActor();

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
		void SetupOutdoorLobbyStateActor();

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void Initialize_PMap();

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void ChangeState(ETowerLobbyStateEnum InLobbyState, ETowerLobbySubstateEnum InLobbySubstate);

	UFUNCTION()
	void OnSubstateChanged();

	void SetVisibilityAll(bool IsVisible);

	UFUNCTION()
		void LobbyInLoadFinished();
	UFUNCTION()
		void LobbyInDayTimeLoadFinished();

	UFUNCTION()
		void ChangeUITowerRaid();

	/** Transition sequence implemented in BP Level Script */
	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void StartTransition();
	
	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void OnTransitionFinished();

	DECLARE_MULTICAST_DELEGATE(FOnStateChanged);
	FOnStateChanged OnStateChanged;

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void CampaignInTransition();
	
	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void OnCampaignInTransitionFinished();

	//// Entry sequence
	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void StartEntry();
	
	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	void OnEntryFinished();
	
	//// Level Actors

	// indexed by (int32)ETowerLobbyStateEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	TArray<ABaseLobbyStateActor*> LobbyStateActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
		AActor* IndoorOriginCoordinate;

	ABaseLobbyStateActor* GetLobbyStateActor(ETowerLobbyStateEnum InLobbyStateEnum);

	UFUNCTION(BlueprintCallable, Category = ATowerLobbyGameMode)
	ABaseLobbyStateActor* GetCurrentLobbyStateActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	ETowerLobbyStateEnum CurrentLobbyState = ETowerLobbyStateEnum::None;
	
	// For transition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	ETowerLobbyStateEnum PreviousLobbyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	ETowerLobbyLighting CurrentLighting = ETowerLobbyLighting::Day;

	DECLARE_MULTICAST_DELEGATE(FOnEnterInEntryCompleteDelegate)
	FOnEnterInEntryCompleteDelegate OnEnterInEntryCompleteDelegate;

	// UObject Interface
	bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	/** for requirement that entry into Lobby Scene involves Outdoor to Indoor transition, lets Level BP know that the transition is actually a part of entry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATowerLobbyGameMode)
	bool IsEntryTransition = false;

	UPROPERTY()
	AIndoorLobbyStateActor* IndoorLobbyStateAct;
	UPROPERTY()
	AOutdoorLobbyStateActor* OutdoorLobbyStateAct;

	UFUNCTION()
		void SetLastMessage(FString nick, FString message);

	bool fromOutdoor = false;

private:
	/*
	entry/transition helpers
	*/
	UFUNCTION()
	void OnEnterInEntryComplete();

	UFUNCTION()
	void FirstLightMapLoadedCompleted();

	ETowerLobbyLighting GetRandomLighting();

	//UFUNCTION()
	//void OnLeaveInTransitionFinished();
	UFUNCTION()
	void OnEnterInTransitionFinished();

	UFUNCTION()
	void OnFinishiedDelegate();
	
	UPROPERTY()
	AActor* m_TowerPivotCoordinate;

	UPROPERTY()
	ETowerLobbySubstateEnum LobbySubstate; // hero management , hero shop

public:
	UPROPERTY()
	ALevelSequenceActor* m_LobbyOut_Transition;
	UPROPERTY()
	ALevelSequenceActor* m_FirstLogin_Transition;
	UPROPERTY()
	ALevelSequenceActor* m_LobbyIn_Transition;
	UPROPERTY()
	ALevelSequenceActor* m_CampaignIn_Transition;
};
