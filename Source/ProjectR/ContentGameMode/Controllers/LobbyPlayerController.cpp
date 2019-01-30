// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LobbyPlayerController.h"
#include "Input/NormalTouchInput.h"
#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "CustomStructures/GameEnvironmentVar.h"
#include "UtilFunctionIntegrated.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UP_ControlBlock.h"
#include "UI/Common/UC_Particle.h"
#include "UI/UI_Common//UP_CommonPopup.h"
#include "NotifyList/NotifyEventList.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UI_Common/TopBar.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "GameState/TowerLobbyState.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(TEXT("ParticleSystem'/Game/FX/UI/FX_4_UI_ButtonClick.FX_4_UI_ButtonClick'"));
	ClickParticle = ParticleFinder.Object;
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RGameInstance = RGAMEINSTANCE(this);
}

void ALobbyPlayerController::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
	
// 	ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(RGAMEINSTANCE(this), FName("Lobby_In_DayTime_Ls"));
// 	if (level)
// 	{
// 		FString str = TEXT("bShouldBeLoaded : ") + FString::FromInt(level->bShouldBeLoaded) + TEXT(" bShouldBeVisible : ") + FString::FromInt(level->bShouldBeVisible);
// 		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, str);
// 	}

	if (0.0f < GestureDelayTime)
	{
		GestureDelayTime -= DeltaSecond;
		if (GestureDelayTime < 0.0f)
			GestureDelayTime = 0.0f;
	}

	if (bTickTest)
	{
		//UE_LOG(LogBattle, Log, TEXT("**** ALobbyPlayerController::Tick Start"));
		for (int32 i = (int32)ETouchIndex::Touch1; i < (int32)ETouchIndex::Touch10; ++i)
		{
			float x, y;
			bool bState = false;
			GetInputTouchState((ETouchIndex::Type)i, x, y, bState);
			//UE_LOG(LogBattle, Log, TEXT("Touch %d [%f][%f] State:[%d]"), i, x, y, bState);

			if (bState)
			{
				FString strText;
				strText = FString::Printf(TEXT("Touch %d [%f][%f] State:[%d]"), i, x, y, bState);
				UKismetSystemLibrary::PrintString(GetWorld(), strText);
			}
		}
		//UE_LOG(LogBattle, Log, TEXT("**** ALobbyPlayerController::Tick End"));
	}
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InputComponent)
	{
		InputComponent->ClearActionBindings();
	}

	Super::EndPlay(EndPlayReason);
}

void ALobbyPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	if (!bGamePaused && PlayerInput && InputHandler)
	{
		InputHandler->UpdateDetection(DeltaTime);
	}

	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
#if !PLATFORM_IOS
		FInputActionBinding& PrevKeyBinding = InputComponent->BindAction(TEXT("PrevKey"), IE_Pressed, this, &ALobbyPlayerController::CheckAndroidBack);
		PrevKeyBinding.bExecuteWhenPaused = true;
#endif
	}

	InputHandler = NewObject<UNormalTouchInput>(this);

	BIND_1P_ACTION(InputHandler, EGameKey::Tap, IE_Pressed, &ALobbyPlayerController::OnTapPressed);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Pressed, &ALobbyPlayerController::OnSwipeStarted);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Repeat, &ALobbyPlayerController::OnSwipeUpdate);
	BIND_1P_ACTION(InputHandler, EGameKey::Swipe, IE_Released, &ALobbyPlayerController::OnSwipeReleased);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Pressed, &ALobbyPlayerController::OnPinchStarted);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Repeat, &ALobbyPlayerController::OnPinchUpdate);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Released, &ALobbyPlayerController::OnPinchReleased);
}

//////////////////////////////////////////////////////////////////////////

void ALobbyPlayerController::CheckAndroidBack()
{
	if (RGAMEINSTANCE(this)->ForceTutorialManager->IsForceTutorialing())
		return;

	auto RWidgetManager = RGAMEINSTANCE(this)->RWidgetManager;
	if (RWidgetManager == nullptr)
		return;

	UE_LOG(LogUI, Log, TEXT("CheckAndroidBack : GetCurrentUIPage() : %d"), (uint8)RWidgetManager->GetCurrentUIPage());

	if (RWidgetManager->GetCurrentUIPage() != EUIPageEnum::UIPage_LobbyIndoor)
	{
		UTopBar* TopBar = Cast<UTopBar>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
		if (!IsValid(TopBar))
			return;

		TopBar->PrevButton();
	}/*
	else
	{
		UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_ASK_GAME_QUIT), ECommonPopupType::VE_OKCANCEL);

		if (popup->IsValidLowLevel())
		{
			popup->OnClickOK.AddDynamic(this, &ALobbyPlayerController::OnQuitPopupOK);
			popup->OnClickCancel.AddDynamic(this, &ALobbyPlayerController::OnQuitPopupCancel);
		}
	}
//*/
}

void ALobbyPlayerController::OnQuitPopupOK()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	UE_LOG(LogUI, Log, TEXT("OnQuitPopupOK"));
	RGAMEINSTANCE(this)->GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void ALobbyPlayerController::OnQuitPopupCancel()
{
	UUIFunctionLibrary::HideCommonPopup();
}

//////////////////////////////////////////////////////////////////////////

void ALobbyPlayerController::OnTapPressed(const FVector2D& ScreenPosition, float DownTime)
{
	// UE_LOG(LogInputMgr, Log, TEXT("OnTapPressed Position %s"), *(ScreenPosition.ToString()));

	// TODO: fix this. These guards block spurious tap event after OnSwipeReleased.
	if (0.0f < GestureDelayTime)
		return;

	if (Swiping)
		return;

	OnOnePointInput_Tap.Broadcast(ScreenPosition, EInputEvent::IE_Pressed);
}

void ALobbyPlayerController::OnHoldPressed(const FVector2D& ScreenPosition, float DownTime)
{
//	UE_LOG(LogInputMgr, Log, TEXT("OnHoldPressed Position %s"), *(ScreenPosition.ToString()));
}

void ALobbyPlayerController::OnHoldReleased(const FVector2D& ScreenPosition, float DownTime)
{
//	UE_LOG(LogInputMgr, Log, TEXT("OnHoldReleased Position %s"), *(ScreenPosition.ToString()));
}

void ALobbyPlayerController::OnSwipeStarted(const FVector2D& AnchorPosition, float DownTime)
{
//	UE_LOG(LogInputMgr, Log, TEXT("OnSwipeStarted Position %s"), *(AnchorPosition.ToString()));

	Swiping = true;

	if (RGameInstance->GameEnvironmentVar.LobbyPinchTest)
	{
		OnTwoPointInput_Pinch.Broadcast(AnchorPosition, AnchorPosition, EInputEvent::IE_Pressed);
	}
	else
	{
		OnOnePointInput_Swipe.Broadcast(AnchorPosition, EInputEvent::IE_Pressed);
	}
}

void ALobbyPlayerController::OnSwipeUpdate(const FVector2D& ScreenPosition, float DownTime)
{
//	UE_LOG(LogInputMgr, Log, TEXT("OnSwipeUpdate Position %s"), *(ScreenPosition.ToString()));

	if (RGameInstance->GameEnvironmentVar.LobbyPinchTest)
	{
		if (IsValid(InputHandler))
		{
			OnTwoPointInput_Pinch.Broadcast(InputHandler->GetTouchAnchor(0), ScreenPosition, EInputEvent::IE_Repeat);
		}
	}
	else
	{
		OnOnePointInput_Swipe.Broadcast(ScreenPosition, EInputEvent::IE_Repeat);
	}
}

void ALobbyPlayerController::OnSwipeReleased(const FVector2D& ScreenPosition, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnSwipeReleased Position %s"), *(ScreenPosition.ToString()));
	
	GestureDelayTime = 0.3f;

	Swiping = false;

	if (RGameInstance->GameEnvironmentVar.LobbyPinchTest)
	{
		if (IsValid(InputHandler))
		{
			OnTwoPointInput_Pinch.Broadcast(InputHandler->GetTouchAnchor(0), ScreenPosition, EInputEvent::IE_Released);
		}
	}
	else
	{
		OnOnePointInput_Swipe.Broadcast(ScreenPosition, EInputEvent::IE_Released);
	}
}

void ALobbyPlayerController::OnSwipeTwoPointsStarted(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnSwipeTwoPointsStarted Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
}

void ALobbyPlayerController::OnSwipeTwoPointsUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnSwipeTwoPointsUpdate Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
}

void ALobbyPlayerController::OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnPinchStarted Position1 %s Position2 %s"), *(AnchorPosition1.ToString()), *(AnchorPosition2.ToString()));
	
	OnTwoPointInput_Pinch.Broadcast(AnchorPosition1, AnchorPosition2, EInputEvent::IE_Pressed);
}

void ALobbyPlayerController::OnPinchUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnPinchUpdate Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
	
	OnTwoPointInput_Pinch.Broadcast(ScreenPosition1, ScreenPosition2, EInputEvent::IE_Repeat);
}

void ALobbyPlayerController::OnPinchReleased(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
	UE_LOG(LogInputMgr, Log, TEXT("OnPinchReleased Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));

	OnTwoPointInput_Pinch.Broadcast(ScreenPosition1, ScreenPosition2, EInputEvent::IE_Released);

	GestureDelayTime = 0.3f;
}

//////////////////////////////////////////////////////////////////////////

FString ALobbyPlayerController::ConsoleCommand(const FString& Command, bool bWriteToLog)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return "";

	RGAMEINSTANCE(this)->ConsoleCheatManager->ConsoleCommand(Command, GetWorld());

	return Super::ConsoleCommand(Command, bWriteToLog);
}