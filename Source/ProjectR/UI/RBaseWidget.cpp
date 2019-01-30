// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RBaseWidget.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "ToolTipList/ToolTipBaseEvent.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/RWidgetManager.h"
#include "Common/UC_Tooltip.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Runtime/MovieScene/Public/MovieScene.h"
#include "Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h"

//float URBaseWidget::DpiScale = 1.0f;

float URBaseWidget::GetDPIScale()
{
	float DpiScale = 1.0f;
	UGameViewportClient* Client = GEngine->GameViewport;
	if (Client)
	{
		FViewport* Viewport = Client->Viewport;
		if (Viewport)
		{
			FVector2D ViewportSize = Viewport->GetSizeXY();
			DpiScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
		}
	}
	return DpiScale;
}

URBaseWidget::URBaseWidget(const class FObjectInitializer &PCIP)
	: Super(PCIP)
{
}

void URBaseWidget::NativeConstruct()
{
// #if LOG_TIME
// 	double StartTime = FPlatformTime::Seconds();
// #endif

	Super::NativeConstruct();

	UWidgetBlueprintGeneratedClass* WidgetBlueprintClass = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
	if (IsValid(WidgetBlueprintClass))
	{
		WidgetAnimationList = WidgetBlueprintClass->Animations;
	}

//	ChildBaseWidgetList.Empty();
	if (IsValid(WidgetTree))
	{
		TArray<UWidget*> outlist;
		WidgetTree->GetAllWidgets(outlist);
		for (UWidget* widget : outlist)
		{
			URBaseWidget* baseWidget = Cast<URBaseWidget>(widget);
			if (IsValid(baseWidget))
			{
				baseWidget->SetParentWidget(this);
//				ChildBaseWidgetList.Emplace(baseWidget);
			}
		}
	}

	IsInWidget = false;
	IsDown = false;
	IsMoved = false;

// #if LOG_TIME
// 	double EndTime = FPlatformTime::Seconds();
// 	double PrintTime = EndTime - StartTime;
// 	UE_LOG(LogGamePlay, Log, TEXT("%f ____basewidget(%s)"),PrintTime, *(this->GetName()));
// #endif
}

void URBaseWidget::NativeDestruct()
{
//	ChildBaseWidgetList.Empty();
	
	WidgetAnimationList.Empty();
	RemoveFromRoot();
	ParentWidget = nullptr;
	Super::NativeDestruct();
}

// void URBaseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
// {
// 	Super::NativeTick(MyGeometry, InDeltaTime);
// 	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(this);
// 	if (CanvasPanel)
// 	{
// 		float DIPScale = URBaseWidget::GetDPIScale();
// 		CanvasPanel->SetRenderScale(FVector2D(1.0f / DIPScale, 1.0f / DIPScale));
// 	}
//
// 	CustomGeometry = MyGeometry;
// 
// 	if (IsUpdateWidgetSize)
// 	{
// 		UpdateWidgetSize(MyGeometry, true);
// 		ChangeWidgetSize();
// 		IsUpdateWidgetSize = false;
// 	}
// 
// 	WidgetRealSize = MyGeometry.GetLocalSize();
// 	//float x = MyGeometry.AbsolutePosition.X - MyGeometry.GetLocalSize().X;
// 	//float y = MyGeometry.AbsolutePosition.Y;
// 
// 	WidgetRealPos = MyGeometry.AbsolutePosition;// FVector2D(x, y);
// 
// 	if (!IsTick)
// 		IsTick = true;
//}

void URBaseWidget::SetParentWidget(URBaseWidget* widget)
{
	if (IsValid(ParentWidget))
		return;

	ParentWidget = widget;
}

UWidget* URBaseWidget::GetParentWidget(UWidget* InChildWidget, FString name)
{
	if (IsValid(InChildWidget) && IsValid(InChildWidget->Slot) && IsValid(InChildWidget->Slot->Parent))
	{
		if (InChildWidget->Slot->Parent->GetFName().ToString().Contains(name))
		{
			return InChildWidget->Slot->Parent;
		}
		else
		{
			return GetParentWidget(InChildWidget->Slot->Parent, name);
		}
	}

	return nullptr;
}

UWidgetAnimation* URBaseWidget::GetWidgetAnimFromName(FString _name)
{
	for (size_t i = 0; i < WidgetAnimationList.Num(); ++i)
	{
		if (WidgetAnimationList[i]->MovieScene->GetName().Contains(_name))
			return WidgetAnimationList[i];
	}
	return nullptr;
}

bool URBaseWidget::GetAbsoluteRWidgetPos(URBaseWidget* InRWidget, FVector2D& location)
{
	if (IsValid(InRWidget))
	{
		if (IsValid(InRWidget->ParentWidget))
		{
			FVector2D pos;
			FVector2D size;
			if (InRWidget->ParentWidget->GetWidgetPosSize(InRWidget->ParentWidget->GetGeometry(), InRWidget, pos, size))
			{
				location += pos;

				if (IsValid(InRWidget->ParentWidget->ParentWidget))
				{
					return GetAbsoluteRWidgetPos(InRWidget->ParentWidget, location);
				}
				else
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool URBaseWidget::GetWidgetPosSize(const FGeometry& MyGeometry, UWidget* InWidget, FVector2D& location, FVector2D& size)
{
	if (IsValid(InWidget))
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InWidget->Slot);
		if (IsValid(CanvasSlot))
		{
			FAnchorData anchorData = CanvasSlot->GetLayout();

			FMargin Offset = anchorData.Offsets;
			FVector2D Alignment = anchorData.Alignment;
			FAnchors Anchors = anchorData.Anchors;
			bool AutoSize = CanvasSlot->bAutoSize;

			FMargin AnchorPixels =
				FMargin(Anchors.Minimum.X * MyGeometry.Size.X,
					Anchors.Minimum.Y * MyGeometry.Size.Y,
					Anchors.Maximum.X * MyGeometry.Size.X,
					Anchors.Maximum.Y * MyGeometry.Size.Y);

			bool bIsHorizontalStretch = Anchors.Minimum.X != Anchors.Maximum.X;
			bool bIsVerticalStretch = Anchors.Minimum.Y != Anchors.Maximum.Y;

			FVector2D SlotSize = FVector2D(Offset.Right, Offset.Bottom);
			FVector2D WidgetDesiredSize = InWidget->GetDesiredSize();

			FVector2D Size = AutoSize ? WidgetDesiredSize : SlotSize;

			FVector2D AlignmentOffset = Size * Alignment;

			FVector2D LocalPosition, LocalSize;

			if (bIsHorizontalStretch)
			{
				LocalPosition.X = AnchorPixels.Left + Offset.Left;
				LocalSize.X = AnchorPixels.Right - LocalPosition.X - Offset.Right;
			}
			else
			{
				LocalPosition.X = AnchorPixels.Left + Offset.Left - AlignmentOffset.X;
				LocalSize.X = Size.X;
			}

			if (bIsVerticalStretch)
			{
				LocalPosition.Y = AnchorPixels.Top + Offset.Top;
				LocalSize.Y = AnchorPixels.Bottom - LocalPosition.Y - Offset.Bottom;
			}
			else
			{
				LocalPosition.Y = AnchorPixels.Top + Offset.Top - AlignmentOffset.Y;
				LocalSize.Y = Size.Y;
			}

			location = LocalPosition;
			size = LocalSize;

			return true;
		}
	}

	return false;
}

bool URBaseWidget::UpdateFindWidget(const FGeometry& MyGeometry, UWidget* InParentWidget, UWidget* InWidget, FVector2D& location, FVector2D& size)
{
	URBaseWidget* ParentUserWidget = Cast<URBaseWidget>(InParentWidget);
	if (IsValid(ParentUserWidget) && IsValid(ParentUserWidget->WidgetTree) && IsValid(InWidget))
	{
		TArray<UWidget*> outlist;
		ParentUserWidget->WidgetTree->GetAllWidgets(outlist);
		for (UWidget* widget : outlist)
		{
			if (widget == InWidget)
			{
				return GetWidgetPosSize(MyGeometry, widget, location, size);
			}

			URBaseWidget* userWidget = Cast<URBaseWidget>(widget);
			if (IsValid(userWidget))
			{
				if (userWidget->UpdateFindWidget(MyGeometry, userWidget, InWidget, location, size))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void URBaseWidget::UpdateWidgetSize(const FGeometry& MyGeometry, bool bCalSize)
{
	if (IsValid(WidgetTree))
	{
		if (bCalSize)
		{
			WidgetSize.Left = 0.0f;
			WidgetSize.Right = 0.0f;
			WidgetSize.Top = 0.0f;
			WidgetSize.Bottom = 0.0f;
		}

		TArray<UWidget*> outlist;
		WidgetTree->GetAllWidgets(outlist);
		bool bFirst = true;
		for (UWidget* widget : outlist)
		{
			UPanelWidget* PanelWidget = Cast<UPanelWidget>(widget);
			if (IsValid(PanelWidget))
			{
				FVector2D pos = FVector2D::ZeroVector;
				FVector2D size;
				if (GetWidgetPosSize(MyGeometry, widget, pos, size))
				{
					if (bCalSize)
					{
						if (bFirst)
						{
							WidgetSize.Left = pos.X;
							WidgetSize.Top = pos.Y;
							WidgetSize.Right = WidgetSize.Left + size.X;
							WidgetSize.Bottom = WidgetSize.Top + size.Y;
							bFirst = false;
						}
						else
						{
							if (WidgetSize.Left > pos.X)
								WidgetSize.Left = pos.X;
							if (WidgetSize.Top > pos.Y)
								WidgetSize.Top = pos.Y;
							if (WidgetSize.Right < pos.X + size.X)
								WidgetSize.Right = pos.X + size.X;
							if (WidgetSize.Bottom < pos.Y + size.Y)
								WidgetSize.Bottom = pos.Y + size.Y;
						}
					}
				}
			}
		}
	}
}

void URBaseWidget::GatherSWidgetAllChildren(TSharedRef<SWidget> Root, TSet<TSharedRef<SWidget>>& Children)
{
	if (Root != SNullWidget::NullWidget)
	{
		//UE_LOG(LogUI, Log, TEXT("%s %s"), *Rotouchot->ToString(), *Root->GetTypeAsString());

		Children.Emplace(Root);

		FChildren* chidren = Root->GetChildren();
		if (chidren != nullptr)
		{
			for (int32 i = 0; i < chidren->Num(); ++i)
			{
				TSharedRef<SWidget> widget = chidren->GetChildAt(i);
				if (widget != SNullWidget::NullWidget)
				{
					GatherSWidgetAllChildren(widget, Children);
					//Children.Add(widget);
					//UE_LOG(LogUI, Log, TEXT("%s %s"), *widget->ToString(), *widget->GetTypeAsString());
				}
			}
		}
	}
}

//int32 URBaseWidget::GetZOrder(UUserWidget* userwidget)
//{
//	if (IsValid(userwidget) && IsValid(userwidget->Slot))
//	{
//		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(userwidget->Slot);
//		if (IsValid(canvasSlot))
//		{
//			return canvasSlot->GetZOrder();
//		}
//	}
//
//	return -1;
//}

//void URBaseWidget::SetZOrder(UUserWidget* userwidget, int32 ZOrder)
//{
//	if (IsValid(userwidget) && IsValid(userwidget->Slot))
//	{
//		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(userwidget->Slot);
//		if (IsValid(canvasSlot))
//		{
//			canvasSlot->SetZOrder(ZOrder);
//		}
//	}
//}

//void URBaseWidget::SetZOrderOffset(UUserWidget* userwidget, int32 OffsetZOrder)
//{
//	if (IsValid(userwidget))
//	{
//		if (IsValid(userwidget->WidgetTree))
//		{
//			TArray<UWidget*> outlist;
//			userwidget->WidgetTree->GetAllWidgets(outlist);
//			for (UWidget* widget : outlist)
//			{
//				if (IsValid(widget))
//				{
//					SetCanvasPanelZOrderOffset(widget, OffsetZOrder);
//					UE_LOG(LogUI, Log, TEXT("%s %s"), *widget->GetName(), *widget->GetFName().GetPlainNameString());
//				}
//
//			}
//			//UE_LOG(LogUI, Log, TEXT("Child %d"), outlist.Num());
//		}
//	}
//}

void URBaseWidget::SetCanvasPanelZOrderOffset(UWidget* widget, int32 OffsetZOrder)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetZOrder(canvasSlot->GetZOrder() + OffsetZOrder);

			//UE_LOG(LogUI, Log, TEXT("URBaseWidget::SetCanvasPanelZOrderOffset %d"), canvasSlot->GetZOrder());
		}
	}
}

void URBaseWidget::SetCanvasPanelSize(UWidget* widget, FVector2D size)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetSize(size);

			//UE_LOG(LogUI, Log, TEXT("URBaseWidget::SetCanvasPanelSize %f %f"), size.X, size.Y);
		}
	}
}

void URBaseWidget::SetCanvasPanelPos(UWidget* widget, FVector2D pos)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(pos);
		}
	}
}

void URBaseWidget::SetCanvasPanelXPos(UWidget* widget, float xPos)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(FVector2D(xPos, canvasSlot->GetPosition().Y));
		}
	}
}

void URBaseWidget::SetCanvasPanelYPos(UWidget* widget, float yPos)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(FVector2D(canvasSlot->GetPosition().X, yPos));
		}
	}
}

void URBaseWidget::SetCanvasPanelXOffset(UWidget* widget, float xOffset)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(FVector2D(canvasSlot->GetPosition().X + xOffset, canvasSlot->GetPosition().Y));
		}
	}
}

void URBaseWidget::SetCanvasPanelYOffset(UWidget* widget, float yOffset)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(FVector2D(canvasSlot->GetPosition().X, canvasSlot->GetPosition().Y + yOffset));
		}
	}
}

void URBaseWidget::SetCanvasPanelOffset(UWidget* widget, FVector2D Offset)
{
	if (IsValid(widget) && IsValid(widget->Slot))
	{
		UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (IsValid(canvasSlot))
		{
			canvasSlot->SetPosition(FVector2D(canvasSlot->GetPosition().X + Offset.X, canvasSlot->GetPosition().Y + Offset.Y));
		}
	}
}

void URBaseWidget::GetHaveWidget(UWidget* widget, TArray<UWidget*>& outlist)
{
	if (IsValid(widget))
	{
		UPanelWidget* panelwidget = Cast<UPanelWidget>(widget);
		if (IsValid(panelwidget))
		{
			outlist.Reserve(panelwidget->GetChildrenCount());
			for (int32 i = 0; i < panelwidget->GetChildrenCount(); ++i)
			{
				outlist.Emplace(panelwidget->GetChildAt(i));
			}
		}
	}
}

void URBaseWidget::GetHaveWidget(TArray<UWidget*>& outlist)
{
	if (IsValid(WidgetTree))
	{
		WidgetTree->GetAllWidgets(outlist);
#if LOG_UI
		for (UWidget* widget : outlist)
		{
			if (widget)
			{
				UE_LOG(LogUI, Log, TEXT("%s %s"), *widget->GetName(), *widget->GetFName().GetPlainNameString());
			}

		}
		UE_LOG(LogUI, Log, TEXT("Child %d"), outlist.Num());
#endif
	}
}

void URBaseWidget::SetFocus(UWidget* widget, UWorld* world)
{
	if (IsValid(widget) && IsValid(world))
	{
		TSharedPtr<SWidget> SafeWidget = widget->GetCachedWidget();
		if (SafeWidget.IsValid())
		{
			APlayerController* PlayerController = UUtilFunctionLibrary::GetPlayerController(world);
			if (IsValid(PlayerController))
			{
				FLocalPlayerContext Context(PlayerController);
				if (ULocalPlayer* LocalPlayer = Context.GetLocalPlayer())
				{
					int32 UserIndex = LocalPlayer->GetControllerId();

					FSlateApplication::Get().SetUserFocus(UserIndex, SafeWidget);
				}
			}			
		}
	}
}

void URBaseWidget::RefreshWidgetSize()
{
	if (IsValid(WidgetTree))
	{
		WidgetSize.Left = 0.0f;
		WidgetSize.Right = 0.0f;
		WidgetSize.Top = 0.0f;
		WidgetSize.Bottom = 0.0f;

		TArray<UWidget*> outlist;
		WidgetTree->GetAllWidgets(outlist);
		for (UWidget* widget : outlist)
		{
			FVector2D size = widget->GetDesiredSize();
			UPanelWidget* PanelWidget = Cast<UPanelWidget>(widget);
			if (IsValid(PanelWidget))
			{
				//TArray<UPanelSlot*>& slotList = PanelWidget->GetSlots();
				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelWidget->Slot);
				if (IsValid(CanvasSlot))
				{
					FAnchorData anchorData = CanvasSlot->GetLayout();

					if (WidgetSize.Left > anchorData.Offsets.Left)
						WidgetSize.Left = anchorData.Offsets.Left;
					if (WidgetSize.Right < anchorData.Offsets.Right)
						WidgetSize.Right = anchorData.Offsets.Right;
					if (WidgetSize.Top > anchorData.Offsets.Top)
						WidgetSize.Top = anchorData.Offsets.Top;
					if (WidgetSize.Bottom < anchorData.Offsets.Bottom)
						WidgetSize.Bottom = anchorData.Offsets.Bottom;
				}
			}
		}
	}
}

void URBaseWidget::InvalidateData()
{
}

void URBaseWidget::RefreshLocalizingText()
{

}

void URBaseWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
 	IsInWidget = true;
// 	
// 	if (ToolTipEvent != EToolTipEventTypeEnum::VE_None)
// 	{
// 		UE_LOG(LogUI, Log, TEXT("!!!!!!!!!!!!!!!!!!!!!!!This Class Name : %s"), *(this->GetClass()->GetFName().ToString()));
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if ( IsValid(RGameInstance->TooltipEventList))
// 		{
// 			UTooltipBaseEvent* ToolTipBaseEvent = RGameInstance->TooltipEventList->GetTooltipEvent(ToolTipEvent, ToolTipID);
// 			if (IsValid(ToolTipBaseEvent))
// 			{
// 				ToolTipBaseEvent->ShowUI();
// 			}
// 		}
// 	}
}

void URBaseWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
 	IsInWidget = false;
// 
// 	if (ToolTipEvent != EToolTipEventTypeEnum::VE_None)
// 	{
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if ( IsValid(RGameInstance->TooltipEventList))
// 		{
// 			RGameInstance->TooltipEventList->HideTooltipEvent();
// 		}
// 	}
}

FReply URBaseWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	IsInWidget = true;

	DownPos = InGestureEvent.GetScreenSpacePosition();
	
	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchStarted: %s [%f][%f] ToolTipEvent %d"), *(GetName()), DownPos.X, DownPos.Y, (int32)ToolTipEvent);

	if (ToolTipEvent != EToolTipEventTypeEnum::VE_None)
	{
		int32 touchCount = UUtilFunctionLibrary::GetTouchCount(this);
		//UE_LOG(LogUI, Log, TEXT("TouchCount: %d"), touchCount);

		bool bEnableTooltip = false;
		if (touchCount == 0)
		{
			bEnableTooltip = true;
		}
		else if (touchCount == 1)
		{
			APlayerController* PlayerController = UUtilFunctionLibrary::GetPlayerController(GetWorld());
			if (IsValid(PlayerController))
			{
				FVector2D TouchLocation;
				bool bIsCurrentlyPressed;
				PlayerController->GetInputTouchState(ETouchIndex::Touch1, TouchLocation.X, TouchLocation.Y, bIsCurrentlyPressed);
				if (bIsCurrentlyPressed)
				{
					bEnableTooltip = true;
					//UE_LOG(LogUI, Log, TEXT("GetInputTouchState: ETouchIndex::Touch1"));
				}
			}
		}
// #if SHOW_TOOLTIP == 1
// 		if (bEnableTooltip)
// 		{
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			checkf(RGameInstance->TooltipEventList != nullptr, TEXT("%s[ TooltipEventList Is Empty ]"), __FUNCTION__);
// 			FVector2D PixelPosition;
// 			FVector2D ViewportPosition;
// 			USlateBlueprintLibrary::AbsoluteToViewport(this, DownPos, PixelPosition, ViewportPosition);
// 			//UE_LOG(LogUI, Log, TEXT("DownPos AbsoluteToViewport: PixelPosition [%f][%f]"), PixelPosition.X, PixelPosition.Y);
// 			//UE_LOG(LogUI, Log, TEXT("NativeOnTouchStarted: Execute SetShow"));
// 			RGameInstance->TooltipEventList->SetShow(ToolTipEvent, ToolTipID, PixelPosition, this);
// 			IsToolTipShow = true;
// 		}
// #endif
	}

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply URBaseWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	MovePos = InGestureEvent.GetScreenSpacePosition();

	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchMoved: %s [%f][%f] ToolTipEvent %d"), *(GetName()), MovePos.X, MovePos.Y, (int32)ToolTipEvent);
// #if SHOW_TOOLTIP == 1	
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	checkf(RGameInstance->TooltipEventList != nullptr, TEXT("%s[ TooltipEventList Is Empty ]"), __FUNCTION__);
// 	if (RGameInstance->TooltipEventList->IsShowTooltip())
// 	{
// 		if (RGameInstance->TooltipEventList->RelationToolTipWidget(this))
// 		{
// 			FVector2D PixelPosition;
// 			FVector2D ViewportPosition;
// 			USlateBlueprintLibrary::AbsoluteToViewport(this, MovePos, PixelPosition, ViewportPosition);
// 			//UE_LOG(LogUI, Log, TEXT("MovePos AbsoluteToViewport: PixelPosition [%f][%f]"), PixelPosition.X, PixelPosition.Y);
// 			//UE_LOG(LogUI, Log, TEXT("NativeOnTouchMoved: Execute SetMove"));
// 			RGameInstance->TooltipEventList->SetMove(PixelPosition);
// 		}
// 		else
// 		{
// 			//UE_LOG(LogUI, Log, TEXT("NativeOnTouchMoved: Execute SetHide"));
// 			RGameInstance->TooltipEventList->SetHide();
// 			IsToolTipShow = false;
// 		}
// 	}
// #endif
	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply URBaseWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	IsInWidget = true;
	IsDown = false;

	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchEnded: %s ToolTipEvent %d"), *(GetName()), (int32)ToolTipEvent);
// #if SHOW_TOOLTIP == 1
// 	if (ToolTipEvent != EToolTipEventTypeEnum::VE_None)
// 	{
// 		if (IsToolTipShow)
// 		{
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			checkf(RGameInstance->TooltipEventList != nullptr, TEXT("%s[ TooltipEventList Is Empty ]"), __FUNCTION__);
// 			if (RGameInstance->TooltipEventList->IsShowTooltip())
// 			{
// 				//UE_LOG(LogUI, Log, TEXT("NativeOnTouchEnded: Execute SetHide"));
// 				RGameInstance->TooltipEventList->SetHide();
// 				IsToolTipShow = false;
// 
// 				return FReply::Unhandled();
// 			}
// 		}
// 	}
// #endif
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}


bool URBaseWidget::PlayAni(FString name, bool bReverse, int32 num)
{
	UWidgetAnimation* targetAnimation = GetAni(name);
	if (targetAnimation)
	{
		PlayAnimation(targetAnimation, 0.0f, num, (bReverse == true) ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
		return true;
	}
	
	return false;
}

bool URBaseWidget::IsPlayAni(FString name)
{
	UWidgetAnimation* targetAnimation = GetAni(name);
	if (targetAnimation)
	{
		return IsAnimationPlaying(targetAnimation);
	}

	return false;
}

void URBaseWidget::StopAni(FString name)
{
	UWidgetAnimation* targetAnimation = GetAni(name);
	if( targetAnimation )
		StopAnimation(targetAnimation);
}

void URBaseWidget::ResetAni(FString name)
{
	UWidgetAnimation* targetAnimation = GetAni(name);
	if (targetAnimation)
		ReverseAnimation(targetAnimation);
}

bool URBaseWidget::PlayAniFromSpecificStartTimeToEndTime(FString name, float StartTime, float EndTime, bool bReverse)
{
	UWidgetAnimation* targetAnimation = GetAni(name);
	if (targetAnimation)
	{
		PlayAnimationTo(targetAnimation, StartTime, EndTime, 1, EUMGSequencePlayMode::Forward);
		return true;
	}

	return false;
}

UWidgetAnimation* URBaseWidget::GetAni(FString name)
{
	for (UWidgetAnimation* animation : WidgetAnimationList)
	{
		if (IsValid(animation))
		{
			FString aniName;
			if (animation->GetMovieScene())
			{
				animation->GetMovieScene()->GetName(aniName);
				if (aniName == name )
				{
					return animation;
				}
			}
		}
	}
	return nullptr;
}

void URBaseWidget::SetDisable(bool bDisable)
{
	IsDisabled = bDisable;
}

void URBaseWidget::EventClear(TArray<FString>& exceptList, TArray<UWidget*>& exceptWidgetList)
{
	if (IsValid(WidgetTree))
	{
		TArray<UWidget*> outlist;
		WidgetTree->GetAllWidgets(outlist);
		for (UWidget* widget : outlist)
		{
			if (IsValid(widget))
			{
				bool bFind = false;
				for (FString& exceptname : exceptList)
				{
					if (widget->GetName().Contains(exceptname))
					{
						bFind = true;
						exceptWidgetList.Emplace(widget);
						break;
					}						
				}

				if(bFind)
					continue;

				UButton* button = Cast<UButton>(widget);
				if (IsValid(button))
				{
					button->OnClicked.Clear();
				}

				URScrollView* scrollView = Cast<URScrollView>(widget);
				if (IsValid(scrollView))
				{
					scrollView->SetDisable(true);
				}

				UCheckBox_Radio* checkRadio = Cast<UCheckBox_Radio>(widget);
				if (IsValid(checkRadio))
				{
					checkRadio->RadioGroupoComponentStateChanged.Clear();
					checkRadio->OnCheckStateChanged.Clear();
				}

				URBaseWidget* baseWidget = Cast<URBaseWidget>(widget);
				if (IsValid(baseWidget))
				{
					baseWidget->EventClear(exceptList, exceptWidgetList);
				}
			}
		}
	}
}
