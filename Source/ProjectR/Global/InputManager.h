// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "InputCoreTypes.h"
#include "SharedConstants/GlobalConstants.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"

#include "InputManager.generated.h"

class URGameInstance;

USTRUCT(BlueprintType)
struct PROJECTR_API FInputTouchInfo
{
	GENERATED_USTRUCT_BODY()

	bool		IsPress;
	bool		IsRepeat;
	UPROPERTY()
	FVector		StartPos;
	UPROPERTY()
	FVector		LastPos;
	UPROPERTY()
	FVector		CurrentPos;

	void SetData(bool Press, bool Repeat, FVector Start, FVector Last, FVector Current)
	{
		IsPress = Press;
		IsRepeat = Repeat;
		StartPos = Start;
		LastPos = Last;
		CurrentPos = Current;
	}
};

//-----------------------------------------------------------------------------
// Delegate signatures
//-----------------------------------------------------------------------------
// Default Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnInputTouchBegin, ETouchIndex::Type, TouchIndex, FVector, Location );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnInputTouchEnd, ETouchIndex::Type, TouchIndex, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnInputTouchOver, ETouchIndex::Type, TouchIndex, FVector, Location);

// Click
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnInputClick, FVector, Location);

// Drag
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnInputDrag, FDragInfo, Drag);

// Pinch
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnInputPinch, FPinchInfo, Pinch);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnInputAxisValue, EInputCommandTypeEnum, InputCommand, FVector2D, AxisValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnInputSkillAxisValue, EInputCommandTypeEnum, InputCommand, FVector2D, AxisValue);

/*

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EInputStateEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Change 			UMETA(DisplayName = "Change"),
	E_Pooling			UMETA(DisplayName = "Pooling"),
	VE_Max,
};
*/

class ADPadController;
/**
 * 
 */
UCLASS()
class PROJECTR_API UInputManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	

public:
	UInputManager();
	UInputManager(const FObjectInitializer& ObjectInitializer);
	
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	TStatId GetStatId() const override;

	bool IsTickableInEditor() const override;

	FORCEINLINE void Initialize(URGameInstance* Instance) { RGameInstance = Instance; }

	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackInputTouchBegin(ETouchIndex::Type TouchIndex, FVector Location);
	
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackInputTouchOver(ETouchIndex::Type TouchIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackInputTouchEnd(ETouchIndex::Type TouchIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = InputManager)
	void OnInputTouchClick(FVector Location);

	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackAxis_MoveUpDown(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackAxis_MoveLeftRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackAxis_SkillUpDown(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackAxis_SkillLeftRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackSkillAction_Pressed(FKey Key);
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackSkillAction_Released(FKey Key);
	UFUNCTION(BlueprintCallable, Category = InputManager)
	void CallbackSkillAction_Repeat(FKey Key);

public:
	/** If true, all input on the stack below this actor will not be considered */
	UPROPERTY(EditDefaultsOnly, Category = InputManager)
	uint32 bBlockInput : 1;

	/** The priority of this input component when pushed in to the stack. */
	UPROPERTY(EditAnywhere, Category = InputManager)
	int32 InputPriority;

	/** Component that handles input for this actor, if input is enabled. */
	UPROPERTY()
	class UInputComponent* InputComponent;
		
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputTouchBegin			OnInputTouchBeginDelegate;
		
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputTouchEnd			OnInputTouchEndDelegate;
		
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputTouchOver			OnInputTouchOverDelegate;
		
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputClick				OnInputClickDelegate;

	// drag	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputDrag				OnInputDragDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputTouchEnd			OnInputDragEndDelegate;
		
	// Pinch 		
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputPinch				OnInputPinchDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = InputManager)
	FDelegate_OnInputTouchEnd			OnInputPinchEndDelegate;

	// DPad
	UPROPERTY()
	FDelegate_OnInputAxisValue			OnInputAxisValueDelegate;

	UPROPERTY()
	FDelegate_OnInputSkillAxisValue     OnInputSkillAxisValueDelegate;
		
	// Click Tolerance 		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputManager)
	float								ClickToleranceDist;

	// Click Tolerance 		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputManager)
	float								PinchCancelToleranceDist;

public:
	bool IsCheckDragState();
	bool IsCheckPinchState();
	int32 GetTouchCount();

	FORCEINLINE FVector2D GetDPadMoveValue() { return DPadMoveValue; }
	FORCEINLINE void SetDPadMoveValue(FVector2D _inVec) { DPadMoveValue = _inVec; }
	FORCEINLINE FVector2D GetDPadSkillMoveValue() { return DPadSkillValue; }
	FORCEINLINE void SetDPadSkillMoveValue(FVector2D _inVec) { DPadSkillValue = _inVec; }

private:
	void InitDefault();
	void EnableInputComponent();
	void SetBindInputComponent();

	void OnProcessDrag();
	FDragInfo SetDragInfo(FInputTouchInfo TouchInfo);
	void OnProcessPinch();
	FPinchInfo SetPinchInfo(FInputTouchInfo Touch1, FInputTouchInfo Touch2);

private:
	UPROPERTY()
	URGameInstance*			RGameInstance = nullptr;
	EGameStateEnum			CurrentGameStateType;
	bool					IsSetupInputBinding;
	bool					IsEnableInputComponent;
	bool					IsNotPadEvent = false;

	TMap<ETouchIndex::Type, FInputTouchInfo> TouchInfoMap;
	
	bool						IsDrag;
	bool						IsPinch;

	UPROPERTY()
	FVector2D					DPadMoveValue;
	UPROPERTY()
	FVector2D					DPadSkillValue;

	UPROPERTY()
	FKey						LastInputKey;
	UPROPERTY()
	FKey						ReadyToSkillKey;
	bool						UsingSkillDPad;
	

	UPROPERTY()
	FVector2D				StartLocation = FVector2D(0,0);
	UPROPERTY()
	FVector2D				DPadGuideValue;
	float					MaxTime = 0;
	float					ElapsedTime = 0;
	float					DelayTime = 0;

	UPROPERTY()
	ADPadController* DPadController = nullptr;
};
