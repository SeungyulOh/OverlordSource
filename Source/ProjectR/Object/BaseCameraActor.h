// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GameFramework/Actor.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "BaseCameraActor.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FCheckCamera
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CheckCamera)
	AActor* Character = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CheckCamera)
	float	Range = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CheckCamera)
	FVector CheckLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CheckCamera)
	bool	bIsRedirectLocation = false;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTrackingCameraBlocking
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TrackingCameraBlocking)
	FVector		CenterLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TrackingCameraBlocking)
	float		RangeX = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TrackingCameraBlocking)
	float		RangeY = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TrackingCameraBlocking)
	int32		BlockingIndex = 0;
};


UCLASS()
class PROJECTR_API ABaseCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCameraActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;

	void InitCameraData();
	void ChangeCameraOriginDistance(bool bLongDistance, bool bAnimation = true);
	void SetDynamicCamera(bool bEnable);

	void MoveStageCamera(FVector TargetLocation, float InMoveTime);
	void TransformStageCamera(FTransform NewCameraTransform, float InMoveTime, float InMoveExp = 1.0f);

	void AutoZoomCameraByValue(float InZoomValue, float InZoomTime, float InZoomExp = 1.0f);
	void AutoZoomInOutCameraByPercent(bool InZoomInOut, float InZoomPercent, float InStartTime, float InDurationTime, float InEndTime, UCurveFloat* InCurveAsset, UCurveFloat* InAddCurveAsset = nullptr);

	void PingPongCameraForward(FVector InTargetLocation, float InMoveTime);
	void PingPongCameraBackward();

	AActor* GetOwnerCharacter();
	void SetOwnerCharacter(AActor* InCharacter);

	void ChangeFOV(float FOV);

#ifdef WITH_EDITOR
	/**	Save changed CameraDistance value for test	*/
	void SavePrevCameraDistance();
	void SetPrevCameraDistance();
#endif // WITH_EDITOR

	//*************************************************************************
	//InputManager Callback Function
	/*UFUNCTION(BlueprintCallable, Category = StageCamera)
	void CallbackInputTouchPressed(ETouchIndex::Type touchIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = StageCamera)
	void CallbackInputTouchReleased(ETouchIndex::Type touchIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = StageCamera)
	void CallbackInputTouchMoved( ETouchIndex::Type touchIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category = StageCamera)
	void CallbackInputTouchDrag( FDragInfo Drag);*/
	//*************************************************************************

	UFUNCTION()
	void OnReadyToStart();

	UFUNCTION(BlueprintCallable, Category = StageCamera)
	void		AddCameraBlockingInfo(FTrackingCameraBlocking InBlock);

	UFUNCTION(BlueprintCallable, Category = StageCamera)
	void		AddCameraLocationInfo(FTrackingCameraBlocking InBlock);

	void		ShowTargetEffect(bool bShow);
	void		SetTargetEffectLocation(FVector location);
	void		SetScreenEffect(UParticleSystem* ParticleSystem);
	void		ScrollScreen(FVector Location, ETouchIndex::Type touchIndex);
	void		SetCameraDist(float fValue);
	void		ZoomInAndOut(float fValue, bool bIsAddValue = true);
	void		ComputeDynamicCamera();
	void		SetCameraBlocking(bool InOn, int32 InIndex = 0);
	void		ComputeCameraBlockingLocation(FVector& InOutLocation);
	void		ResetAutoZoomSetting();

	FORCEINLINE FVector				GetStaticLocation()				{ return StaticLocation; }
	FORCEINLINE FVector				GetStartLocation()				{ return StartLocation; }
	FORCEINLINE FVector				GetCameraTargetLocation()		{ return TargetLocation; }
	FORCEINLINE UCameraComponent*	GetCameraComponent()			{ return CameraComponent.Get(); }
	FORCEINLINE UArrowComponent*	GetDialoguePosComponent()		{ return DialoguePosComponent; }
	FORCEINLINE USpringArmComponent*	GetSpringArmComponent() { return SpringArmComponent; }
	FORCEINLINE float				GetCamShotDist()				{ return CamShortDist; }
	FORCEINLINE float				GetCamLongDist()				{ return CamLongDist; }
	FORCEINLINE FRotator			GetStaticRotation()				{ return StaticRotation; }
	FORCEINLINE bool				IsCameraMoving()				{ return IsMoving; }
	FORCEINLINE bool				GetCameraMoveEnable()			{ return IsCameraMoveEnable; }
	FORCEINLINE void				SetCameraMoveEnable(bool InSet) { IsCameraMoveEnable = InSet; }
	FORCEINLINE void				SetOriginDistance(float InValue) { Camera_OriginDistance = InValue; }
	FORCEINLINE void				SetDragLock(bool bLock)			{ bDragLock = bLock; }
	FORCEINLINE void				SetRefreshArm(bool InRefresh)	{ IsRefreshSpringArm = InRefresh; }
	FORCEINLINE void				SetOwnerRotationCamera(bool InSet) { IsOwnerRotationCamera = InSet; }
	FORCEINLINE void				SetOwnerDelayCamera(bool InSet) { IsOwnerDelayCamera = InSet; }
	FORCEINLINE bool				GetOwnerRotationCamera()		{ return IsOwnerRotationCamera; }
	FORCEINLINE void				SetArenaCamera(bool InArenaCamera) { IsArenaCamera = InArenaCamera; }
	FORCEINLINE bool				GetArenaCamera() { return IsArenaCamera; }
	FORCEINLINE float				GetCameraOriginDist() { return Camera_OriginDistance; }

	void		ArenaCameraZoomInOut();
	
	void SetIsSequenceCameraState(bool InValue) { IsSequenceCameraState = InValue; }
	
	


	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float CharacterCameraZ = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float CharacterCameraPitch = 35.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	//float MinFOV = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float Zoom_Speed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float Camera_Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float XAxisScrollRateBase = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float YAxisScrollRateBase = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float ZoomMinBase = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float ZoomMaxBase = 3000.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	//float AngleRotStartDistanceBase = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	float AngleMaxBase = 340.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	//float AngleMinBase = 320.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	bool IsCameraMoveEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageCamera|Ref")
	UStaticMeshComponent*	TargetEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageCamera|Ref")
	UArrowComponent*		DialoguePosComponent = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StageCamera)
	//float CameraMove_Distance = 30.0f;

	//testdefine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicCamera)
	bool		IsDynamicCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicCamera)
	float		CheckDynamic_Range = 1100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicCamera)
	float		Dynamic_ZoomTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicCamera)
	float		Dynamic_ZoomExp = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicCamera)
	float		Dynamic_MoveTime = 1.0f;

	//Tracking Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrackingCamera)
	TArray<FTrackingCameraBlocking>		TrackingCameraBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseCameraActor)
	TArray<FTrackingCameraBlocking>		AddCameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseCameraActor)
	UCurveFloat*	Curve_BaseCameraMove = nullptr;

private:
	bool		IsMove(FVector startPos, FVector endPos, float fDiff);
	FVector		GetMoveVectorBase(FVector startPos, FVector endPos);
	void		MoveCameraBase(FVector startPos, FVector endPos, FVector firstStartPos);
	void		ZoomInOut();
	void		RefreshSpringArm();

	void		GetCameraBlockingVolume();
	bool		IsInCameraVolume(FVector NewTarget);
	void		OnTickMove(float DeltaTime);
	void		OnTickZoom(float DeltaTime);
	void		OnTickZoomInOut(float DeltaTime);

	void		IsInCheckLocation();

private:
	UPROPERTY()
	AActor*	OwnerCharacter = nullptr;

	
	UPROPERTY()
	FVector		StartLocation;
	UPROPERTY()
	FVector		TargetLocation;

	UPROPERTY()
	FRotator	StartRotation;
	UPROPERTY()
	FRotator	TargetRotation;

	bool		IsArenaCamera = false;
	bool		IsAutoZoom = false;
	float		ZoomMaxValue = 0.0f;
	float		ZoomOriginValue = 0.0f;
	float		ZoomTime = 1.0f;
	float		ZoomElapsedTime = 0.0f;
	float		ZoomExp = 1.0f;
	float		Camera_OriginDistance = 0.0f;
	float		Camera_SequenceDistance = 0.0f;
	bool		bDragLock = false;
	bool		bReadyToStart = false;

	float		Camera_Angle = 0.0f;

	float		CamLongDist = 0.0f;
	float		CamLongAngle = 0.0f;
	float		CamLongFOV = 0.0f;

	float		CamShortDist = 0.0f;
	float		CamShortAngle = 0.0f;
	float		CamShortFOV = 0.0f;
	
	// ZoomInOutPercent
	bool		IsAutoZoomInOut = false;
	float		ZoomInOutPercent = 1.0f;
	float		ZoomInOutStartTime = 1.0f;
	float		ZoomInOutDurationTime = 1.0f;
	float		ZoomInOutEndTime = 1.0f;
	float		ZoomInOutElapsedTime = 0.0f;
	float		ZoomInOutTargetValue = 0.0f;

	UPROPERTY()
	UCurveFloat*	Curve_ZoomInOut = nullptr;
	UPROPERTY()
	UCurveFloat*	AddCurve_ZoomInOut = nullptr;

	UPROPERTY()
	TWeakObjectPtr< ACameraBlockingVolume > CameraBlockingVolume;
	UPROPERTY()
	FBox		BlockingBox;

	UPROPERTY()
	FVector		StaticLocation;
	UPROPERTY()
	FRotator	StaticRotation;

	bool		UsePingPongCamera = false;
	UPROPERTY()
	FVector		PingPongCameraBackUpStartLocation;

	EGamePlayModeTypeEnum CheckGameMode = EGamePlayModeTypeEnum::VE_None;
	
	UPROPERTY()
	TArray<FVector>		TouchStartPos;
	UPROPERTY()
	TArray<FVector>		TouchMovePos;
	int32				TouchCount = 0;
	//bool				IsTouchOver = false;
	UPROPERTY()
	FVector				TouchPrevPos;
	bool				IsZoomIn;
	bool				IsAutoMode = false;
	bool				IsRecoveryZoomValue = true;
	bool				IsRefreshSpringArm = true;
	bool				IsOwnerDelayCamera = false;
	bool				IsOwnerRotationCamera = false;
	float				OwnerOffset = 0.0f;
	bool				IsMoving = false;
	float				MoveTime = 1.0f;
	float				MoveElapsedTime = 0.0f;
	float				MoveExp = 1.0f;

	bool				IsSequenceCameraState = false;

	//Tracking Camera
	bool				IsTrackingBlock = false;
	int32				BlockIndex = 0;

	UPROPERTY()
	TWeakObjectPtr< ABasePlayerController >				BasePlayerController;
	UPROPERTY()
	TWeakObjectPtr< UCameraComponent >					CameraComponent;
	UPROPERTY()
	TWeakObjectPtr< UParticleSystemComponent >			ScreenEffectComponent;

	//// For fading out FadableStaticMeshActors
public:
	// Checked at BeginPlay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	bool bEnableFadeOut = true;
	
	// (Profiling)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
	float FadeOutCheckInterval = 0.1;

private:
	void InitFadeOutOnBeginPlay();
	void ClearFadeOutOnEndPlay();

	// possible that there is more than one actor faded out at a time
	UPROPERTY()
	TArray<AActor*> ActorsCurrentlyFadedOut;
	
	// for array reuse
	TArray<FHitResult> HitResultsArray;
	UPROPERTY()
	TArray<AActor*> FadableActorsArray;

	FTimerHandle FadeCheckTimerHandle;

	UFUNCTION()
	void CheckForFadeOut();

#ifdef WITH_EDITOR
	float PrevCameraDistance = 0.0f;
#endif
	


};