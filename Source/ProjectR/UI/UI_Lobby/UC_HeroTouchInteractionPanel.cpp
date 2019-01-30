#include "ProjectR.h"
#include "UC_HeroTouchInteractionPanel.h"

#include "UtilFunctionIntegrated.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UUC_HeroTouchInteractionPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(this, 0));

	//// Hook up to LobbyPlayerController touch inputs for spinning hero

	// Get touchable area
	HeroClickAreaLeftTop = UUIFunctionLibrary::GetPanelWidgetPos(this);
	FVector2D HeroClickAreaSize = UUIFunctionLibrary::GetPanelWidgetSize(this);
	HeroClickAreaRightBottom = HeroClickAreaLeftTop + HeroClickAreaSize;

	// Get IndoorLobbyStateActor
	auto TowerLobbyGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(TowerLobbyGameMode))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::NativeConstruct: failed to get TowerLobbyGameMode"));
		return;
	}
	if (TowerLobbyGameMode->CurrentLobbyState != ETowerLobbyStateEnum::Indoor)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::NativeConstruct: not in indoor state"));
		return;
	}
	IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetCurrentLobbyStateActor());
	if (!IndoorLobbyStateActor.IsValid())
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::NativeConstruct: failed to get IndoorLobbyStateActor"));
		return;
	}

	// get LobbyPlayerController
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::NativeConstruct: failed to get PlayerController"));
		return;
	}
	LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
	if (!LobbyPlayerController.IsValid())
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::NativeConstruct: failed to get LobbyPlayerController"));
		return;
	}

	// Hook up to touch events
	LobbyPlayerController->OnOnePointInput_Tap.AddDynamic(this, &UUC_HeroTouchInteractionPanel::OnOnePointInput_Tap);
	LobbyPlayerController->OnOnePointInput_Swipe.AddDynamic(this, &UUC_HeroTouchInteractionPanel::OnOnePointInput_Swipe);
	LobbyPlayerController->OnTwoPointInput_Pinch.AddDynamic(this, &UUC_HeroTouchInteractionPanel::OnTwoPointInput_Pinch);
}

void UUC_HeroTouchInteractionPanel::NativeDestruct()
{
	if (LobbyPlayerController.IsValid())
	{
		LobbyPlayerController->OnOnePointInput_Tap.RemoveAll(this);
		LobbyPlayerController->OnOnePointInput_Swipe.RemoveAll(this);
		LobbyPlayerController->OnTwoPointInput_Pinch.RemoveAll(this);

		LobbyPlayerController.Reset();
	}

	if (IndoorLobbyStateActor.IsValid())
	{
		IndoorLobbyStateActor.Reset();
	}

	Super::NativeDestruct();
}

void UUC_HeroTouchInteractionPanel::OnOnePointInput_Tap(FVector2D ScreenPosition, EInputEvent InputType)
{
	if (Swiping || Pinching)
		return;

	if (InputType == EInputEvent::IE_Pressed)
	{
		if (IsInHeroClickArea(ScreenPosition))
		{
			IndoorLobbyStateActor->HeroTapAction();
		}
	}
}

void UUC_HeroTouchInteractionPanel::OnOnePointInput_Swipe(FVector2D ScreenPosition, EInputEvent InputType)
{
	switch (InputType)
	{
	case EInputEvent::IE_Pressed:
		if (IsInHeroClickArea(ScreenPosition))
		{
			Swiping = true;
			PrevScreenPosition = ScreenPosition;
			break;
		}
	case EInputEvent::IE_Repeat:
	{
		if (!Swiping)
		{
			//UE_LOG(LogUI, Error, TEXT("UUC_HeroTouchInteractionPanel::OnOnePointInput_Swipe: not swiping"));
			return;
		}
		float Distance = (ScreenPosition.X - PrevScreenPosition.X);
		if (!FMath::IsNearlyZero(Distance))
		{
			IndoorLobbyStateActor->RotateHero(Distance * SwipeScreenPositionToYawMultiplier);
			PrevScreenPosition = ScreenPosition;
		}
	}
	break;
	case EInputEvent::IE_Released:
		Swiping = false;
		break;
	}
}

void UUC_HeroTouchInteractionPanel::OnTwoPointInput_Pinch(FVector2D ScreenPosition1, FVector2D ScreenPosition2, EInputEvent InputType)
{
	switch (InputType)
	{
	case EInputEvent::IE_Pressed:
		if (IsInHeroClickArea(ScreenPosition1) && IsInHeroClickArea(ScreenPosition2))
		{
			Pinching = true;
			PrevPinchingDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
		}
		break;

	case EInputEvent::IE_Repeat:
	{
		if (!Pinching)
		{
			UE_LOG(LogUI, Log, TEXT("UUC_HeroTouchInteractionPanel::OnTwoPointInput_Pinch: not pinching"));
			return;
		}
		float CurrentDistance = FVector2D::Distance(ScreenPosition1, ScreenPosition2);
		float DistanceDelta = CurrentDistance - PrevPinchingDistance;
		if (!FMath::IsNearlyZero(DistanceDelta))
		{
			IndoorLobbyStateActor->ZoomInToHero(DistanceDelta * PinchScreenDistanceToAlphaMultiplier);
			PrevPinchingDistance = CurrentDistance;
		}
	}
	break;

	case EInputEvent::IE_Released:
		Pinching = false;
		break;
	}
}

bool UUC_HeroTouchInteractionPanel::IsInHeroClickArea(const FVector2D& Position)
{
	if (HeroClickAreaLeftTop.X <= Position.X)
	{
		if (HeroClickAreaLeftTop.Y <= Position.Y)
		{
			if (HeroClickAreaRightBottom.X >= Position.X)
			{
				if (HeroClickAreaRightBottom.Y >= Position.Y)
				{
					return true;
				}
			}
		}
	}

	return false;
}
