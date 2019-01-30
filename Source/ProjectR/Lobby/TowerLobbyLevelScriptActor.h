// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Network/PacketDataStructures.h"
#include "TowerLobbyLevelScriptActor.generated.h"



UCLASS()
class PROJECTR_API ALobbyOutScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OutDoor")
	AActor*		LobbyOutDoorCampaignObj;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OutDoor")
	AActor*		LobbyOutDoorPVPObj;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OutDoor")
	AActor*		LobbyOutDoorDungeonObj;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OutDoor")
	AActor*		LobbyOutDoorWorldBossObj;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OutDoor")
	AActor*		LobbyOutDoorRaidObj;

};


/**
 * 
 */
UCLASS()
class PROJECTR_API ATowerLobbyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()


	
public:
	virtual void BeginPlay() override;
	
public:
	/*Hero Shop*/
	UFUNCTION(BlueprintCallable, Category = "Level Sequence")
	void OnSpawnGacha();

	UFUNCTION(BlueprintCallable, Category = "Level Sequence")
	void PauseSequence();

	UFUNCTION(BlueprintCallable, Category = "Level Sequence")
	void BGParticleActivate();
	
	UFUNCTION()
	void EndProcess_ShopHero();

	UPROPERTY()
	TArray<FHERO> GachaHeroList;

	UPROPERTY()
	class ARHeroGacha* HeroGachaActor;
	
private:
	bool	bDisableEvent = false;
public:
	FORCEINLINE void SetDiableEvent(bool InBool) { bDisableEvent = InBool; }
	/////////////////////////Lobby

public:
	UFUNCTION(BlueprintCallable, Category = "Level Sequence")
	void TransitionStart();

	UFUNCTION(BlueprintCallable, Category = "Level Sequence")
	void TransitionFinished();


	/*Load/UnLoad Light*/
	UFUNCTION()
	void UnLoadLightCompleted();
	UFUNCTION()
	void LoadLightCompleted();
};
