// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRGameMode.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "Network/RTSProtocol/rts.s2c_generated.h"
#include "RaidGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ARaidRGameMode : public AProjectRGameMode
{
	GENERATED_BODY()
public:	
	ARaidRGameMode(const FObjectInitializer& ObjectInitializer);
public:
	virtual void InitGameMode() override;
	virtual void PrepareGameMode() override;
	virtual void ReadyGameMode() override;
	virtual void StartGameMode() override;
	virtual void EndGameMode() override;
	virtual void ResultGameMode() override;
	virtual void StopGameMode() override;
	virtual void GamePlayModeEnd() override;
	virtual void ResultUI() override;
	virtual EStageContextEnum GetStageContext() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void OnCompleteResourcePreLoading();
// 	void StartCrewAI();
// 	void StopCrewAI();
	void OnACK_BT_ACTOR_SPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_SPAWNT* Packet);
	void OnACK_BT_CHANGE_CREW(FRTS_ACTOR_UPDATE* CrewInfo);
	UFUNCTION()
	void OnReadyToStart();

	void AttachBattleRecordManager();

	UFUNCTION()
	void OnBattleRecord_UpdateHP();
	// 	UFUNCTION()
// 	void OnSequenceEndColosseumStart(FName InSequenceKey);


	UPROPERTY()
	bool				ShowUI_PVP_VS = false;
	UPROPERTY()
	bool				ShowUI_PVP_Start = false;
	void OnACK_BT_DOOR_EVENT(uint16_t doorId, uint8 event);

	UPROPERTY()
		int32						MyGroupKey;
	UPROPERTY()
		int32						OtherGroupKey;
	UPROPERTY()
		int32						m_nMyHeroesMaxHP = 0;
	UPROPERTY()
		int32						m_nTargetHeroesMaxHP = 0;
	UPROPERTY()
		int32						m_nMyHeroesCurrentHP = 0;
	UPROPERTY()
		int32						m_nTargetHeroesCurrentHP = 0;


	/*Colosseum*/
	UPROPERTY()
	AActor* RightPosActor = nullptr;
	UPROPERTY()
	AActor* LeftPosActor = nullptr;
	UPROPERTY()
	AActor* MyLeader = nullptr;
	UPROPERTY()
	AActor* MyCrew1 = nullptr;
	UPROPERTY()
	AActor* MyCrew2 = nullptr;
	UPROPERTY()
	AActor* EnemyLeader = nullptr;
	UPROPERTY()
	AActor* EnemyCrew1 = nullptr;
	UPROPERTY()
	AActor* EnemyCrew2 = nullptr;
	//

};
