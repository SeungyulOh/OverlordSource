// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RDragButton.h"

#include "RScrollPanel.h"
#include "UtilFunctionIntegrated.h"


void URDragButton::NativeConstruct()
{
	Super::NativeConstruct();

	//SetCanvasPanelSize(this, Size);
}

void URDragButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsDown)
	{
		//UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeTick"));

		FPointerEvent event;
		FSlateApplication::Get().ProcessTouchEndedEvent(event);

		//int32 a;
		//a = 10;
	}
}

//FReply URDragButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	IsDown = true;
//
//	OnButtonDown_Delegate.Broadcast(InMouseEvent.GetScreenSpacePosition());
//
//	if (IsInWidget)
//		return FReply::Handled();
//
//	return FReply::Unhandled();
//}
//
//FReply URDragButton::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	if (IsDown && IsInWidget)
//	{
//		OnButtonClicked_Delegate.Broadcast(InMouseEvent.GetScreenSpacePosition());
//	}
//
//	IsDown = false;
//	if (!IsMoved)
//	{
//	}
//	IsMoved = false;
//
//	OnButtonUp_Delegate.Broadcast(InMouseEvent.GetScreenSpacePosition());
//
//	return FReply::Unhandled();
//}
//
//FReply URDragButton::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	OnButtonMove_Delegate.Broadcast(InMouseEvent.GetScreenSpacePosition());
//
//	if (IsInWidget && IsDown)
//	{
//		IsMoved = true;
//		return FReply::Handled();
//	}
//
//	return FReply::Unhandled();
//}

void URDragButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsInWidget = true;

	UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeOnMouseEnter"));
}

void URDragButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	IsInWidget = false;
	//IsDown = false;

	UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeOnMouseLeave"));
}

FReply URDragButton::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeOnTouchStarted"));
	IsDown = true;
	IsMoved = false;
	DownPos = InGestureEvent.GetScreenSpacePosition();

	OnButtonDown_Delegate.Broadcast(InGestureEvent.GetScreenSpacePosition());

	return FReply::Handled();
}

FReply URDragButton::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	static int32 count = 0;
	UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeOnTouchMoved %d %d %d"), InGestureEvent.IsTouchEvent(), InGestureEvent.IsMouseButtonDown(FKey("LeftMouseButton")), ++count);
	OnButtonMove_Delegate.Broadcast(InGestureEvent.GetScreenSpacePosition());

	if (IsScrollEnable)
	{
		if (IsHovered())
		{
			if (ScrollPanel && ParentCanvasPanel)
			{
				FVector2D diff = InGestureEvent.GetScreenSpacePosition() - DownPos;
				ScrollPanel->SetScrollUpdate(ParentCanvasPanel, diff);
			}
			IsMoved = true;
			return FReply::Handled();
		}
	}
	else
	{
		if (!InGestureEvent.GetScreenSpacePosition().Equals(DownPos, UIMOVE_DIFF))
		{
			IsMoved = true;
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

FReply URDragButton::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{	
	UE_LOG(LogBattle, Log, TEXT("URDragButton::NativeOnTouchEnded"));
//#if !PLATFORM_IOS && !PLATFORM_ANDROID
	if (IsInWidget)
//#endif
	{
		if (IsScrollEnable)
		{
			if (InGestureEvent.GetScreenSpacePosition().Equals(DownPos, UIMOVE_DIFF))
			{
				OnButtonClicked_Delegate.Broadcast(InGestureEvent.GetScreenSpacePosition());
			}
		}
		else
		{
			if (!IsMoved)
			{
				OnButtonClicked_Delegate.Broadcast(InGestureEvent.GetScreenSpacePosition());
			}			
		}			
	}

	IsDown = false;
	if (IsMoved)
	{
		OnButtonClicked_Drag.Broadcast(InGestureEvent.GetScreenSpacePosition());
	}

	IsMoved = false;

	OnButtonUp_Delegate.Broadcast(InGestureEvent.GetScreenSpacePosition());

	return FReply::Unhandled();
}
