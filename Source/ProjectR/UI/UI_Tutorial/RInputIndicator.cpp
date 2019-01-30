// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"
#include "Blueprint/WidgetLayoutLibrary.h"



#include "RInputIndicator.h"

void URInputIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputButton)
	{
		InputButton->OnClicked.RemoveDynamic(this, &URInputIndicator::BtnEventOnClicked);
		InputButton->OnClicked.AddDynamic(this, &URInputIndicator::BtnEventOnClicked);
		InputButton->OnPressed.RemoveDynamic(this, &URInputIndicator::BtnEventOnPressed);
		InputButton->OnPressed.AddDynamic(this, &URInputIndicator::BtnEventOnPressed);
		InputButton->OnReleased.RemoveDynamic(this, &URInputIndicator::BtnEventOnRelease);
		InputButton->OnReleased.AddDynamic(this, &URInputIndicator::BtnEventOnRelease);
	}
}

void URInputIndicator::NativeDestruct()
{
	/*InputButton = nullptr;
	Image_Icon = nullptr;
	HandAni_Ref = nullptr;*/

	Super::NativeDestruct();
}

void URInputIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(IsTickTemp)
	{
		if (IsAnimationPlaying(HandAni_Ref) == false)
		{
			PlayAnimation(HandAni_Ref);
		}
	}
}

void URInputIndicator::BtnEventOnClicked()
{
#if LOG_UIINPUT
	UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnClicked"));
#endif
	if (IndicatorTypeClicker || IsPressIcon)
	{
		return;
	}
#if LOG_UIINPUT
	UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnClicked ok"));
#endif
	IsClicked = true;
}

void URInputIndicator::BtnEventOnPressed()
{
#if LOG_UIINPUT
	UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnPressed"));
#endif
	
	if (IndicatorTypeClicker)
	{
		auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
		float X = 0.0f;
		float Y = 0.0f;

#if PLATFORM_WINDOWS
		BasePlayerController->GetMousePosition(X, Y);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		UE_LOG(LogBattle, Log, TEXT("URInputIndicator::GetMousePosition   x - %f y - %f"), X, Y);
		for (int32 i = 1; i < EKeys::NUM_TOUCH_KEYS; ++i)
		{
			FVector2D TouchLocation;
			FVector2D TouchLocation1;
			bool bPressed = false;
			bool bPressed1 = false;
			BasePlayerController->GetInputTouchState((ETouchIndex::Type)(i - 1), TouchLocation.X, TouchLocation.Y, bPressed);
			BasePlayerController->GetInputTouchState((ETouchIndex::Type)i, TouchLocation1.X, TouchLocation1.Y, bPressed1);

			if (!bPressed1)
			{
				FingerIndex = i - 1;
				X = TouchLocation.X;
				Y = TouchLocation.Y;
				break;
			}
		}
		UE_LOG(LogBattle, Log, TEXT("URInputIndicator::GetInputTouchState   x - %f y - %f"), X, Y);
#endif
#if LOG_UIINPUT
		UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnPressed definecheck"));
#endif
		DownPos = FVector2D(X, Y);
	}
	else if (IsPressIcon)
	{
		IsCheckRelease = true;
		//IsClicked = true;
		IsTempAttack = true;
	}
}

void URInputIndicator::BtnEventOnRelease()
{
#if LOG_UIINPUT
	UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnRelease"));
#endif
	if (IndicatorTypeClicker)
	{
		auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
		if (IsValid(BasePlayerController))
		{
			float X = 0.0f;
			float Y = 0.0f;

#if PLATFORM_WINDOWS
			BasePlayerController->GetMousePosition(X, Y);
#elif PLATFORM_IOS || PLATFORM_ANDROID
			UE_LOG(LogBattle, Log, TEXT("URInputIndicator::GetMousePosition   x - %f y - %f"), X, Y);
			FVector2D TouchLocation;
			bool bPressed = false;
			BasePlayerController->GetInputTouchState((ETouchIndex::Type)FingerIndex, TouchLocation.X, TouchLocation.Y, bPressed);

			X = TouchLocation.X;
			Y = TouchLocation.Y;
			FingerIndex = 0;
			UE_LOG(LogBattle, Log, TEXT("URInputIndicator::GetInputTouchState   x - %f y - %f"), X, Y);
#endif
#if LOG_UIINPUT
			UE_LOG(LogBattle, Log, TEXT("URInputIndicator::BtnEventOnRelease definecheck"));
#endif
			FVector2D curPos(X, Y);
			if (DownPos.Equals(curPos, 50.0f))
			{
				IsClicked = false;
			}
			else
			{
				IsClicked = true;
			}
		}

		//IsClicked = true;
	}
	else if (IsPressIcon)
	{
		IsCheckRelease = false;
	}
}

void URInputIndicator::SetIconImageSize(FVector2D InSize, bool InIsMovePad /*= false*/)
{
	/*if (IndicatorTypeClicker)
	{
		IsClicked = false;
		return;
	}*/

	if (InSize.IsNearlyZero())
	{
		IsRefreshSize = true;
	}
	else
	{
		IsRefreshSize = false;
	}

	//default value
	if (IndicatorTypeClicker)
		InSize = FVector2D(70, 70);

	auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_Icon);
	if (CanvasSlot)
	{
		CanvasSlot->SetSize(InSize);
	}

	if(InputButton)
	{
		auto ButtonCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InputButton);
		ButtonCanvasSlot->SetSize(InSize);
	}
	
	IsClicked = false;
}

void URInputIndicator::SetMovePad(bool InActive)
{
	auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_Icon);
	if (CanvasSlot)
	{
		if (InActive)
		{
			FAnchors Anchors;
			Anchors.Maximum.X = 0.5f;
			Anchors.Minimum.X = 0.5f;
			Anchors.Maximum.Y = 0.5f;
			Anchors.Minimum.Y = 0.5f;
			CanvasSlot->SetAnchors(Anchors);
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));
		}
		else
		{
			FAnchors Anchors;
			Anchors.Maximum.X = 0.0f;
			Anchors.Minimum.X = 0.0f;
			Anchors.Maximum.Y = 0.0f;
			Anchors.Minimum.Y = 0.0f;
			CanvasSlot->SetAnchors(Anchors);
			CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
			CanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));
		}
	}

	if (InputButton)
	{
		auto ButtonCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InputButton);
		if (InActive)
		{
			FAnchors Anchors;
			Anchors.Maximum.X = 0.5f;
			Anchors.Minimum.X = 0.5f;
			Anchors.Maximum.Y = 0.5f;
			Anchors.Minimum.Y = 0.5f;
			ButtonCanvasSlot->SetAnchors(Anchors);
			ButtonCanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			ButtonCanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));
		}
		else
		{
			FAnchors Anchors;
			Anchors.Maximum.X = 0.0f;
			Anchors.Minimum.X = 0.0f;
			Anchors.Maximum.Y = 0.0f;
			Anchors.Minimum.Y = 0.0f;
			ButtonCanvasSlot->SetAnchors(Anchors);
			ButtonCanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
			ButtonCanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));
		}
	}
}

void URInputIndicator::TempSlide()
{
	if(InputButton)
	{
		InputButton->SetVisibility(ESlateVisibility::Collapsed);
	}
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	RGameInstance->EventManager->OnSlotAction2.AddDynamic(this, &URInputIndicator::TempCheck);
}

void URInputIndicator::TempCheck(int32 HeroSlotIndex, AActor* SelectedCharacter)
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	RGameInstance->EventManager->OnSlotAction2.RemoveDynamic(this, &URInputIndicator::TempCheck);
	IsClicked = true;
}
