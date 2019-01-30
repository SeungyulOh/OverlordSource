// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "IGameState.generated.h"


class URGameInstance;
/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UIGameState : public UObject
{
	GENERATED_BODY()
		
public:	
	UIGameState(){};
	virtual ~UIGameState() {};
	
	FORCEINLINE void SetGameInstance(URGameInstance* InGameInstance){ GameInstance = InGameInstance; }

	virtual		void Enter(){}
	virtual		void Leave(){}
	virtual		void BeginState();
	virtual		void ComplateState();
	virtual		void Tick(float DeltaTime) {}
	virtual		void Refresh() {}

	//AGameState
	FORCEINLINE EGameStateEnum	GetStateType(){ return StateType;  }
	double GetStartMainTime() { return StartMainTime; }

protected:
	UPROPERTY()
	EGameStateEnum		StateType = EGameStateEnum::VE_None;

	UPROPERTY()
	URGameInstance*		GameInstance = nullptr;						// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..

	double			StartMainTime = 0.0;
};
