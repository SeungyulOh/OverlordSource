// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RScrollItem.h"
#include "RScrollView.h"

#include "WidgetBlueprintLibrary.h"

URScrollItem::URScrollItem(const class FObjectInitializer &PCIP)
	: Super(PCIP)	
	, ParentScrollView(NULL)
{
}

void URScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
	SetCanvasPanelSize(this, Size);

	if(ParentScrollView)
		IsSelectEnable = ParentScrollView->IsSelectEnable;
	else
		IsSelectEnable = true;

	IsSelected = false;
	IsDisabled = false;
}

void URScrollItem::NativeDestruct()
{
	//ParentScrollView = nullptr;

	Super::NativeDestruct();
}

void URScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsSizeUpdate)
	{
		SetSizeChange(WidgetRealSize, false);
	}

	if (IsTimeEvent)
	{
		if (IsDown && !IsExeTimeEvent)
		{
			CurTime -= InDeltaTime;
			if (CurTime <= 0.0f)
			{
				if (!IsMoved)
				{
					if (IsValid(ParentScrollView))
					{
						ParentScrollView->SetTimeEvent(this);
					}

					IsExeTimeEvent = true;
				}
			}
		}
	}
}

FReply URScrollItem::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if(IsDisabled)
		return FReply::Unhandled();

	//UE_LOG(LogBattle, Log, TEXT("URScrollItem::NativeOnTouchStarted %f %f"), InGestureEvent.GetScreenSpacePosition().X, InGestureEvent.GetScreenSpacePosition().Y);

	IsDown = true;
	IsMoved = false;
	IsExeTimeEvent = false;

	DownPos = InGestureEvent.GetScreenSpacePosition();

	/*
	if (IsValid(ParentScrollView))
	{
		ParentScrollView->ScrollItemOnTouchStarted();
	}
	*/

	if (IsTimeEvent)
	{
		CurTime = TimeEventTime;
	}

	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	
	return FReply::Unhandled();
}

FReply URScrollItem::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (IsDisabled)
		return FReply::Unhandled();

	//UE_LOG(LogBattle, Log, TEXT("URScrollItem::NativeOnTouchMoved %f %f"), InGestureEvent.GetScreenSpacePosition().X, InGestureEvent.GetScreenSpacePosition().Y);

	MovePos = InGestureEvent.GetScreenSpacePosition();

	if (!InGestureEvent.GetScreenSpacePosition().Equals(DownPos, UIMOVE_DIFF))
	{
		IsMoved = true;
	}

	/*
	if (IsInWidget)
	{
		ScrollUpdate(InGestureEvent);
		return FReply::Handled();
	}
	*/

	return FReply::Unhandled();
}

FReply URScrollItem::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (IsDisabled)
		return FReply::Unhandled();

	//UE_LOG(LogBattle, Log, TEXT("URScrollItem::NativeOnTouchEnded %f %f [%d]"), InGestureEvent.GetScreenSpacePosition().X, InGestureEvent.GetScreenSpacePosition().Y, IsMoved);

	IsDown = false;

	//if (InGestureEvent.GetScreenSpacePosition().Equals(DownPos, UIMOVE_DIFF))
	if(!IsMoved)
	{
		if (IsTimeEvent)
		{
			if (!IsExeTimeEvent)
			{
				if (IsSelectEnable)
				{
					Selected();
				}
			}
		}
		else
		{
			if (IsSelectEnable)
			{
				Selected();
			}
		}
	}

	IsExeTimeEvent = false;
	IsMoved = false;

	/*
	if (IsValid(ParentScrollView))
	{
		ParentScrollView->ScrollItemOnTouchEnded();
	}
	*/

	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Unhandled();
}

void URScrollItem::ScrollUpdate(const FPointerEvent& InEvent)
{	

	if (!IsValid(ParentScrollView))
		return;

	ParentScrollView->ScrollItemOnTouchMoved(InEvent);
}

void URScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	ItemIndex = Index;
	SetItemData(Index, item, ScrollView);
}

void URScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
#if WITH_EDITOR
	Editor_SetItemData(Index, item, ScrollView);
#endif
}

void URScrollItem::SetSelected(bool bSelected)
{
#if WITH_EDITOR
	Editor_SetSelected(bSelected);
#endif
}

void URScrollItem::ForceSelect()
{
	Selected();
}

void URScrollItem::Selected()
{
	OnSelectedItem.Broadcast(this);

	if (!IsValid(ParentScrollView))
		return;

	ParentScrollView->SetSelected(this);
}

void URScrollItem::SetSizeChange(FVector2D ChangeSize, bool bParentUpdate)
{
	Size = ChangeSize;

	SetCanvasPanelSize(this, Size);

	if (bParentUpdate)
	{
		if (!IsValid(ParentScrollView))
			return;

		ParentScrollView->SetScrollUpdate(this, FVector2D::ZeroVector);
	}
}

void URScrollItem::SetParentScrollView(URScrollView* InScrollView)
{
	ParentScrollView = InScrollView;
}