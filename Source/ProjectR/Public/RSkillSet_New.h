// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "RSkillSet_New.generated.h"

UENUM(BlueprintType)
enum class ESkillIconSeq : uint8
{
	LEADER_ACTIVE1,
	LEADER_ACTIVE2,
	LEADER_ACTIVE3,
	CREW1_ACTIVE3,
	CREW2_ACTIVE3,
	CREW1_ACTIVE2,
	CREW2_ACTIVE2,
	MAX
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables
{
	GENERATED_USTRUCT_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URSpiritIcon*			Spirit1SkillIcon= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URSpiritIcon*			Spirit2SkillIcon= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	UWidgetSwitcher*				Crew_SpiritBoxSwitcher = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	UImage*							BG_3Combine = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	class URVirtualJoystick*		SkillJoystick = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	TArray<class USkillIcon_New*>			SkillIconArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URAttackIcon*				AttackIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URFocusAttackIcon*		FocusAttackIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URSearchTargetIcon*		SearchTargetIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URMeetingIcon*			MeetingIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSkillSet|Ref")
	class URCancelIcon*				CancelIcon;
	
	TArray<FIntRect>		SkillIconPosArray;
	TArray<FIntRect>		ButtonIconPosArray;
	FMargin					safe_margin;

	void Prepare();
	void InitUIPos();

};

USTRUCT(BlueprintType)
struct PROJECTR_API FBlackboard_Skillset
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	class UPlayStateInfo* PlayStateInfo;

	FVector2D InitialTouchPos;
	FVector2D CurrentTouchPos;

	/*Blackboard*/
	TBitArray<FDefaultBitArrayAllocator>	BlackBoardArray;

	/*Blackboard Data for Dead*/
	bool bCrewDeadState = false;
	int32 SlotIdx = INDEX_NONE;

	/*Blackboard Data for cancel*/
	bool bFingerOnCancelButton = false;

	void SetBlackboardData(ESkillIconSeq Indata , bool Inbool);
	void ClearBlackboardData();
	void SetDeadData_OnBlackboard(bool isDead, int32 slotIdx);

};

USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render();

	void ChangeOpacityExceptFor(USkillIcon_New* InException, FLinearColor InColor);

	FVariables* variables;
	FBlackboard_Skillset* blackboard;
	struct FStateMachine* StateMachine;
};

UENUM(BlueprintType)
enum class EStateEnum : uint8
{
	IDLE,
	LEADERSKILL,
	CREWSKILL3,
	CREWSKILL2,
	CREWDEAD,
	CREWDROP,
	END,
};

UCLASS()
class PROJECTR_API UUserInputState : public UObject
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();

	FORCEINLINE void SetBlackboard(FBlackboard_Skillset* Inblackboard) { blackboard = Inblackboard; }
	FORCEINLINE void SetVariables(FVariables* InVariables) { variables = InVariables; }
	FORCEINLINE void SetSkillIconSeq(ESkillIconSeq InIconSeq) { SkillIconSeq = InIconSeq; }
	FORCEINLINE ESkillIconSeq GetSkillIconSeq() { return SkillIconSeq; }
	FORCEINLINE void SetOwnerRole(ECharacterRoleEnum role) { OwnerRole = role; }

public:
	UPROPERTY()
	class USkillIcon_New* SelectedSkillIcon;

protected:
	FBlackboard_Skillset* blackboard;
	FVariables* variables;
	ESkillIconSeq SkillIconSeq = ESkillIconSeq::MAX;
	ECharacterRoleEnum OwnerRole = ECharacterRoleEnum::VE_None;

	EStateEnum CheckBlackboardData();

};

UCLASS()
class PROJECTR_API UIdle_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();
};

UCLASS()
class PROJECTR_API ULeaderSkill_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();

};

UCLASS()
class PROJECTR_API UCrewSkill3_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();
};

UCLASS()
class PROJECTR_API UCrewSkill2_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();

public:
	void CrewChange_BeforeSpawn();
	AActor* CrewChange_Spawn();
	void CrewChange_AfterSpawn_Cam(AActor* SpawnedActor);
	void CrewChange_AfterSpawn_Skill(AActor* SpawnedActor);
	void CrewChange_AfterSpawn_Anim(ECharacterRoleEnum InRole);

private:
	UPROPERTY()
	AActor*		DummyActor;

	FHeroSlotInfo* SlotInfo;

	FVector		SkillLocation;
	bool		isInDeadZone = false;
};

UCLASS()
class PROJECTR_API UCrewDead_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();

private:
	FVector		SkillLocation;
	
};

UCLASS()
class PROJECTR_API UCrewDrop_UserInputState : public UUserInputState
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Leave();
	virtual EStateEnum Update();

private:
	UPROPERTY()
	AActor*		DummyActor;

	FHeroSlotInfo* SlotInfo;

	FVector		SkillLocation;
	bool		isInDeadZone = false;


};

USTRUCT(BlueprintType)
struct PROJECTR_API FStateMachine
{
	GENERATED_USTRUCT_BODY()


	FVariables* variables;
	FBlackboard_Skillset* blackboard;

	void Prepare();
	bool Update();


	/*States*/
	UPROPERTY()
	TMap<EStateEnum, UUserInputState*> StateMap;
	UPROPERTY()
	UUserInputState* CurrentState;

	FORCEINLINE EStateEnum GetCurrentState() { return *(StateMap.FindKey(CurrentState)); }
	FORCEINLINE ESkillIconSeq GetCurrentSkillIconSeq() { return CurrentState->GetSkillIconSeq(); }

	
	

};



UCLASS()
class PROJECTR_API URSkillSet_New : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();
	void NativeDestruct();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


	/*UFunctions*/
public:
	UFUNCTION()
	void TouchBegin(ETouchIndex::Type Intype, FVector Inlocation);
	UFUNCTION()
	void TouchOver(ETouchIndex::Type Intype, FVector Inlocation);
	UFUNCTION()
	void TouchEnd(ETouchIndex::Type Intype, FVector Inlocation);
	UFUNCTION()
	void OnCrewDead(int32 SlotIdx);


public:
	class UISkill_V2* GetCurrentSkill();

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "SkillSet")
	FVariables Variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSet")
	FRenderer Renderer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSet")
	FBlackboard_Skillset Blackboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSet")
	FStateMachine FSM;

};
