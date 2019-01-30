// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_MaterialSpinBox.h"

// TODO: all

void UUC_MaterialSpinBox::NativeConstruct()
{
	Super::NativeConstruct();

	Slider_Count->OnValueChanged.AddDynamic(this, &UUC_MaterialSpinBox::OnSliderValueChanged);
}

void UUC_MaterialSpinBox::NativeDestruct()
{
	Slider_Count->OnValueChanged.RemoveDynamic(this, &UUC_MaterialSpinBox::OnSliderValueChanged);

	Super::NativeDestruct();
}

void UUC_MaterialSpinBox::InvalidateData()
{
	Super::InvalidateData();

	// update Text_Count
	// "1 / 1000"

	FText CountText = FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentValue, CountMaterialOwned));
	Text_Count->SetText(CountText);

	// The slider is disabled if there isn't anything to slide
	Slider_Count->SetIsEnabled(ValueMin != ValueMax);
}

void UUC_MaterialSpinBox::OnSliderValueChanged(float Value)
{
	CurrentValue = ValueMin + FMath::RoundToInt((float)(ValueMax - ValueMin) * Value);
	
	InvalidateData();

	OnValueChanged.Broadcast(CurrentValue);
}
