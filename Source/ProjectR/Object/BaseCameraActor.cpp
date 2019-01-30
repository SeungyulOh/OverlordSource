// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "BaseCameraActor.h"

#include "GlobalIntegrated.h"
//


#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Table/MapTableInfo.h"
#include "Object/FadableInterface.h"
#include "Object/FadableStaticMeshActor.h"
#include "Object/FadableSkeletalMeshActor.h"
#include "LeaderControlHelper.h"

// Sets default values
ABaseCameraActor::ABaseCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCameraActor::BeginPlay()
{
	Super::BeginPlay();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	
	{
		
		RGameInstance->CameraActor = this;
		CheckGameMode = BaseStageInfo->GetGameMode();

		if (CheckGameMode != EGamePlayModeTypeEnum::VE_Arena && 
			CheckGameMode != EGamePlayModeTypeEnum::VE_MirrorDungeon)
		{
			IsDynamicCamera = false;
			IsArenaCamera = false;
		}

		if (IsDynamicCamera)
//			|| CheckGameMode == EGamePlayModeTypeEnum::VE_NewTutorial)
		{
			IsRecoveryZoomValue = false;
		}

		if (IsValid(RGameInstance->EventManager))
		{
			if (IsDynamicCamera)
			{
				RGameInstance->EventManager->OnGameMode_ReadyToStart.RemoveDynamic(this, &ABaseCameraActor::OnReadyToStart);
				RGameInstance->EventManager->OnGameMode_ReadyToStart.AddDynamic(this, &ABaseCameraActor::OnReadyToStart);
			}
		}
	}

	TouchCount = 0;
	//IsTouchOver = false;

	SpringArmComponent = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
	if (IsValid(SpringArmComponent))
	{
		Camera_Distance = SpringArmComponent->TargetArmLength;
	}
	CameraComponent = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
	
	Camera_OriginDistance	= Camera_Distance;
	if (!IsRecoveryZoomValue)
	{
		ZoomOriginValue = Camera_Distance;
	}

	InitCameraData();

	ScreenEffectComponent = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
	if (BasePlayerController.IsValid())
	{
		BasePlayerController->SetCameraActor(this);
		//BasePlayerController->SetViewTargetWithBlend(this);
		//BasePlayerController->SetAudioListenerOverride(this->GetRootComponent(), FVector(), FRotator());
	}

	

	ShowTargetEffect(false);

	StaticLocation = GetActorLocation();
	StaticRotation = GetActorRotation();

	GetCameraBlockingVolume();

	ChangeCameraOriginDistance(true, false);
	if (BaseStageInfo)
	{
		bool bPlayAlone = BaseStageInfo->bPlayAlone;
		if (bPlayAlone)
		{
			SetCameraDist(RGameInstance->GameEnvironmentVar.PlayAloneCameraDistance);
		}
	}



	if (IsDynamicCamera)
	{
		SetDragLock(IsDynamicCamera);
		ComputeDynamicCamera();
	}

	InitFadeOutOnBeginPlay();
}

void ABaseCameraActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearFadeOutOnEndPlay();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABaseCameraActor::Tick( float DeltaTime )
{
	//IsAlreadySpawned
	Super::Tick( DeltaTime );



	if (IsDynamicCamera && bReadyToStart)
		ComputeDynamicCamera();
	if (GetArenaCamera())
	{
		if (CameraComponent.IsValid())
			CameraComponent->SetFieldOfView(60);
		ArenaCameraZoomInOut();
	}


	if (IsValid(OwnerCharacter))
	{
		/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		int32 SelectedSlotIndex = RGameInstance->PlayStateInfo->GetSelectedSlotIndex();
		FHeroSlotInfo& HeroSlotInfo = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(SelectedSlotIndex);*/

		bool bMoveOwnerCameraMode = !(IsAutoMode && (BasePlayerController->IsSelectedCharacter(OwnerCharacter) == false));
		/*if (IsAutoMode)
		{
			//if (HeroSlotInfo.SpawnedTargetActor != OwnerCharacter ||
			//	HeroSlotInfo.SlotSpawnState != EHeroSpawnStateEnum::VE_Spawned)
			if(BasePlayerController->IsSelectedCharacter(OwnerCharacter) == false)
			{
				OwnerCharacter = nullptr;
				bMoveOwnerCameraMode = false;
			}
			//else if (OwnerCharacter->CurrentAttackTarget == nullptr
			//	&& CheckGameMode == EGamePlayModeTypeEnum::VE_KillBoss)
			//{
			//	return;
			//}
		}*/

		if(bMoveOwnerCameraMode)
		{
			if (IsDynamicCamera == false)
			{
				//FVector NewPosition = OwnerCharacter->GetActorLocation();
				//NewPosition.Z = GetActorLocation().Z;
				FVector NewPosition = UMapFunctionLibrary::GetNavAgentLocation(OwnerCharacter);
				
				if (IsTrackingBlock)
				{
					ComputeCameraBlockingLocation(NewPosition);
				}

				if (IsOwnerDelayCamera)
				{
					if (SpringArmComponent)
					{
						SpringArmComponent->CameraLagSpeed = 5;
					}

					TargetLocation = NewPosition;
					TargetRotation = OwnerCharacter->GetTransform().GetRotation().Rotator();
					IsMoving = true;
				}
				else
				{
					if (SpringArmComponent)
					{
						SpringArmComponent->CameraLagSpeed = 100;
					}
					SetActorLocation(NewPosition);
				}
			}
			else
			{
				IsMoving = false;
			}
		}
		else
		{
			OwnerCharacter = nullptr;
		}
	}
	
	if (IsMoving)
	{
		OnTickMove(DeltaTime);
	}

	if (IsAutoZoom)
	{
		OnTickZoom(DeltaTime);
	}
	
	if (IsAutoZoomInOut)
	{
		OnTickZoomInOut(DeltaTime);
	}

	if (IsOwnerRotationCamera)
	{
		if (IsValid(OwnerCharacter))
		{
			SetActorRotation(OwnerCharacter->GetActorRotation());
		}
	}
}

void ABaseCameraActor::OnTickMove(float DeltaTime)
{
	MoveElapsedTime += DeltaTime;	

	if (MoveElapsedTime >= MoveTime)
	{
		MoveElapsedTime = MoveTime;
		if (IsValid(OwnerCharacter))
		{
			if(IsAutoMode)
				OwnerCharacter = nullptr;
			//MoveExp = 1.0f;
		}

		//IsMoving = false;
	}

	float ElapsedTimeAlpha = MoveElapsedTime / MoveTime;
	float CurveValue = ElapsedTimeAlpha;

	if (IsValid(Curve_BaseCameraMove))
	{
		CurveValue = Curve_BaseCameraMove->GetFloatValue(ElapsedTimeAlpha);
	}
	
	FVector NewPosition = FMath::Lerp(StartLocation, TargetLocation, CurveValue);
	//FVector NewPosition = FMath::InterpEaseInOut(StartLocation, TargetLocation, CurveValue, MoveExp);
	
	if (GetArenaCamera() == false)
	{
		if (OwnerCharacter)
		{
			NewPosition.Z = UMapFunctionLibrary::GetNavAgentLocation(OwnerCharacter).Z;
		}
		else
		{
			NewPosition.Z = GetActorLocation().Z;
		}
		SetActorLocation(NewPosition);
	}



	if (ElapsedTimeAlpha > 0.999f)
	{
		IsMoving = false;
		//MoveExp = 1.0f;
		if (UsePingPongCamera)
		{
			PingPongCameraBackward();
		}
	}
}

void ABaseCameraActor::OnTickZoom(float DeltaTime)
{
	ZoomElapsedTime += DeltaTime;
	float CurrentAlpha = ZoomElapsedTime / ZoomTime;
	if (ZoomElapsedTime >= ZoomTime)
		CurrentAlpha = 1.0f;

	float ZoomAlphaValue = 0.f;
	if(ZoomMaxValue > ZoomMinBase)
		ZoomAlphaValue = FMath::InterpEaseInOut(ZoomOriginValue, ZoomMaxValue, CurrentAlpha, ZoomExp);
	else if(ZoomMaxValue < 0)
		ZoomAlphaValue = FMath::InterpEaseInOut(ZoomOriginValue + ZoomMaxValue, ZoomOriginValue, CurrentAlpha, ZoomExp);

	
	ZoomInAndOut(ZoomAlphaValue, false);

	if (ZoomElapsedTime >= ZoomTime)
	{
		ResetAutoZoomSetting();
	}
}

void ABaseCameraActor::OnTickZoomInOut(float DeltaTime)
{
	if (0.0f < ZoomInOutStartTime)
	{
		ZoomInOutElapsedTime += DeltaTime;

		float ElapsedTimeAlpha = ZoomInOutElapsedTime / ZoomInOutStartTime;
		if (1.0f < ElapsedTimeAlpha)
		{
			ElapsedTimeAlpha = 1.0f;
		}

		float CurveValue = ElapsedTimeAlpha;
		if (IsValid(Curve_ZoomInOut))
		{
			CurveValue = Curve_ZoomInOut->GetFloatValue(ElapsedTimeAlpha);
		}

		float TargetDistance = ((IsSequenceCameraState) ? Camera_SequenceDistance : Camera_OriginDistance);
		float ZoomAlphaValue = FMath::Lerp(TargetDistance, ZoomInOutTargetValue, CurveValue);
		ZoomInAndOut(ZoomAlphaValue, false);

		if (ZoomInOutStartTime <= ZoomInOutElapsedTime)
		{
			ZoomInOutElapsedTime = 0.0f;
			ZoomInOutStartTime = 0.0f;
		}
	}
	else if (0.0f < ZoomInOutDurationTime)
	{
		ZoomInOutElapsedTime += DeltaTime;
		if (ZoomInOutDurationTime <= ZoomInOutElapsedTime)
		{
			ZoomInOutElapsedTime = 0.0f;
			ZoomInOutDurationTime = 0.0f;
		}
	}
	else if (0.0f < ZoomInOutEndTime)
	{
		ZoomInOutElapsedTime += DeltaTime;

		float ElapsedTimeAlpha = ZoomInOutElapsedTime / ZoomInOutEndTime;
		if (1.0f < ElapsedTimeAlpha)
		{
			ElapsedTimeAlpha = 1.0f;
		}

		float CurveValue = ElapsedTimeAlpha;
		if (IsValid(AddCurve_ZoomInOut))
		{
			CurveValue = AddCurve_ZoomInOut->GetFloatValue(ElapsedTimeAlpha);
		}
		else if (IsValid(Curve_ZoomInOut))
		{
			CurveValue = Curve_ZoomInOut->GetFloatValue(ElapsedTimeAlpha);
		}
		
		float TargetDistance = ((IsSequenceCameraState) ? Camera_SequenceDistance : Camera_OriginDistance);
		float ZoomAlphaValue = FMath::Lerp(ZoomInOutTargetValue, TargetDistance, CurveValue);
		ZoomInAndOut(ZoomAlphaValue, false);

		if (ZoomInOutEndTime <= ZoomInOutElapsedTime)
		{
			ZoomInOutElapsedTime = 0.0f;
			ZoomInOutEndTime = 0.0f;
			
			if (IsRecoveryZoomValue)
			{
				if (Camera_Distance != TargetDistance)
				{
					ZoomInAndOut(TargetDistance, false);
				}
			}
			else
			{
				if (Camera_Distance != ZoomOriginValue)
				{
					ZoomInAndOut(ZoomOriginValue, false);
				}
			}

			IsAutoZoomInOut = false;
			Curve_ZoomInOut = nullptr;
			AddCurve_ZoomInOut = nullptr;
		}
	}
	else
	{
		IsAutoZoomInOut = false;
	}
}


//////////////////////////////////////////////////////////////////////////


void ABaseCameraActor::InitCameraData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(BaseStageInfo->MapTableKey);
	if (MapTableInfo)
	{
		CamLongDist = MapTableInfo->CamLongDist;
		CamLongAngle = MapTableInfo->CamLongAngle;
		CamLongFOV = MapTableInfo->CamLongFOV;

		CamShortDist = MapTableInfo->CamShortDist;
		CamShortAngle = MapTableInfo->CamShortAngle;
		CamShortFOV = MapTableInfo->CamShortFOV;

		if (0.0f != MapTableInfo->StartRot)
		{
			FVector CameraAngle = GetActorRotation().Euler();
			CameraAngle.Z = MapTableInfo->StartRot;
			FQuat NewCameraRotation = FQuat::MakeFromEuler(CameraAngle);
			SetActorRotation(NewCameraRotation);
		}

		FString str2 = TEXT("CamLongDist : ") + FString::SanitizeFloat(CamLongDist) + TEXT("MapTableKey : ") + BaseStageInfo->MapTableKey.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, str2);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Invalid MapTableKey"));

		Camera_OriginDistance = 1600.f;
		CamLongDist = Camera_OriginDistance;
		CamLongAngle = AngleMaxBase;
		if (CameraComponent.IsValid())
			CamLongFOV = CameraComponent->FieldOfView;

		CamShortDist = Camera_OriginDistance;
		CamShortAngle = AngleMaxBase;
		if (CameraComponent.IsValid())
			CamShortFOV = CameraComponent->FieldOfView;
	}
}

void ABaseCameraActor::ChangeCameraOriginDistance(bool bLongDistance, bool bAnimation)
{
	if (bLongDistance)
	{
		ChangeFOV(CamLongFOV);
		Camera_Angle = CamLongAngle;
		Camera_OriginDistance = CamLongDist;

		if (bAnimation)
		{
			AutoZoomCameraByValue(CamLongDist, 0.3f, Dynamic_ZoomExp);
		}
		else
		{
			SetCameraDist(CamLongDist);
		}
	}
	else
	{
		ChangeFOV(CamShortFOV);
		Camera_Angle = CamShortAngle;
		Camera_OriginDistance = CamShortDist;

		if (bAnimation)
		{
			AutoZoomCameraByValue(CamShortDist, 0.3f, Dynamic_ZoomExp);
		}
		else
		{
			SetCameraDist(CamShortDist);
		}
	}
}

void ABaseCameraActor::SetDynamicCamera(bool bEnable)
{
	bool need_compute_dynamic_cam = (false == IsDynamicCamera && true == bEnable);

	IsDynamicCamera = bEnable;
	SetDragLock(bEnable);

	if (need_compute_dynamic_cam)
	{
		ComputeDynamicCamera();
	}
}

void ABaseCameraActor::MoveStageCamera(FVector InTargetLocation, float InMoveTime)
{
	StartLocation = GetActorLocation();
	this->TargetLocation = InTargetLocation;

	if (SpringArmComponent && IsOwnerDelayCamera == false)
	{
		SpringArmComponent->CameraLagSpeed = 5;
	}

	IsMoving = true;
	this->MoveTime = InMoveTime;
	if (MoveTime <= 0.0f)
	{
		//MoveTime = 0.1f;

		FVector newPosition = InTargetLocation;
		newPosition.Z = GetActorLocation().Z;
		SetActorLocation(newPosition);
		IsMoving = false;
	}
		
	MoveElapsedTime = 0.0f;
	
	StartRotation = GetActorRotation();
	TargetRotation = StartRotation;
}

void ABaseCameraActor::TransformStageCamera(FTransform NewCameraTransform, float InMoveTime, float InMoveExp/* = 1.0f*/)
{
	StartLocation = GetActorLocation();
	this->TargetLocation = NewCameraTransform.GetLocation();

	IsMoving = true;
	this->MoveTime = InMoveTime;
	if (MoveTime < 0.1f)
	{
		IsMoving = false;
		SetActorTransform(NewCameraTransform);
		return;
	}
	MoveElapsedTime = 0.0f;
	MoveExp = InMoveExp;

	StartRotation = GetActorRotation();
	TargetRotation = NewCameraTransform.GetRotation().Rotator();
}

void ABaseCameraActor::AutoZoomCameraByValue(float InZoomValue, float InZoomTime, float InZoomExp/* = 1.0f*/)
{
	ZoomMaxValue = InZoomValue;
	//Value Min,Max
	
	ZoomOriginValue = Camera_Distance;

	this->ZoomTime = InZoomTime;
	if (ZoomTime < 0.1f)
		ZoomTime = 0.1f;
	ZoomElapsedTime = 0.0f;
	ZoomExp = InZoomExp;
	IsAutoZoom = true;
}


void ABaseCameraActor::AutoZoomInOutCameraByPercent(bool InZoomInOut, 
	float InZoomPercent, float InStartTime, float InDurationTime, float InEndTime, 
	UCurveFloat* InCurveAsset, UCurveFloat* InAddCurveAsset /*=nullptr*/)
{
	if (!IsRecoveryZoomValue)
	{
		if (IsAutoZoomInOut == false)
		{
			ZoomOriginValue = Camera_Distance;
		}
	}

	IsAutoZoomInOut = true;
	ZoomInOutPercent = InZoomPercent;
	ZoomInOutStartTime = InStartTime;
	ZoomInOutDurationTime = InDurationTime;
	ZoomInOutEndTime = InEndTime;
	Curve_ZoomInOut = InCurveAsset;
	AddCurve_ZoomInOut = InAddCurveAsset;

	ZoomInOutElapsedTime = 0.0f;
	float TargetDistance = Camera_OriginDistance; 
	//float TargetDistance = ((IsSequenceCameraState) ? Camera_SequenceDistance : Camera_OriginDistance);
	if (IsSequenceCameraState)
	{
		TargetDistance = Camera_Distance;
		Camera_SequenceDistance = Camera_Distance;
	}

	if (InZoomInOut)
	{
		ZoomInOutTargetValue = TargetDistance - (TargetDistance * InZoomPercent);
	}
	else
	{
		ZoomInOutTargetValue = TargetDistance + (TargetDistance * InZoomPercent);
	}
}

void ABaseCameraActor::PingPongCameraForward(FVector InTargetLocation, float InMoveTime)
{
	StartLocation = GetActorLocation();
	this->TargetLocation = InTargetLocation;

	IsMoving = true;
	
	this->MoveTime = InMoveTime;
	if (MoveTime < 0.1f)
		MoveTime = 0.1f;
	MoveElapsedTime = 0.0f;

	StartRotation = GetActorRotation();
	TargetRotation = StartRotation;

	UsePingPongCamera = true;
	PingPongCameraBackUpStartLocation = StartLocation;
}

void ABaseCameraActor::PingPongCameraBackward()
{
	StartLocation = GetActorLocation();
	this->TargetLocation = PingPongCameraBackUpStartLocation;

	IsMoving = true;
	this->MoveTime = 0.5f;
	if (MoveTime < 0.1f)
		MoveTime = 0.1f;
	MoveElapsedTime = 0.0f;

	StartRotation = GetActorRotation();
	TargetRotation = StartRotation;

	UsePingPongCamera = false;
	AStagePlayerController* StagePlayerController = Cast<AStagePlayerController>(BasePlayerController.Get());
	if( IsValid(StagePlayerController))
		StagePlayerController->ActorSettingOnTheCamera(StagePlayerController->GetLeaderControlHelper()->LeaderCharacter.Get());
}

AActor* ABaseCameraActor::GetOwnerCharacter()
{
	return OwnerCharacter;
}

void ABaseCameraActor::SetOwnerCharacter(AActor* InCharacter)
{
	OwnerCharacter = InCharacter;

	if (IsAutoZoomInOut)
	{
		IsAutoZoomInOut = false;
		ZoomInAndOut(Camera_OriginDistance, false);

	}
}

#ifdef WITH_EDITOR
void ABaseCameraActor::SavePrevCameraDistance()
{
	if(PrevCameraDistance == 0)
		PrevCameraDistance = Camera_Distance;
}

void ABaseCameraActor::SetPrevCameraDistance()
{
	if(0 < PrevCameraDistance)
	{
		ZoomInAndOut(PrevCameraDistance, false);
		PrevCameraDistance = 0.0f;
	}
	else
		ZoomInAndOut(Camera_OriginDistance, false);

}
#endif // WITH_EDITOR

void ABaseCameraActor::ChangeFOV(float FOV)
{
	if ( CameraComponent.IsValid() )
	{
		CameraComponent->FieldOfView = FOV;
	}
}

void ABaseCameraActor::ZoomInOut()
{
	if (TouchCount > 1)
	{
		if (TouchStartPos.Num() > 1 && TouchMovePos.Num() > 1)
		{
			FVector first = TouchStartPos[0] - TouchStartPos[1];
			FVector second = TouchMovePos[0] - TouchMovePos[1];
			
			//Check ZoomIn or ZoomOut
			//float PrevDist = FVector::Dist(first, TouchPrevPos);
			float CurrentDist = FVector::Dist(first, second);

			if (CurrentDist > 0)
			{
				if (IsZoomIn == false)
				{
					second = TouchPrevPos;
					TouchStartPos[0] = TouchMovePos[0];
					TouchStartPos[1] = TouchMovePos[1];
				}
				
				IsZoomIn = true;
			}
			else
			{
				if (IsZoomIn == true)
				{
					second = TouchPrevPos;
					TouchStartPos[0] = TouchMovePos[0];
					TouchStartPos[1] = TouchMovePos[1];
				}
				IsZoomIn = false;
			}
			TouchPrevPos = second;
		

			float fDiff = (first.Size() - second.Size()) * Zoom_Speed;
			Camera_Distance += fDiff;
			
			RefreshSpringArm();

#if LOG_INPUT
			UE_LOG(LogBattle, Log, TEXT("ZoomInOut"));
#endif
		}
	}
}

void ABaseCameraActor::SetCameraDist(float fValue)
{
	Camera_OriginDistance = fValue;
	
	ZoomInAndOut(fValue, false);
}

void ABaseCameraActor::ZoomInAndOut(float fValue, bool bIsAddValue/* = true*/)
{
	if (bIsAddValue)
	{
		Camera_Distance += fValue;
	}
	else
	{
		Camera_Distance = fValue;
	}

	RefreshSpringArm();
}

void ABaseCameraActor::RefreshSpringArm()
{
	if (IsRefreshSpringArm == false)
		return;

	Camera_Distance = FMath::Clamp<float>(Camera_Distance, ZoomMinBase, ZoomMaxBase);

	if (IsValid(SpringArmComponent))
	{
		SpringArmComponent->TargetArmLength = Camera_Distance;
		SpringArmComponent->SetRelativeRotation(FRotator(Camera_Angle, 0.0f, 0.0f));
	}
}

bool ABaseCameraActor::IsMove(FVector startPos, FVector endPos, float fDiff)
{
	FVector diff = endPos - startPos;
	if (diff.SizeSquared() > fDiff * fDiff)
		return true;

	return false;
}

FVector ABaseCameraActor::GetMoveVectorBase(FVector startPos, FVector endPos)
{
	float XRate = (startPos.X - endPos.X) * YAxisScrollRateBase;
	float YRate = (endPos.Y - startPos.Y) * XAxisScrollRateBase;

	return GetActorForwardVector() * YRate + GetActorRightVector() * XRate;
}

void ABaseCameraActor::MoveCameraBase(FVector startPos, FVector endPos, FVector firstStartPos)
{
	FTransform transform;
	FVector location = GetMoveVectorBase(startPos, endPos);

	FVector NewCameraLocation = GetActorLocation() + location;
	if (!IsInCameraVolume(NewCameraLocation))
	{
		NewCameraLocation = BlockingBox.GetClosestPointTo(NewCameraLocation);
	}
	
	if (IsTrackingBlock)
	{
		float ZPos = NewCameraLocation.Z;
		ComputeCameraBlockingLocation(NewCameraLocation);
		NewCameraLocation.Z = ZPos;
	}

	SetActorLocation(NewCameraLocation);	

	if (IsMove(firstStartPos, endPos, 50.0f))
	{
		//UnSelect
		if (IsAutoMode)
		{
			OwnerCharacter = nullptr;
		}
	}
}


void ABaseCameraActor::ShowTargetEffect(bool bShow)
{
	if (IsValid(TargetEffect))
	{
		TargetEffect->SetVisibility(bShow);
	}
}

void ABaseCameraActor::SetTargetEffectLocation(FVector location)
{
	if (IsValid(TargetEffect))
	{
		FTransform transform;
		transform.SetLocation(location + FVector(0.0f, 0.0f, 1.0f));
		TargetEffect->SetWorldTransform(transform);
	}
}

void ABaseCameraActor::GetCameraBlockingVolume()
{
	for (TObjectIterator<ACameraBlockingVolume> Itr; Itr; ++Itr)
	{
		CameraBlockingVolume = Cast< ACameraBlockingVolume >(*Itr);
		if (CameraBlockingVolume.IsValid())
			break;
	}

	if (CameraBlockingVolume.IsValid())
		BlockingBox = FBox::BuildAABB(CameraBlockingVolume->GetBrushComponent()->Bounds.Origin, CameraBlockingVolume->GetBrushComponent()->Bounds.BoxExtent);
}

bool ABaseCameraActor::IsInCameraVolume(FVector NewTarget)
{
	if (CameraBlockingVolume == nullptr)
		return true;	
	return BlockingBox.IsInside(NewTarget);
}

void ABaseCameraActor::SetScreenEffect(UParticleSystem* ParticleSystem)
{
	if (IsValid(ParticleSystem) && ScreenEffectComponent.IsValid() )
	{
		ScreenEffectComponent->SetTemplate(ParticleSystem);
	}
}

void ABaseCameraActor::ScrollScreen(FVector Location, ETouchIndex::Type touchIndex)
{
	//test : Value
	auto Viewport				= CastChecked<ULocalPlayer>(BasePlayerController->Player)->ViewportClient->Viewport;
	float Velocity				= 10.0f;
	float ViewprotEdgeOffset	= 50.0f;
	auto ViewportSize = Viewport->GetSizeXY();

	if (!TouchMovePos.IsValidIndex(touchIndex))
		return;

	if (Location.X <= ViewprotEdgeOffset)
	{
		auto LocationDir = Location - TouchMovePos[touchIndex];
		if (LocationDir.X <= 0)
		{
			SetActorLocation(GetActorLocation() + (LocationDir)* Velocity);
			Viewport->SetMouse(ViewprotEdgeOffset, Viewport->GetMouseY());
		}
	}
	if (Location.X >= ViewportSize.X - ViewprotEdgeOffset)
	{
		auto LocationDir = Location - TouchMovePos[touchIndex];
		if (LocationDir.X >= 0)
		{
			SetActorLocation(GetActorLocation() + (LocationDir)* Velocity);
			Viewport->SetMouse(ViewportSize.X - ViewprotEdgeOffset, Viewport->GetMouseY());
		}
	}
	if (Location.Y <= ViewprotEdgeOffset)
	{
		auto LocationDir = Location - TouchMovePos[touchIndex];
		if (LocationDir.Y <= 0)
		{
			SetActorLocation(GetActorLocation() + (LocationDir)* Velocity);
			Viewport->SetMouse(Viewport->GetMouseX(), ViewprotEdgeOffset);
		}
	}
	if (Location.Y >= ViewportSize.Y - ViewprotEdgeOffset)
	{
		auto LocationDir = Location - TouchMovePos[touchIndex];
		if (LocationDir.Y >= 0)
		{
			SetActorLocation(GetActorLocation() + (LocationDir)* Velocity);
			Viewport->SetMouse(Viewport->GetMouseX(), ViewportSize.Y - ViewprotEdgeOffset);
		}
	}
#if LOG_INPUT
	UE_LOG(LogBattle, Log, TEXT("ScrollScreen"));
#endif
}

void ABaseCameraActor::OnReadyToStart()
{
	bReadyToStart = true;
}

void ABaseCameraActor::ComputeDynamicCamera()
{
// 	UGameObjectManager* objectMgr = UUtilFunctionLibrary::GetGameObjectManagerFromWorld(this->GetWorld());
// 	if (IsValid(objectMgr) == false)
// 		return;

	float StartFuncTime = UGameplayStatics::GetWorldDeltaSeconds(this->GetWorld());

	FVector CamCenter = GetActorLocation();// FVector(0, 0, 0);
	CamCenter.Z = 0;
	//Set CameraFocus Point

	TArray<TWeakObjectPtr<AActor>> CharacterList;
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->GetObjectList(CharacterList);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.GetObjectList(CharacterList);
	}
// 	objectMgr->GetObjectList(CharacterList);


	int32 IndexMax = (int32)ECharacterDirection4REnum::VE_End;
	TArray<FCheckCamera> CheckCharacterList;
	CheckCharacterList.SetNumZeroed(IndexMax);
	float	CameraRenge = 0.0f;
		
	for (auto Element : CharacterList)
	{
		if(Element == nullptr)
			continue;
		FVector TargetLoc = Element->GetActorLocation();
		TargetLoc.Z = 0;

		FVector SourceDir = GetActorForwardVector();
		FVector destDir = TargetLoc - CamCenter;
		destDir = destDir.GetSafeNormal();
			

		float targetRange = FVector::Dist(TargetLoc, CamCenter);
		//float targetDegree = UUtilFunctionLibrary::GetBetweenDegree(SourceDir, destDir);
		ECharacterDirection4REnum targetDir = UUtilFunctionLibrary::GetBetweenDir4R(SourceDir, destDir);
		int32 targetDirNum = (int32)targetDir;

		if (CheckCharacterList[targetDirNum].Character == nullptr)
		{
			CheckCharacterList[targetDirNum].Character = Element.Get();
			CheckCharacterList[targetDirNum].Range = targetRange;
		}
		else
		{
			if (targetRange > CheckCharacterList[targetDirNum].Range)
			{
				CheckCharacterList[targetDirNum].Character = Element.Get();
				CheckCharacterList[targetDirNum].Range = targetRange;
			}
		}

		float CenterPositionRenge = FVector::Dist(TargetLoc, FVector::ZeroVector);
		if (CenterPositionRenge >= CameraRenge)
			CameraRenge = CenterPositionRenge;
	}
	SetCameraDist(CameraRenge*5.0f);
	//ComputeZoom();
	if (CharacterList.Num() <= 2)
	{
		AutoZoomCameraByValue(Camera_OriginDistance, Dynamic_ZoomTime, Dynamic_ZoomExp);
	}
	else
	{
		float MaxRange = 0.0f;
		for (int32 i = 0; i + 2 < IndexMax; ++i)
		{
			if (CheckCharacterList[i].Character != nullptr
				&& CheckCharacterList[i + 2].Character != nullptr)
			{
				float CurrentRange = CheckCharacterList[i].Range + CheckCharacterList[i + 2].Range;
				if (CurrentRange > MaxRange)
					MaxRange = CurrentRange;
			}
		}
		if(MaxRange > 0)
		{
			float fZoValue = MaxRange - CheckDynamic_Range;
			AutoZoomCameraByValue(Camera_OriginDistance + fZoValue, Dynamic_ZoomTime, Dynamic_ZoomExp);
		}
		else
		{
			//AutoZoomCameraByValue(Camera_OriginDistance, Dynamic_ZoomTime, Dynamic_ZoomExp);
		}
	}

	//ComputeMove();
	for (int32 i = 0; i < IndexMax; ++i)
	{
		if (CheckCharacterList[i].Character != nullptr)
		{
			CheckCharacterList[i].CheckLocation = CheckCharacterList[i].Character->GetActorLocation();
			CheckCharacterList[i].CheckLocation.Z = 0;
		}
		else
		{
			CheckCharacterList[i].CheckLocation = CamCenter;
			CheckCharacterList[i].bIsRedirectLocation = true;
		}
	}
	//CenterPoint from 3point
	FVector FrontV = CheckCharacterList[0].CheckLocation - CamCenter;
	FVector BackV = CheckCharacterList[2].CheckLocation - CamCenter;
	FVector CheckFBV = 0.5f * (BackV - FrontV);
	FVector FrontBackV = CheckCharacterList[0].CheckLocation + CheckFBV;
	
	FVector RightV = CheckCharacterList[1].CheckLocation - CamCenter;
	FVector LeftV = CheckCharacterList[3].CheckLocation - CamCenter;
	FVector CheckRLV = 0.5f * (LeftV - RightV);
	FVector RightLeftV = CheckCharacterList[1].CheckLocation + CheckRLV;
	
	FVector ResultV(FrontBackV.X, RightLeftV.Y, 0);

	
	bool bCheckX = false;
	if (CheckCharacterList[0].bIsRedirectLocation
		|| CheckCharacterList[2].bIsRedirectLocation)
	{
		/*if (CharacterList.Num() > 2)
		{

		}
		else*/
		{
			if (CheckCharacterList[1].bIsRedirectLocation == false
				&& CheckCharacterList[3].bIsRedirectLocation == false)
			{
				ResultV.X = RightLeftV.X;
			}
			else
			{
				ResultV.X = GetActorLocation().X;
				bCheckX = true;
			}
		}
	}

	bool bCheckY = false;
	if (CheckCharacterList[1].bIsRedirectLocation
		|| CheckCharacterList[3].bIsRedirectLocation)
	{
		/*if (CharacterList.Num() > 2)
		{

		}
		else*/
		{
			if (CheckCharacterList[0].bIsRedirectLocation == false
				&& CheckCharacterList[2].bIsRedirectLocation == false)
			{
				ResultV.Y = FrontBackV.Y;
			}
			else
			{
				ResultV.Y = GetActorLocation().Y;
				bCheckY = true;
			}
		}
	}

	if (bCheckX && bCheckY)
	{
		AActor* Source = nullptr;
		AActor* Dest = nullptr;
		for (int32 i = 0; i < IndexMax; ++i)
		{
			if (CheckCharacterList[i].Character != nullptr)
			{
				if (Source == nullptr)
				{
					Source = CheckCharacterList[i].Character;
					continue;
				}
				if (Dest == nullptr)
				{
					Dest = CheckCharacterList[i].Character;
					continue;
				}
			}
		}

		if (Source && Dest)
		{
			FVector SourceLoc = Source->GetActorLocation();
			SourceLoc.Z = 0;
			FVector DestLoc =  Dest->GetActorLocation();
			DestLoc.Z = 0;
			FVector RightVec = SourceLoc - CamCenter;
			FVector LeftVec = DestLoc - CamCenter;
			FVector CheckRLVec = 0.5f * (LeftVec - RightVec);
			FVector RightLeftVec = SourceLoc + CheckRLVec;
			ResultV = RightLeftVec;
		}
		else if(Source)
		{
			ResultV = Source->GetActorLocation();
		}
	}
	ResultV.Z = GetActorLocation().Z;
	
	MoveStageCamera(ResultV, Dynamic_MoveTime);
	//SetActorLocation(ResultV);
	/*
	//Debug
	float CurrentFuncTime = UGameplayStatics::GetWorldDeltaSeconds(this->GetWorld());
	//UE_LOG(LogBattle, Log, TEXT("StartTime - %f ------- CurrentTime - %f"), StartFuncTime, CurrentFuncTime);

	FVector Fvec = GetActorForwardVector();//GetActorLocation() + GetActorForwardVector() * 500.0f;
	Fvec = Fvec.RotateAngleAxis(45, FVector(0, 0, 1));
	for (int32 i = 0; i < 4; ++i)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Fvec * 500.0f, FColor(1, 0, 0), false, 1.0f);
		Fvec = Fvec.RotateAngleAxis(90, FVector(0, 0, 1));
	}
	*/
}


void ABaseCameraActor::SetCameraBlocking(bool InOn, int32 InIndex /*= 0*/)
{
	IsTrackingBlock = InOn;

	int32 IndexNum = 0;
	for (auto Element : TrackingCameraBlocking)
	{
		if (Element.BlockingIndex == InIndex)
		{
			InIndex = IndexNum;
			break;
		}
		++IndexNum;
	}

	BlockIndex		= InIndex;
}

void ABaseCameraActor::ComputeCameraBlockingLocation(FVector& InOutLocation)
{
	if (TrackingCameraBlocking.IsValidIndex(BlockIndex) == false)
		return;
	
	FVector TrackingLocation = InOutLocation - TrackingCameraBlocking[BlockIndex].CenterLocation;
	TrackingLocation.Z = InOutLocation.Z;

	float SizeX = FMath::Sqrt(TrackingLocation.X * TrackingLocation.X);
	float SizeY = FMath::Sqrt(TrackingLocation.Y * TrackingLocation.Y);

	float CheckSizeX = FMath::Sqrt(TrackingCameraBlocking[BlockIndex].RangeX * TrackingCameraBlocking[BlockIndex].RangeX);
	float CheckSizeY = FMath::Sqrt(TrackingCameraBlocking[BlockIndex].RangeY * TrackingCameraBlocking[BlockIndex].RangeY);

	bool bResultSum = false;
	bool bSumX = false;
	bool bSumY = false;
	if (SizeX > CheckSizeX)
	{
		bResultSum = true;
		bSumX = true;
	}

	if (SizeY > CheckSizeY)
	{
		bResultSum = true;
		bSumY = true;
	}

	if (bResultSum)
	{
		if (bSumX)
		{
			TrackingLocation.X = GetActorLocation().X;
		}
		else
			TrackingLocation.X = InOutLocation.X;
		
		if(bSumY)
		{
			TrackingLocation.Y = GetActorLocation().Y;
		}
		else
			TrackingLocation.Y = InOutLocation.Y;

		InOutLocation = TrackingLocation;
	}
}

void ABaseCameraActor::ResetAutoZoomSetting()
{
	IsAutoZoom = false;
	ZoomTime = 1.0f;
	ZoomElapsedTime = 0.0f;
	ZoomExp = 1.0f;
}

void ABaseCameraActor::AddCameraBlockingInfo(FTrackingCameraBlocking InBlock)
{
	TrackingCameraBlocking.Emplace(InBlock);
}

void ABaseCameraActor::AddCameraLocationInfo(FTrackingCameraBlocking InBlock)
{
	AddCameraLocation.Emplace(InBlock);
}

void ABaseCameraActor::IsInCheckLocation()
{
	if (AddCameraLocation.Num() == 0)
		return;

	FVector TrackingLocation = GetActorLocation() - AddCameraLocation[0].CenterLocation;

	float SizeX = FMath::Sqrt(TrackingLocation.X * TrackingLocation.X);
	float SizeY = FMath::Sqrt(TrackingLocation.Y * TrackingLocation.Y);

	float CheckSizeX = FMath::Sqrt(AddCameraLocation[0].RangeX * AddCameraLocation[0].RangeX);
	float CheckSizeY = FMath::Sqrt(AddCameraLocation[0].RangeY * AddCameraLocation[0].RangeY);

	bool bResultSum = ((SizeX > CheckSizeX) && (SizeY > CheckSizeY));
	bool bOn = true;
	if (bResultSum != bOn)
	{
		bOn = bResultSum;
		//auto UUIFunctionLibrary::GetInGameHUD()->OnSpecial(true);
		//todo : incomplete func
	}
}

void ABaseCameraActor::InitFadeOutOnBeginPlay()
{
	// Start timer
	if (bEnableFadeOut)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		TimerManager.SetTimer(FadeCheckTimerHandle, this, &ABaseCameraActor::CheckForFadeOut, FadeOutCheckInterval, true);
	}
}

void ABaseCameraActor::ClearFadeOutOnEndPlay()
{
	// End timer
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(FadeCheckTimerHandle); // Hopefully ok with invalid timer handle due to !bEnableFadeOut

	// Clear things
	ActorsCurrentlyFadedOut.Reset();
}

void ABaseCameraActor::CheckForFadeOut()
{
	// Handle occluding of FadableStaticMeshActors
	if (IsValid(OwnerCharacter) && CameraComponent.IsValid()) {
		// Raycast test to the OwnerCharacter
		FVector Start = CameraComponent->GetComponentLocation();
		FVector End = OwnerCharacter->GetActorLocation();

		FCollisionQueryParams Params(FName(TEXT("LineTraceForFadableActors")));

		FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldStatic);
		
		UWorld* World = GetWorld();
		if (!IsValid(World))
			return;
		bool const bHit = World->LineTraceMultiByObjectType(HitResultsArray, Start, End, ObjectParams, Params);

		if (bHit)
		{
			for (int32 i = 0; i < HitResultsArray.Num(); i++)
			{
				if (HitResultsArray[i].Actor.IsValid())
				{
					IFadableInterface* HitActor = Cast<IFadableInterface>(HitResultsArray[i].Actor.Get());
					if (HitActor != nullptr)
					{
						FadableActorsArray.Add(Cast<AActor>(HitActor)); // better be
						if(!ActorsCurrentlyFadedOut.Contains(Cast<AActor>(HitActor)))
						{
							HitActor->FadeOut();
						}
					}
				}
			}
		}

		HitResultsArray.Reset();
		
		// ActorsCurrentlyFadedOut is modified during iteration
		for (int32 i = 0; i < ActorsCurrentlyFadedOut.Num();)
		{
			IFadableInterface* FadedOut = Cast<IFadableInterface>(ActorsCurrentlyFadedOut[i]); // better be
			if (!FadableActorsArray.Contains(Cast<AActor>(FadedOut)))
			{
				FadedOut->FadeIn();
				ActorsCurrentlyFadedOut.RemoveAtSwap(i);
			}
			else
			{
				i++;
			}
		}

		ActorsCurrentlyFadedOut.Append(FadableActorsArray);
		FadableActorsArray.Reset();
	}
}

void ABaseCameraActor::ArenaCameraZoomInOut()
{
	TArray<TWeakObjectPtr<AActor>> CharacterList;

// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->GetObjectList(CharacterList);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.GetObjectList(CharacterList);
	}

	float	CameraRenge = 0.0f;

	for (auto Element : CharacterList)
	{
		if (Element == nullptr)
			continue;
		FVector TargetLoc = Element->GetActorLocation();

		float CenterPositionRenge = FVector::Dist(TargetLoc, FVector::ZeroVector);
		if (CenterPositionRenge >= CameraRenge)
			CameraRenge = CenterPositionRenge;
	}

	float	Percent		=	CameraRenge / 900 * 100;
	Percent				=	FMath::Min(100.0f, Percent);
	float	LocationY	=	(2250 - 770)*Percent/100;
	float	LocationZ	=	(1350 - 420)*Percent/100;
	float	RotateY		=	10*Percent / 100;

	FTransform TargetTransform;
	FVector		Vector		=	{ 0.0f, 550.0f+ LocationY, 320.0f+ LocationZ };
	FVector		Rotation	=	{ 0.0f, -20-RotateY, -90.0f};

	SetActorLocation(Vector);
	FQuat NewCameraRotation = FQuat::MakeFromEuler(Rotation);
	SetActorRotation(NewCameraRotation);
	ZoomInAndOut(300.0f, false);
	if (SpringArmComponent)
	{
		SpringArmComponent->SetRelativeLocation(FVector::ZeroVector);
		SpringArmComponent->SetRelativeRotation(FQuat::Identity);
		SpringArmComponent->TargetArmLength = 300;
	}
}
