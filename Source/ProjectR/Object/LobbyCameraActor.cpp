// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LobbyCameraActor.h"

#include "GlobalIntegrated.h"


#include "Table/LobbyCharacterTableInfo.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "CustomStructures/GameEnvironmentVar.h"
#include "UtilFunctionIntegrated.h"
// Sets default values
ALobbyCameraActor::ALobbyCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyCameraActor::BeginPlay()
{
	Super::BeginPlay();

	auto FindComp = GetComponentByClass(UCameraComponent::StaticClass());
	if (IsValid(FindComp))
	{
		CameraComponent = Cast<UCameraComponent>(FindComp);
	}

	RGameInstance = RGAMEINSTANCE(this);
}

void ALobbyCameraActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ALobbyPlayerController* LobbyPlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(this->GetWorld());
	if (IsValid(LobbyPlayerController))
	{
		LobbyPlayerController->OnOnePointInput_Tap.RemoveDynamic(this, &ALobbyCameraActor::OnOnePointInput_Tap);
		LobbyPlayerController->OnOnePointInput_Swipe.RemoveDynamic(this, &ALobbyCameraActor::OnOnePointInput_Swipe);
		LobbyPlayerController->OnTwoPointInput_Pinch.RemoveDynamic(this, &ALobbyCameraActor::OnTwoPointInput_Pinch);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALobbyCameraActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (RestoringPinch)
	{
		RestoreLobbyCamera(DeltaTime);
	}
}

void ALobbyCameraActor::SetLobbyCharacter(FName InLobbyCharacterKey)
{
	if (!RGameInstance.IsValid())
		return;

	LobbyCharacterTableInfo = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRow(InLobbyCharacterKey);
	if (LobbyCharacterTableInfo == nullptr)
		return;

	InputSwipe = false;
	InputPinch = false;
	SwipeAnchor = FVector2D::ZeroVector;

	MaxPinchToCameraDistance = FVector::Distance(LobbyCharacterTableInfo->DefaultCameraPosition, LobbyCharacterTableInfo->ZoomInCameraPosition);
	CurPinchToCameraDistance = 0.0f;
	PrevPinchDistance = 0.0f;
	RestoringPinch = false;

	SetActorLocation(LobbyCharacterTableInfo->DefaultCameraPosition);
	SetActorRotation(LobbyCharacterTableInfo->DefaultCameraRotator);

	if (CameraComponent.IsValid())
	{
		//if (LobbyCharacterTableInfo->EnablePostProcessSetting)
		//{
		//	CameraComponent->PostProcessSettings.bOverride_DirectionalLightDirectionMovable = 1;
		//	CameraComponent->PostProcessSettings.bOverride_DirectionalLightIntensityMovable = 1;
		//	CameraComponent->PostProcessSettings.bOverride_DirectionalLightColorMovable = 1;
		//	CameraComponent->PostProcessSettings.DirectionalLightDirectionMovable = LobbyCharacterTableInfo->DirectionalLightDirectionMovable;
		//	CameraComponent->PostProcessSettings.DirectionalLightIntensityMovable = LobbyCharacterTableInfo->DirectionalLightIntensityMovable;
		//	CameraComponent->PostProcessSettings.DirectionalLightColorMovable = LobbyCharacterTableInfo->DirectionalLightColorMovable;

		//	CameraComponent->PostProcessSettings.bOverride_AmbientLightIntensityMovable = 1;
		//	CameraComponent->PostProcessSettings.bOverride_AmbientLightColorMovable = 1;
		//	CameraComponent->PostProcessSettings.AmbientLightIntensityMovable = LobbyCharacterTableInfo->AmbientLightIntensityMovable;
		//	CameraComponent->PostProcessSettings.AmbientLightColorMovable = LobbyCharacterTableInfo->AmbientLightColorMovable;

		//	CameraComponent->PostProcessSettings.bOverride_RimLightViewSpaceDirection = 1;
		//	CameraComponent->PostProcessSettings.bOverride_RimLightIntensity = 1;
		//	CameraComponent->PostProcessSettings.bOverride_RimLightColor = 1;
		//	CameraComponent->PostProcessSettings.bOverride_RimLightWidth = 1;
		//	CameraComponent->PostProcessSettings.bOverride_RimLightFallOff = 1;
		//	CameraComponent->PostProcessSettings.RimLightViewSpaceDirection = LobbyCharacterTableInfo->RimLightViewSpaceDirection;
		//	CameraComponent->PostProcessSettings.RimLightIntensity = LobbyCharacterTableInfo->RimLightIntensity;
		//	CameraComponent->PostProcessSettings.RimLightColor = LobbyCharacterTableInfo->RimLightColor;
		//	CameraComponent->PostProcessSettings.RimLightWidth = LobbyCharacterTableInfo->RimLightWidth;
		//	CameraComponent->PostProcessSettings.RimLightFallOff = LobbyCharacterTableInfo->RimLightFallOff;

		//	CameraComponent->PostProcessSettings.bOverride_DynamicShadowIntensity = 1;
		//	CameraComponent->PostProcessSettings.DynamicShadowIntensity = LobbyCharacterTableInfo->DynamicShadowIntensity;

		//	CameraComponent->PostProcessSettings.bOverride_FogColor = 1;
		//	CameraComponent->PostProcessSettings.bOverride_FogStartDistance = 1;
		//	CameraComponent->PostProcessSettings.bOverride_FogDensity = 1;
		//	CameraComponent->PostProcessSettings.bOverride_FogMaxOpacity = 1;
		//	CameraComponent->PostProcessSettings.FogColor = LobbyCharacterTableInfo->FogColor;
		//	CameraComponent->PostProcessSettings.FogStartDistance = LobbyCharacterTableInfo->FogStartDistance;
		//	CameraComponent->PostProcessSettings.FogDensity = LobbyCharacterTableInfo->FogDensity;
		//	CameraComponent->PostProcessSettings.FogMaxOpacity = LobbyCharacterTableInfo->FogMaxOpacity;
		//}

		CameraComponent->SetFieldOfView(LobbyCharacterTableInfo->FieldOfView);
	}

	ALobbyPlayerController* LobbyPlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(this->GetWorld());
	if (IsValid(LobbyPlayerController))
	{
		if (false == LobbyPlayerController->OnOnePointInput_Tap.IsAlreadyBound(this, &ALobbyCameraActor::OnOnePointInput_Tap))
		{
			LobbyPlayerController->OnOnePointInput_Tap.AddDynamic(this, &ALobbyCameraActor::OnOnePointInput_Tap);
		}
		if (false == LobbyPlayerController->OnOnePointInput_Swipe.IsAlreadyBound(this, &ALobbyCameraActor::OnOnePointInput_Swipe))
		{
			LobbyPlayerController->OnOnePointInput_Swipe.AddDynamic(this, &ALobbyCameraActor::OnOnePointInput_Swipe);
		}
		if (false == LobbyPlayerController->OnTwoPointInput_Pinch.IsAlreadyBound(this, &ALobbyCameraActor::OnTwoPointInput_Pinch))
		{
			LobbyPlayerController->OnTwoPointInput_Pinch.AddDynamic(this, &ALobbyCameraActor::OnTwoPointInput_Pinch);
		}
	}
}

void ALobbyCameraActor::SetLobbyPostProcessSetting()
{
	UWorld* InWorld = GetWorld();
	if (false == IsValid(InWorld))
		return;

	if (LobbyCharacterTableInfo == nullptr)
		return;

	if (LobbyCharacterTableInfo->EnablePostProcessSetting)
	{
		for (auto& postProcessVolume : InWorld->PostProcessVolumes)
		{
			APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
			if (PPVolume)
			{
				PPVolume->Settings.bOverride_DirectionalLightDirectionMovable = 1;
				PPVolume->Settings.bOverride_DirectionalLightIntensityMovable = 1;
				PPVolume->Settings.bOverride_DirectionalLightColorMovable = 1;
				PPVolume->Settings.DirectionalLightDirectionMovable = LobbyCharacterTableInfo->DirectionalLightDirectionMovable;
				PPVolume->Settings.DirectionalLightIntensityMovable = LobbyCharacterTableInfo->DirectionalLightIntensityMovable;
				PPVolume->Settings.DirectionalLightColorMovable = LobbyCharacterTableInfo->DirectionalLightColorMovable;

				PPVolume->Settings.bOverride_AmbientLightIntensityMovable = 1;
				PPVolume->Settings.bOverride_AmbientLightColorMovable = 1;
				PPVolume->Settings.AmbientLightIntensityMovable = LobbyCharacterTableInfo->AmbientLightIntensityMovable;
				PPVolume->Settings.AmbientLightColorMovable = LobbyCharacterTableInfo->AmbientLightColorMovable;

				PPVolume->Settings.bOverride_RimLightViewSpaceDirection = 1;
				PPVolume->Settings.bOverride_RimLightIntensity = 1;
				PPVolume->Settings.bOverride_RimLightColor = 1;
				PPVolume->Settings.bOverride_RimLightWidth = 1;
				PPVolume->Settings.bOverride_RimLightFallOff = 1;
				PPVolume->Settings.RimLightViewSpaceDirection = LobbyCharacterTableInfo->RimLightViewSpaceDirection;
				PPVolume->Settings.RimLightIntensity = LobbyCharacterTableInfo->RimLightIntensity;
				PPVolume->Settings.RimLightColor = LobbyCharacterTableInfo->RimLightColor;
				PPVolume->Settings.RimLightWidth = LobbyCharacterTableInfo->RimLightWidth;
				PPVolume->Settings.RimLightFallOff = LobbyCharacterTableInfo->RimLightFallOff;

				PPVolume->Settings.bOverride_DynamicShadowIntensity = 1;
				PPVolume->Settings.DynamicShadowIntensity = LobbyCharacterTableInfo->DynamicShadowIntensity;

				PPVolume->Settings.bOverride_FogColor = 1;
				PPVolume->Settings.bOverride_FogStartDistance = 1;
				PPVolume->Settings.bOverride_FogDensity = 1;
				PPVolume->Settings.bOverride_FogMaxOpacity = 1;
				PPVolume->Settings.FogColor = LobbyCharacterTableInfo->FogColor;
				PPVolume->Settings.FogStartDistance = LobbyCharacterTableInfo->FogStartDistance;
				PPVolume->Settings.FogDensity = LobbyCharacterTableInfo->FogDensity;
				PPVolume->Settings.FogMaxOpacity = LobbyCharacterTableInfo->FogMaxOpacity;
			}
		}
	}
}

void ALobbyCameraActor::SetLobbyCharacterClickArea(FVector2D InLeftTop, FVector2D InRightBottom)
{
	ClickAreaLeftTop = InLeftTop;
	ClickAreaRightBottom = InRightBottom;
}

void ALobbyCameraActor::LobbyCameraPinch(float InPinchDistance)
{
	if (LobbyCharacterTableInfo == nullptr)
		return;

	float DiffDistance = InPinchDistance - PrevPinchDistance;
	float CalcDiffDistance = DiffDistance * LobbyCharacterTableInfo->TouchSensitivity;

	if (LobbyCharacterTableInfo->ZoonInInvert)
	{
		CurPinchToCameraDistance -= DiffDistance;
	}
	else
	{
		CurPinchToCameraDistance += DiffDistance;
	}
	
	PrevPinchDistance = InPinchDistance;

//	UE_LOG(LogInputMgr, Log, TEXT("LobbyCameraPinch InPinchDistance %f PrevPinchDistance %f DiffDistance %f CurPinchToCameraDistance %f"), InPinchDistance, PrevPinchDistance, DiffDistance, CurPinchToCameraDistance);

	float LerpAlpha = CurPinchToCameraDistance / MaxPinchToCameraDistance;
	
	if (1.0f <= LerpAlpha)
	{
		LerpAlpha = 1.0f;
		CurPinchToCameraDistance = MaxPinchToCameraDistance;
	}
	else if (0.0f >= LerpAlpha)
	{
		LerpAlpha = 0.0f;
		CurPinchToCameraDistance = 0.0f;
	}
	
	FVector CurPosition = FMath::Lerp(LobbyCharacterTableInfo->DefaultCameraPosition, LobbyCharacterTableInfo->ZoomInCameraPosition, LerpAlpha);
	FRotator CurRotator = FMath::Lerp(LobbyCharacterTableInfo->DefaultCameraRotator, LobbyCharacterTableInfo->ZoomInCameraRotator, LerpAlpha);

	SetActorLocation(CurPosition);
	SetActorRotation(CurRotator);
}

void ALobbyCameraActor::RestoreLobbyCamera(float DeltaTime)
{
	if (LobbyCharacterTableInfo == nullptr)
		return;

	float RestoreValue = RGameInstance->GameEnvironmentVar.LobbyPinchRestoreSensitivity * MaxPinchToCameraDistance * DeltaTime;

	if (LobbyCharacterTableInfo->ZoonInInvert)
	{
		CurPinchToCameraDistance += RestoreValue;
	}
	else
	{
		CurPinchToCameraDistance -= RestoreValue;
	}
	

	if (0.0f < CurPinchToCameraDistance)
	{
		float LerpAlpha = CurPinchToCameraDistance / MaxPinchToCameraDistance;

		FVector CurPosition = FMath::Lerp(LobbyCharacterTableInfo->DefaultCameraPosition, LobbyCharacterTableInfo->ZoomInCameraPosition, LerpAlpha);
		FRotator CurRotator = FMath::Lerp(LobbyCharacterTableInfo->DefaultCameraRotator, LobbyCharacterTableInfo->ZoomInCameraRotator, LerpAlpha);

		SetActorLocation(CurPosition);
		SetActorRotation(CurRotator);
	}
	else
	{
		SetActorLocation(LobbyCharacterTableInfo->DefaultCameraPosition);
		SetActorRotation(LobbyCharacterTableInfo->DefaultCameraRotator);

		RestoringPinch = false;
	}
}

void ALobbyCameraActor::ResetLobbyCamera()
{
	if (LobbyCharacterTableInfo == nullptr)
		return;

	InputSwipe = false;
	InputPinch = false;
	SwipeAnchor = FVector2D::ZeroVector;

	CurPinchToCameraDistance = 0.0f;
	PrevPinchDistance = 0.0f;
	RestoringPinch = false;

	SetActorLocation(LobbyCharacterTableInfo->DefaultCameraPosition);
	SetActorRotation(LobbyCharacterTableInfo->DefaultCameraRotator);
}

//////////////////////////////////////////////////////////////////////////

bool ALobbyCameraActor::IsInClickArea(FVector2D& CheckPosition)
{
	if (ClickAreaLeftTop.X <= CheckPosition.X)
	{
		if (ClickAreaLeftTop.Y <= CheckPosition.Y)
		{
			if (ClickAreaRightBottom.X >= CheckPosition.X)
			{
				if (ClickAreaRightBottom.Y >= CheckPosition.Y)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void ALobbyCameraActor::OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType)
{
	if (!RGameInstance.IsValid())
		return;
	if (InputSwipe)
		return;
	if (InputPinch)
		return;

// 	if (IsInClickArea(ScreenPosition))
// 	{
// 		UE_LOG(LogInputMgr, Log, TEXT("OnOnePointInput_Tap Position %s"), *(ScreenPosition.ToString()));
// 		RGameInstance->EventManager->OnLobbyCharacterClick.Broadcast(false);
// 	}
}
void ALobbyCameraActor::OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType)
{
	if (!RGameInstance.IsValid())
		return;
	
	if (EInputEvent::IE_Pressed == InputType)
	{
		if (InputPinch)
			return;

		if (IsInClickArea(ScreenPosition))
		{
//			UE_LOG(LogInputMgr, Log, TEXT("OnOnePointInput_Swipe EInputEvent::IE_Pressed Position %s"), *(ScreenPosition.ToString()));
			SwipeAnchor = ScreenPosition;
		}
	}
	else if (EInputEvent::IE_Repeat == InputType)
	{
 		if (InputPinch)
 			return;
 
 		InputSwipe = true;
// 		RGameInstance->EventManager->OnLobbyCharacterSwipe.Broadcast(SwipeAnchor, ScreenPosition);
	}
	else if (EInputEvent::IE_Released == InputType)
	{
// 		if (InputSwipe)
// 		{
// 			UE_LOG(LogInputMgr, Log, TEXT("OnOnePointInput_Swipe EInputEvent::IE_Released Position %s"), *(ScreenPosition.ToString()));
// 			RGameInstance->EventManager->OnLobbyCharacterSwipeEnd.Broadcast();
// 		}
		InputSwipe = false;
	}
}
void ALobbyCameraActor::OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType)
{
	if (!RGameInstance.IsValid())
		return;

	if (EInputEvent::IE_Pressed == InputType)
	{
		if (IsInClickArea(ScreenPosition1) && IsInClickArea(ScreenPosition2))
		{
//			UE_LOG(LogInputMgr, Log, TEXT("OnTwoPointInput_Pinch EInputEvent::IE_Pressed Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
			InputPinch = true;
			PrevPinchDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
		}
	}
	else if (EInputEvent::IE_Repeat == InputType)
	{
		if (InputPinch)
		{
			float CurDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
			LobbyCameraPinch(CurDistance);
		}
	}
	else if (EInputEvent::IE_Released == InputType)
	{
		if (InputPinch)
		{
//			UE_LOG(LogInputMgr, Log, TEXT("OnTwoPointInput_Pinch EInputEvent::IE_Released Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
			PrevPinchDistance = 0.0f;
			//RestoringPinch = true;
		}
		InputPinch = false;
	}
}