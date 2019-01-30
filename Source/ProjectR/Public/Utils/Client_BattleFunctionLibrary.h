// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/DamageResultRate.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Table/SkillUnlockLevelTableInfo.h"
#include "Table/BattleObjectTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "CustomStructures/BattleObjectInfo.h"
#include "Network/PacketDataStructures.h"
#include "Global/RGameInstance.h"

#include "Client_BattleFunctionLibrary.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UClient_BattleFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UClient_BattleFunctionLibrary() {};
	
	UFUNCTION(BlueprintCallable, Category = Client_BattleFunctionLibrary)
	static int32					GetEquipedHeroCombatStat(FEQUIPED_HERO hero);

	UFUNCTION(BlueprintCallable, Category = Client_BattleFunctionLibrary)
	static int32					GetHeroCombatStat(FString InHeroKey);

	UFUNCTION(BlueprintCallable, Category = Client_BattleFunctionLibrary)
	static int32					GetTargetHeroCombatStat(FBATTLE_USER& MatchList, FString& InHeroKey);
	
	UFUNCTION(BlueprintCallable, Category = Client_BattleFunctionLibrary)
	static void						GetEquipItem(FHERO& HeroInfo, TArray<FITEM>& EquipItem);

	UFUNCTION(BlueprintCallable, Category = Client_BattleFunctionLibrary)
	static int32 CalcHeroCombatStat(UCharacterBattleInfo* BattleInfo, FWorkedCharacterTableInfo& InTableInfo, TArray<FITEM> EquipItem);
};