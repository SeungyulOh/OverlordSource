// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_GenericSplash.h"

#include "GlobalIntegrated.h"

void UUP_GenericSplash::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Swap in the actual content widget
	if (IsValid(ContentWidgetClass.Get()))
	{
		if(!IsValid(ContentWidget))
			ContentWidget = CreateWidget<UUserWidget>(GetWorld(), ContentWidgetClass.Get());

		if (!ensure(IsValid(ContentWidget)))
			return;
#if WITH_EDITOR
			ContentWidget->SetDesignerFlags(GetDesignerFlags());
#endif
		if (Panel_Content)
		{
			Panel_Content->ClearChildren();
			Panel_Content->AddChild(ContentWidget);
		}
	}

	// PositionYOffset
	if (Panel_Splash)
	{
		UCanvasPanelSlot* ContentSlot = Cast<UCanvasPanelSlot>(Panel_Splash->Slot);
		if (!ensure(IsValid(ContentSlot)))
			return;

		FVector2D Position = ContentSlot->GetPosition();
		Position.Y = Position.Y + PositionYOffset;
		ContentSlot->SetPosition(Position);
	}


	// header type
	if(Switcher_Header)
		Switcher_Header->SetActiveWidgetIndex(bShowIconHeader ? 1 : 0);

	// caption
	if(Text_Caption)
		Text_Caption->SetText(CaptionText.GetText());

	// bottom label
	if(Text_BottomLabel)
		Text_BottomLabel->SetVisibility(bHideBottomLabel ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UUP_GenericSplash::InvalidateData()
{
	Super::InvalidateData();

	auto Widget = Cast<URBaseWidget>(ContentWidget);
	if (IsValid(Widget))
	{
		Widget->InvalidateData();
	}
}

UUserWidget* UUP_GenericSplash::GetContentWidget()
{
	return ContentWidget;
}
