#include "ProjectR.h"
#include "UC_OutdoorTouchInteractionPanel.h"

#include "UtilFunctionIntegrated.h"

#include "UI/UI_Lobby/UP_Lobby_Outdoor.h"
#include "UI/RWidgetManager.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UUC_OutdoorTouchInteractionPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(this, 0));

	//// Hook up to LobbyPlayerController touch inputs for spinning hero

	// Get touchable area
	ClickAreaLeftTop = UUIFunctionLibrary::GetPanelWidgetPos(this);
	FVector2D ClickAreaSize = UUIFunctionLibrary::GetPanelWidgetSize(this);
	ClickAreaRightBottom = ClickAreaLeftTop + ClickAreaSize;

	// Get IndoorLobbyStateActor
	auto TowerLobbyGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(TowerLobbyGameMode))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_OutdoorTouchInteractionPanel::NativeConstruct: failed to get TowerLobbyGameMode"));
		return;
	}
	if (TowerLobbyGameMode->CurrentLobbyState != ETowerLobbyStateEnum::Outdoor)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_OutdoorTouchInteractionPanel::NativeConstruct: not in outdoor state"));
		return;
	}
	OutdoorLobbyStateActor = Cast<AOutdoorLobbyStateActor>(TowerLobbyGameMode->GetCurrentLobbyStateActor());
	if (!OutdoorLobbyStateActor.IsValid())
	{
		UE_LOG(LogUI, Log, TEXT("UUC_OutdoorTouchInteractionPanel::NativeConstruct: failed to get OutdoorLobbyStateActor"));
		return;
	}

	// get LobbyPlayerController
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_OutdoorTouchInteractionPanel::NativeConstruct: failed to get PlayerController"));
		return;
	}
	LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
	if (!LobbyPlayerController.IsValid())
	{
		UE_LOG(LogUI, Log, TEXT("UUC_OutdoorTouchInteractionPanel::NativeConstruct: failed to get LobbyPlayerController"));
		return;
	}
	//OutdoorLobbyUMG = Cast<UUP_Lobby_Outdoor>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LobbyOutdoor));
	// Hook up to touch events
	LobbyPlayerController->OnOnePointInput_Tap.AddDynamic(this, &UUC_OutdoorTouchInteractionPanel::OnOnePointInput_Tap);
	LobbyPlayerController->OnOnePointInput_Swipe.AddDynamic(this, &UUC_OutdoorTouchInteractionPanel::OnOnePointInput_Swipe);
	//LobbyPlayerController->OnTwoPointInput_Pinch.AddDynamic(this, &UUC_OutdoorTouchInteractionPanel::OnTwoPointInput_Pinch);
}

void UUC_OutdoorTouchInteractionPanel::NativeDestruct()
{
	if (LobbyPlayerController.IsValid())
	{
		LobbyPlayerController->OnOnePointInput_Tap.RemoveAll(this);
		LobbyPlayerController->OnOnePointInput_Swipe.RemoveAll(this);
		//LobbyPlayerController->OnTwoPointInput_Pinch.RemoveAll(this);

		LobbyPlayerController.Reset();
	}

	if (OutdoorLobbyStateActor.IsValid())
	{
		OutdoorLobbyStateActor.Reset();
	}

	Super::NativeDestruct();
}

void UUC_OutdoorTouchInteractionPanel::OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType)
{
	if (Swiping || Pinching)
		return;

	if (InputType == EInputEvent::IE_Pressed)
	{
		if (IsInClickArea(ScreenPosition))
		{
			//OutdoorLobbyStateActor->HeroTapAction();
		}
	}
}

void UUC_OutdoorTouchInteractionPanel::OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType)
{
	switch (InputType)
	{
	case EInputEvent::IE_Pressed:
		if (IsInClickArea(ScreenPosition))
		{
			Swiping = true;
			PrevScreenPosition = ScreenPosition;
			break;
		}
	case EInputEvent::IE_Repeat:
	{
		//float Distance;
		if (!Swiping)
		{
			//UE_LOG(LogUI, Error, TEXT("UUC_HeroTouchInteractionPanel::OnOnePointInput_Swipe: not swiping"));
			return;
		}
		/*if (PortraitMode)
		{
			if ((ScreenPosition.X - PrevScreenPosition.X) >= (ScreenPosition.Y - PrevScreenPosition.Y))
			{
				Distance = ScreenPosition.Y - PrevScreenPosition.Y;
				if (!FMath::IsNearlyZero(Distance))
				{
					OutdoorLobbyStateActor->RotateTowerHorizontal(Distance * SwipeScreenPositionToYawMultiplier);
					PrevScreenPosition = ScreenPosition;
				}
			}
			else
			{
				Distance = ScreenPosition.X - PrevScreenPosition.X;
				if (!FMath::IsNearlyZero(Distance))
				{
					OutdoorLobbyStateActor->RotateTowerVertical(Distance * SwipeScreenPositionToYawMultiplier);
					PrevScreenPosition = ScreenPosition;
				}
			}
		}
		else
		{
			if ((ScreenPosition.X - PrevScreenPosition.X) >= (ScreenPosition.Y - PrevScreenPosition.Y))
			{
				Distance = ScreenPosition.X - PrevScreenPosition.X;
				if (!FMath::IsNearlyZero(Distance))
				{
					OutdoorLobbyStateActor->RotateTowerHorizontal(Distance * SwipeScreenPositionToYawMultiplier);
					PrevScreenPosition = ScreenPosition;
				}
			}
			else
			{
				Distance = ScreenPosition.Y - PrevScreenPosition.Y;
				if (!FMath::IsNearlyZero(Distance))
				{
					OutdoorLobbyStateActor->RotateTowerVertical(Distance * SwipeScreenPositionToYawMultiplier);
					PrevScreenPosition = ScreenPosition;
				}
			}
		}*/
		float Distance = PortraitMode ? (ScreenPosition.Y - PrevScreenPosition.Y) : (ScreenPosition.X - PrevScreenPosition.X);
		if (!FMath::IsNearlyZero(Distance))
		{
			OutdoorLobbyStateActor->RotateTower(Distance * SwipeScreenPositionToYawMultiplier);
			PrevScreenPosition = ScreenPosition;
		}
	}
	break;
	case EInputEvent::IE_Released:
		Swiping = false;
		break;
	}
}

//void UUC_HeroTouchInteractionPanel::OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType)
//{
//	switch (InputType)
//	{
//	case EInputEvent::IE_Pressed:
//		if (IsInHeroClickArea(ScreenPosition1) && IsInHeroClickArea(ScreenPosition2))
//		{
//			Pinching = true;
//			PrevPinchingDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
//		}
//		break;
//
//	case EInputEvent::IE_Repeat:
//	{
//		if (!Pinching)
//		{
//			UE_LOG(LogUI, Error, TEXT("UUC_HeroTouchInteractionPanel::OnTwoPointInput_Pinch: not pinching"));
//			return;
//		}
//		float CurrentDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
//		float DistanceDelta = CurrentDistance - PrevPinchingDistance;
//		if (!FMath::IsNearlyZero(DistanceDelta))
//		{
//			IndoorLobbyStateActor->ZoomInToHero(DistanceDelta * PinchScreenDistanceToAlphaMultiplier);
//			PrevPinchingDistance = CurrentDistance;
//		}
//	}
//	break;
//
//	case EInputEvent::IE_Released:
//		Pinching = false;
//		break;
//	}
//}

bool UUC_OutdoorTouchInteractionPanel::IsInClickArea(const FVector2D& Position)
{
	if (ClickAreaLeftTop.X <= Position.X && ClickAreaRightBottom.X >= Position.X && ClickAreaLeftTop.Y <= Position.Y && ClickAreaRightBottom.Y >= Position.Y)
		return true;
	else
		return false;
}
