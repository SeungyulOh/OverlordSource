// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"


void UCheckBox_Radio::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (OnCheckStateChanged.IsAlreadyBound(this, &UCheckBox_Radio::RadioButtonOnCheckStateChanged))
		OnCheckStateChanged.RemoveDynamic(this, &UCheckBox_Radio::RadioButtonOnCheckStateChanged);

	OnCheckStateChanged.AddDynamic(this, &UCheckBox_Radio::RadioButtonOnCheckStateChanged);
}

void UCheckBox_Radio::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UCheckBox_Radio::PostLoad()
{
	Super::PostLoad();
}

void UCheckBox_Radio::RadioButtonOnCheckStateChanged(bool bIsChecked)
{
	if (bLock)
	{
		OnCheckRadioLockEvent.Broadcast();
		return;
	}

	if (bIsChecked == true)
		SetIsChecked(true);
	else
	{
		bIsChecked = true;
		SetIsChecked(true);
	}

	OnCheckRadioCheck.Broadcast(bIsChecked);
	OnCheckRadioCheck_StaticDelegate.ExecuteIfBound(bIsChecked);

	if (bIsChecked)
	{
		RadioGroupoComponentStateChanged.ExecuteIfBound(this->GetUniqueID(), bIsChecked);
	}
}