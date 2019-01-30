// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RCameraIcon.h"

#include "GlobalIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"


void URCameraIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(CameraView_Short))
	{
		CameraView_Short->OnClicked.RemoveDynamic(this, &URCameraIcon::OnClick_CameraView);
		CameraView_Short->OnClicked.AddDynamic(this, &URCameraIcon::OnClick_CameraView);
	}

	if (IsValid(CameraView_Dynamic))
	{
		CameraView_Dynamic->OnClicked.RemoveDynamic(this, &URCameraIcon::OnClick_CameraView);
		CameraView_Dynamic->OnClicked.AddDynamic(this, &URCameraIcon::OnClick_CameraView);
	}

	CheckGameMode = UUtilFunctionLibrary::GetGamePlayModeEnum();
	if (CheckGameMode == EGamePlayModeTypeEnum::VE_Arena || 
		CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		CurrentCameraViewType = ECameraType::VE_Dynamic;

		ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
		if (BasePlayerController)
		{
			ABaseCameraActor* Camera = BasePlayerController->GetCameraActor();
			if (Camera)
			{
				CameraInitLocation = Camera->GetActorLocation();
			}
		}
	}
	else
	{
		CurrentCameraViewType = ECameraType::VE_NormalLong;
	}

	RefreshCameraView();
}

void URCameraIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void URCameraIcon::RefreshCameraView()
{
	int32 WidgetIndex = (int32)CurrentCameraViewType;

	if (Switcher_IconImage)
	{
		Switcher_IconImage->SetActiveWidgetIndex(WidgetIndex);
	}

	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	if (BasePlayerController)
	{
		ABaseCameraActor* Camera = BasePlayerController->GetCameraActor();
		if (Camera)
		{
			if (CurrentCameraViewType == ECameraType::VE_Dynamic)
			{
				if (CheckGameMode == EGamePlayModeTypeEnum::VE_Arena || 
					CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
				{
					Camera->ChangeCameraOriginDistance(true);
					Camera->SetOwnerCharacter(nullptr);
					FTransform cam_transform(CameraInitLocation);
					Camera->TransformStageCamera(cam_transform, 0.5f, 0.5f);
				}
				Camera->SetDynamicCamera(true);
			}
			else if (CurrentCameraViewType == ECameraType::VE_NormalLong || CurrentCameraViewType == ECameraType::VE_NormalShort)
			{
				Camera->SetDynamicCamera(false);
				Camera->ChangeCameraOriginDistance((CurrentCameraViewType == ECameraType::VE_NormalLong));
			}
		}
	}
}

void URCameraIcon::OnClick_CameraView()
{
	if (CheckGameMode == EGamePlayModeTypeEnum::VE_Arena ||
		CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		if (CurrentCameraViewType == ECameraType::VE_Dynamic)
		{
			CurrentCameraViewType = ECameraType::VE_NormalLong;
		}
		else
		{
			CurrentCameraViewType = ECameraType::VE_Dynamic;
		}
	}
	else
	{
		if (CurrentCameraViewType == ECameraType::VE_NormalLong)
		{
			CurrentCameraViewType = ECameraType::VE_NormalShort;
		}
		else
		{
			CurrentCameraViewType = ECameraType::VE_NormalLong;
		}
	}

	RefreshCameraView();
}

