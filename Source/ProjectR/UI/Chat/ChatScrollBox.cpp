// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChatScrollBox.h"
#include "SChatScrollBox.h"
#include "ChatScrollBoxSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UScrollBox

UChatScrollBox::UChatScrollBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;

	Orientation = Orient_Vertical;

	SChatScrollBox::FArguments Defaults;
	Visibility = UWidget::ConvertRuntimeToSerializedVisibility(Defaults._Visibility.Get());

	WidgetStyle = *Defaults._Style;
	WidgetBarStyle = *Defaults._ScrollBarStyle;

	AlwaysShowScrollbar = false;
	ScrollbarThickness = FVector2D(5, 5);
	ScrollBarVisibility = ESlateVisibility::Visible;

	ConsumeMouseWheel = EConsumeMouseWheel::WhenScrollingPossible;
}

void UChatScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyScrollBox.Reset();
}

UClass* UChatScrollBox::GetSlotClass() const
{
	return UChatScrollBoxSlot::StaticClass();
}

void UChatScrollBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyScrollBox.IsValid())
	{
		CastChecked<UChatScrollBoxSlot>(InSlot)->BuildSlot(MyScrollBox.ToSharedRef());
	}
}

void UChatScrollBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyScrollBox.IsValid())
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyScrollBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

void UChatScrollBox::OnUserScrolled(float ScrollOffset)
{
	// Determine if user has scrolled to the end
	const float ScrollOffsetEnd = MyScrollBox->GetScrollOffsetEnd();

	bool PrevIsUserScrolled = bIsUserScrolled;
	bIsUserScrolled = !FMath::IsNearlyEqual(ScrollOffset, ScrollOffsetEnd) && ScrollOffsetEnd >= 0.0;
	if (PrevIsUserScrolled && !bIsUserScrolled)
	{
		OnUserScrolledToEnd.Broadcast();
	}

	// UE_LOG(LogBattle, Log, TEXT("OnUserScrolled: %f / %f bIsUserScrolled = %d"), ScrollOffset, ScrollOffsetEnd, bIsUserScrolled ? 1 : 0);
}

TSharedRef<SWidget> UChatScrollBox::RebuildWidget()
{
	MyScrollBox = SNew(SChatScrollBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.Orientation(Orientation)
		.ConsumeMouseWheel(ConsumeMouseWheel)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, OnUserScrolled));


	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UChatScrollBoxSlot* TypedSlot = Cast<UChatScrollBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
		}
	}

	bIsUserScrolled = false;

	return MyScrollBox.ToSharedRef();
}

void UChatScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyScrollBox->SetScrollOffset(DesiredScrollOffset);
	MyScrollBox->SetOrientation(Orientation);
	MyScrollBox->SetScrollBarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollBarVisibility));
	MyScrollBox->SetScrollBarThickness(ScrollbarThickness);
	MyScrollBox->SetScrollBarAlwaysVisible(AlwaysShowScrollbar);
}

float UChatScrollBox::GetScrollOffset() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetScrollOffset();
	}

	return 0;
}

void UChatScrollBox::SetScrollOffset(float NewScrollOffset)
{
	DesiredScrollOffset = NewScrollOffset;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollOffset(NewScrollOffset);
	}
}

void UChatScrollBox::ScrollToStart()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToStart();
	}
}

void UChatScrollBox::ScrollToEnd()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToEnd();
	}
}

void UChatScrollBox::ScrollWidgetIntoView(UWidget* WidgetToFind, bool AnimateScroll)
{
	TSharedPtr<SWidget> SlateWidgetToFind;
	if (WidgetToFind)
	{
		SlateWidgetToFind = WidgetToFind->GetCachedWidget();
	}
	if (MyScrollBox.IsValid())
	{
		// NOTE: Pass even if null! This, in effect, cancels a request to scroll which is necessary to avoid warnings/ensures when we request to scroll to a widget and later remove that widget!
		MyScrollBox->ScrollDescendantIntoView(SlateWidgetToFind, AnimateScroll);
	}
}

void UChatScrollBox::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS)
	{
		if (Style_DEPRECATED != nullptr)
		{
			const FScrollBoxStyle* StylePtr = Style_DEPRECATED->GetStyle<FScrollBoxStyle>();
			if (StylePtr != nullptr)
			{
				WidgetStyle = *StylePtr;
			}

			Style_DEPRECATED = nullptr;
		}

		if (BarStyle_DEPRECATED != nullptr)
		{
			const FScrollBarStyle* StylePtr = BarStyle_DEPRECATED->GetStyle<FScrollBarStyle>();
			if (StylePtr != nullptr)
			{
				WidgetBarStyle = *StylePtr;
			}

			BarStyle_DEPRECATED = nullptr;
		}
	}
}

#if WITH_EDITOR

const FText UChatScrollBox::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

void UChatScrollBox::OnDescendantSelectedByDesigner(UWidget* DescendantWidget)
{
	UWidget* SelectedChild = UWidget::FindChildContainingDescendant(this, DescendantWidget);
	if (SelectedChild)
	{
		ScrollWidgetIntoView(SelectedChild, true);

		if (TickHandle.IsValid())
		{
			FTicker::GetCoreTicker().RemoveTicker(TickHandle);
			TickHandle.Reset();
		}
	}
}

void UChatScrollBox::OnDescendantDeselectedByDesigner(UWidget* DescendantWidget)
{
	if (TickHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	// because we get a deselect before we get a select, we need to delay this call until we're sure we didn't scroll to another widget.
	TickHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float) -> bool
	{
		this->ScrollToStart();
		return false;
	}));
}

void UChatScrollBox::OnDescendantSelected(UWidget* DescendantWidget)
{
	UWidget* SelectedChild = UWidget::FindChildContainingDescendant(this, DescendantWidget);
	if (SelectedChild)
	{
		ScrollWidgetIntoView(SelectedChild, true);

		if (TickHandle.IsValid())
		{
			FTicker::GetCoreTicker().RemoveTicker(TickHandle);
			TickHandle.Reset();
		}
	}
}

void UChatScrollBox::OnDescendantDeselected(UWidget* DescendantWidget)
{
	if (TickHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	// because we get a deselect before we get a select, we need to delay this call until we're sure we didn't scroll to another widget.
	TickHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float) -> bool
	{
		this->ScrollToStart();
		return false;
	}));
}

#endif

bool UChatScrollBox::GetIsUserScrolled() const
{
	return bIsUserScrolled;
}

void UChatScrollBox::RequestForceScroll()
{
	ScrollToEnd();
	bIsUserScrolled = false;
}

void UChatScrollBox::SetAnimateScroll()
{
	MyScrollBox->bAnimateScroll = true;
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
