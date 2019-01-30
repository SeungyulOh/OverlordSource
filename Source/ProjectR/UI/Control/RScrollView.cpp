// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Blueprint/UserWidget.h"
#include "UI/Control/RScrollItem.h"
#include "RScrollBar.h"
#include "RScrollView.h"
#include "RScrollItem.h"
#include "ToolTipList/ToolTipDefs.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UI/Common/HeroScrollItemData.h"


URScrollView::URScrollView(const class FObjectInitializer &PCIP)
	: Super(PCIP)
	, CurScrollOffset(0.0f)
	, IsScrollAble(true)
	, CreateCount(0)
	, VisibleMaxCount(0)
	, VisibleCreateCount(0)
	, StartIdx(0)
{
	ItemInitSize = FVector2D::ZeroVector;

	/*ScrollBar = (SNew(SScrollBar)
		.Style(ScrollWidgetStyle)
		.Orientation(ScrollOrientation)
		.OnUserScrolled(this, &SScrollBox::ScrollBar_OnUserScrolled));
	ScrollBar->SetThickness(ScrollThickness);
	ScrollBar->SetUserVisibility(PCIP._ScrollBarVisibility);
	ScrollBar->SetScrollBarAlwaysVisible(bAlwaysShowScrollbar);*/
	
	bUserScrolling = false;
	AmountScrolledByUser = 0.f;
	TickScrollDelta = 0.f;
	bIsScrollingActiveTimerRegistered = false;
}

void URScrollView::NativeConstruct()
{
	Super::NativeConstruct();

	IsUpdateWidgetSize = AutoSizeEnable;

	// DragTriggerDistance = FSlateApplication::Get().GetDragTriggerDistance();
	DragTriggerDistance = 10.0; // Custom amount - by default 5

	
								
	/* AutoSizeEnable Definition*/

	// if you set some value in ScrollViewSize of blueprint widget,
	// that value will be used only for calculating "CreateCount" in Init function

	// AutoSizeEnable true -> CanvasPanel and BackgroundPanel size will be changed to the same value with the value(ScrollView Size) you set in widget blueprint.
	// AutoSizeEnable false -> CanvasPanel and BackgroundPanel size will be set with the Size X and Size Y in CanvasPanelSlot that UI Designer usually changes.
	
	if(AutoSizeEnable)
		SetCanvasPanelSize(this, ScrollViewSize);

	if (IsValid(WidgetTree))
	{
		//ScrollItemList_RemoveAllElement();

		UWidget* uWidget = WidgetTree->FindWidget("BackGroundPanel");
		if (IsValid(uWidget))
		{
			BGPanel = Cast<UCanvasPanel>(uWidget);
			if (BGPanel)
			{
				if (AutoSizeEnable)
					SetCanvasPanelSize(BGPanel, ScrollViewSize);
			}
		}
	}

	ResetScrollAnimationState();
}

void URScrollView::ChangeWidgetSize()
{
	if (AutoSizeEnable)
	{
		float width = WidgetSize.Right - WidgetSize.Left;
		float height = WidgetSize.Bottom - WidgetSize.Top;

		ScrollViewSize.X = width;
		ScrollViewSize.Y = height;

		SetCanvasPanelSize(this, ScrollViewSize);

		if (IsValid(BGPanel))
		{
			SetCanvasPanelSize(BGPanel, ScrollViewSize);
		}
	}
}

void URScrollView::NativeDestruct()
{
	OnGetSelectedScrollItemDataEvent.Unbind();
	EventDelegator = nullptr;

	if (!bInitNoClear)
	{
		ScrollItemList_RemoveAllElement();
		ClearScrollItemData();
	}

	Super::NativeDestruct();
}

void URScrollView::GetListPosByIndex(int32 Index, FIntPoint& GridPos) const
{
	if (SpecialtyType == EScrollViewTypeEnum::VE_Grid)
	{
		if (Horizontal)
		{
			GridPos.X = Index / GridCount;
			GridPos.Y = Index % GridCount;
		}
		else
		{
			GridPos.X = Index % GridCount;
			GridPos.Y = Index / GridCount;
		}
	}
	else
	{
		if (Horizontal)
		{
			GridPos.X = ScrollItemList.Num() - 1;
			GridPos.Y = 0;
		}
		else
		{
			GridPos.X = 0;
			GridPos.Y = ScrollItemList.Num() - 1;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// ScrollItemList Element
void URScrollView::ScrollItemList_AddElement(int32 InElementNumber /*= 1*/)
{	
	if (!IsValid(BGPanel) )
		return;
		
	UClass* ResultScrollItemBpClass = GetScrollItemBpClass();
	if (ResultScrollItemBpClass == nullptr)
		return;

	for (int32 i = 0; i < InElementNumber; ++i)
	{
		URScrollItem* uiWidget = CreateWidget<URScrollItem>(GetWorld(), ResultScrollItemBpClass);
		if (!IsValid(uiWidget))
			break;

		uiWidget->SetParentWidget(this);
		//FPlatformProcess::Sleep(0.001f);
		uiWidget->SetParentScrollView(this);
		//FPlatformProcess::Sleep(0.001f);
		if (IsCustomItemSize && ItemDataList.IsValidIndex(0))
		{
			uiWidget->Size += ItemDataList[0]->WidgetSize;
		}

		ItemInitSize = uiWidget->Size;
		BGPanel->AddChild(uiWidget);

		uiWidget->IsTimeEvent = TimeEvent;
		uiWidget->TimeEventTime = TimeEventTime;

		int32 addIndex = ScrollItemList.Emplace(uiWidget);

		FIntPoint GridPos;
		GetListPosByIndex(addIndex, GridPos);

		FVector2D Offset;
		Offset.X = (float)GridPos.X * uiWidget->Size.X;
		Offset.Y = (float)GridPos.Y * uiWidget->Size.Y;

		SetCanvasPanelOffset(uiWidget, Offset);
	}
}

void URScrollView::ScrollItemList_RemoveElement(URScrollItem* InElement)
{
	if (ScrollItemList.Contains(InElement))
	{
		ScrollItemList.Remove(InElement);
	}
}

void URScrollView::ScrollItemList_RemoveAllElement(int32 InSize /*=0*/)
{
	for (URScrollItem* Item : ScrollItemList)
	{ 
		if (IsValid(Item))
		{
			Item->RemoveFromParent();
		}
	}

	ScrollItemList.Reset(InSize);
}

void URScrollView::SetSizeChangeUpdate()
{
	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (SpecialtyType == EScrollViewTypeEnum::VE_LimitEnable)
	{
		FVector2D fTotal = FVector2D::ZeroVector;
		for (int32 i = 0; i < ItemListLength; ++i)
		{
			Item = ScrollItemList[i];
			if (IsValid(Item))
			{
				if (Horizontal)
				{
					fTotal.X += Item->Size.X;
				}
				else
				{
					fTotal.Y += Item->Size.Y;
				}
			}
		}

		if (ScrollViewSize.X < fTotal.X || ScrollViewSize.Y < fTotal.Y)
			IsScrollAble = true;
	}
	else
	{
		if (DataListLength > VisibleMaxCount)
		{
			IsScrollAble = true;
		}
	}
}

void URScrollView::SetDisable(bool bDisable)
{
	Super::SetDisable(bDisable);

	for (URScrollItem* Item : ScrollItemList)
	{
		if (IsValid(Item))
		{
			Item->IsDisabled = bDisable;
		}
	}
}

void URScrollView::ScrollItemList_SetItemDataList(bool bResetSelectList /*= true*/)
{
	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	for (int32 i = 0; i < ItemListLength; ++i)
	{
		Item = ScrollItemList[i];
		if (IsValid(Item))
		{
			Item->Count = i;

			if (i < DataListLength)
			{
				Item->SetVisibility(ESlateVisibility::Visible);
				Item->ItemData = ItemDataList[i];
								
				Item->SetScrollItemData(i, ItemDataList[i], this);
				Item->InvalidateData();
			}
			else
			{
				Item->SetVisibility(ESlateVisibility::Hidden);
			}

//			SetTooltipInfo(Item);
		}
	}

	
	// IsScrollAble = ForceScrollAble;

	SetSizeChangeUpdate();

	if(bResetSelectList)
	{
		ClearAllSelected();

		if (InitFirstSelect)
		{
			if (ScrollItemList.Num() > 0)
			{
				SetSelected(ScrollItemList[0]);
			}
		}
	}
}

void URScrollView::SetTooltipInfo(URScrollItem* InItem)
{
	if (!bShowTooltip)
	{
		InItem->ToolTipEvent = EToolTipEventTypeEnum::VE_None;
	}
}


void URScrollView::ComputeSpawnAni(int32 InIdx)
{
	if (IsSpawnAni && IsFirstInit)
	{
		int32 ScrollValue = InIdx;
		int32 ItemNum = ScrollItemList.Num();
		int32 ItemDataNum = ItemDataList.Num();
		bool IsTemp = false;
		if (ScrollValue <= 0)
		{
			ScrollValue = 0;
		}
		else if(ScrollValue >= ItemNum)
		{
			if (ScrollValue + VisibleCreateCount > ItemDataNum)
			{
				IsTemp = true;
			}

			ScrollValue -= ItemNum;
		}
		//VisibleCreateCount - VisibleMaxCount
		int32 ComputeSwap = ScrollValue - 1;
		if (ComputeSwap <= 0)
		{
			ComputeSwap = 0;
		}
		else
		{
			ComputeSwap *= -1;
		}

		//
		ScrollValue += ComputeSwap;
		if (IsTemp)
		{
			ScrollValue += 1;
		}

		for (int32 i = 0; i < VisibleCreateCount; ++i)
		{
			int32 Result = i + ScrollValue;
			//i = 3;
			if (Result >= ItemNum)
			{
				Result = i;
			}

			if (ScrollItemList.IsValidIndex(Result))
			{
				ScrollItemList[Result]->PlayAni("SpawnAni");
			}
			else
			{
				break;
			}
		}
	}
	IsFirstInit = false;
}

void URScrollView::Force_DeSelect(URScrollItem* SelecteItem)
{
	SelecteItem->IsSelected = false;
	SelecteItem->ItemData->bSelected = false;
	SelectedScrollItemList.Remove(SelecteItem->Count);
	SelectedScrollItemDataList.Remove(SelecteItem->ItemData);

}

void URScrollView::ScrollToItemAnimation(int32 ScrollItemDataIndex)
{
	UE_LOG(LogUI, Log, TEXT("ENTER ScrollToItem(%d)"), ScrollItemDataIndex);
	if (ScrollItemDataIndex < 0 || ScrollItemDataIndex >= ItemDataList.Num())
	{
		UE_LOG(LogUI, Log, TEXT("ScrollToItem: given ScrollItemDataIndex outside of range"));
		return;
	}

	if (SpecialtyType == EScrollViewTypeEnum::VE_LimitEnable)
	{
		UE_LOG(LogUI, Log, TEXT("ScrollToItem: doesn't support SpecialtyType of VE_LimitEnable"));
		return;
	}

	//// Determine DesiredScrollOffset

	// code mirrored from SetScrollUpdate
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (ItemListLength == 0)
		return;

	if (DataListLength == 0)
		return;
	
	// Values needed to be determined to calculate target offset
	float fMoveDist = 0.f;
	float ItemOffset = 0.f;
	float ItemLength = 0.f;

	switch (SpecialtyType)
	{
		case EScrollViewTypeEnum::VE_None:
		{
			// fMoveDist
			if (Horizontal)
			{
				fMoveDist = (float)DataListLength * ItemInitSize.X - ScrollViewSize.X;
			} else
			{
				fMoveDist = (float)DataListLength * ItemInitSize.Y - ScrollViewSize.Y;
			}

			// ItemLength
			if (Horizontal)
			{
				ItemLength = ItemInitSize.X;
			}
			else
			{
				ItemLength = ItemInitSize.Y;
			}

			// ItemOffset
			ItemOffset = ItemLength * ScrollItemDataIndex;
		}
		break;
		case EScrollViewTypeEnum::VE_Grid:
		{
			// fMoveDist
			FIntPoint ItemPos;
			GetListPosByIndex(ScrollItemDataIndex, ItemPos);
			FIntPoint DataPos;
			GetListPosByIndex(DataListLength, DataPos);

			int32 GridDataLength = 0;

			int32 ItemRank;

			if (Horizontal)
			{
				ItemRank = ItemPos.X;
				GridDataLength = DataPos.X + (DataPos.Y ? 1 : 0);

				fMoveDist = (float)GridDataLength * ItemInitSize.X - ScrollViewSize.X;
			} else
			{
				ItemRank = ItemPos.Y;
				GridDataLength = DataPos.Y + (DataPos.X ? 1 : 0);

				fMoveDist = (float)GridDataLength * ItemInitSize.Y - ScrollViewSize.Y;
			}

			// ItemLength
			if (Horizontal)
			{
				ItemLength = ItemInitSize.X;
			} else
			{
				ItemLength = ItemInitSize.Y;
			}

			// ItemOffset
			ItemOffset = ItemLength * ItemRank;
		}
		break;
		case EScrollViewTypeEnum::VE_LimitEnable:
		{
			// Unreachable
			// VE_LimitEnable is not supported now
			// because, in order to calculate the offset of an item, you need a way to determine the sizes of the data items that don't have views, which is not possible.
		}
		break;
	}

	// Determine offset of the item

	float ScrollViewLength;
	if (Horizontal)
	{
		ScrollViewLength = ScrollViewSize.X;
	}
	else
	{
		ScrollViewLength = ScrollViewSize.Y;
	}

	// Adjust to be at the center of the view
	float ItemOffsetCenter = ItemOffset - ScrollViewLength * 0.5 + ItemLength * 0.5;
	// clamp
	if (fMoveDist < 0.0f)				// ItemTotalLength < ScrollViewLength
	{
		ItemOffsetCenter = 0.0f; // Just return here
		return;
	}
	else if (ItemOffsetCenter >= fMoveDist)	// ScrollIng Tail
	{
		ItemOffsetCenter = fMoveDist;
	}
	else if (ItemOffsetCenter <= 0.0f)	// Scrolling Head
	{
		ItemOffsetCenter = 0.0f;
	}

	DesiredScrollOffset = -ItemOffsetCenter;
	bAnimating = true;
	bScrolling = true;

	bIsScrollingActiveTimerRegistered = false;

	UE_LOG(LogUI, Log, TEXT("EXIT ScrollToItem(%d): Cur=%f Desired=%f"), ScrollItemDataIndex, CurScrollOffset, DesiredScrollOffset);
}

float URScrollView::CalculateMoveDist() const
{
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (ItemListLength == 0)
		return 0.f;

	if (DataListLength == 0)
		return 0.f;

	float fMoveDist = 0.0f;

	switch (SpecialtyType)
	{
	case EScrollViewTypeEnum::VE_None:
	{
		// Check Distance
		if (Horizontal)
		{
			fMoveDist = (float)DataListLength * ItemInitSize.X - ScrollViewSize.X;
		}
		else
		{
			fMoveDist = (float)DataListLength * ItemInitSize.Y - ScrollViewSize.Y;
		}
	}
	break;
	case EScrollViewTypeEnum::VE_Grid:
	{
		FIntPoint DataPos;
		GetListPosByIndex(DataListLength, DataPos);

		int32 GridDataLength = 0;

		// common distance
		if (Horizontal)
		{
			GridDataLength = DataPos.X + (DataPos.Y ? 1 : 0);

			fMoveDist = (float)GridDataLength * ItemInitSize.X - ScrollViewSize.X;
		}
		else
		{
			GridDataLength = DataPos.Y + (DataPos.X ? 1 : 0);

			fMoveDist = (float)GridDataLength * ItemInitSize.Y - ScrollViewSize.Y;
		}
	}
	break;
	case EScrollViewTypeEnum::VE_LimitEnable:
	{
		float fTotalDist = 0.0f;
		URScrollItem* Item = nullptr;
		for (int32 i = 0; i < ItemListLength; ++i)
		{
			Item = ScrollItemList[i];
			if (IsValid(Item))
			{
				if (Horizontal)
				{
					fTotalDist += Item->Size.X;
				}
				else
				{
					fTotalDist += Item->Size.Y;
				}
			}
		}

		// Check Distance
		if (Horizontal)
		{
			fMoveDist = fTotalDist - ScrollViewSize.X;
		}
		else
		{
			fMoveDist = fTotalDist - ScrollViewSize.Y;
		}
	}
	break;
	}

	if (fMoveDist < 0.f) {
		fMoveDist = 0.f;
	}

	return fMoveDist;
}

void URScrollView::ResetScrollAnimationState()
{
	bAnimating = false;
	bUserScrolling = false;
	AmountScrolledByUser = 0.f;
	TickScrollDelta = 0.f;
	bIsScrollingActiveTimerRegistered = false;
	InertialScrollManager.ClearScrollVelocity();
	InertialScrollManager.ScrollDecceleration = InertialScrollDecceleration;

	DesiredScrollOffset = 0.f;
}

void URScrollView::ScrollItemOnTouchStarted()
{
	InertialScrollManager.ClearScrollVelocity();

	bAnimating = false;
	bIsScrollingActiveTimerRegistered = false;
	bUserScrolling = true;
	AmountScrolledByUser = 0.f;
}

void URScrollView::ScrollItemOnTouchMoved(const FPointerEvent& InEvent)
{
	if (bUserScrolling) {
		float diff = GetScrollComponentFromVector(InEvent.GetCursorDelta());
		AmountScrolledByUser += FMath::Abs(diff);

		TickScrollDelta += diff;

		if (AmountScrolledByUser > DragTriggerDistance) {
			bScrolling = true;
		}
	}
}

void URScrollView::ScrollItemOnTouchEnded()
{
//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, TEXT("TouchEnd"));
	bUserScrolling = false;
	if (bScrolling)
	{
		bIsScrollingActiveTimerRegistered = true;
	}
	else
	{
		//UE_LOG(LogUI, Log, TEXT("User didn't move enough to cause inertia: %f"), AmountScrolledByUser);
	}
	AmountScrolledByUser = 0.f;
}

void URScrollView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bUserScrolling && bIsScrollingActiveTimerRegistered)
	{
		UE_LOG(LogUI, Log, TEXT("NativeTick: [ERROR] bInInteraction and bIsScrollingActiveTimerRegistered at the same time"));
	}


	bool bKeepScrolling = false;

	if (bScrolling)
	{
		if (bAnimating)
		{
			if (FMath::IsNearlyEqual(DesiredScrollOffset, CurScrollOffset, 0.01f))
			{
				bAnimating = false;
			}
			else
			{
				bKeepScrolling = true;
			}
			float Dist = DesiredScrollOffset - CurScrollOffset;
			const float InterpSpeed = 10.f;
			float diff = (bAnimating) ? Dist * FMath::Clamp(InDeltaTime * InterpSpeed, 0.0f, 1.0f) : Dist;

			TickScrollDelta += diff;
		}
		else if (bUserScrolling)
		{
			InertialScrollManager.AddScrollSample(TickScrollDelta, FPlatformTime::Seconds());
			bKeepScrolling = true;
		}
		else if (bIsScrollingActiveTimerRegistered)
		{
			// Gliding

			// UE_LOG(LogUI, Log, TEXT("[DEBUG] Timer active"));
			// UpdateInertialScroll
			const float ScrollVelocity = InertialScrollManager.GetScrollVelocity();

			if (ScrollVelocity != 0.f)
			{
				// UE_LOG(LogUI, Log, TEXT("[DEBUG] sliding with ScrollVelocity %f"), ScrollVelocity);
				TickScrollDelta += ScrollVelocity * InDeltaTime;
				bKeepScrolling = true;

				InertialScrollManager.UpdateScrollVelocity(InDeltaTime);
			}
			else
			{
				// Stop gliding
				InertialScrollManager.ClearScrollVelocity();
				bIsScrollingActiveTimerRegistered = false;
				// UE_LOG(LogUI, Log, TEXT("[DEBUG] Timer deactivated"));
			}
		}
		else
		{
			// Scrolling to animate overscroll relief
		}

		// Execute
		if (TickScrollDelta != 0.f)
		{
			bKeepScrolling = true;
		}

		FVector2D diffVector(0.f, 0.f);
		if (Horizontal)
		{
			diffVector.X = TickScrollDelta;
		}
		else
		{
			diffVector.Y = TickScrollDelta;
		}
		SetScrollUpdate(nullptr, diffVector);

		TickScrollDelta = 0.f;

		// Relieve overscroll
		if (Overscroll.GetOverscroll() != 0.f)
		{
			bKeepScrolling = true;

			// Clear inertia if overscrolling
			InertialScrollManager.ClearScrollVelocity();
			bIsScrollingActiveTimerRegistered = false;

			if (!bUserScrolling)
			{
				Overscroll.UpdateOverscroll(InDeltaTime);
			}
		}
	}

	bScrolling = bKeepScrolling;
}

float URScrollView::GetScrollComponentFromVector(FVector2D Vector) const
{
	if (Horizontal)
	{
		return Vector.X;
	} else
	{
		return Vector.Y;
	}
}

void URScrollView::ShiftScrollItemList(int32 dataIndex)
{
	// Shift ScrollItemList such that the item of Count of dataIndex should be at index 0

	int32 ItemListLength = ScrollItemList.Num();

	int32 TotalRanks = ItemListLength;
	if (SpecialtyType == EScrollViewTypeEnum::VE_Grid)
	{
		FIntPoint ItemPos;
		GetListPosByIndex(ItemListLength, ItemPos);

		// common distance
		if (Horizontal)
		{
			TotalRanks = ItemPos.X + (ItemPos.Y ? 1 : 0);
		} else
		{
			TotalRanks = ItemPos.Y + (ItemPos.X ? 1 : 0);
		}
	}

	// Determine shift amount. Just find a pivot item
	bool bDetermined = false;
	int32 ShiftAmount = 0;

	for (int32 i = 0; i < ItemListLength; i++)
	{
		if (ScrollItemList[i]->GetVisibility() == ESlateVisibility::Visible)
		{

			ShiftAmount = (ScrollItemList[i]->Count / GridCount) - (i / GridCount) - dataIndex;
			bDetermined = true;
		}
	}

	if (!bDetermined || ShiftAmount <= -TotalRanks || ShiftAmount >= TotalRanks)
	{
		// No overlap
		return;
	}

	if (ShiftAmount == 0)
	{
		// Full overlap
		return;
	}

	// Shift in place
	if (ShiftAmount > 0) {
		for (int32 i = TotalRanks - 1; i >= ShiftAmount; i--)
		{
			ScrollItemList_SwapRow(i, i - ShiftAmount);
		}
	}
	else
	{
		for (int32 i = 0; i < TotalRanks + ShiftAmount; i++)
		{
			ScrollItemList_SwapRow(i, i - ShiftAmount);
		}
	}

}

void URScrollView::ScrollItemList_SwapRow(int32 a, int32 b)
{
	for (int32 i = 0; i < GridCount; i++)
	{
		int32 ida = a * GridCount + i;
		int32 idb = b * GridCount + i;
		if (ScrollItemList.IsValidIndex(ida) && ScrollItemList.IsValidIndex(idb))
		{
			auto temp = ScrollItemList[ida];
			ScrollItemList[ida] = ScrollItemList[idb];
			ScrollItemList[idb] = temp;
		}
	}
}

FReply URScrollView::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchStarted"));
	ScrollItemOnTouchStarted();
	return FReply::Handled();
}

FReply URScrollView::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{

	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchMoved"));
	ScrollItemOnTouchMoved(InGestureEvent);
	return FReply::Handled();
}

FReply URScrollView::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{

	//UE_LOG(LogUI, Log, TEXT("NativeOnTouchEnded"));
	ScrollItemOnTouchEnded();
	return FReply::Handled();
}

void URScrollView::PrintScrollItemList()
{
	UE_LOG(LogUI, Log, TEXT("PrintScrollItemList"));
	for (int32 i = 0; i < ScrollItemList.Num(); i++) {
		UE_LOG(LogUI, Log, TEXT("ScrollItemList[%d] - %d"), i, ScrollItemList[i]->Count);
	}
}

void URScrollView::ScrollItemList_ChangeItemDataList(int32 dataIndex)
{
	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();
	
	if (SpecialtyType == EScrollViewTypeEnum::VE_Grid)
	{
		dataIndex = dataIndex * GridCount;
	}
	
	for (int32 i = 0; i < ItemListLength; ++i)
	{
		Item = ScrollItemList[i];
		if (IsValid(Item))
		{
			int32 PrevItemCount = Item->Count;

			Item->Count = dataIndex + i;

			if (Item->Count < DataListLength)
			{
				if (Item->Count != PrevItemCount || Item->GetVisibility() != ESlateVisibility::Visible)
				{
					Item->SetVisibility(ESlateVisibility::Visible);
					Item->ItemData = ItemDataList[Item->Count];
					Item->SetScrollItemData(i, ItemDataList[Item->Count], this);
					Item->InvalidateData();
				}
			}
			else
			{
				Item->SetVisibility(ESlateVisibility::Hidden);
			}

			Item->IsSelected = false;
			Item->SetSelected(false);
		}
	}

	for (int32 idx : SelectedScrollItemList)
	{
		for (URScrollItem* ItemElem : ScrollItemList)
		{
			if (IsValid(ItemElem))
			{
				if (ItemElem->Count == idx)
				{
					ItemElem->IsSelected = true;
					ItemElem->SetSelected(true);
				}
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////
// ScrollItemList Scroll

void URScrollView::SetScrollUpdate(URScrollItem* CurItem, const FPointerEvent& InEvent)
{
	SetScrollUpdate(CurItem, InEvent.GetCursorDelta());
}

void URScrollView::SetScrollUpdate(URScrollItem* CurItem, FVector2D diff) {

	// UE_LOG(LogUI, Log, TEXT("SetScrollUpdate(%s) with Cur=%f"), *diff.ToString(), CurScrollOffset);

	if (!IsScrollAble)
		return;

	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (ItemListLength == 0)
		return;

	if (DataListLength == 0)
		return;

	// Handle overscroll
	if (!bIsScrollingActiveTimerRegistered) {
		float fMoveDist = CalculateMoveDist();
		//UE_LOG(LogUI, Log, TEXT("-fMoveDist=%f CurScrollOffset=%f"), -fMoveDist, CurScrollOffset);

		float ScrollAmount = -GetScrollComponentFromVector(diff);

		if (Overscroll.ShouldApplyOverscroll(CurScrollOffset == 0, CurScrollOffset == -fMoveDist, ScrollAmount))
		{
			//UE_LOG(LogUI, Log, TEXT("Exerting overscroll: %f"), ScrollAmount);
			bIsScrollingActiveTimerRegistered = false;
			InertialScrollManager.ClearScrollVelocity();
			Overscroll.ScrollBy(ScrollAmount);
		}
	}


	float fMoveDist = 0.0f;
	float fMoveValue = 0.0f;

	switch (SpecialtyType)
	{
	case EScrollViewTypeEnum::VE_None:
	{
		// Check Distance
		if (Horizontal)
		{
			fMoveDist = (float)DataListLength * ItemInitSize.X - ScrollViewSize.X;
			CurScrollOffset += diff.X;
		}
		else
		{
			fMoveDist = (float)DataListLength * ItemInitSize.Y - ScrollViewSize.Y;
			CurScrollOffset += diff.Y;
		}

		// min max condition
		if (fMoveDist < 0.0f)					// ItemTotalLength < ScrollViewLength
		{
			CurScrollOffset = 0.0f;
		}
		else if (CurScrollOffset <= -fMoveDist)	// ScrollIng Tail
		{
			CurScrollOffset = -fMoveDist;
		}
		else if (CurScrollOffset >= 0.0f)		// ScrollIng Head
		{
			CurScrollOffset = 0.0f;
		}

		float PhysicalScrollOffset = CurScrollOffset - Overscroll.GetOverscroll();

		fMoveValue = PhysicalScrollOffset;

		// if (DataListLength > ItemListLength)
		{
			int CurIdx = 0;

			if (Horizontal)
			{
				if (PhysicalScrollOffset > -ItemInitSize.X)
				{
					CurIdx = 0;
					fMoveValue = PhysicalScrollOffset;
				}
				else if ((DataListLength >= ItemListLength) && (PhysicalScrollOffset <= ((float)(DataListLength - ItemListLength) * ItemInitSize.X) * -1.0f))
				{
					CurIdx = DataListLength - ItemListLength;
					fMoveValue = PhysicalScrollOffset + (float)(DataListLength - ItemListLength) * ItemInitSize.X;
				}
				else
				{
					CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.X * -1.0f) - 1;
					fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.X) - ItemInitSize.X;
				}
			}
			else
			{
				if (PhysicalScrollOffset > -ItemInitSize.Y)
				{
					CurIdx = 0;
					fMoveValue = PhysicalScrollOffset;
				}
				else if ((DataListLength >= ItemListLength) && (PhysicalScrollOffset <= ((float)(DataListLength - ItemListLength) * ItemInitSize.Y) * -1.0f))
				{
					CurIdx = DataListLength - ItemListLength;
					fMoveValue = PhysicalScrollOffset + (float)(DataListLength - ItemListLength) * ItemInitSize.Y;
				}
				else
				{
					CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.Y * -1.0f) - 1;
					fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.Y) - ItemInitSize.Y;
				}
			}

			if (CurIdx != StartIdx)
			{
				ShiftScrollItemList(CurIdx);
				ScrollItemList_ChangeItemDataList(CurIdx);
				StartIdx = CurIdx;
			}
		}
	}
	break;
	case EScrollViewTypeEnum::VE_Grid:
	{
		FIntPoint ItemPos;
		GetListPosByIndex(ItemListLength, ItemPos);
		FIntPoint DataPos;
		GetListPosByIndex(DataListLength, DataPos);

		int32 GridItemLength = 0;
		int32 GridDataLength = 0;

		// common distance
		if (Horizontal)
		{
			GridItemLength = ItemPos.X + (ItemPos.Y ? 1 : 0);
			GridDataLength = DataPos.X + (DataPos.Y ? 1 : 0);

			fMoveDist = (float)GridDataLength * ItemInitSize.X - ScrollViewSize.X;
			CurScrollOffset += diff.X;
		}
		else
		{
			GridItemLength = ItemPos.Y + (ItemPos.X ? 1 : 0);
			GridDataLength = DataPos.Y + (DataPos.X ? 1 : 0);

			fMoveDist = (float)GridDataLength * ItemInitSize.Y - ScrollViewSize.Y;
			CurScrollOffset += diff.Y;
		}

		// min max condition
		if (fMoveDist < 0.0f)					// ItemTotalLength < ScrollViewLength
		{
			CurScrollOffset = 0.0f;
		}
		else if (CurScrollOffset <= -fMoveDist)	// ScrollIng Tail
		{
			CurScrollOffset = -fMoveDist;
		}
		else if (CurScrollOffset >= 0.0f)		// ScrollIng Head
		{
			CurScrollOffset = 0.0f;
		}

		float PhysicalScrollOffset = CurScrollOffset - Overscroll.GetOverscroll();

		fMoveValue = PhysicalScrollOffset;


		//if (DataListLength > ItemListLength)
		//if (GridDataLength > GridItemLength)
		{
			int CurIdx = 0;

			if (Horizontal)
			{
				if (PhysicalScrollOffset > -ItemInitSize.X)
				{
					CurIdx = 0;
					fMoveValue = PhysicalScrollOffset;
				}
				else if ((GridDataLength >= GridItemLength) && (PhysicalScrollOffset <= ((float)(GridDataLength - GridItemLength) * ItemInitSize.X) * -1.0f))
				{
					CurIdx = GridDataLength - GridItemLength;
					fMoveValue = PhysicalScrollOffset + (float)(GridDataLength - GridItemLength) * ItemInitSize.X;
				}
				else
				{
					CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.X * -1.0f) - 1;
					fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.X) - ItemInitSize.X;
				}
			}
			else
			{
				if (PhysicalScrollOffset > -ItemInitSize.Y)
				{
					CurIdx = 0;
					fMoveValue = PhysicalScrollOffset;
				}
				else if ((GridDataLength >= GridItemLength) && (PhysicalScrollOffset <= ((float)(GridDataLength - GridItemLength) * ItemInitSize.Y) * -1.0f))
				{
					CurIdx = GridDataLength - GridItemLength;
					fMoveValue = PhysicalScrollOffset + (float)(GridDataLength - GridItemLength) * ItemInitSize.Y;
				}
				else
				{
					CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.Y * -1.0f) - 1;
					fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.Y) - ItemInitSize.Y;
				}
			}

			if (CurIdx != StartIdx)
			{
				ShiftScrollItemList(CurIdx);
				ScrollItemList_ChangeItemDataList(CurIdx);
				StartIdx = CurIdx;
			}
		}
	}
	break;
	case EScrollViewTypeEnum::VE_LimitEnable:
	{
		float fTotalDist = 0.0f;
		for (int32 i = 0; i < ItemListLength; ++i)
		{
			Item = ScrollItemList[i];
			if (IsValid(Item))
			{
				if (Horizontal)
				{
					fTotalDist += Item->Size.X;
				}
				else
				{
					fTotalDist += Item->Size.Y;
				}
			}
		}

		// Check Distance
		if (Horizontal)
		{
			fMoveDist = fTotalDist - ScrollViewSize.X;
			CurScrollOffset += diff.X;
		}
		else
		{
			fMoveDist = fTotalDist - ScrollViewSize.Y;
			CurScrollOffset += diff.Y;
		}

		// min max condition
		if (fMoveDist < 0.0f)					// ItemTotalLength < ScrollViewLength
		{
			CurScrollOffset = 0.0f;
		}
		else if (CurScrollOffset <= -fMoveDist)	// ScrollIng Tail
		{
			CurScrollOffset = -fMoveDist;
		}
		else if (CurScrollOffset >= 0.0f)		// ScrollIng Head
		{
			CurScrollOffset = 0.0f;
		}

		float PhysicalScrollOffset = CurScrollOffset - Overscroll.GetOverscroll();

		fMoveValue = PhysicalScrollOffset;
	}
	break;
	}

	SetScrollUpdate(fMoveValue);
}
void URScrollView::SetScrollUpdate(float fMoveValue)
{
	float fTotal = 0.0f;
	int32 ListLength = ScrollItemList.Num();
	for (int32 i = 0; i < ListLength; ++i)
	{
		if (URScrollItem* Item = ScrollItemList[i])
		{
			switch (SpecialtyType)
			{
			case EScrollViewTypeEnum::VE_None:
			{
				if (Horizontal)
				{
					SetCanvasPanelXPos(Item, fMoveValue + (float)i * ItemInitSize.X);
				}
				else
				{
					SetCanvasPanelYPos(Item, fMoveValue + (float)i * ItemInitSize.Y);
				}
			}
			break;
			case EScrollViewTypeEnum::VE_Grid:
			{
				int32 Line = i / GridCount;
				if (Horizontal)
				{
					SetCanvasPanelXPos(Item, fMoveValue + (float)Line * ItemInitSize.X);
				}
				else
				{
					SetCanvasPanelYPos(Item, fMoveValue + (float)Line * ItemInitSize.Y);
				}
			}
			break;
			case EScrollViewTypeEnum::VE_LimitEnable:
			{
				if (Horizontal)
				{
					SetCanvasPanelXPos(Item, fMoveValue + fTotal);
					fTotal += Item->Size.X;
				}
				else
				{
					SetCanvasPanelYPos(Item, fMoveValue + fTotal);
					fTotal += Item->Size.Y;
				}
			}
			break;
			}
		}
	}
}

void URScrollView::SetTimeEvent(URScrollItem* SelecteItem)
{
	if (IsValid(SelecteItem))
	{
		if (IsValid(SelecteItem->ItemData))
			OnScrollViewTimeEventItemData.Broadcast(SelecteItem->ItemData);

		OnScrollViewTimeEventItem.Broadcast(SelecteItem);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// ScrollItemList Select

void URScrollView::SetSelected(URScrollItem* SelecteItem)
{
	if (!IsValid(SelecteItem))
		return;

	if (IsValid(SelecteItem->ItemData))
	{
		if (!SelecteItem->ItemData->bSelectEnable)
			return;
	}

	bool bExe = true;
	if (MultiSelect)
	{
		if (SelecteItem->IsSelected)
		{
			SelecteItem->IsSelected = false;
			SelecteItem->SetSelected(false);
			SelecteItem->ItemData->bSelected = false;
			SelectedScrollItemList.Remove(SelecteItem->Count);
			SelectedScrollItemDataList.Remove(SelecteItem->ItemData);
		}
		else
		{
			SelecteItem->IsSelected = true;
			SelecteItem->SetSelected(true);
			SelecteItem->ItemData->bSelected = true;
			SelectedScrollItemList.Emplace(SelecteItem->Count);
			SelectedScrollItemDataList.Emplace(SelecteItem->ItemData);
		}
		
		OnScrollViewSelectedItemData.Broadcast(SelecteItem->ItemData);
	}
	else
	{
		for (URScrollItem* Item : ScrollItemList)
		{
			if (IsValid(Item))
			{
				if (Item->Count != SelecteItem->Count)
				{
					Item->IsSelected = false;
					Item->SetSelected(false);

					if (IsValid(Item->ItemData))
						Item->ItemData->bSelected = false;
				}
				else
				{
					if (SelecteItem->IsSelected)
					{
						if (CheckSelect)
						{
							SelectedScrollItemList.Reset();
							SelectedScrollItemDataList.Reset();

							SelecteItem->IsSelected = false;
							SelecteItem->SetSelected(false);

							if (IsValid(SelecteItem->ItemData))
								SelecteItem->ItemData->bSelected = false;

							OnScrollViewSelectedItemData.Broadcast(SelecteItem->ItemData);
						}
						else
						{
							bExe = false;
						}
					}
					else
					{
						SelectedScrollItemList.Reset();
						SelectedScrollItemDataList.Reset();

						SelecteItem->IsSelected = true;
						SelecteItem->SetSelected(true);
						SelectedScrollItemList.Emplace(SelecteItem->Count);
						SelectedScrollItemDataList.Emplace(SelecteItem->ItemData);

						if (IsValid(SelecteItem->ItemData))
							SelecteItem->ItemData->bSelected = true;

						OnScrollViewSelectedItemData.Broadcast(SelecteItem->ItemData);
					}					
				}
			}
		}
	}

	if (bExe)
	{
		OnScrollViewSelectedItem.Broadcast(SelecteItem);

		OnGetSelectedScrollItemDataEvent.ExecuteIfBound(SelectedScrollItemDataList);
	}
}

void URScrollView::ClearAllSelected()
{
	for (URScrollItem* Item : ScrollItemList)
	{
		if (IsValid(Item))
		{
			Item->IsSelected = false;
			Item->SetSelected(false);
		}
	}
	SelectedScrollItemList.Reset();
	SelectedScrollItemDataList.Reset();
}

URScrollItem* URScrollView::GetScrollItem(int32 ListIndex)
{
	if (ScrollItemList.IsValidIndex(ListIndex))
	{
		return ScrollItemList[ListIndex];
	}

	return nullptr;
}

void URScrollView::SelectInvalidateData()
{
	for (URScrollItem* Item : ScrollItemList)
	{
		if (IsValid(Item) && IsValid(Item->ItemData))
		{		
			Item->IsSelected = Item->ItemData->bSelected;
			Item->SetSelected(Item->ItemData->bSelected);
		}
	}
}

void URScrollView::SetSelectScrollListIndex(int32 ListIndex)
{
	SetSelected(GetScrollItem(ListIndex));
	//if (MultiSelect)
	//{
	//	if (ItemDataList.IsValidIndex(ListIndex))
	//	{
	//		if (IsValid(ItemDataList[ListIndex]))
	//		{
	//			ItemDataList[ListIndex]->bSelected = true;
	//			SelectedScrollItemDataList.Add(ItemDataList[ListIndex]);
	//			OnScrollViewSelectedItemData.Broadcast(ItemDataList[ListIndex]);
	//		}
	//	}
	//}
	//else
	//{
	//	SelectedScrollItemList.Reset();
	//	SelectedScrollItemDataList.Reset();
	//	for (int32 i = 0; i < ItemDataList.Num(); ++i)
	//	{
	//		URScrollItemData* itemData = ItemDataList[i];
	//		if (IsValid(itemData))
	//		{
	//			if (i == ListIndex)
	//			{
	//				itemData->bSelected = true;
	//				SelectedScrollItemDataList.Add(itemData);
	//				OnScrollViewSelectedItemData.Broadcast(itemData);
	//			}
	//			else
	//			{
	//				itemData->bSelected = false;
	//			}
	//		}
	//	}
	//}

	//SelectInvalidateData();
}

void URScrollView::SetSelectItemDataListIndex(int32 ListIndex)
{
	URScrollItem* SelectScrollItem = nullptr;

	if (ItemDataList.IsValidIndex(ListIndex))
	{
		URScrollItemData* TargetItemData = ItemDataList[ListIndex];

		if (IsValid(TargetItemData))
		{
			for (URScrollItem* Item : ScrollItemList)
			{
				if (IsValid(Item))
				{
					if (Item->ItemData == TargetItemData)
					{
						SelectScrollItem = Item;
						break;
					}
				}
			}
		}
	}

	if (IsValid(SelectScrollItem))
	{
		SetSelected(SelectScrollItem);
	}
}

void URScrollView::SetScrollPos(URScrollItemData* SelectItemData)
{
	int32 FindIdx = -1;
	for (int32 i = 0; i < ItemDataList.Num(); ++i)
	{
		if (IsValid(ItemDataList[i]))
		{
			if (SelectItemData == ItemDataList[i])
			{
				FindIdx = i;
				break;
			}
		}
	}

	if (FindIdx != -1)
	{
		FVector2D diff = FVector2D::ZeroVector;

		if (Horizontal)
		{
			diff.X = -ItemInitSize.X * FindIdx;
		}
		else
		{
			diff.Y = -ItemInitSize.Y * FindIdx;
		}

		if (ScrollItemList.Num() > 0)
		{
			CurScrollOffset = 0;
			SetScrollUpdate(ScrollItemList[0], diff);
		}
	}
}

void URScrollView::SetSelectScrollListData(URScrollItemData* SelectItemData)
{
	if (IsValid(SelectItemData))
	{
		if (!SelectItemData->bSelectEnable)
			return;
	}

	bool bExe = false;
	if (MultiSelect)
	{		
		for (int32 i = 0; i < ItemDataList.Num(); ++i)
		{
			URScrollItemData* itemData = ItemDataList[i];
			if (IsValid(itemData))
			{
				if (itemData == SelectItemData)
				{
					bExe = true;
					if (itemData->bSelected)
					{
						itemData->bSelected = false;
						SelectedScrollItemDataList.Remove(itemData);

						if (SelectedScrollItemList.Contains(i))
						{
							SelectedScrollItemList.Remove(i);
						}
					}
					else
					{
						itemData->bSelected = true;
						SelectedScrollItemDataList.Emplace(itemData);

						if (!SelectedScrollItemList.Contains(i))
						{
							SelectedScrollItemList.Emplace(i);
						}
					}					
					
					OnScrollViewSelectedItemData.Broadcast(itemData);
				}
			}
		}
	}
	else
	{
		for (int32 i = 0; i < ItemDataList.Num(); ++i)
		{
			URScrollItemData* itemData = ItemDataList[i];
			if (IsValid(itemData))
			{
				if (itemData == SelectItemData)
				{
					if (itemData->bSelected)
					{
						if (CheckSelect)
						{
							bExe = true;

							SelectedScrollItemList.Reset();
							SelectedScrollItemDataList.Reset();

							itemData->bSelected = false;

							OnScrollViewSelectedItemData.Broadcast(itemData);
						}
					}
					else
					{
						bExe = true;

						SelectedScrollItemList.Reset();
						SelectedScrollItemDataList.Reset();

						itemData->bSelected = true;
						SelectedScrollItemDataList.Emplace(itemData);
						SelectedScrollItemList.Emplace(i);
						OnScrollViewSelectedItemData.Broadcast(itemData);
					}					
				}
				else
				{
					itemData->bSelected = false;
				}
			}
		}
	}

	if (bExe)
	{
		OnGetSelectedScrollItemDataEvent.ExecuteIfBound(SelectedScrollItemDataList);
	}

	SelectInvalidateData();
}

TArray<int32>& URScrollView::GetSelectedScrollItem()
{
	return SelectedScrollItemList;
}

bool URScrollView::IsSelectedScrollItem()
{
	return SelectedScrollItemList.Num() > 0 ? true : false;
}

TArray<URScrollItemData*>& URScrollView::GetSelectedScrollItemData()
{
	return SelectedScrollItemDataList;
}

URScrollItemData* URScrollView::GetSelectedScrollItemDataOnly()
{
	if (SelectedScrollItemDataList.Num() > 0)
		return SelectedScrollItemDataList[0];

	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////
// ItemDataList Element

int32 URScrollView::GetScrollItemDataCount()
{
	return ItemDataList.Num();
}

void URScrollView::ClearScrollItemData(int32 InSize /*=0*/)
{
	ItemDataList.Reset(InSize);
}

URScrollItemData* URScrollView::AddScrollItemData(TSubclassOf<class URScrollItemData> dataClass)
{
	URScrollItemData* ScrollItemData = CreateScrollItemData(dataClass);
	if (ScrollItemData != nullptr)
	{
		ScrollItemData->WidgetSize = ItemInitSize;
		ItemDataList.Emplace(ScrollItemData);
	}

	return ScrollItemData;
}

void URScrollView::RemoveScrollItemData(URScrollItemData* dataClass)
{
	if (ItemDataList.Contains(dataClass))
	{
		ItemDataList.Remove(dataClass);
	}
}

void URScrollView::UpdateScrollItemData(URScrollItemData* dataClass)
{
	if (IsValid(dataClass))
	{
		for (URScrollItem* Item : ScrollItemList)
		{
			if (IsValid(Item) && IsValid(Item->ItemData) && Item->ItemData == dataClass)
			{
				Item->SetScrollItemData(Item->Count, Item->ItemData, this);
				Item->InvalidateData();
				break;
			}
		}
	}
}


URScrollItemData* URScrollView::CreateScrollItemData(TSubclassOf<class URScrollItemData> dataClass)
{
	return NewObject<URScrollItemData>((UObject*)GetTransientPackage(), dataClass);
}

void URScrollView::RefreshScrollItemData()
{
	ScrollItemList_ChangeItemDataList(StartIdx);
}

void URScrollView::SetData_Implementation(TArray<URScrollItemData*>& list)
{
	ItemDataList = list;

	Init();
}

void URScrollView::AddItemDataList(URScrollItemData* InScrollItemData)
{
	ItemDataList.Emplace(InScrollItemData);
}





void URScrollView::Init()
{
	if (IsValid(BGPanel))
	{
		BGPanel->SetRenderTransformPivot(FVector2D::ZeroVector);
		BGPanel->SetRenderScale(InitScale);
	}

	ScrollItemList_RemoveAllElement();

	CurScrollOffset = 0.0f;
	StartIdx = 0;

	ScrollItemList_AddElement();

	switch (SpecialtyType)
	{
	case EScrollViewTypeEnum::VE_None:
	{
		if (Horizontal)
		{
			if (ItemInitSize.X > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.X / ItemInitSize.X);
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.X / ItemInitSize.X);
				CreateCount = VisibleCreateCount + 2;
			}
		}
		else
		{
			if (ItemInitSize.Y > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.Y / ItemInitSize.Y);
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.Y / ItemInitSize.Y);
				CreateCount = VisibleCreateCount + 2;
			}
		}
	}
	break;
	case EScrollViewTypeEnum::VE_Grid:
	{
		if (Horizontal)
		{
			if (ItemInitSize.X > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.X / ItemInitSize.X) * GridCount;
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.X / ItemInitSize.X) * GridCount;
				CreateCount = VisibleCreateCount + (2 * GridCount);				
			}
		}
		else
		{
			if (ItemInitSize.Y > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.Y / ItemInitSize.Y) * GridCount;
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.Y / ItemInitSize.Y) * GridCount;
				CreateCount = VisibleCreateCount + (2 * GridCount);
			}
		}
	}
	break;
	case EScrollViewTypeEnum::VE_LimitEnable:
	{
		if (Horizontal)
		{
			if (ItemInitSize.X > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.X / ItemInitSize.X);
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.X / ItemInitSize.X);
				CreateCount = VisibleCreateCount + 2;
			}
		}
		else
		{
			if (ItemInitSize.Y > 0.0f)
			{
				VisibleMaxCount = FMath::FloorToInt(ScrollViewSize.Y / ItemInitSize.Y);
				VisibleCreateCount = FMath::CeilToInt(ScrollViewSize.Y / ItemInitSize.Y);
				CreateCount = VisibleCreateCount + 2;
			}
		}

// 		VisibleMaxCount = ItemDataList.Num();
// 		VisibleCreateCount = ItemDataList.Num();
// 		CreateCount = ItemDataList.Num();
	}
	break;
	}

	

	for (int32 i = 1; i < CreateCount; ++i)
	{
		ScrollItemList_AddElement();
	}

	ResetScrollAnimationState();

	/*if (IsSpawnAni && IsFirstInit)
	{
		for (int32 i = 0; i < VisibleCreateCount; ++i)
		{
			if (ScrollItemList.IsValidIndex(i))
			{
				ScrollItemList[i]->PlayAni("SpawnAni");
			}
			else
			{
				break;
			}
		}
	}
	IsFirstInit = false;*/
}

void URScrollView::Init_NoClear(int32 InCreateCount)
{
	if (IsValid(BGPanel))
	{
		BGPanel->SetRenderTransformPivot(FVector2D::ZeroVector);
		BGPanel->SetRenderScale(InitScale);
	}

	CurScrollOffset = 0.0f;
	StartIdx = 0;
	CreateCount = InCreateCount;

	//for (int32 i = 0; i < InCreateCount; ++i)
	//	ScrollItemList_AddElement();

	ScrollItemList_AddElement(InCreateCount);
	ResetScrollAnimationState();
	
	bInitNoClear = true;
	
}

void URScrollView::InvalidateData()
{
	CurScrollOffset = 0.0f;
	StartIdx = 0;
	ScrollItemList_SetItemDataList();
	SetScrollUpdate(CurScrollOffset);
	ResetScrollAnimationState();
}

void URScrollView::InvalidateDataWithoutResetSelect()
{
	CurScrollOffset = 0.0f;
	StartIdx = 0;

	bool bResetSelectList = false;
	ScrollItemList_SetItemDataList(bResetSelectList);
	SetScrollUpdate(CurScrollOffset);

	ResetScrollAnimationState();
}


void URScrollView::UpdateTargetScrollItem(URScrollItem* InTargetScrollItem)
{
	if (!IsValid(InTargetScrollItem))
		return;

	if (!ItemDataList.IsValidIndex(InTargetScrollItem->Count))
		return;

	InTargetScrollItem->SetVisibility(ESlateVisibility::Visible);
	InTargetScrollItem->ItemData = ItemDataList[InTargetScrollItem->Count];
	InTargetScrollItem->SetScrollItemData(InTargetScrollItem->Count, InTargetScrollItem->ItemData, this);
	InTargetScrollItem->InvalidateData();
}

void URScrollView::Refresh()
{
	for (URScrollItem* Item : ScrollItemList)
	{
		if (IsValid(Item) && IsValid(Item->ItemData))
		{
			Item->SetScrollItemData(Item->Count, Item->ItemData, this);
			Item->InvalidateData();
		}
	}

	if (IsValid(BGPanel))
	{	
		if ((VisibleMaxCount < ScrollItemList.Num() ) && ( ScrollItemList.Num() % GridCount > 0 ) )
			BGPanel->SetVisibility(ESlateVisibility::Visible);
		else
			BGPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


UClass*	URScrollView::GetScrollItemBpClass()
{
	UClass* ResultClass = ScrollItemBpClass.Get();
	if (ResultClass == nullptr )
	{
		ScrollItemBpClass.ToSoftObjectPath().TryLoad();
		ResultClass = ScrollItemBpClass.Get();
	}
	return ResultClass;
}