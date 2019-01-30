// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GenericPopup.h"

#include "GlobalIntegrated.h"
#include "Table/WidgetBluePrintTableInfo.h"
#include "UC_GenericPopupContent.h"


void UUC_GenericPopup::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Swap in Widget_Content from WidgetBlueprint
	if (IsValid(WidgetClass.Get()))
	{
		if (CachedWidgetMap.Contains(WidgetClass->GetName()))
		{
			UUserWidget** widget = CachedWidgetMap.Find(WidgetClass->GetName());
			if (widget && IsValid(*widget))
				Widget_Content = *widget;
		}
		else
		{
			Widget_Content = CreateWidget<UUserWidget>(GetWorld(), WidgetClass.Get());
			CachedWidgetMap.Emplace(WidgetClass->GetName(), Widget_Content);
		}
		
		if (!ensure(IsValid(Widget_Content)))
			return;
		

#if WITH_EDITOR
		// needed to prevent NativeConstruct to be called from Widget Editor Preview
		Widget_Content->SetDesignerFlags(GetDesignerFlags());
#endif
		auto GenericPopupContent = Cast<UUC_GenericPopupContent>(Widget_Content);
		if (IsValid(GenericPopupContent))
		{
			GenericPopupContent->ParentPopup = this;
		}
		if(Panel_Header)
			Panel_Header->SetVisibility(bHideTitleBar ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

		InvalidateTitleBar();
		if( Panel_Body )
		{
			Panel_Body->ClearChildren();
			Panel_Body->AddChild(Widget_Content);
		}
	}
}

void UUC_GenericPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Back))
	{
		Button_Back->OnClicked.Clear();
		Button_Back->OnClicked.AddDynamic(this, &UUC_GenericPopup::OnClick_Back);
	}

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_GenericPopup::OnClick_Back);
	}
}

void UUC_GenericPopup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GenericPopup::InvalidateTitleBar()
{
	if (!TitleLocalizingKey.IsNone())
	{
		Text_PopupTitle->LocalizingKey = TitleLocalizingKey;
		Text_PopupTitle->StringTableType = TitleStringTableType;
		Text_PopupTitle->ApplyLocalizedText();
	}
	else
	{
		Text_PopupTitle->SetText(TitleOverrideText);
	}
}

UUserWidget* UUC_GenericPopup::GetContentWidget()
{
	return Widget_Content;
}

void UUC_GenericPopup::Close()
{
	RemoveFromParent();
}

void UUC_GenericPopup::InvalidateData()
{
	Super::InvalidateData();

	if (IsValid(Widget_Content))
	{
		auto RBaseWidget = Cast<URBaseWidget>(Widget_Content);
		if (IsValid(RBaseWidget))
		{
			RBaseWidget->InvalidateData();
		}
	}
}

void UUC_GenericPopup::OnClick_Back()
{
	if (IsInViewport())
	{
		RemoveFromViewport();
	}
}
