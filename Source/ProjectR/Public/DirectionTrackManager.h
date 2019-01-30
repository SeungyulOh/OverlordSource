// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DirectionTrackManager.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDirectionStateTypeEnum : uint8
{
	VE_IdleDireciton					UMETA(DisplayName = "0.IdleDireciton"),
	VE_BattleDirection					UMETA(DisplayName = "1.BattleDirection"),
	VE_StoryDirection					UMETA(DisplayName = "2.StoryDirection"),
	VE_StageDirection					UMETA(DisplayName = "3.StageDirection"),
	VE_RTSDirection						UMETA(DisplayName = "4.RTSDirection"),
	VE_Max,
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDirectionEffectTypeEnum : uint8
{
	DE_Stage_BossDead						UMETA(DisplayName = "0. BattleBossDead"),
	DE_Battle_CombineEffect					UMETA(DisplayName = "1. CombineEffect"),
	DE_Stage_FailEvent						UMETA(DisplayName = "2. StageFail"),
	DE_Story_Dialogue						UMETA(DisplayName = "3. StoryDialogue"),
	DE_Stage_VictoryPosition				UMETA(DisplayName = "4. VictoryPosition"),
	DE_Stage_BossSpawnSequence				UMETA(DisplayName = "5. BossSpawnSequence"),
	DE_Battle_CrewChangeEffect				UMETA(DisplayName = "6. CrewChangeEffect"),
	DE_Battle_Vignette						UMETA(DisplayName = "7. Vignette"),
	DE_RTS_CS_PLAYLOSE						UMETA(DisplayName = "8. RTS_CS_PLAYLOSE"),
	DE_RTS_CS_PLAYWIN						UMETA(DisplayName = "9. RTS_CS_PLAYWIN"),
	DE_Max,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPostProcessStruct
{
	GENERATED_USTRUCT_BODY()

	enum PPType {BLOOM , VIGNETTE , RADIALBLUR , SLOW,  MAX};

	PPType ePPType = MAX;
	float InitialValue = 0.f;
	float TargetValue = 0.f;
	float ElapsedTime = 0.f;
	float Duration = 0.f;

};

/**
 * Every Direction Effect ex) Vignette, blur, slow, shake... should be defined and executed here.
 * FSM + State Design pattern + Singletone
 */
UCLASS()
class PROJECTR_API UDirectionTrackManager : public UObject
{
	friend class UBattleDirectionState;
	friend class UStageDirectionState;
	friend class URTSDirectionState;

	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Update(float Delta);
	
	void Empty_Heap();
	FORCEINLINE class UDirectionState* GetCurrentState() { return CurrentState; }
	FORCEINLINE EDirectionEffectTypeEnum GetTopEffect() { return DE_Heap.Num() > 0 ? DE_Heap.HeapTop() : EDirectionEffectTypeEnum::DE_Max; }
	UDirectionState* GetDirectionState(EDirectionStateTypeEnum InState);
	int32 GetSizeHeap();

	void Push_DirectionEffect(EDirectionEffectTypeEnum EffectType);
	void Pop_DirectionEffect(EDirectionEffectTypeEnum EffectType);
	void Force_IdleState();
	EDirectionStateTypeEnum Check_CorresspondingState();

private:
	UPROPERTY()
	class UDirectionState* CurrentState = nullptr;

	UPROPERTY()
	EDirectionStateTypeEnum NextState = EDirectionStateTypeEnum::VE_Max;

	UPROPERTY()
	TArray<EDirectionEffectTypeEnum> DE_Heap;

private:
	/*
	TMap : if TMap have UObject Pointer, it could be garbage collected!.
	*/
	UPROPERTY()
	TArray<class UDirectionState*> DirectionStateArray;
	
public:
	UFUNCTION()
	void Initialize();
	
};

UCLASS()
class PROJECTR_API UDirectionState : public UObject
{
	GENERATED_BODY()

	friend class UDirectionTrackManager;

public:
	virtual void Enter() PURE_VIRTUAL(UDirectionState::Enter, return;);
	virtual void Leave() PURE_VIRTUAL(UDirectionState::Leave, return;);
	virtual EDirectionStateTypeEnum Update(float Delta);
	virtual void PostProcessUpdate(float Delta);

	FORCEINLINE void AddPostProcessStruct(FPostProcessStruct InStruct) { PPArray.Emplace(InStruct); }
	FORCEINLINE APostProcessVolume* GetCurrentPPVolume() { return PPVolume.Get(); }

protected:

	UPROPERTY()
	EDirectionStateTypeEnum CurrentStateEnum = EDirectionStateTypeEnum::VE_Max;

	/** Post process settings */
	UPROPERTY()
	TWeakObjectPtr<APostProcessVolume> PPVolume;

	UPROPERTY()
	FPostProcessSettings InitialPPSettings;

	UPROPERTY()
	TArray<FPostProcessStruct> PPArray;

};

/*Usually for dialogue*/
UCLASS()
class PROJECTR_API UIdleDirectionState : public UDirectionState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();


};

/*Usually for dialogue*/
UCLASS()
class PROJECTR_API UStoryDirectionState : public UDirectionState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EDirectionStateTypeEnum Update(float Delta);

	void Cache_PPSettings();
	void Revert_PPSettings();

private:
	UPROPERTY()
	FPostProcessSettings CachePPSettings;


};

/*Combine Skills, targetting indicator , crew change skills , */
UCLASS()
class PROJECTR_API UBattleDirectionState : public UDirectionState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EDirectionStateTypeEnum Update(float Delta);

	EDirectionStateTypeEnum Exceptional_Transition();

};



USTRUCT(BlueprintType)
struct PROJECTR_API FBossDeadHelper
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UCurveFloat* BossDeadSlowCurve;
	UPROPERTY()
	UCurveFloat* BossDeadBlurCurve;
	UPROPERTY()
	USoundCue* BossDeadSound;

	float		Duration = 0.f;
	float		ElapsedTime = 0.f;
	float		FirstCameraDistance = 0.f;
	float		CameraDecreasedValue = 800.f;
};

/*Boss spawn sequence , victory position*/
UCLASS()
class PROJECTR_API UStageDirectionState : public UDirectionState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EDirectionStateTypeEnum Update(float Delta);

	void PostProcessUpdate_WithCurve(float Delta);

public:
	UPROPERTY()
	FBossDeadHelper BossDeadHelper;
	
};


UCLASS()
class PROJECTR_API URTSDirectionState : public UDirectionState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EDirectionStateTypeEnum Update(float Delta);

	void PostProcessUpdate_WithCurve(float Delta);

	UPROPERTY()
	FBossDeadHelper ColosseumEndHelper;
};