// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CheckBox_Radio.h"
#include "CanvasPanel_RadioGroup.h"

#include "UI/RBaseWidget.h"

void UCanvasPanel_RadioGroup::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (!IsValid(PanelSlot))
			continue;

		if (PanelSlot->Content == nullptr)
			continue;

		UCheckBox_Radio* RadioButton = Cast<UCheckBox_Radio>(PanelSlot->Content);
		if (!IsValid(RadioButton))
			continue;

		RadioButton->RadioGroupoComponentStateChanged.Unbind();
		RadioButton->RadioGroupoComponentStateChanged.BindDynamic(this, &UCanvasPanel_RadioGroup::RadioGroupoComponentStateChanged);
	}
}

void UCanvasPanel_RadioGroup::RadioGroupoComponentStateChanged(int32 UniqueID, bool bIsChecked)
{
	int32 CheckIdx = 0;
	int32 UnCheckIdx = 1;

	if (bSwitcherInverse)
	{
		CheckIdx = 1;
		UnCheckIdx = 0;
	}

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (!IsValid(PanelSlot))
			continue;

		if (PanelSlot->Content == nullptr)
			continue;

		UCheckBox_Radio* RadioButton = Cast<UCheckBox_Radio>(PanelSlot->Content);
		if (!IsValid(RadioButton))
			continue;

		if (RadioButton->GetUniqueID() != UniqueID)
		{
			RadioButton->SetIsChecked(!bIsChecked);
		}

		if (bWidgetSwitcher)
		{
			if (false == RadioButton->bLock)
			{
				if (IsValid(RadioButton->Switcher))
				{
					RadioButton->Switcher->SetActiveWidgetIndex(RadioButton->CheckedState == ECheckBoxState::Checked ? CheckIdx : UnCheckIdx);
				}
				else
				{
					for (int32 i = 0; i < RadioButton->GetChildrenCount(); ++i)
					{
						UWidgetSwitcher* switcher = URBaseWidget::FindChildWidget<UWidgetSwitcher>(RadioButton);
						//UWidgetSwitcher* switcher = Cast<UWidgetSwitcher>(RadioButton->GetChildAt(i));
						if (IsValid(switcher))
						{
							switcher->SetActiveWidgetIndex(RadioButton->CheckedState == ECheckBoxState::Checked ? CheckIdx : UnCheckIdx);
						}
					}
				}
			}
		}
	}
}

void UCanvasPanel_RadioGroup::SetActiveRadioIndex(int32 InIndex, bool bEvent)
{
	int32 CheckIndex = 0;
	int32 CheckIdx = 0;
	int32 UnCheckIdx = 1;
	CacheIndex = InIndex;

	if (bSwitcherInverse)
	{
		CheckIdx = 1;
		UnCheckIdx = 0;
	}

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (!IsValid(PanelSlot))
			continue;

		if (PanelSlot->Content == nullptr)
			continue;

		UCheckBox_Radio* RadioButton = Cast<UCheckBox_Radio>(PanelSlot->Content);
		if (!IsValid(RadioButton))
			continue;

		bool bChecked = (CheckIndex == InIndex ? true : false);
		RadioButton->SetIsChecked(bChecked);

		if (bEvent)
		{
			if (bChecked)
			{
				RadioButton->RadioButtonOnCheckStateChanged(bChecked);
			}
		}

		if (bWidgetSwitcher)
		{
			if (false == RadioButton->bLock)
			{
				for (int32 i = 0; i < RadioButton->GetChildrenCount(); ++i)
				{
					UWidgetSwitcher* switcher = URBaseWidget::FindChildWidget<UWidgetSwitcher>(RadioButton);
					//UWidgetSwitcher* switcher = Cast<UWidgetSwitcher>(RadioButton->GetChildAt(i));
					if (IsValid(switcher))
					{
						switcher->SetActiveWidgetIndex(RadioButton->CheckedState == ECheckBoxState::Checked ? CheckIdx : UnCheckIdx);
					}
				}
			}
		}

		++CheckIndex;
	}
}