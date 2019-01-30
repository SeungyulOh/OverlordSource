// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLobbyStateActor.h"
#include "IndoorLobbyStateActor.generated.h"

class ALobbyCharacter;
class ABattleBaseCharacter;
class AWeaponBase;

/**
*/
UCLASS()
class PROJECTR_API AIndoorLobbyStateActor : public ABaseLobbyStateActor
{
	GENERATED_BODY()
public:

	void EnterSublevel() override;
	void LeaveSublevel() override;

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveEnterSublevel();
	UFUNCTION(BlueprintImplementableEvent, Category = ABaseLobbyStateActor)
	void ReceiveLeaveSublevel();
	
	//UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
	//	void OnReceiveEnterSublevelComplete();

	void BeginSublevel() override;
	void EndSublevel() override;

	////Callback
	

	UFUNCTION()
	void LoadLobbyInLevelCompleted();

	//// Level BP Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIndoorLobbyStateActor)
	FName HeroNameOverride = NAME_None;

	// indoor origin
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIndoorLobbyStateActor)
	FVector IndoorOriginLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIndoorLobbyStateActor)
	TWeakObjectPtr<ACameraActor> IndoorNormalViewCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIndoorLobbyStateActor)
	TWeakObjectPtr<ACameraActor> IndoorHeroManagementViewCameraActor;
	
	//// Interface

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ChangeState(ETowerLobbySubstateEnum InSubstate) override;
	
	//// Hero

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ChangeToRandomHero();

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ChangeToHeroName(FName HeroName);
	
	// Show hero preset by Blueprint
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ChangeHero_LobbyCharacterTable(FName HeroName);

	// For displaying hero owned by user
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ChangeHero_Owned(FString InHeroUD);

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void PlayHeroLobbySocialAnimation();

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void PlayHeroTouchActionAnimationAndSequence();


	/*
	* Touch interaction.
	* * swipe to rotate
	* * pinch to zoom
	*/
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void RotateHero(float YawDelta);

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ZoomInToHero(float LerpAlphaDelta);

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void ResetHeroTouchInteraction();
	
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void HeroTapAction();

	//// State variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIndoorLobbyStateActor)
	//ALobbyCharacter* CurrentLobbyCharacter = nullptr;
	AActor* CurrentLobbyCharacter = nullptr;
	UPROPERTY()
	AActor* CurrentRWeapon = nullptr;
	UPROPERTY()
	AActor* CurrentLWeapon = nullptr;
	FText heroName;
// 	FText heroNickName;
	FString CurrentHeroUD;
	UPROPERTY()
	UPaperSprite*	heroSprite;
	int32			heroGrade = 0;
	
	FName CurrentHeroName = NAME_None;
	////
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void GoToHeroInventoryView();

	//UFUNCTION(BlueprintImplementableEvent, Category = AIndoorLobbyStateActor)
	//void ReceiveGoToHeroInventoryView();

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void GoToHeroInventoryViewComplete();
	
	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void GoToNormalView();

	//UFUNCTION(BlueprintImplementableEvent, Category = AIndoorLobbyStateActor)
	//void ReceiveGoToNormalView();

	UFUNCTION(BlueprintCallable, Category = AIndoorLobbyStateActor)
	void GoToNormalViewComplete();

	////
	UFUNCTION(BlueprintCallable, Category = ABaseLobbyStateActor)
		virtual void SetVisibilityAll(bool IsVisible);

	void SetVisibilityHero(bool IsVisible);
	void DestroyCurrentLobbyCharacter();
	//// Delegate functions

	UFUNCTION()
	void LoadLobbyHeroCharacter(FName HeroName, TSoftClassPtr<ABattleBaseCharacter> LobbyHeroClass, TSoftClassPtr<AWeaponBase> RWeaponType, TSoftClassPtr<AWeaponBase> LWeaponType);
	// Day : Lobby_Out_DayTime_Ls, Night : Lobby_Out_NightTime_Ls
	UFUNCTION()
		FName GetCurrentLightingInFName();

private:

	void ApplyIndoorHeroManagementViewTableCamera();
	void ApplyIndoorHeroManagementViewTableHero();
	//void RevertFromIndoorHeroManagementViewTable();

	void ApplyLobbyCharacterViewTableCamera();

	void SetCurrentLobbyHeroInfo();
	void SetLobbyHeroInfo(FString& InHeroUD);

	// shim helper
	// from "26006" to "6"
	static FName HeroIDToLobbyCharacterTableIndex(FName HeroID);

	float LobbyCharacterYawBase = 0.0f; // Kept because for some reason heros have varying yaws about 90 degrees

	float CurrentZoomInLerpAlpha = 0.0f;

	void UpdateNormalViewCameraTransform();

	TSharedPtr<FStreamableHandle> CharacterLoadRequestHandle;
	void ClearCharacterLoadRequest();

	

	FVector DefaultCameraLocation;
	FRotator DefaultCameraRotation;

	FVector ZoomInCameraLocation;
	FRotator ZoomInCameraRotation;

	FName PlayingLobbyTouchSequenceName = NAME_None;
};
