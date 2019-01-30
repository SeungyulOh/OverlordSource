// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedConstants/GlobalConstants.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include <queue>
#include "LeaderControlHelper.generated.h"


class URGameInstance;
class AStagePlayerController;
class ULeaderControlHelper;


//Manual Mode AI BlackBoard
// Only Controls the moment when the character stops AI in ManualMode
USTRUCT(BlueprintType)
struct PROJECTR_API FManualAI_BlackBoard
{
	GENERATED_USTRUCT_BODY()

	//set these values true if you want to stop AI
	enum class EAIStopFrom : uint8 
	{
		STOPAI_FROMMOVE,  
		STOPAI_FROMATTACK,
		STAYAI_FROMATTACK,
		STOPAI_MAX
	};

	void SetBlackBoardData(EAIStopFrom InData, bool InValue);
	bool GetBlackBoardData(EAIStopFrom InData);

	TBitArray<FDefaultBitArrayAllocator>	BlackBoardArray;
};


UCLASS()
class PROJECTR_API ULeaderControlHelper : public UObject
{
	GENERATED_BODY()

public:
	void Tick(float DeltaTime);
	
	/*UFUNCTIONS*/
public:
	UFUNCTION()
	void CallbackInputDPadValue(EInputCommandTypeEnum InputCommand, FVector2D AxisValue);

	
public:
	void InitDeferred();
	void BindingDelegates();

	bool CheckLeaderCharacterAttackAnim();
	bool CheckInputCommandOfNormalAttack();

	/*Manul AI related*/
	bool AttackProcess_OnlyManualMode();
	void ShowAttackRange_OnlyManualMode(bool isOn);
	/**/

	void TargetEffectProcess();
	AActor* FindCurrentBattleCrew(int32 InType);


	void							ExcuteDPadState();
	void							SetDPadStateFlag(EDPadControllerStateFlagEnum CheckState, bool bState);
	FORCEINLINE bool				IsDPadStateFlag(EDPadControllerStateFlagEnum CheckState) const { return ((DPadStateFlag & (int32)CheckState) != 0); }

public:
	void SetAIEnableOnce();

	FORCEINLINE AStagePlayerController* GetBasePC() const { return BasePC;  }
	FORCEINLINE void SetBasePC(AStagePlayerController* inPC) { BasePC = inPC; }
public:
	UPROPERTY()
	TWeakObjectPtr< class URSkillSet_New > DPadSkillInterface = nullptr;
	UPROPERTY()
	TWeakObjectPtr< AActor >	LeaderCharacter = nullptr;
	UPROPERTY() //TargetEffect
	TWeakObjectPtr<AActor> CurrentAttackTarget;
		
	UPROPERTY()
	AStagePlayerController* BasePC;
	
	UPROPERTY()
	FManualAI_BlackBoard ManualAIBlackBoard;
	

	int32					DPadStateFlag = 0;

	bool				bCrewAIEnable = false;
public:
	ULeaderControlHelper(const FObjectInitializer& ObjectInitializer);
};
