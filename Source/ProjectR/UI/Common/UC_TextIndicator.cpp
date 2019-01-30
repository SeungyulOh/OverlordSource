// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "UC_TextIndicator.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"


void UUC_TextIndicator::NativeConstruct()
{
	Super::NativeConstruct();
	DisplayAni = GetAni("Display");

	if (IsValid(TextWidgetSwitcher))
	{
		TextWidgetSwitcher->SetActiveWidgetIndex(0);
	}

	if (Lock_Message)
	{
		Lock_Message->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_TextIndicator::ShowIndicatorString(FText& InText, float InDuration/* = 0.0f*/)
{
	if (Lock_Message)
	{
		Lock_Message->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(TextWidgetSwitcher))
		TextWidgetSwitcher->SetActiveWidgetIndex(0);
	
	IndicatorText->SetText(InText);

	bPlaying = true;
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAni("Display");

	fDuration = InDuration;

	if (fDuration == 0.0f)
	{
		fDuration = DisplayAni->GetEndTime() - DisplayAni->GetStartTime();
	}
}

void UUC_TextIndicator::ShowIndicatorStringNoAni(FText& InText, float InDuration/* = 0.0f*/)
{
	if (Lock_Message)
	{
		Lock_Message->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(TextWidgetSwitcher))
		TextWidgetSwitcher->SetActiveWidgetIndex(1);
	
	RichIndicatorText->SetRichText(InText);

//	bPlaying = true;
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//PlayAni("Display");

	bPlaying = true;
	fDuration = InDuration;

	if (fDuration == 0.0f)
	{
		fDuration = DisplayAni->GetEndTime() - DisplayAni->GetStartTime();
	}
}

void UUC_TextIndicator::ShowIndicatorStringBottom(FText& InText, float InDuration /*= 0.0f*/)
{
	if (IsValid(TextWidgetSwitcher))
	{
		TextWidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Lock_Message && LocalizingTextBlock_0)
	{
		LocalizingTextBlock_0->SetText(InText);
		Lock_Message->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	bPlaying = true;
	fDuration = InDuration;

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUC_TextIndicator::HideWidget()
{
	bPlaying = false;
	if (IsValid(TextWidgetSwitcher))
	{
		TextWidgetSwitcher->SetActiveWidgetIndex(0);
		TextWidgetSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (RichIndicatorText)
		RichIndicatorText->SetRichText(FText::GetEmpty());

	if (Lock_Message)
		Lock_Message->SetVisibility(ESlateVisibility::Collapsed);

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_TextIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlaying)
	{
		fDuration -= InDeltaTime;
		if (fDuration <= 0)
		{
			HideWidget();
		}
	}
}

bool UUC_TextIndicator::IsShowIndicator()
{
	return bPlaying;
}

void UUC_TextIndicator::SetIndicatorOffset(float InOffsetX, float InOffsetY)
{
	if(IsValid(NoAniBorder))
		NoAniBorder->SetRenderTranslation(FVector2D(InOffsetX, InOffsetY));
}