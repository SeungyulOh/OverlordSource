// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "UI/UIDefs.h"
#include "GamePlayFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UGamePlayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*
	Game mode
	*/
	UFUNCTION(BlueprintCallable, Category = GamePlayFunctionLibrary)
	static void					PauseInGamePlayTimer(bool InbPause);


	/*
	Stage
	*/
	
	static ECharacterCrewStateEnum	IndexToCrewState(int32 index);
	static ECharacterRoleEnum		IndexToRoleEnum(int32 index);
	static bool IsValidDeckHeroUDs(const TArray<FString>& heroUDs);
	static void RequestGameStart(FPlay_Heroes_Info PlayHeroInfo);
	static void RequestGameEnd(int32 StarGrade);
	static bool IsCampaignMode(EGamePlayModeTypeEnum InGameMode);
};