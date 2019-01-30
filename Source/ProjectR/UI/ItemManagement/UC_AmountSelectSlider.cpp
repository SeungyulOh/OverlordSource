// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_AmountSelectSlider.h"


void UUC_AmountSelectSlider::OnSliderValueChanged(float Value)
{
	CurrentCount = FMath::RoundToInt((float)MaxCount * Value);

	InvalidateOnCountChanged();
	OnCountChangedDelegate.ExecuteIfBound(CurrentCount);
}

void UUC_AmountSelectSlider::OnButtonSliderMinusClicked()
{
	if (CurrentCount > 0)
	{
		CurrentCount -= 1;
		InvalidateOnCountChanged();
		OnCountChangedDelegate.ExecuteIfBound(CurrentCount);
	}
}

void UUC_AmountSelectSlider::OnButtonSliderPlusClicked()
{
	if (CurrentCount < MaxCount)
	{
		CurrentCount += 1;
		InvalidateOnCountChanged();
		OnCountChangedDelegate.ExecuteIfBound(CurrentCount);
	}
}

void UUC_AmountSelectSlider::OnButtonMaxClicked()
{
	CurrentCount = MaxCount;

	InvalidateOnCountChanged();
	OnCountChangedDelegate.ExecuteIfBound(CurrentCount);
}

void UUC_AmountSelectSlider::InvalidateOnCountChanged()
{
	Slider_SelectedAmount->SetValue(MaxCount == 0 ? 0.0 : (float)CurrentCount / MaxCount);
}

void UUC_AmountSelectSlider::NativeConstruct()
{
	Super::NativeConstruct();

	Slider_SelectedAmount->OnValueChanged.AddDynamic(this, &UUC_AmountSelectSlider::OnSliderValueChanged);

	Button_Decrease->OnClicked.AddDynamic(this, &UUC_AmountSelectSlider::OnButtonSliderMinusClicked);
	Button_Increase->OnClicked.AddDynamic(this, &UUC_AmountSelectSlider::OnButtonSliderPlusClicked);
	Button_Max->OnClicked.AddDynamic(this, &UUC_AmountSelectSlider::OnButtonMaxClicked);
}

void UUC_AmountSelectSlider::NativeDestruct()
{
	if (IsValid(Button_Max))
		Button_Max->OnClicked.RemoveAll(this);
	if (IsValid(Button_Increase))
		Button_Increase->OnClicked.RemoveAll(this);
	if (IsValid(Button_Decrease))
		Button_Decrease->OnClicked.RemoveAll(this);
	if (IsValid(Slider_SelectedAmount))
		Slider_SelectedAmount->OnValueChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_AmountSelectSlider::InvalidateData()
{
	Super::InvalidateData();

	InvalidateOnCountChanged();
}
