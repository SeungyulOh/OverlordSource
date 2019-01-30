// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RDragButton.h"
#include "UI/Control/RScrollPanel.h"


FMargin URScrollPanel::GetComposePanelSlot(UCanvasPanel* panel)
{
	FMargin rtMargin;

	if (IsValid(panel) && panel->Slot)
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(panel->Slot);
		if (canvasSlot)
		{
			const TArray<UPanelSlot*>& slots = panel->GetSlots();
			for (UPanelSlot* slot : slots)
			{
				UCanvasPanelSlot* childCanvasSlot = Cast<UCanvasPanelSlot>(slot);
				if (childCanvasSlot)
				{
					FMargin margin = childCanvasSlot->GetOffsets();
					if (rtMargin.Left >= margin.Left)
						rtMargin.Left = margin.Left;
					if (rtMargin.Top >= margin.Top)
						rtMargin.Top = margin.Top;

					if (rtMargin.Right <= margin.Left + margin.Right)
						rtMargin.Right = margin.Left + margin.Right;
					if (rtMargin.Bottom <= margin.Top + margin.Bottom)
						rtMargin.Bottom = margin.Top + margin.Bottom;
				}
			}
		}
	}

	return rtMargin;
}

void URScrollPanel::SeekDragButton(UCanvasPanel* panel)
{
	if (IsValid(panel) && panel->Slot)
	{
		TArray<UWidget*> outlist;
		GetHaveWidget(panel, outlist);
		for (UWidget* widget : outlist)
		{
			URDragButton* dragButton = Cast<URDragButton>(widget);
			if (dragButton)
			{
				dragButton->IsScrollEnable = true;
				dragButton->ScrollPanel = this;
				dragButton->ParentCanvasPanel = panel;
			}
		}
	}
}

void URScrollPanel::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshPanel();
}

void URScrollPanel::NativeDestruct()
{
	ScrollHorUsePanelList.Empty();
	ScrollVerUsePanelList.Empty();
	Super::NativeDestruct();
}

void URScrollPanel::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsInWidget = true;
}

void URScrollPanel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	IsInWidget = false;
	IsDown = false;
}

FReply URScrollPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBattle, Log, TEXT("URScrollPanel::NativeOnTouchStarted"));
	IsDown = true;
	DownPos = InGestureEvent.GetScreenSpacePosition();

	for (ScrollInfo::FScrollInfo& info : ScrollHorUsePanelList)
	{
		if (info.Panel)
		{
			if (info.Panel->IsHovered())
			{
				return FReply::Handled();
			}
		}
	}

	for (ScrollInfo::FScrollInfo& info : ScrollVerUsePanelList)
	{
		if (info.Panel)
		{
			if (info.Panel->IsHovered())
			{
				return FReply::Handled();
			}
		}
	}

	return FReply::Unhandled();
}

FReply URScrollPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBattle, Log, TEXT("URScrollPanel::NativeOnTouchMoved"));
	if (IsDown)
	{
		FVector2D diff = InGestureEvent.GetScreenSpacePosition() - DownPos;
		for (ScrollInfo::FScrollInfo& info : ScrollHorUsePanelList)
		{
			if (info.Panel)
			{
				if (info.Panel->IsHovered())
				{
					SetScrollUpdate(info, diff, true);
					return FReply::Handled();
				}
			}
		}

		for (ScrollInfo::FScrollInfo& info : ScrollVerUsePanelList)
		{
			if (info.Panel)
			{
				if (info.Panel->IsHovered())
				{
					SetScrollUpdate(info, diff, false);
					return FReply::Handled();
				}
			}
		}
	}

	return FReply::Unhandled();
}

FReply URScrollPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBattle, Log, TEXT("URScrollPanel::NativeOnTouchEnded"));
	IsDown = false;

	return FReply::Unhandled();
}

void URScrollPanel::SetScrollUpdate(UCanvasPanel* canvaspanel, FVector2D diff)
{
	if (IsValid(canvaspanel))
	{
		for (ScrollInfo::FScrollInfo& info : ScrollHorUsePanelList)
		{
			if (info.Panel == canvaspanel)
			{
				SetScrollUpdate(info, diff, true);
				break;
			}
		}

		for (ScrollInfo::FScrollInfo& info : ScrollVerUsePanelList)
		{
			if (info.Panel == canvaspanel)
			{
				SetScrollUpdate(info, diff, false);
				break;
			}
		}
	}
}

void URScrollPanel::SetScrollUpdate(ScrollInfo::FScrollInfo& info, FVector2D diff, bool bHor)
{
	if (IsValid(info.Panel) && info.Panel->Slot)
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(info.Panel->Slot);
		if (canvasSlot)
		{			
			FMargin marginSource = canvasSlot->GetOffsets();
			FMargin marginDest = GetComposePanelSlot(info.Panel);

			float fTotalDist = 0.0f;
			float fMoveDist = 0.0f;
			float fScrollPos = info.ScrollPos;
			if (bHor)
			{
				fTotalDist = marginDest.Right - marginDest.Left;
				fMoveDist = fTotalDist - marginSource.Right;
				fScrollPos += diff.X;
			}
			else
			{
				fTotalDist = marginDest.Bottom - marginDest.Top;
				fMoveDist = fTotalDist - marginSource.Bottom;
				fScrollPos += diff.Y;
			}

			//UE_LOG(LogBattle, Log, TEXT("URScrollPanel::SetScrollUpdate %f %f = %f %f"), diff.X, diff.Y, fTotalDist, fMoveDist);

			if (fMoveDist < 0.0f)
			{
				fScrollPos = 0.0f;
			}
			else
			{
				if (fScrollPos <= -fMoveDist)
					fScrollPos = -fMoveDist;
				else if (fScrollPos >= 0.0f)
					fScrollPos = 0.0f;
			}

			float fDiff = fScrollPos - info.ScrollPos;		

			for (int i = 0; i < info.Panel->GetChildrenCount(); ++i)
			{
				UWidget* widget = info.Panel->GetChildAt(i);
				if (IsValid(widget))
				{
					if (bHor)
						SetCanvasPanelXOffset(widget, fDiff);
					else
						SetCanvasPanelYOffset(widget, fDiff);
				}
			}

			info.ScrollPos = fScrollPos;
		}
	}
}

void URScrollPanel::RefreshPanel()
{
	ScrollHorUsePanelList.Empty();
	ScrollVerUsePanelList.Empty();

	for (UCanvasPanel* panel : ScrollPanelList)
	{
		if (IsValid(panel) && panel->Slot)
		{
			SeekDragButton(panel);
			UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(panel->Slot);
			if (canvasSlot)
			{
				FMargin marginSource = canvasSlot->GetOffsets();
				FMargin marginDest = GetComposePanelSlot(panel);
				if (marginSource.Left + marginSource.Right < marginDest.Left + marginDest.Right)
				{
					ScrollInfo::FScrollInfo info;
					info.Panel = panel;
					info.ScrollPos = 0.0f;
					ScrollHorUsePanelList.Emplace(info);
				}
				if (marginSource.Top + marginSource.Bottom < marginDest.Top + marginDest.Bottom)
				{
					ScrollInfo::FScrollInfo info;
					info.Panel = panel;
					info.ScrollPos = 0.0f;
					ScrollVerUsePanelList.Emplace(info);
				}
			}
		}
	}
}