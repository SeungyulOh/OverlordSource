// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SharedConstants/GlobalConstants.h"
//#include "UI/RBaseWidget.h"
//#include "UI/UI_Battle/RSkillSet.h"
#include "BasePlayerController.generated.h"

class ABaseCameraActor;
class UBattleEntityComponent;
class UISkill_V2;
class ULeaderControlHelper;
/**
 * 
 */
UCLASS()
class PROJECTR_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

// 	friend class ULeaderSkillControl;
// 	friend class ULeaderMoveControl;
	
public:
	ABasePlayerController(const FObjectInitializer& ObjectInitializer);
	FString								ConsoleCommand(const FString& Command, bool bWriteToLog = true) override;
	void								BeginPlay() override;
	void								TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void						EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	virtual void						ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void						SetupInputComponent() override;

public:
	void								OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime);
	void								OnPinchUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);
	void								OnPinchReleased(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime);
	UFUNCTION(BlueprintCallable)
	virtual void						CheckAndroidBack();
public:


	void								PlayCameraShake(TSubclassOf<class UCameraShake> ShakeEffect, float Intensity);
	void								SetCameraActor(ABaseCameraActor* actor);
	void								ActorSettingOnTheCamera(AActor* character, float InCameraMove = 0.5f, float InCameraExp = 0.5f);
		
	void								CreateJoystick();
	void								RemoveJoystick();
	void								ResetJoystickForChannelSkillMovingCast();

	void								StageDirectionProcess();
	void								SetStageDirection(bool InAttach, FVector InTargetLocation = FVector(0, 0, 0));
	FORCEINLINE bool					IsStageDirectionProcess() { return (Particle_Hero_StageDirection != nullptr); }
	
	//////////////////////////////////////////////////////////////////////////
	// ReceiveInput GamePause 
	FORCEINLINE void					SetReceiveInputFromGamePause(bool ReceiveInput) { this->bShouldPerformFullTickWhenPaused = ReceiveInput; }
	void								HideAvoidState();
	bool								IsAroundEnemy();

	void SetLeaderCharacterAIAutoMode(EAIControlModeEnum InControlType);
	EAIControlModeEnum GetLeaderAIModeType();
	FORCEINLINE AActor* GetLeaderCharacter() { return LeaderCharacter.Get(); }

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasePlayerController)
	TArray< TSubclassOf<class UCameraShake> > 	CameraShakeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_CrewChange | Freeze or Slow")
	bool								bisFreeze = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_CrewChange | Freeze or Slow")
	float								SlowSpeed = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_CrewChange | Camera")
	float								CameraDistance_OffSet = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_CrewChange | Camera")
	float								CameraShakeIntensity = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_CrewChange | Camera")
	float								CameraZoomTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_Combine | Camera")
	float								CameraBledTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test_Combine | Camera")
	TEnumAsByte<enum EViewTargetBlendFunction> BlendFunction = VTBlend_Linear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Boss)
	float								StageBossSlowTime	= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Boss)
	float								StageBossSlowRate	= 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Boss)
	float								StageBossPostprocessTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Mode)
	bool								IsWangRealMode = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Mode)
	FName								RealMode_BoneName = FName("Bip001-Head");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Mode)
	float								RealModeBoneScale = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Skill)
	float								SkillPadLimitValue = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test_Camp)
	bool								IsStageDirection_Head = true;

	UPROPERTY()
	EAIControlModeEnum					AIControlMode_UserInput = EAIControlModeEnum::VE_ManualMode;
	
protected:
	UPROPERTY()
	ABaseCameraActor*					CameraActor = nullptr;
	UPROPERTY()
	TWeakObjectPtr< AActor >			LeaderCharacter = nullptr;
	int32								TouchCount = 0;
	bool								IsReadyTargetLocationSkillFire = false;
	
public:
	//Move DPad
	UPROPERTY()
	EInputCommandTypeEnum				CurInputCommandType = EInputCommandTypeEnum::VE_None;
	UPROPERTY()
	UParticleSystemComponent*			Particle_Hero_Gather = nullptr;
	UPROPERTY()
	UParticleSystemComponent*			Particle_Hero_StageDirection = nullptr;
	UPROPERTY()
	class URBaseWidget*					InGameHUD;

	
	UPROPERTY()
	class UNormalTouchInput*			InputHandler = nullptr;
public:
	FVector								StageLocation;
	bool								IsCustomGame = false; //tutorial
	bool								IsInputWait = false; //tutorial
	bool								IsResetSkillCoolTime = false; //del?
	bool								IsSetCameraArm = false;
	bool								IsStopMovePad = false;
	bool								IsButtonOn = false;
	bool								IsJoystickActivate = false;
	float								JoystickScale = 0.0f;
	float								ZPos_SkillPickArea = 0;
	bool								InputAttackHold = false;
	bool								DPadEnable = true;
	bool								IsTempIdle = false;
	bool								IsFocusAttackButton = false;
	bool								IsAvoidButton = false;
	float								AvoidRestTime = 0.0f;
	float								AvoidTime = 5.0f;
	ETouchIndex::Type					CheckSkillTouchType;
	FTransform							PrevViewTargetTransform;
	FTransform							PrevCameraComponentTransform;

	/*
	Editor
	*/
	bool								IsShowDebug = false;
	bool								IsGameTimerPause = false;
	bool								IsAbnormalState = false;



public:
	
	FORCEINLINE AActor*					GetSelectedCharacter() { return LeaderCharacter.Get(); }
	FORCEINLINE bool					IsSelectedCharacter(AActor* TargetActor) { return (LeaderCharacter.Get() == TargetActor); }
	

	FORCEINLINE	ABaseCameraActor*		GetCameraActor() { return CameraActor; }

	
	FORCEINLINE bool					GetIsWangRealMode() { return IsWangRealMode; }
	
	FORCEINLINE bool					GetIsFocusAttack() { return IsFocusAttackButton; }
	FORCEINLINE EInputCommandTypeEnum	GetCurInputCommandType() { return CurInputCommandType; }
	FORCEINLINE void					SetCurInputCommandType(EInputCommandTypeEnum _type) { CurInputCommandType = _type; }
	FORCEINLINE bool					GetIsAvoidState() { return IsAvoidButton; }
	void								SetAvoidState(bool InAvoidState);
	FORCEINLINE float					GetAvoidRestTime() { return AvoidRestTime; }
	FORCEINLINE float					GetAvoidTime() { return AvoidTime; }
	FORCEINLINE float					GetAvoidTimePercent() { return (AvoidRestTime / AvoidTime); }
	FORCEINLINE bool					GetJoystickActivate() { return IsJoystickActivate; }
	FORCEINLINE void					SetJoystickScale(float InValue) { JoystickScale = InValue; }
	FORCEINLINE void					SetInputAttackHold(bool _input) { InputAttackHold = _input; }
	FORCEINLINE bool					IsDPadEnable() { return DPadEnable; }
	FORCEINLINE void					SetDPadEnable(bool InEnable) { DPadEnable = InEnable; }
	FORCEINLINE bool					GetIsTempIdle() { return IsTempIdle; }
	FORCEINLINE void					SetIsTempIdle(bool Input) { IsTempIdle = Input; }
	FORCEINLINE bool					GetIsShowDebug() { return IsShowDebug; }
	FORCEINLINE void					SetIsShowDebug(bool _inDebug) { IsShowDebug = _inDebug; }
	FORCEINLINE bool					GetIsGameTimerPause() { return IsGameTimerPause; }
	FORCEINLINE void					SetIsGameTimerPause(bool _input) { IsGameTimerPause = _input; }
	FORCEINLINE bool					GetIsInputWait() { return IsInputWait; }
};

