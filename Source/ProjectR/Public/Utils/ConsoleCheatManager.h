// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "ConsoleCheatManager.generated.h"

//class URGameInstance;

/**
 * 
 */
UCLASS()
class PROJECTR_API UConsoleCheatManager : public UObject
{
	GENERATED_BODY()
	
public:
	UConsoleCheatManager();
	
	UPROPERTY()
	UImage* TargetTexture;

	//////////////////////////////////////////////////////////////////////////
	// ULegoCheatManager Command Help Code

	//////////////////////////////////////////////////////////////////////////
	// 1. Engine Initialize Create

	// 1-1 ConsoleVariable
	// CM_ConsoleVariableStatic
	// CM_ConsoleVariableRegister

	// 1-2 ConsoleCommand Regist
	// CM_ConsoleCommandStatic
	// CM_ConsoleCommandDelegate
	// CM_ConsoleCommandWithArgsDelegate
	/*
	static void CM_ConsoleCommandStatic();
	static void CM_ConsoleCommandDelegate();
	static void CM_ConsoleCommandWithArgsDelegate(const TArray< FString >& params);
	*/
	/*
	//////////////////////////////////////////////////////////////////////////
	// 2. EXEC Function
	UFUNCTION(exec)
	void CM_ConsoleCommandExecFunction();
	UFUNCTION(exec)
	void CM_ConsoleCommandExecFunctionInt(int32 cheatInt);
	UFUNCTION(exec)
	void CM_ConsoleCommandExecFunctionFloat(float cheatFloat);
	UFUNCTION(exec)
	void CM_ConsoleCommandExecFunctionString(FString& cheatString);

	// Blueprint 에서 override 가능한 함수
	UFUNCTION(exec, BlueprintNativeEvent)
	void CM_BlueprintImplementableEvent();
	void CM_BlueprintImplementableEvent_Implementation(){};
	*/
public:

	//void		SetGameInstance(URGameInstance* RGameInstance){ GameInstance = RGameInstance; }

	//////////////////////////////////////////////////////////////////////////
	// SERVER CHEAT
	UFUNCTION(exec)
	void ServerCheat(FString& cheatCommand);


	//////////////////////////////////////////////////////////////////////////
	// CLIENT CHEAT
	UFUNCTION(exec)
	void VisibleUI();

	UFUNCTION(exec)
	void VisibleHPBar();

	UFUNCTION(exec)
	void VisibleUIAll();

	UFUNCTION(exec)
	void CoolTimeZero();

	UFUNCTION(exec)
	void SkillCostZero();

	void SpawnObject(FName& CharacterID, int32 Level, ECharacterCategoryTypeEnum Category);
	UFUNCTION(exec)
	void CreateHero(FName& CharacterID, int32 Level);
	UFUNCTION(exec)
	void CreateNPC(FName& CharacterID, int32 Level);
	UFUNCTION(exec)
	void CreateMinion(FName& CharacterID, int32 Level);
	UFUNCTION(exec)
	void CreateNeutralNpc(FName& CharacterID, int32 Level);
	UFUNCTION(exec)
	void SlotMode(int32 mode);
	UFUNCTION(exec)
	void StageClear(int32 state);
	UFUNCTION(exec)
	void GetNewHero(FName& CharacterID);
	UFUNCTION(exec)
	void Spawn();

	// For debugging displaying error message box on editor crash
	UFUNCTION(exec)
	void ToggleIgnoreDebugger();

	/** If 0, auto-select LOD level. if >0, force to (ForcedLodModel-1). */
	UFUNCTION(exec)
	void ForceLod(int32 LodModel);

	void ConsoleCommand(const FString& Command, UWorld* _world);
		

public:
	bool bCoolTimeZero = false;
	bool bSkillCostZero = false;
	bool bStageClear = false;
	int32 StageClearState = 0;
	bool bVisibleUI = true;
	bool bVisibleInGameHPBar = true;

	
// private:
// 	URGameInstance*		GameInstance = nullptr;			// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..

};
